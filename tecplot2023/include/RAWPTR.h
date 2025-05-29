#pragma once

#include "TecUtil.h"

/**
 */
struct RawValuePtr_t
{
    FieldData_pa              FieldData;
    void                     *RawDataPtr; /* may not always be available */
    FieldValueGetFunction_pf  GetFieldValue;
    FieldValueSetFunction_pf  SetFieldValue;
    FieldDataType_e           FieldDataType;
    ValueLocation_e           ValueLocation;
    LgIndex_t                 ValueCount;

    RawValuePtr_t()
    : FieldData(nullptr)
    , RawDataPtr(nullptr)
    , GetFieldValue(nullptr)
    , SetFieldValue(nullptr)
    , FieldDataType(FieldDataType_Invalid)
    , ValueLocation(ValueLocation_Invalid)
    , ValueCount(0)
    {}
};

/**
 */
struct RawNodePtr_t
{
    NodeMap_pa       nodeMap;
    EntIndex_t       section;
    OffsetDataType_e nodemapDataType;
    int32_t*         rawNodeMapPtr;  // Use int32_t to avoid casting for 32-bit.
    LgIndex_t        cellCount;
    int32_t          cornersPerCell;
    int32_t          nodesPerCell;

    RawNodePtr_t(
        NodeMap_pa       nodeMap,
        EntIndex_t       section,
        OffsetDataType_e nodemapDataType,
        int32_t*         rawNodeMapPtr,
        LgIndex_t        cellCount,
        int32_t          cornersPerCell,
        int32_t          nodesPerCell)
        : nodeMap(nodeMap)
        , section(section)
        , nodemapDataType(nodemapDataType)
        , rawNodeMapPtr(rawNodeMapPtr)
        , cellCount(cellCount)
        , cornersPerCell(cornersPerCell)
        , nodesPerCell(nodesPerCell)
    {
    }

    RawNodePtr_t()
    : nodeMap(nullptr)
    , section(1)
    , nodemapDataType(OffsetDataType_Invalid)
    , rawNodeMapPtr(nullptr)
    , cellCount(0)
    , cornersPerCell(0)
    , nodesPerCell(0)
    {}
};

/**
 * Indicates if the raw value pointer item is valid.
 *
 * @param RawValuePtr
 *     Raw value pointer item in question.
 *
 * @return
 *     TRUE if valid, FALSE otherwise.
 */
inline Boolean_t RawValuePtrIsValid(RawValuePtr_t RawValuePtr)
{
    return ((RawValuePtr.FieldData != NULL  || RawValuePtr.RawDataPtr != NULL) &&
            VALID_ENUM(RawValuePtr.FieldDataType, FieldDataType_e) &&
            RawValuePtr.ValueCount > 0);
}

/**
 * Gets the specified value from the raw value pointer item at the specified
 * index.
 *
 * @param RawValuePtr
 *     Raw value pointer item from which the value is fectched.
 * @param Index
 *     Index at which the point is desired.
 *
 * @return
 *     Value at the specified index.
 */
inline double RawValuePtrGetValue(
    RawValuePtr_t RawValuePtr,
    LgIndex_t     Index)
{
    REQUIRE(RawValuePtrIsValid(RawValuePtr));
    REQUIRE(0 < Index && Index <= RawValuePtr.ValueCount);

    return (RawValuePtr.RawDataPtr != NULL
            ? (double)((RawValuePtr.FieldDataType == FieldDataType_Float ? ((float *)RawValuePtr.RawDataPtr)[Index - 1] :
                       (RawValuePtr.FieldDataType == FieldDataType_Double ? ((double *)RawValuePtr.RawDataPtr)[Index - 1] :
                       (RawValuePtr.FieldDataType == FieldDataType_LongInt ? ((int32_t *)RawValuePtr.RawDataPtr)[Index - 1] :
                       (RawValuePtr.FieldDataType == FieldDataType_ShortInt ? ((short *)RawValuePtr.RawDataPtr)[Index - 1] :
                       (RawValuePtr.FieldDataType == FieldDataType_Byte ? ((uint8_t *)RawValuePtr.RawDataPtr)[Index - 1] :
                       ((((uint8_t *)RawValuePtr.RawDataPtr)[(Index - 1) / 8] >> ((Index - 1) % 8)) & (uint8_t)0x1)))))))
            : RawValuePtr.GetFieldValue(RawValuePtr.FieldData, Index - 1));
}

