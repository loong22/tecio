#pragma once

#include "ThirdPartyHeadersBegin.h"
    #include <type_traits>
#include "ThirdPartyHeadersEnd.h"

#include "CodeContract.h"
#include "StandardIntegralTypes.h"

namespace tecplot {

/**
 * Calculates the size of the identifier in bits.
 */
#define SIZE_IN_BITS(identifier) (sizeof(identifier)*8u)

/**
 * Macro to verify that a item's partition has not exceeded the allowable number of bits
 * required to be represented by a tecplot::ItemAddress64 partition.
 */
#define VALID_ITEM_ADDRESS_PARTITION(itemAddressPartition) \
    ((itemAddressPartition) != tecplot::ItemAddress64::INVALID_PARTITION && \
     (SIZE_IN_BITS(tecplot::ItemAddress64::Partition_t) == tecplot::ItemAddress64::PartitionBitSize || \
      (SIZE_IN_BITS(tecplot::ItemAddress64::Partition_t) > tecplot::ItemAddress64::PartitionBitSize && \
       uint64_t(itemAddressPartition) >> tecplot::ItemAddress64::PartitionBitSize == uint64_t(0))))

/**
 * Macro to verify that a item's section offset has not exceeded the allowable number of bits
 * required to be represented by a tecplot::ItemAddress64 section offset.
 */
#define VALID_ITEM_ADDRESS_SECTION_OFFSET(itemAddressSectionOffset) \
    ((itemAddressSectionOffset) != tecplot::ItemAddress64::INVALID_SECTION_OFFSET && \
     (SIZE_IN_BITS(tecplot::ItemAddress64::SectionOffset_t) == tecplot::ItemAddress64::SectionOffsetBitSize || \
      (SIZE_IN_BITS(tecplot::ItemAddress64::SectionOffset_t) > tecplot::ItemAddress64::SectionOffsetBitSize && \
       uint64_t(itemAddressSectionOffset) >> tecplot::ItemAddress64::SectionOffsetBitSize == uint64_t(0))))

/**
 * Macro to verify that a item's subzone offset has not exceeded the allowable number of bits
 * required to be represented by a tecplot::ItemAddress64 subzone offset.
 */
#define VALID_ITEM_ADDRESS_SUBZONE_OFFSET(itemAddressSubzoneOffset) \
    ((itemAddressSubzoneOffset) != tecplot::ItemAddress64::INVALID_SUBZONE_OFFSET && \
     (SIZE_IN_BITS(tecplot::ItemAddress64::SubzoneOffset_t) == tecplot::ItemAddress64::SubzoneOffsetBitSize || \
      (SIZE_IN_BITS(tecplot::ItemAddress64::SubzoneOffset_t) > tecplot::ItemAddress64::SubzoneOffsetBitSize && \
       uint64_t(itemAddressSubzoneOffset) >> tecplot::ItemAddress64::SubzoneOffsetBitSize == uint64_t(0))))

/**
 * Macro to verify that a subzone offset has not exceeded the allowable number of bits
 * required to be represented by a tecplot::ItemAddress64 item.
 */
#define VALID_ITEM_ADDRESS_ITEM_OFFSET(itemAddressItemOffset) \
    ((itemAddressItemOffset) != tecplot::ItemAddress64::INVALID_ITEM_OFFSET && \
     (SIZE_IN_BITS(tecplot::ItemAddress64::ItemOffset_t) == tecplot::ItemAddress64::ItemOffsetBitSize || \
      (SIZE_IN_BITS(tecplot::ItemAddress64::ItemOffset_t) > tecplot::ItemAddress64::ItemOffsetBitSize && \
       uint64_t(itemAddressItemOffset) >> tecplot::ItemAddress64::ItemOffsetBitSize == uint64_t(0))))

/**
 * Macro to verify that an item address is valid.
 */
#define VALID_UNIFORM_ITEM_ADDRESS(itemAddress) \
    ((itemAddress).isUniform())
#define VALID_SZL_ITEM_ADDRESS(itemAddress) \
    ((itemAddress).isSzlItem() && \
     VALID_ITEM_ADDRESS_PARTITION((itemAddress).partition()) && \
     VALID_ITEM_ADDRESS_SECTION_OFFSET((itemAddress).sectionOffset()) && \
     VALID_ITEM_ADDRESS_SUBZONE_OFFSET((itemAddress).subzoneOffset()) && \
     VALID_ITEM_ADDRESS_ITEM_OFFSET((itemAddress).itemOffset()))
#define VALID_ITEM_ADDRESS(itemAddress) \
    (VALID_UNIFORM_ITEM_ADDRESS(itemAddress) || \
     VALID_SZL_ITEM_ADDRESS(itemAddress))
#define VALID_ITEM_ADDRESS_SUBZONE_ADDRESS(subzoneAddress) \
    (VALID_ITEM_ADDRESS_PARTITION((subzoneAddress).partition()) && \
     VALID_ITEM_ADDRESS_SECTION_OFFSET((subzoneAddress).sectionOffset()) && \
     VALID_ITEM_ADDRESS_SUBZONE_OFFSET((subzoneAddress).subzoneOffset()))


/**
 * An item address that support SZL addressing with partition/sectionOffset/subzoneOffset/itemOffset
 * and uniform addressing with sectionOffset/uniformOffset. The class represents the address of an
 * item (cell, node or face) within a subzone using a partition and subzone offset, or uniform
 * addressing of a classic block of data. The copy performance of the class is nearly identical to a
 * word of the same size.
 *
 * A 32 bit ItemAddress is only useful for uniform addressing and only used where arrays of 32 bit
 * packing is needed to minimize memory use. Note that 1 bit is reserved for the item address type,
 * 4 bits for the section offset and the rest is reserved for the signed uniform address. Therefore
 * the 32 bit maximum value is less than an int32_t. For the maximum value allowed, refer to
 * MAX_UNIFORM_OFFSET.
 */
template <typename ITEM_TYPE>
class ItemAddress
{
public:
    static_assert(std::is_same<ITEM_TYPE,int32_t>::value ||
                  std::is_same<ITEM_TYPE,int64_t>::value,
                  "ITEM_TYPE must be a signed int32_t or int64_t type");

