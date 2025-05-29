#pragma once

#include "ThirdPartyHeadersBegin.h"
    #include <algorithm>
    #include <array>
    #include <vector>
#include "ThirdPartyHeadersEnd.h"

#include "ClassMacros.h"
#include "CodeContract.h"
#include "MiscMacros.h"

namespace tecplot {

/**
 * A simple and fast abstraction over raw arrays to allow for dynamic resizing. The raw array can be
 * stack or heap allocated. If a size request exceeds the initial capacity the array will be
 * reallocated using operator new[] and operator delete[]. Consequently it is important to not
 * exceed the capacity of a stack allocated array. The main responsibility of this class is to
 * maintain a raw array with its capacity and size and if necessary resize the underlying
 * representation. Ownership of the array remains with the client, even if reallocated by this
 * class.
 *
 * The motivation for this class is to provide high performance access to raw arrays that need to be
 * dynamically sized where the performance of std::vector would not suffice because of its
 * requirements to be more general and Tecplot's need to adapt raw arrays for various interfaces in
 * the code base.
 */
template <typename T>
class RawArray
{
public:
    void logMemoryAllocation(
        char const* container,
        size_t numElements)
    {
#ifdef LARGE_ARRAY_MEMORY_LOGGING
        size_t const MEMTRACK_CUTOFF = size_t(1000)*size_t(1000);
        if ( numElements * sizeof(T) >= MEMTRACK_CUTOFF )
        {
            FILE *file = fopen("memtrack.txt", "at");
            if ( file )
            {
                fprintf(file, "%s\t%" "I64u" "\t%" "I64u" "\t%s\n", container, numElements, sizeof(T), typeid(T).name());
                fclose(file);
            }
            else
                throw std::bad_alloc();
        }
#else
        UNUSED(container);
        UNUSED(numElements);
#endif
    }

    /**
     * Value type managed by the array.
     */
    typedef T value_type;


    /*
     * We don't want RawArray's to be copyable because they hold onto the owner location and only
     * one RawArray can represent that location. They can however be moveable so for C++11 allow
     * RawArray's to be movable thereby allowing them to be place in container that support moving.
     */
    #if (defined _MSC_VER && __cplusplus > 199711L) || __cplusplus >= 201103L
        RawArray(RawArray const&) = delete;
        RawArray& operator=(RawArray const&) = delete;

        #if defined _MSC_VER && _MSC_VER <= 1800 // Visual Studio 2013 or earlier
            RawArray(RawArray&& other) : m_valuesRef(std::move(other.m_valuesRef)), m_values(std::move(other.m_values)), m_capacity(std::move(other.m_capacity)), m_size(std::move(other.m_size)) {}
            RawArray& operator=(RawArray&& rhs) { if (this != &rhs) { m_valuesRef = std::move(rhs.m_valuesRef); m_values = std::move(rhs.m_values); m_capacity = std::move(rhs.m_capacity); m_size = std::move(rhs.m_size); } return *this; }
        #else
            RawArray(RawArray&&) = default;
            RawArray& operator=(RawArray&&) = default;
        #endif
    #else
        private:
        RawArray(RawArray const&);
        RawArray& operator=(RawArray const&);
        public:
    #endif

    /**
     * Constructs a raw array pointing to NULL. It cannot be resized and is primarily used as a
     * placeholder for later replacement.
     */
    inline RawArray()
        : m_valuesRef(NULL)
        , m_values(NULL)
        , m_capacity(0)
        , m_size(0)
    {
    }

    /**
     * Creates a simple abstraction over a std::array with the specified size that must be less or
     * equal to the values array's capacity. It cannot be sized larger than the std::array's size
     * definition.
     * @param values
     *     The address of an array of simple types.
     * @param size
     *     Current size of the array. If not supplied it is zero.
     */
    template <size_t N>
    inline RawArray(
        std::array<T,N>& values,
        size_t           size = 0)
        : m_valuesRef(NULL)
        , m_values(values.data())
        , m_capacity(values.size())
        , m_size(size)
    {
        REQUIRE(VALID_REF(this->m_values));
        REQUIRE(m_capacity != 0);
        REQUIRE(this->m_capacity >= this->m_size);
    }