/**
 * Gets the value location from the raw value pointer item.
 *
 * @param RawValuePtr
 *     Raw value pointer item from which the value location is needed.
 *
 * @return
 *     Value location of the raw value pointer item.
 */
inline ValueLocation_e RawValuePtrGetValueLoc(RawValuePtr_t RawValuePtr)
{
    REQUIRE(RawValuePtrIsValid(RawValuePtr));
    return RawValuePtr.ValueLocation;
}


/**
 * Gets the field data type from the raw value pointer item.
 *
 * @param RawValuePtr
 *     Raw value pointer item from which the field data type is needed.
 *
 * @return
 *     field data type of the raw value pointer item.
 */
inline FieldDataType_e RawValuePtrGetValueType(RawValuePtr_t RawValuePtr)
{
    REQUIRE(RawValuePtrIsValid(RawValuePtr));

    return RawValuePtr.FieldDataType;
}

/**
 * Gets the number of values from the raw value pointer item.
 *
 * @param RawValuePtr
 *     Raw value pointer item from which the value count is needed.
 *
 * @return
 *     value count of the raw value pointer item.
 */
inline LgIndex_t RawValuePtrGetValueCount(RawValuePtr_t RawValuePtr)
{
    REQUIRE(RawValuePtrIsValid(RawValuePtr));

    return RawValuePtr.ValueCount;
}

/**
 * Assigns the value to the raw value pointer item at the specified index.
 *
 * @param RawValuePtr
 *     Raw value pointer item to receive the new value.
 * @param Index
 *     Index to which the value is assigned.
 * @param Value
 *     New value to assign.
 */
inline void RawValuePtrSetValue(
    RawValuePtr_t RawValuePtr,
    LgIndex_t     Index,
    double        Value)
{
    REQUIRE(RawValuePtrIsValid(RawValuePtr));
    REQUIRE(0 < Index && Index <= RawValuePtr.ValueCount);

    if (RawValuePtr.RawDataPtr != NULL)
    {
        switch(RawValuePtr.FieldDataType)
        {
            case FieldDataType_Float:
                ((float *)(RawValuePtr.RawDataPtr))[Index - 1] = (float)(Value);
                break;
            case FieldDataType_Double:
                ((double *)(RawValuePtr.RawDataPtr))[Index - 1] = Value;
                break;
            case FieldDataType_LongInt:
                ((int32_t *)(RawValuePtr.RawDataPtr))[Index - 1] = (int32_t)(Value);
                break;
            case FieldDataType_ShortInt:
                ((short *)(RawValuePtr.RawDataPtr))[Index - 1] = (short)(Value);
                break;
            case FieldDataType_Byte:
                ((uint8_t *)(RawValuePtr.RawDataPtr))[Index - 1] = (uint8_t)(Value);
                break;
            case FieldDataType_Bit:
            {
                LgIndex_t ByteOffset = (Index - 1) / 8;
                uint8_t    BitMask    = (uint8_t)(01 << ((Index - 1)%8));
                if (Value < 1.0)
                    ((uint8_t *)(RawValuePtr.RawDataPtr))[ByteOffset] &= ~BitMask;
                else
                    ((uint8_t *)(RawValuePtr.RawDataPtr))[ByteOffset] |= BitMask;
            } break;
            default:
                CHECK(FALSE);
                break;
        }
    }
    else
    {
        RawValuePtr.SetFieldValue(RawValuePtr.FieldData, Index - 1, Value);
    }
}

/**
 * Indicates if the raw node pointer item is valid.
 *
 * @param RawNodePtr
 *     Raw node pointer item in question.
 *
 * @return
 *     TRUE if valid, FALSE otherwise.
 */
inline Boolean_t RawNodePtrIsValid(RawNodePtr_t RawNodePtr)
{
    return ((RawNodePtr.rawNodeMapPtr != NULL || RawNodePtr.nodeMap != NULL) &&
            RawNodePtr.section > 0 &&
            RawNodePtr.cellCount > 0 &&
            2 <= RawNodePtr.cornersPerCell &&
            RawNodePtr.cornersPerCell <= 8);
}

/**
 * Gets the specified node from the raw node pointer item at the specified
 * index.
 *
 * @param RawNodePtr
 *     Raw node pointer item from which the node is fectched.
 * @param Cell
 *     Cell index
 * @param NodeIndex
 *     Node index within the cell (Corner for linear cells).
 *
 * @return
 *     Node at the specified index.
 */