    /**
     * Uniform offsets must use a signed type to support signed indexing in Tecplot.
     * IMPORTANT:
     *     Courtesy of Visual Studio, all bit field members within a structure must be of the same
     *     type to instruct the compiler to pack into one 64 bit space. Because of this limitation,
     *     and because we must preserve the sign bit required by uniform addresses in Tecplot (e.g.
     *     we use -1 and -2 as flags in cell and node numbering), we have to make them all members
     *     of the UniformAddress_s structure signed 64 bit integers. The same is true for the
     *     SzlItemAddress_s structure except all the members must be 64 bit unsigned values.
     *     Basically, a structure containing bit fields must either all be signed or unsigned to
     *     work.
     */
    using SIGNED_INTEGER = ITEM_TYPE;

    /**
     * The base and SZL addresses are all unsigned.
     */
    using UNSIGNED_INTEGER = typename std::make_unsigned<SIGNED_INTEGER>::type;

    /**
     * Type of the partition containing the sections. All partition values must fit within the number
     * of bits specified by PartitionBitSize.
     */
    using Partition_t = uint32_t;

    /**
     * Type of the section containing the subzones. All section offset values must fit within the
     * number of bits specified by SectionOffsetBitSize.
     */
    using SectionOffset_t = uint32_t;

    /**
     * For counting all subzones over all partitions, which could get over 4.29 billion subzones at
     * some point.
     */
    using SubzoneIndex_t = uint64_t;

    /**
     * Type of the subzone containing the cell, node, or face item. All subzone offset values must
     * fit within the number of bits specified by SubzoneOffsetBitSize.
     */
    using SubzoneOffset_t = uint32_t;

    /**
     * Type of the cell, node, or face item within the subzone. All item values must fit within the
     * number of bits specified by ItemOffsetBitSize.
     */
    using ItemOffset_t = uint16_t;

    /**
     * The type used to hold a uniform address of an item. This allows non-SZL addressing to be
     * held by an ItemAddress for that existing algorithms in Tecplot work with both forms of data.
     * The uniform offset address is passed with a int64_t because it needs to hold uniform node
     * value addresses which are NumElements*PtsPerElement in size which is 2B*8.
     */
    using UniformOffset_t = SIGNED_INTEGER;

    /**
     * Number of bits needed to represent the integer identifying the addressing scheme used by
     * ItemAddress.
     */
    static constexpr uint32_t AddressTypeBitSize = 1u;

    /**
     * Number of bits needed to represent the section offset.
     */
    static constexpr uint32_t SectionOffsetBitSize = 4u;

    /**
     * Number of bits needed to represent the item within the subzone.
     */
    static constexpr uint32_t ItemOffsetBitSize = 8u;

    /**
     * Number of bits needed to represent the subzone offset. For 64 bit SZL addresses, we give half
     * of a UNSIGNED_INTEGER word to the subzone and section. That allows for addressing just over 1
     * trillion distinct node or cells per partition: 2^(subzone:28+section:4+item:8).
     */
    static_assert(SIZE_IN_BITS(UNSIGNED_INTEGER)/2u > SectionOffsetBitSize,
                  "Not sufficient bits available for subzones.");
    static constexpr uint32_t SubzoneOffsetBitSize =
        SIZE_IN_BITS(UNSIGNED_INTEGER)/2u - SectionOffsetBitSize;

    /**
     * Number of bits needed to represent the partition. For 64 bit SZL addresses, we give the remaining
     * bits not reserved for all the other SZL bit fields, 23 bits, to partitions. That allows for
     * addressing just over 8 million distinct partitions: 2^(partition:23).
     */
    static_assert(SIZE_IN_BITS(UNSIGNED_INTEGER) > AddressTypeBitSize + SectionOffsetBitSize + SubzoneOffsetBitSize + ItemOffsetBitSize,
                  "Not sufficient bits available for partitions.");
    static constexpr uint32_t PartitionBitSize =
        SIZE_IN_BITS(UNSIGNED_INTEGER) - (AddressTypeBitSize + SectionOffsetBitSize + SubzoneOffsetBitSize + ItemOffsetBitSize);

    /**
     * Number of bits needed to represent a sectioned uniform offset. Uniform addresses do not use
     * SZL partition/subzone/item (aka non-uniform) addressing. For 64 bit uniform addresses, this
     * allows for addressing 2^(uniformOffset:58) signed, distinct nodes or cells and for 32 bit
     * uniform addresses, this allows for addressing 2^(uniformOffset:26) signed, distinct nodes or
     * cells. Note that really there are 59 and 27 bits however uniform offsets are signed so one
     * more bit is reserved for the signed value.
     */
    static_assert(SIZE_IN_BITS(UNSIGNED_INTEGER) > AddressTypeBitSize + SectionOffsetBitSize,
                  "Not sufficient bits available for uniform offsets.");
    static constexpr uint32_t UniformOffsetBitSize =
        SIZE_IN_BITS(UNSIGNED_INTEGER) - (AddressTypeBitSize + SectionOffsetBitSize);

    static constexpr uint32_t SzlAddressType     = 0u;
    static constexpr uint32_t UniformAddressType = 1u;

    /**
     * Invalid section offset value. Unlike the invalid partition or subzone offset which is one
     * less than the maximum value that can be represented by the bits, we don't have that
     * flexibility with section offsets because we need all 4 bits to represent 16 offsets so
     * technically there is no invalid 4 bit section offset however since the value is passed around
     * as an SectionOffset_t which is wider than 4 bits we can declare an invalid section offset, it
     * just can't be used to test an 4 bit section offset.
     */
    static constexpr SectionOffset_t INVALID_SECTION_OFFSET =
        static_cast<SectionOffset_t>((uint64_t(1) << SectionOffsetBitSize));