    /**
     * Creates a simple abstraction over a std::vector with the specified size that must be less or
     * equal to the values vector's size at construction of the RawArray. It cannot be sized larger
     * than the std::vector's current size at construction of the RawArray.
     * @note:
     *     The underlying vector must not be independently manipulated in any way that would cause
     *     the vector to reallocate it's underlying buffer, thereby releasing the buffer held onto
     *     by the RawArray.
     * @param values
     *     The address of an array of simple types.
     * @param size
     *     Current size of the array. If not supplied it is zero.
     */
    inline RawArray(
        std::vector<T>& values,
        size_t          size = 0)
        : m_valuesRef(NULL)
        , m_values(values.data())
        , m_capacity(values.size())
        , m_size(size)
    {
        REQUIRE(VALID_REF(this->m_values));
        REQUIRE(m_capacity != 0);
        REQUIRE(this->m_capacity >= this->m_size);
    }

    /**
     * Creates a simple abstraction over a raw C array such as "int a[5]". It cannot be sized larger
     * than the underlying C array capacity.
     * @param values
     *     The address of an array of simple types.
     * @param capacity
     *     The capacity of the array.
     * @param size
     *     Current size of the array. If not supplied it is zero.
     */
    inline RawArray(
        T* const& values,
        size_t    capacity,
        size_t    size = 0)
        : m_valuesRef(NULL)
        , m_values(const_cast<T*>(values))
        , m_capacity(capacity)
        , m_size(size)
    {
        REQUIRE(VALID_REF(this->m_values));
        REQUIRE(m_capacity != 0);
        REQUIRE(this->m_capacity >= this->m_size);
    }

    /**
     * Creates a simple abstraction over a pointer to a raw C array such as "int* a = new int[5]".
     * If during the life of the RawArray instance, the array size grows larger such that the
     * current array capacity is insufficient, the underlying array is deleted and is reallocated to
     * accommodate the larger capacity need.
     * @param values
     *     The address of an array of simple types.
     * @param capacity
     *     The capacity of the array. If zero the values array must also be NULL.
     * @param size
     *     Current size of the array. If not supplied it is zero.
     */
    inline RawArray(
        T*&    values,
        size_t capacity = 0,
        size_t size = 0)
        : m_valuesRef(&values)
        , m_values(values)
        , m_capacity(capacity)
        , m_size(size)
    {
        REQUIRE(VALID_REF(this->m_values) || this->m_values == NULL);
        REQUIRE(EQUIVALENCE(this->m_values == NULL, m_capacity == 0));
        REQUIRE(this->m_capacity >= this->m_size);
    }

    /**
     * Nullify the object effectively making it point to NULL. Effectively it puts the RawArray into
     * a state as if it was constructed with the default constructor. After nullifying it cannot be
     * resized and is primarily used as a placeholder for later replacement. Also, nullifying does
     * not perform any cleanup.
     * @sa the default constructor, RawArray()
     */
    inline void nullify()
    {
        if (m_valuesRef != NULL)
            *m_valuesRef = NULL; // ...make sure the referencing array is nullified
        m_valuesRef = NULL;
        m_values = NULL;
        m_capacity = 0;
        m_size = 0;
    }

    /**
     * @return true if the raw array is empty otherwise false
     */
    inline bool empty() const
    {
        return m_size == 0;
    }

    /**
     * @return current array size
     */
    inline size_t size() const
    {
        return m_size;
    }

    /**
     * Ensures that the array has sufficient capacity to hold the specified number of items. If the
     * capacity needs to grow to accommodate a larger size it is expanded to exactly the requested
     * number.
     * @param size
     *     Requested capacity.
     * @throws std::bad_alloc
     *     if the capacity needed to be enlarged but sufficient resources were not available
     */
    inline void reserve(size_t size)
    {
        if (size > m_capacity)
            enlargeCapacity(size);
    }