inline NodeMap_t RawNodePtrGetNode(
    RawNodePtr_t RawNodePtr,
    LgIndex_t    Cell,
    int32_t      NodeIndex)
{
    REQUIRE(RawNodePtrIsValid(RawNodePtr));
    REQUIRE(0 < Cell && Cell <= RawNodePtr.cellCount);
    REQUIRE(0 < NodeIndex && NodeIndex <= RawNodePtr.nodesPerCell);

    return (RawNodePtr.rawNodeMapPtr != NULL
            ? (RawNodePtr.nodemapDataType == OffsetDataType_32Bit
               ? (RawNodePtr.rawNodeMapPtr[((LgIndex_t)(Cell) - 1) * RawNodePtr.nodesPerCell + NodeIndex - 1] + 1)
               : (((int64_t*)(RawNodePtr.rawNodeMapPtr))[((LgIndex_t)(Cell) - 1) * RawNodePtr.nodesPerCell + NodeIndex - 1] + 1))
            : TecUtilDataNodeGetByRef(RawNodePtr.nodeMap, Cell, NodeIndex));
}

/**
 * Gets the number of corner nodes for each cell of the raw node map pointer item.
 *
 * @param RawNodePtr
 *     Raw node pointer item from which the information is fetched.
 *
 * @return
 *     Number of corner nodes for each cell.
 */
inline int32_t RawNodePtrGetCornersPerCell(RawNodePtr_t RawNodePtr)
{
    REQUIRE(RawNodePtrIsValid(RawNodePtr));
    return RawNodePtr.cornersPerCell;
}

/**
 * Gets the number of nodes for each cell of the raw node map pointer item.
 *
 * @param RawNodePtr
 *     Raw node pointer item from which the information is fetched.
 *
 * @return
 *     Number of nodes for each cell.
 */
inline int32_t RawNodePtrGetNodesPerCell(RawNodePtr_t RawNodePtr)
{
    REQUIRE(RawNodePtrIsValid(RawNodePtr));
    return RawNodePtr.nodesPerCell;
}

/**
 * Assigns the node to the raw node pointer item at the specified index.
 *
 * @param RawNodePtr
 *     Raw node pointer item to receive the new node.
 * @param Cell
 *     Cell index to which the node is assigned.
 * @param NodeIndex
 *     Node index within the cell (Corner for linear cells).
 * @param Node
 *     New node value to assign.
 */
inline void RawNodePtrSetNode(
    RawNodePtr_t RawNodePtr,
    LgIndex_t    Cell,
    int32_t      NodeIndex,
    NodeMap_t    Node)
{
    REQUIRE(RawNodePtrIsValid(RawNodePtr));
    REQUIRE(0 < Cell && Cell <= RawNodePtr.cellCount);
    REQUIRE(0 < NodeIndex && NodeIndex <= RawNodePtr.nodesPerCell);
    REQUIRE(Node > 0);

    if (RawNodePtr.rawNodeMapPtr != NULL)
    {
        if (RawNodePtr.nodemapDataType == OffsetDataType_32Bit)
        {
            RawNodePtr.rawNodeMapPtr[((LgIndex_t)(Cell) - 1) * RawNodePtr.nodesPerCell + NodeIndex - 1] = (int32_t)(Node) - 1;
        }
        else
        {
            int64_t* rawNodeMapPtr64 = (int64_t*)(RawNodePtr.rawNodeMapPtr);
            rawNodeMapPtr64[((LgIndex_t)(Cell) - 1) * RawNodePtr.nodesPerCell + NodeIndex - 1] = Node - 1;
        }
    }
    else
    {
        TecUtilDataNodeSetByRef(RawNodePtr.nodeMap, Cell, NodeIndex, Node);
    }
}

/**
 * Convenience function for fetching the read-only raw native field data pointer
 * for the specified zone and variable of the current frame and wrapping it in a
 * raw value pointer item. Modifying values through this handle results in
 * undefined behavior.
 *
 * @param Zone
 *     Zone for which the raw variable data is desired.
 * @param Var
 *     Variable for which the raw variable data is desired.
 *
 * @return
 *     Raw value pointer item wrapping the read-only raw native field data pointer
 *     for the specified zone and variable.
 */
RawValuePtr_t RawValuePtrGetReadableNativePtr(
    EntIndex_t Zone,
    EntIndex_t Var);