    /**
     * Maximum value for a item address section offset. See INVALID_SECTION_OFFSET note.
     */
    static constexpr SectionOffset_t MAX_SECTION_OFFSET =
        INVALID_SECTION_OFFSET - static_cast<SectionOffset_t>(1);

    /**
     * Invalid item offset value. Unlike the invalid partition or subzone offset which is one less
     * than the maximum value that can be represented by the bits, we don't have that flexibility
     * with item offsets because we need all 8 bits to represent 256 offsets so technically there is
     * no invalid 8 bit item offset however since the value is passed around as an ItemOffset_t
     * which is wider than 8 bits we can declare an invalid item, it just can be used to test an 8
     * bit item offset.
     */
    static constexpr ItemOffset_t INVALID_ITEM_OFFSET =
        static_cast<ItemOffset_t>((uint64_t(1) << ItemOffsetBitSize));

    /**
     * Maximum value for a item address item. See INVALID_ITEM_OFFSET note.
     */
    static constexpr ItemOffset_t MAX_ITEM_OFFSET =
        INVALID_ITEM_OFFSET - static_cast<ItemOffset_t>(1);

    /**
     * Invalid partition value. Since we are using unsigned integers we sacrifice the last item.
     */
    static constexpr Partition_t INVALID_PARTITION =
        static_cast<Partition_t>((uint64_t(1) << PartitionBitSize) - uint64_t(1));

    /**
     * To construct a valid ItemAddress when we neither know nor care what partition we're in.
     */
    static constexpr Partition_t UNKNOWN_PARTITION = 0u;

    /**
     * Maximum value for a item address partition. The maximum value is one less than the invalid index.
     */
    static constexpr Partition_t MAX_PARTITION =
        INVALID_PARTITION - static_cast<Partition_t>(1);

    /**
     * Invalid subzone offset value. Since we are using unsigned integers we sacrifice the last item.
     */
    static constexpr SubzoneOffset_t INVALID_SUBZONE_OFFSET =
        static_cast<SubzoneOffset_t>((uint64_t(1) << SubzoneOffsetBitSize) - uint64_t(1));

    /**
     * Maximum value for a item address subzone offset. The maximum value is one less than the invalid index.
     */
    static constexpr SubzoneOffset_t MAX_SUBZONE_OFFSET =
        INVALID_SUBZONE_OFFSET - static_cast<SubzoneOffset_t>(1);

    /**
     * Invalid uniform offset value. Note we shift by one more bit than the uniform offset bit size
     * because we need an extra bit for the sign bit.
     */
    static constexpr UniformOffset_t INVALID_UNIFORM_OFFSET =
        static_cast<UniformOffset_t>((uint64_t(1) << (UniformOffsetBitSize - 1u)));

    /**
     * Maximum value for a item address uniform offset. The maximum value is one less than the invalid index.
     */
    static constexpr UniformOffset_t MAX_UNIFORM_OFFSET =
        INVALID_UNIFORM_OFFSET - static_cast<UniformOffset_t>(1);

private:
    /*
     * IMPORTANT:
     *     Courtesy of Visual Studio, all bit field members within a structure must be associated
     *     with types that are exactly the same size, either 32 or 64 depending on the ItemAddress
     *     type, to instruct the compiler to pack into one 64 bit space. Because of this limitation,
     *     and because we must preserve the sign bit required by uniform addresses in Tecplot (e.g.
     *     we use -1 and -2 as flags in cell and node numbering), we have to make them all members
     *     of the UniformAddress_s structure signed 64 bit integers. The same is true for the
     *     SzlItemAddress_s structure except all the members must be 64 bit unsigned values.
     *     Basically, a structure containing bit fields must either all be signed or unsigned to
     *     work.
     */

    /**
     * The address type and section offset are common to both SZL and Uniform addresses.
     */
    struct BaseAddress_s
    {
        // IMPORTANT: see note above for explanation of type choices for bit fields.
        UNSIGNED_INTEGER addressType:AddressTypeBitSize;
        UNSIGNED_INTEGER sectionOffset:SectionOffsetBitSize;
    };

    /**
     * SZL address. Note the layout of the base address items must be the same ordering as the
     * BaseAddress_s.
     */
    struct SzlItemAddress_s
    {
        // IMPORTANT: see note above for explanation of type choices for bit fields.
        UNSIGNED_INTEGER addressType:AddressTypeBitSize;
        UNSIGNED_INTEGER sectionOffset:SectionOffsetBitSize;
        UNSIGNED_INTEGER partition:PartitionBitSize;
        UNSIGNED_INTEGER subzoneOffset:SubzoneOffsetBitSize;
        UNSIGNED_INTEGER itemOffset:ItemOffsetBitSize;
    };

    /**
     * Uniform address. Note the layout of the base address items must be the same ordering as the
     * BaseAddress_s.
     */
    struct UniformAddress_s
    {
        // IMPORTANT: see note above for explanation of type choices for bit fields.
        UNSIGNED_INTEGER addressType:AddressTypeBitSize;
        UNSIGNED_INTEGER sectionOffset:SectionOffsetBitSize;
        SIGNED_INTEGER   offset:UniformOffsetBitSize;
    };

public:
    /**
     */
    class SubzoneAddress
    {
    public:
        /**
         * Constructs an uninitialized subzone address. This is intentional so that a subzone
         * address has similar performance to a 64 bit word.
         * IMPORTANT:
         *     For performance reasons the member variables are not initialized when constructing using
         *     the default constructor, much in the way a 64 bit word is not initialized. This is
         *     particularly important when allocating large arrays of subzone addresses where the
         *     number of items being added to the array is not known upfront. In these cases the
         *     Linux/Windows memory allocator will not commit actual RAM until the memory is touched
         *     so we don't want to initialize items that may never be used.
         */
        SubzoneAddress();

        SubzoneAddress(
            Partition_t     partition,
            SectionOffset_t sectionOffset,
            SubzoneOffset_t subzoneOffset);

        Partition_t partition() const;

        SectionOffset_t sectionOffset() const;

        SubzoneOffset_t subzoneOffset() const;