    /**
     * Sets the size of the array, ensuring there is sufficient capacity.
     * @param size
     *     The new size of the array.
     */
    inline void setSize(size_t size)
    {
        REQUIRE(size <= m_capacity);
        m_size = size;
        ENSURE(m_size <= m_capacity);
    }

    /**
     * Sets the size of the array to zero (empty)
     */
    inline void clear()
    {
        m_size = 0;
    }

    /**
     * @return current array capacity
     */
    inline size_t capacity() const
    {
        return m_capacity;
    }

    /**
     * Copy the size and values from the other raw array to this one.
     * @param other
     *     Other array from which to copy the values.
     * @return
     *     This raw array reference.
     * @throws std::bad_alloc
     *     if the capacity needed to be enlarged but sufficient resources were not available
     */
    inline RawArray& copy(RawArray const& other)
    {
        reserve(other.m_size);
        setSize(other.m_size);
        for (size_t offset = 0; offset < m_size; ++offset)
            m_values[offset] = other.m_values[offset];
        return *this;
    }

    /**
     * Append the values from the other array to the end of this array.
     * @param other
     *     Other array from which to append the values.
     * @return
     *     This raw array reference.
     * @throws std::bad_alloc
     *     if the capacity needed to be enlarged but sufficient resources were not available
     */
    inline RawArray& append(RawArray const& other)
    {
        size_t const origSize = m_size;
        reserve(m_size + other.m_size);
        setSize(m_size + other.m_size);
        for (size_t offset = origSize; offset < m_size; ++offset)
            m_values[offset] = other.m_values[offset-origSize];
        return *this;
    }

    /**
     * Appends a new value at the end of this array.
     * @note:
     *     This function will efficiently enlarge the array's capacity if the size bumps into the current capacity.
     * @param value
     *     The value to append.
     * @throws std::bad_alloc
     *     if the capacity needed to be enlarged but sufficient resources were not available
     */
    inline void push_back(T const& value)
    {
        size_t const requestedCapacity = m_size + 1ul;
        if (requestedCapacity > m_capacity)
        {
            size_t const defaultCapacity = 32ul;
            size_t const blockSize = std::max(defaultCapacity, m_capacity / 2ul);
            size_t const adjustedRequest = ((requestedCapacity - 1ul) / blockSize + 1ul) * blockSize;
            CHECK(adjustedRequest >= requestedCapacity);
            reserve(adjustedRequest);
        }

        size_t const valuePos = m_size;
        setSize(m_size + 1ul);
        m_values[valuePos] = value;
    }

    /**
     * Copy the values of this array to the target C array. The caller must make sure the target
     * C array has sufficient capacity to receive the values.
     * @param target
     *     The raw array to receive the values and size.
     */
    inline void copyTo(T* target)
    {
        for (size_t offset = 0; offset < m_size; ++offset)
            target[offset] = m_values[offset];
    }

    /**
     * Access the array item at the base of the array for modification.
     * @return address of the array member at the base of the array
     */
    inline T& front()
    {
        REQUIRE(this->m_size != 0 ||
                this->m_capacity != 0); // ...special case to access the array base
        return m_values[0];
    }

    /**
     * Access the array item at the base of the array for examination.
     * @return address of the array member at the base of the array
     */
    inline T const& front() const
    {
        REQUIRE(this->m_size != 0 ||
                this->m_capacity != 0); // ...special case to access the array base
        return m_values[0];
    }

    /**
     * Access the array item at the end of the array for modification.
     * @return address of the array member at the end of the array
     */
    inline T& back()
    {
        REQUIRE(this->m_size != 0);
        return m_values[m_size-1];
    }

    /**
     * Access the array item at the end of the array for examination.
     * @return address of the array member at the end of the array
     */
    inline T const& back() const
    {
        REQUIRE(this->m_size != 0);
        return m_values[m_size-1];
    }