/**
 * Convenience function for fetching the read/write raw native field data pointer
 * for the specified zone and variable of the current frame and wrapping it in a
 * raw value pointer item. If the original source is immutable Tecplot will
 * replace the original with a mutable copy.
 *
 * @param Zone
 *     Zone for which the raw variable data is desired.
 * @param Var
 *     Variable for which the raw variable data is desired.
 *
 * @return
 *     Raw value pointer item wrapping the read/write raw native field data pointer
 *     for the specified zone and variable.
 */
RawValuePtr_t RawValuePtrGetWritableNativePtr(
    EntIndex_t Zone,
    EntIndex_t Var);

/**
 * Convenience function for fetching the read-only raw derived field data pointer
 * for the specified zone and variable of the current frame and wrapping it in a
 * raw value pointer item. Modifying values through this handle results in
 * undefined behavior.
 *
 * @param Zone
 *     Zone for which the raw variable data is desired.
 * @param Var
 *     Variable for which the raw variable data is desired.
 *
 * @return
 *     Raw value pointer item wrapping the read-only raw derived field data pointer
 *     for the specified zone and variable.
 */
RawValuePtr_t RawValuePtrGetReadableDerivedPtr(
    EntIndex_t Zone,
    EntIndex_t Var);

/**
 * @deprecated
 *   Please use RawValuePtrGetReadableNativePtr() instead.
 */
RawValuePtr_t RawValuePtrGetReadablePtr(
    EntIndex_t Zone,
    EntIndex_t Var);

/**
 * @deprecated
 *   Please use RawValuePtrGetWritableNativePtr() instead.
 */
RawValuePtr_t RawValuePtrGetWritablePtr(
    EntIndex_t Zone,
    EntIndex_t Var);

/**
 * @deprecated
 *   Please use RawValuePtrGetReadablePtr() or
 *   RawValuePtrGetWritablePtr() instead.
 */
RawValuePtr_t RawValuePtrGetPtr(
    EntIndex_t Zone,
    EntIndex_t Var);

/**
 * Allocates the data value array of a raw value pointer item with the number
 * of values of the specified type.
 *
 * @param FieldDataType
 *     Data type of the data to allocate.
 * @param ValueLocation
 *     Value location of the data to allocate.
 * @param ValueCount
 *     Number of data values to allocate.
 *
 * @return
 *     Raw value pointer item wrapping the allocated data value array.
 */
RawValuePtr_t RawValuePtrAllocatePtr(
    FieldDataType_e FieldDataType,
    ValueLocation_e ValueLocation,
    LgIndex_t       ValueCount);

/**
 * Deallocates the data value array of the raw value pointer item and assigns
 * it to an invalid item.
 *
 * @param RawValuePtr
 *     Pointer to a raw value pointer item. Upon return RawValuePtr will be
 *     assigned an invalid raw value pointer item.
 */
void RawValuePtrDealloc(RawValuePtr_t* RawValuePtr);

/**
 * Convenience function for fetching a readable version of the node
 * map pointer for the specified zone of the current frame and wrapping
 * it in a raw node pointer item.
 *
 * @param zone
 *     Zone for which the raw node map is desired.
 * @param section
 *     Section number (one-based) for FEMixed zones, must be one for all other zone types.
 *
 * @return
 *     Readable raw node map pointer item wrapping the node map pointer
 *     for the specified zone.
 */
RawNodePtr_t RawNodePtrGetReadablePtr(
    EntIndex_t zone,
    EntIndex_t section);

/**
 * Convenience function for fetching a writable version of the node
 * map pointer for the specified zone of the current frame and wrapping
 * it in a raw node pointer item.
 *
 * @param Zone
 *     Zone for which the raw node map is desired.
 * @param section
 *     Section number (one-based) for FEMixed zones, must be one for all other zone types.
 *
 * @return
 *     Writable raw node map pointer item wrapping the node map pointer
 *     for the specified zone.
 */
RawNodePtr_t RawNodePtrGetWritablePtr(
    EntIndex_t zone,
    EntIndex_t section);

/**
 * Calculates the minimum and maximum values for the raw value pointer item.
 *
 * @param RawNodePtr
 *     Raw node pointer item to analyze.
 * @param MinVal
 *     Pointer to the resulting minimum data value.
 * @param MinVal
 *     Pointer to the resulting maximum data value.
 */
void RawValuePtrGetMinMax(
    RawValuePtr_t RawValuePtr,
    double*       MinVal,
    double*       MaxVal);