        bool operator==(SubzoneAddress const& other) const;
        bool operator!=(SubzoneAddress const& other) const;
        bool operator<(SubzoneAddress const& other) const;
        bool operator<=(SubzoneAddress const& other) const;
        bool operator>(SubzoneAddress const& other) const;
        bool operator>=(SubzoneAddress const& other) const;

        /**
         * @return
         *     Subzone address raw bit pattern. Note that this is not the same as a uniform offset.
         *     In addition it is important to note that the raw bit pattern varies between compilers
         *     and should not be stored in a file for later use. The raw bits are exposed primarily
         *     so that a subzone addresses can be temporarily stored in a 64 bit field.
         */
        UNSIGNED_INTEGER toRawBits() const;

        /**
         * Creates a subzone address from the raw bit pattern representing the subzone address with
         * those supplied.
         * @param
         *     Raw bits of a subzone address.
         * @return
         *     A subzone address constructed from the raw bits of another item address. Note that
         *     this is not the same as a uniform offset. In addition it is important to note that
         *     the raw bit pattern varies between compilers and should not be stored in a file for
         *     later use. The raw bits are exposed primarily so that an item addresses can be
         *     temporarily stored in a 64 bit field.
         */
        static SubzoneAddress fromRawBits(UNSIGNED_INTEGER rawBits);

    private:
        /*
         * SubzoneAddress uses the same 64 bit address layout as it's parent class, ItemAddress, but
         * only maintains the partition, section offset, and subzone.
         */
        union
        {
            // IMPORTANT: see note above for explanation of type choices for bit fields.
            BaseAddress_s    baseAddress;
            SzlItemAddress_s szlItemAddress;
            UNSIGNED_INTEGER rawBits; // ...used for bitwise equality
        } m;
    };

    /**
     * Constructs an uninitialized item address. This is intentional so that an item address has
     * similar performance to a 64 bit word.
     * IMPORTANT:
     *     For performance reasons the member variables are not initialized when constructing using
     *     the default constructor, much in the way a 64 bit word is not initialized. This is
     *     particularly important when allocating large arrays of item addresses where the number of
     *     items being added to the array is not known upfront. In these cases the Linux/Windows
     *     memory allocator will not commit actual RAM until the memory is touched so we don't want
     *     to initialize items that may never be used.
     */
    ItemAddress();

    // copy and move constructors
    ItemAddress(ItemAddress<int32_t> const& other);
    ItemAddress(ItemAddress<int64_t> const& other);
    ItemAddress(ItemAddress&& other) = default;

    /**
     * Constructs an item address from a partition/section/subzone/item.
     * @param partition
     *     The partition containing the subzone. All partition values must fit within the number of
     *     bits specified by PartitionBitSize.
     * param sectionOffset
     *     The offset of the section containing the subzones. All section offset values must fit
     *     within the number of bits specified by SectionOffsetBitSize.
     * @param subzoneOffset
     *     The offset of the subzone containing the cell, node, or face item. All subzone offset
     *     values must fit within the number of bits specified by SubzoneOffsetBitSize.
     * @param itemOffset
     *     The cell, node, or face item within the subzone. All item values must fit within the
     *     number of bits specified by ItemOffsetBitSize.
     * @pre VALID_ITEM_ADDRESS_PARTITION(partition)
     * @pre VALID_ITEM_ADDRESS_SECTION_OFFSET(sectionOffset)
     * @pre VALID_ITEM_ADDRESS_SUBZONE_OFFSET(subzoneOffset)
     * @pre VALID_ITEM_ADDRESS_ITEM_OFFSET(itemOffset)
     */
    ItemAddress(
        Partition_t     partition,
        SectionOffset_t sectionOffset,
        SubzoneOffset_t subzoneOffset,
        ItemOffset_t    itemOffset);

    /**
     * Constructs an item address from a subzone address and an node, cell, or face within the subzone.
     * @param subzoneAddress
     *     subzone address in which the item is located
     * @param itemOffset
     *     item offset of the node, cell or face within the subzone
     */
    ItemAddress(
        SubzoneAddress subzoneAddress,
        ItemOffset_t   itemOffset);

    /**
     * Constructs a item address from a uniform offset.
     */
    explicit ItemAddress(UniformOffset_t uniformOffset);

    /**
     * Constructs a item address from a section and uniform offset.
     */
    ItemAddress(
        SectionOffset_t sectionOffset,
        UniformOffset_t uniformOffset);

    /**
     * @return
     *     true if the item address is using uniform addressing, false otherwise
     */
    bool isUniform() const;

    /**
     * @return
     *     true if the item address is using single-item addressing, false otherwise
     */
    bool isSzlItem() const;

    /**
     * @return
     *     The type of addressing of the item address
     */
    uint32_t addressType() const;

    /**
     * @return
     *     The subzone address of this item address.
     */
    SubzoneAddress subzoneAddress() const;

    /**
     * @return
     *     partition in which the item address is located
     */
    Partition_t partition() const;

    /**
     * @return
     *     section in which the item address is located
     */
    SectionOffset_t sectionOffset() const;

    /**
     * @return
     *     subzone in which the item address is located
     */
    SubzoneOffset_t subzoneOffset() const;

    /**
     * @return
     *     node, cell, or face within the subzone
     */
    ItemOffset_t itemOffset() const;

    /**
     * @return
     *     uniform offset of the item (cell, node, or face)
     */
    UniformOffset_t uniformOffset() const;

    /**
     * @return
     *     Item address raw bit pattern. Note that this is not the same as a uniform offset. In
     *     addition it is important to note that the raw bit pattern varies between compilers and
     *     should not be stored in a file for later use. The raw bits are exposed primarily so that
     *     an item addresses can be temporarily stored in a 64 bit field.
     */
    UNSIGNED_INTEGER toRawBits() const;