    /**
     * Access the array item at the specified offset for modification.
     * @note
     *     If you intend to assign many values to the array you should instead get the base address
     *     of the array into a local copy and access the members via the local pointer:
     *         int* localCopy = &myValues[0];
     *         for (i in 1..1000000)
     *             localCopy[i] = ...
     * @return address of the array member at offset
     */
    inline T& operator[](size_t offset)
    {
        REQUIRE(offset < this->m_size ||
                (offset == 0 && this->m_capacity != 0)); // ...special case to access the array base
        return m_values[offset];
    }

    /**
     * Access the array item at the specified offset for examination.
     * @note
     *     If you intend to read many values from the array you should instead get the base address
     *     of the array into a local copy and access the members via the local pointer:
     *         int* localCopy = &myValues[0];
     *         for (i in 1..1000000)
     *             doSomthing(localCopy[i]);
     * @return constant address of the array member at offset
     */
    inline T const& operator[](size_t offset) const
    {
        REQUIRE(offset < this->m_size ||
                (offset == 0 && this->m_capacity != 0)); // ...special case to access the array base
        return m_values[offset];
    }

    /**
     * @return true if the contents of the container are equal, false otherwise
     */
    bool operator==(RawArray<T> const& rhs) const
    {
        bool equal = m_size == rhs.m_size;
        for (size_t offset = 0; equal && offset < m_size; ++offset)
            equal = m_values[offset] == rhs.m_values[offset];
        return equal;
    }

    /**
     * @return true if the contents of the container are not equal, false otherwise
     */
    bool operator!=(RawArray<T> const& rhs) const
    {
        return !(*this == rhs);
    }

    /**
     * @return true if the raw array is pointing to NULL, false otherwise
     */
    inline bool isNull() const
    {
        return (m_valuesRef == NULL || *m_valuesRef == NULL);
    }

    /**
     * @return a direct pointer to the memory array used internally by the raw array to store its elements.
     */
    inline T* data()
    {
        return m_values;
    }

    /**
     * @return a direct pointer to the memory array used internally by the raw array to store its elements.
     */
    inline T const* data() const
    {
        return m_values;
    }

    typedef T const* const_iterator;
    inline const_iterator begin() const
    {
        return m_values;
    }
    inline const_iterator end() const
    {
        return m_values+m_size;
    }

    typedef T* iterator;
    inline iterator begin()
    {
        return m_values;
    }
    inline iterator end()
    {
        return m_values+m_size;
    }

private:
    /**
     * Enlarges the array capacity such that it can receive values between 0 and capacity-1. A new
     * array is created using operator new[] into which the new values are copied, after which the
     * original array is deleted using operator delete[]. Consequently it is important, if the
     * capacity is to be enlarged, that the underlying array was allocated with operator new[].
     *
     * @note
     *     This method is only exception safe for array member types who's copy constructors do not
     *     throw.
     *
     * @param capacity
     *     Needed capacity.
     * @throws std::bad_alloc
     *     if sufficient resources were not available to enlarge the capacity
     */
    inline void enlargeCapacity(size_t capacity)
    {
        REQUIRE(capacity > m_capacity);
        REQUIRE(m_valuesRef != NULL); // if this assertion fires a fixed sized array (aka stack based) capacity has been exceeded
        logMemoryAllocation("RawArray", capacity);
        T* values = new T[capacity];
        if (m_size != 0)
        {
            #if defined LINUX
                #pragma GCC diagnostic push
                // These warnings are triggered when compiling with gcc 13.0
                #pragma GCC diagnostic ignored "-Warray-bounds"
                #pragma GCC diagnostic ignored "-Wstringop-overflow"
            #endif
            std::copy(m_values, m_values+m_size, values);
            #if defined LINUX
                #pragma GCC diagnostic pop
            #endif
        }
        delete [] m_values;
        *m_valuesRef = values;
        m_values     = values;
        m_capacity   = capacity;
    }

    T**    m_valuesRef; // ...allows the class to reallocate
    T*     m_values;    // ...helps the optimizer create high performance access code
    size_t m_capacity;
    size_t m_size;
};

}