    /**
     * Creates an item address from the raw bit pattern representing the item address with those
     * supplied.
     * @param
     *     Raw bits of an item address.
     * @return
     *     An item address constructed from the raw bits of another item address. Note that
     *     this is not the same as a uniform offset. In addition it is important to note that
     *     the raw bit pattern varies between compilers and should not be stored in a file for
     *     later use. The raw bits are exposed primarily so that an item addresses can be
     *     temporarily stored in a 64 bit field.
     */
    static ItemAddress fromRawBits(UNSIGNED_INTEGER rawBits);

    // copy and move assignment
    ItemAddress& operator=(ItemAddress const& other) = default;
    ItemAddress& operator=(ItemAddress&& other) = default;

    /**
     * See binary comparison operations for other operations.
     * @return
     *     true if the two items have bitwise equality
     */
    bool operator==(ItemAddress const& other) const;
    bool operator!=(ItemAddress const& other) const;

    /**
     * Indicates if this instance is member-wise less than the other.
     */
    bool operator<(ItemAddress const& other) const;
    bool operator<=(ItemAddress const& other) const;
    bool operator>(ItemAddress const& other) const;
    bool operator>=(ItemAddress const& other) const;

private:
    #if !defined NO_ASSERTS
    static bool validInvariants();
    #endif

    union
    {
        // IMPORTANT: see note above for explanation of type choices for bit fields.
        BaseAddress_s    baseAddress;
        SzlItemAddress_s szlItemAddress;
        UniformAddress_s uniformAddress;
        UNSIGNED_INTEGER rawBits; // ...used for bitwise equality
    } m;
};

/*
 * Convenience types. A 32 bit ItemAddress is only useful for uniform addressing and only used where
 * arrays of 32 bit packing is needed to minimize memory use. Note that 1 bit is reserved for the
 * item address type, 4 bits for the section offset and the rest is reserved for the signed uniform
 * address. Therefore the 32 bit maximum value is less than an int32_t. For the maximum value
 * allowed, refer to MAX_UNIFORM_OFFSET.
 */
using ItemAddress32 = ItemAddress<int32_t>;
using ItemAddress64 = ItemAddress<int64_t>;

static_assert(sizeof(ItemAddress32) == sizeof(ItemAddress32::SIGNED_INTEGER) &&
              sizeof(ItemAddress64) == sizeof(ItemAddress64::SIGNED_INTEGER),
              "The size of the ItemAddress union must be the same size as the ITEM_TYPE.");
/*
 * ItemAddress<ITEM_TYPE> method implementations
 */

/**
 */
template <typename ITEM_TYPE>
inline ItemAddress<ITEM_TYPE>::ItemAddress()
{
    INVARIANT(validInvariants());
}

/**
 * Upcasting from an ItemAddress<int32_t> to an ItemAddress<int64_t> will always fit, however we
 * cannot use imply copy the raw bits as they are different sizes, therefore, we are saddled with
 * copying each bit field into place.
 */
template <>
inline ItemAddress<int64_t>::ItemAddress(ItemAddress<int32_t> const& other)
{
    m.baseAddress.addressType   = other.addressType();
    m.baseAddress.sectionOffset = other.sectionOffset();
    if (other.isSzlItem())
    {
        m.szlItemAddress.partition     = other.partition();
        m.szlItemAddress.subzoneOffset = other.subzoneOffset();
        m.szlItemAddress.itemOffset    = other.itemOffset();
    }
    else
    {
        m.uniformAddress.offset = other.uniformOffset();
    }
}

/**
 * Downcasting from an ItemAddress<int64_t> to an ItemAddress<int32_t> may not fit so assert that
 * all the bit fields will hold the requested values. Additionally, we cannot simply copy the raw
 * bits as they are different sizes, therefore, we are saddled with copying each bit field into
 * place.
 */
template <>
inline ItemAddress<int32_t>::ItemAddress(ItemAddress<int64_t> const& other)
{
    // verify that the downcast will fit
    REQUIRE(IMPLICATION(other.isSzlItem() && sizeof(ItemAddress) < sizeof(other),
                        other.partition()     <= MAX_PARTITION &&
                        other.sectionOffset() <= MAX_SECTION_OFFSET &&
                        other.subzoneOffset() <= MAX_SUBZONE_OFFSET &&
                        other.itemOffset()    <= MAX_ITEM_OFFSET));
    REQUIRE(IMPLICATION(other.isUniform() && sizeof(ItemAddress) < sizeof(other),
                        other.sectionOffset() <= MAX_SECTION_OFFSET &&
                        other.uniformOffset() <= MAX_UNIFORM_OFFSET));

    m.baseAddress.addressType   = other.addressType();
    m.baseAddress.sectionOffset = other.sectionOffset();
    if (other.isSzlItem())
    {
        m.szlItemAddress.partition     = other.partition();
        m.szlItemAddress.subzoneOffset = other.subzoneOffset();
        m.szlItemAddress.itemOffset    = other.itemOffset();
    }
    else
    {
        m.uniformAddress.offset = other.uniformOffset();
    }
}

/**
 */
template <>
inline ItemAddress<int32_t>::ItemAddress(ItemAddress<int32_t> const& other) = default;

/**
 */
template <>
inline ItemAddress<int64_t>::ItemAddress(ItemAddress<int64_t> const& other) = default;

/**
 */
template <typename ITEM_TYPE>
inline ItemAddress<ITEM_TYPE>::ItemAddress(
    Partition_t     partition,
    SectionOffset_t sectionOffset,
    SubzoneOffset_t subzoneOffset,
    ItemOffset_t    itemOffset)
{
    INVARIANT(validInvariants());
    REQUIRE(SIZE_IN_BITS(ITEM_TYPE) == 64u);  // 32 bit ItemAddress's is too small for SZL
    REQUIRE(VALID_ITEM_ADDRESS_PARTITION(partition));
    REQUIRE(VALID_ITEM_ADDRESS_SECTION_OFFSET(sectionOffset));
    REQUIRE(VALID_ITEM_ADDRESS_SUBZONE_OFFSET(subzoneOffset));
    REQUIRE(VALID_ITEM_ADDRESS_ITEM_OFFSET(itemOffset));

    m.baseAddress.addressType   = SzlAddressType;
    m.baseAddress.sectionOffset = sectionOffset;

    m.szlItemAddress.partition     = partition;
    m.szlItemAddress.subzoneOffset = subzoneOffset;
    m.szlItemAddress.itemOffset    = itemOffset;
}

/**
 */
template <typename ITEM_TYPE>
inline ItemAddress<ITEM_TYPE>::ItemAddress(
    SubzoneAddress subzoneAddress,
    ItemOffset_t   itemOffset)
{
    INVARIANT(validInvariants());
    REQUIRE(SIZE_IN_BITS(ITEM_TYPE) == 64u);  // 32 bit ItemAddress's is too small for SZL
    REQUIRE(VALID_ITEM_ADDRESS_SUBZONE_ADDRESS(subzoneAddress));
    REQUIRE(VALID_ITEM_ADDRESS_ITEM_OFFSET(itemOffset));

    m.baseAddress.addressType   = SzlAddressType;
    m.baseAddress.sectionOffset = subzoneAddress.sectionOffset();

    m.szlItemAddress.partition     = subzoneAddress.partition();
    m.szlItemAddress.subzoneOffset = subzoneAddress.subzoneOffset();
    m.szlItemAddress.itemOffset    = itemOffset;
}

/**
 */
template <typename ITEM_TYPE>
inline ItemAddress<ITEM_TYPE>::ItemAddress(UniformOffset_t uniformOffset)
{
    INVARIANT(validInvariants());
    // Uniform addresses can be 32 or 64 bit; see MAX_UNIFORM_OFFSET for 32 bit limits
    REQUIRE(SIZE_IN_BITS(ITEM_TYPE) == 32u || SIZE_IN_BITS(ITEM_TYPE) == 64u);

    m.baseAddress.addressType   = UniformAddressType;
    m.baseAddress.sectionOffset = 0u; // no section

    m.uniformAddress.offset = uniformOffset;
}

/**
 */
template <typename ITEM_TYPE>
inline ItemAddress<ITEM_TYPE>::ItemAddress(
    SectionOffset_t sectionOffset,
    UniformOffset_t uniformOffset)
{
    INVARIANT(validInvariants());
    // Uniform addresses can be 32 or 64 bit; see MAX_UNIFORM_OFFSET for 32 bit limits
    REQUIRE(SIZE_IN_BITS(ITEM_TYPE) == 32u || SIZE_IN_BITS(ITEM_TYPE) == 64u);

    m.baseAddress.addressType   = UniformAddressType;
    m.baseAddress.sectionOffset = sectionOffset;

    m.uniformAddress.offset = uniformOffset;
}

/**
 */
template <typename ITEM_TYPE>
inline bool ItemAddress<ITEM_TYPE>::isUniform() const
{
    ENSURE(m.baseAddress.addressType == SzlAddressType || m.baseAddress.addressType == UniformAddressType);
    return m.baseAddress.addressType == UniformAddressType;
}

/**
 */
template <typename ITEM_TYPE>
inline bool ItemAddress<ITEM_TYPE>::isSzlItem() const
{
    ENSURE(m.baseAddress.addressType == SzlAddressType || m.baseAddress.addressType == UniformAddressType);
    return m.baseAddress.addressType == SzlAddressType;
}

/**
 */
template <typename ITEM_TYPE>
inline uint32_t ItemAddress<ITEM_TYPE>::addressType() const
{
    REQUIRE(isSzlItem() || isUniform()); // both address types support sections
    return m.baseAddress.addressType;
}

/**
 */
template <typename ITEM_TYPE>
inline typename ItemAddress<ITEM_TYPE>::SubzoneAddress ItemAddress<ITEM_TYPE>::subzoneAddress() const
{
    REQUIRE(isSzlItem());
    return SubzoneAddress(
        m.szlItemAddress.partition,
        m.baseAddress.sectionOffset,
        m.szlItemAddress.subzoneOffset);
}

/**
 */
template <typename ITEM_TYPE>
inline typename ItemAddress<ITEM_TYPE>::Partition_t ItemAddress<ITEM_TYPE>::partition() const
{
    REQUIRE(isSzlItem());
    ENSURE(VALID_ITEM_ADDRESS_PARTITION(m.szlItemAddress.partition));
    return m.szlItemAddress.partition;
}

/**
 */
template <typename ITEM_TYPE>
inline typename ItemAddress<ITEM_TYPE>::SectionOffset_t ItemAddress<ITEM_TYPE>::sectionOffset() const
{
    REQUIRE(isSzlItem() || isUniform()); // both address types support sections
    ENSURE(VALID_ITEM_ADDRESS_SECTION_OFFSET(m.baseAddress.sectionOffset));
    return m.baseAddress.sectionOffset;
}

/**
 */
template <typename ITEM_TYPE>
inline typename ItemAddress<ITEM_TYPE>::SubzoneOffset_t ItemAddress<ITEM_TYPE>::subzoneOffset() const
{
    REQUIRE(isSzlItem());
    ENSURE(VALID_ITEM_ADDRESS_SUBZONE_OFFSET(m.szlItemAddress.subzoneOffset));
    return m.szlItemAddress.subzoneOffset;
}

/**
 */
template <typename ITEM_TYPE>
inline typename ItemAddress<ITEM_TYPE>::ItemOffset_t ItemAddress<ITEM_TYPE>::itemOffset() const
{
    REQUIRE(isSzlItem());
    ENSURE(VALID_ITEM_ADDRESS_ITEM_OFFSET(m.szlItemAddress.itemOffset));
    return m.szlItemAddress.itemOffset;
}

/**
 */
template <typename ITEM_TYPE>
inline typename ItemAddress<ITEM_TYPE>::UniformOffset_t ItemAddress<ITEM_TYPE>::uniformOffset() const
{
    REQUIRE(isUniform());
    return m.uniformAddress.offset;
}

/**
 */
template <typename ITEM_TYPE>
inline typename ItemAddress<ITEM_TYPE>::UNSIGNED_INTEGER ItemAddress<ITEM_TYPE>::toRawBits() const
{
    return m.rawBits;
}

/**
 */
template <typename ITEM_TYPE>
inline typename tecplot::ItemAddress<ITEM_TYPE> ItemAddress<ITEM_TYPE>::fromRawBits(ItemAddress<ITEM_TYPE>::UNSIGNED_INTEGER rawBits)
{
    ItemAddress<ITEM_TYPE> result;
    result.m.rawBits = rawBits;
    ENSURE(VALID_ITEM_ADDRESS(result));
    return result;
}

/**
 */
template <typename ITEM_TYPE>
inline bool ItemAddress<ITEM_TYPE>::operator==(ItemAddress const& other) const
{
    REQUIRE(VALID_ITEM_ADDRESS(other));
    #if defined LINUX
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
    #endif
    return m.rawBits == other.m.rawBits;
    #if defined LINUX
        #pragma GCC diagnostic pop
    #endif
}

/**
 */
template <typename ITEM_TYPE>
inline bool ItemAddress<ITEM_TYPE>::operator!=(ItemAddress const& other) const
{
    return !(*this == other);
}

/**
 */
template <typename ITEM_TYPE>
inline bool ItemAddress<ITEM_TYPE>::operator<(ItemAddress const& other) const
{
    REQUIRE(VALID_ITEM_ADDRESS(other));

    uint32_t const thisType = addressType();
    uint32_t const otherType = other.addressType();
    if (thisType != otherType)
    {
        return thisType < otherType;
    }
    else if (thisType == tecplot::ItemAddress<ITEM_TYPE>::UniformAddressType)
    {
        if (sectionOffset() == other.sectionOffset())
            return uniformOffset() < other.uniformOffset();
        else
            return sectionOffset() < other.sectionOffset();
    }
    else if (partition() == other.partition())
    {
        if (sectionOffset() == other.sectionOffset())
        {
            if (subzoneOffset() == other.subzoneOffset())
                return itemOffset() < other.itemOffset();
            else
                return subzoneOffset() < other.subzoneOffset();
        }
        else
        {
            return sectionOffset() < other.sectionOffset();
        }
    }
    else
    {
        return partition() < other.partition();
    }
}

/**
 */
template <typename ITEM_TYPE>
inline bool ItemAddress<ITEM_TYPE>::operator<=(ItemAddress const& other) const
{
    return *this < other || *this == other;
}

template <typename ITEM_TYPE>
inline bool ItemAddress<ITEM_TYPE>::operator>(ItemAddress const& other) const
{
    return !(*this <= other);
}

template <typename ITEM_TYPE>
inline bool ItemAddress<ITEM_TYPE>::operator>=(ItemAddress const& other) const
{
    return !(*this < other);
}

#if !defined NO_ASSERTS
/**
 */
template <typename ITEM_TYPE>
inline bool ItemAddress<ITEM_TYPE>::validInvariants()
{
    /*
     * IMPORTANT:
     *     If the bit fields do not consume all bits we would be forced to first
     *     initialize the entire word to zero so that the equality test, see
     *     operator== will succeed: i.e.:
     *         m.rawBits = 0u;
     */
    return (SIZE_IN_BITS(Partition_t)     >= PartitionBitSize     &&
            SIZE_IN_BITS(SectionOffset_t) >  SectionOffsetBitSize && // ...unlike the partition and subzone, we need all 4 bits so SectionOffset_t must be larger for correct loop termination
            SIZE_IN_BITS(SubzoneOffset_t) >= SubzoneOffsetBitSize &&
            SIZE_IN_BITS(ItemOffset_t)    >  ItemOffsetBitSize    && // ...unlike the partition and subzone, we need all 8 bits so ItemOffset_t must be larger for correct loop termination
            SIZE_IN_BITS(UniformOffset_t) >= UniformOffsetBitSize &&
            AddressTypeBitSize + SectionOffsetBitSize + UniformOffsetBitSize == SIZE_IN_BITS(ItemAddress) &&
            AddressTypeBitSize + PartitionBitSize + SectionOffsetBitSize + SubzoneOffsetBitSize + ItemOffsetBitSize == SIZE_IN_BITS(ItemAddress));
}
#endif

/*
 * ItemAddress<ITEM_TYPE>::SubzoneAddress method implementations
 */

/**
 */
template <typename ITEM_TYPE>
inline ItemAddress<ITEM_TYPE>::SubzoneAddress::SubzoneAddress()
{
    INVARIANT(SIZE_IN_BITS(ITEM_TYPE) == 64u); // 32 bit ItemAddress's is too small for SZL
}

/**
 */
template <typename ITEM_TYPE>
inline ItemAddress<ITEM_TYPE>::SubzoneAddress::SubzoneAddress(
    Partition_t     partition,
    SectionOffset_t sectionOffset,
    SubzoneOffset_t subzoneOffset)
{
    INVARIANT(SIZE_IN_BITS(ITEM_TYPE) == 64u); // 32 bit ItemAddress's is too small for SZL
    REQUIRE(VALID_ITEM_ADDRESS_PARTITION(partition) ||
            partition == ItemAddress<ITEM_TYPE>::INVALID_PARTITION); // used when subzoning data
    REQUIRE(VALID_ITEM_ADDRESS_SECTION_OFFSET(sectionOffset));
    REQUIRE(VALID_ITEM_ADDRESS_SUBZONE_OFFSET(subzoneOffset));

    m.baseAddress.addressType   = SzlAddressType;
    m.baseAddress.sectionOffset = sectionOffset;

    m.szlItemAddress.partition     = partition;
    m.szlItemAddress.subzoneOffset = subzoneOffset;
    m.szlItemAddress.itemOffset    = 0u; // not used for subzone address
}

/**
 */
template <typename ITEM_TYPE>
inline typename ItemAddress<ITEM_TYPE>::Partition_t ItemAddress<ITEM_TYPE>::SubzoneAddress::partition() const
{
    REQUIRE(VALID_ITEM_ADDRESS_PARTITION(m.szlItemAddress.partition) ||
            m.szlItemAddress.partition == ItemAddress<ITEM_TYPE>::INVALID_PARTITION); // used when subzoning data
    REQUIRE(VALID_ITEM_ADDRESS_SECTION_OFFSET(m.baseAddress.sectionOffset));
    REQUIRE(VALID_ITEM_ADDRESS_SUBZONE_OFFSET(m.szlItemAddress.subzoneOffset));
    return m.szlItemAddress.partition;
}

/**
 */
template <typename ITEM_TYPE>
inline typename ItemAddress<ITEM_TYPE>::SectionOffset_t ItemAddress<ITEM_TYPE>::SubzoneAddress::sectionOffset() const
{
    REQUIRE(VALID_ITEM_ADDRESS_PARTITION(m.szlItemAddress.partition) ||
            m.szlItemAddress.partition == ItemAddress<ITEM_TYPE>::INVALID_PARTITION); // used when subzoning data
    REQUIRE(VALID_ITEM_ADDRESS_SECTION_OFFSET(m.baseAddress.sectionOffset));
    REQUIRE(VALID_ITEM_ADDRESS_SUBZONE_OFFSET(m.szlItemAddress.subzoneOffset));
    return m.baseAddress.sectionOffset;
}

/**
 */
template <typename ITEM_TYPE>
inline typename ItemAddress<ITEM_TYPE>::SubzoneOffset_t ItemAddress<ITEM_TYPE>::SubzoneAddress::subzoneOffset() const
{
    REQUIRE(VALID_ITEM_ADDRESS_PARTITION(m.szlItemAddress.partition) ||
            m.szlItemAddress.partition == ItemAddress<ITEM_TYPE>::INVALID_PARTITION); // used when subzoning data
    REQUIRE(VALID_ITEM_ADDRESS_SECTION_OFFSET(m.baseAddress.sectionOffset));
    REQUIRE(VALID_ITEM_ADDRESS_SUBZONE_OFFSET(m.szlItemAddress.subzoneOffset));
    return m.szlItemAddress.subzoneOffset;
}

/**
 */
template <typename ITEM_TYPE>
inline bool ItemAddress<ITEM_TYPE>::SubzoneAddress::operator==(SubzoneAddress const& other) const
{
    REQUIRE(VALID_ITEM_ADDRESS_PARTITION(m.szlItemAddress.partition) ||
            m.szlItemAddress.partition == ItemAddress<ITEM_TYPE>::INVALID_PARTITION); // used when subzoning data
    REQUIRE(VALID_ITEM_ADDRESS_SECTION_OFFSET(m.baseAddress.sectionOffset));
    REQUIRE(VALID_ITEM_ADDRESS_SUBZONE_OFFSET(m.szlItemAddress.subzoneOffset));
    #if defined LINUX
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
    #endif
    return m.rawBits == other.m.rawBits;
    #if defined LINUX
        #pragma GCC diagnostic pop
    #endif
}

/**
 */
template <typename ITEM_TYPE>
inline bool ItemAddress<ITEM_TYPE>::SubzoneAddress::operator!=(SubzoneAddress const& other) const
{
    return !(*this == other);
}

/**
 */
template <typename ITEM_TYPE>
inline bool ItemAddress<ITEM_TYPE>::SubzoneAddress::operator<(SubzoneAddress const& other) const
{
    REQUIRE(VALID_ITEM_ADDRESS_PARTITION(m.szlItemAddress.partition) ||
            m.szlItemAddress.partition == ItemAddress<ITEM_TYPE>::INVALID_PARTITION); // used when subzoning data
    REQUIRE(VALID_ITEM_ADDRESS_SECTION_OFFSET(m.baseAddress.sectionOffset));
    REQUIRE(VALID_ITEM_ADDRESS_SUBZONE_OFFSET(m.szlItemAddress.subzoneOffset));

    if (partition() == other.partition())
    {
        if (sectionOffset() == other.sectionOffset())
            return subzoneOffset() < other.subzoneOffset();
        else
            return sectionOffset() < other.sectionOffset();
    }
    else
    {
        return partition() < other.partition();
    }
}

/**
 */
template <typename ITEM_TYPE>
inline bool ItemAddress<ITEM_TYPE>::SubzoneAddress::operator<=(SubzoneAddress const& other) const
{
    return *this < other || *this == other;
}

/**
 */
template <typename ITEM_TYPE>
inline bool ItemAddress<ITEM_TYPE>::SubzoneAddress::operator>(SubzoneAddress const& other) const
{
    return !(*this <= other);
}

/**
 */
template <typename ITEM_TYPE>
inline bool ItemAddress<ITEM_TYPE>::SubzoneAddress::operator>=(SubzoneAddress const& other) const
{
    return !(*this < other);
}

/**
 */
template <typename ITEM_TYPE>
inline typename ItemAddress<ITEM_TYPE>::UNSIGNED_INTEGER ItemAddress<ITEM_TYPE>::SubzoneAddress::toRawBits() const
{
    return m.rawBits;
}

/**
 */
template <typename ITEM_TYPE>
inline typename tecplot::ItemAddress<ITEM_TYPE>::SubzoneAddress ItemAddress<ITEM_TYPE>::SubzoneAddress::fromRawBits(ItemAddress<ITEM_TYPE>::UNSIGNED_INTEGER rawBits)
{
    ItemAddress<ITEM_TYPE>::SubzoneAddress result;
    result.m.rawBits = rawBits;
    ENSURE(VALID_ITEM_ADDRESS_PARTITION(result.m.szlItemAddress.partition) ||
            result.m.szlItemAddress.partition == ItemAddress<ITEM_TYPE>::INVALID_PARTITION); // used when subzoning data
    ENSURE(VALID_ITEM_ADDRESS_SECTION_OFFSET(result.m.baseAddress.sectionOffset));
    ENSURE(VALID_ITEM_ADDRESS_SUBZONE_OFFSET(result.m.szlItemAddress.subzoneOffset));
    return result;
}

}
