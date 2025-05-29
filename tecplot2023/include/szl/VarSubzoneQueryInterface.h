#pragma once

#include "ItemAddress.h"
#include "RawArray.h"
#include "tpsdkbase_Exports.h"

namespace tecplot { namespace szl {

/**
 * SZL zone/variable query service interface registered with Tecplot. The query service is
 * responsible for reporting the query results or throwing an appropriate exception as documented.
 */
class tpsdkbase_API VarSubzoneQueryInterface
{
public:
    /**
     * Notifies the service prior to making any queries for this zone's variable through this
     * interface. This method may never get called if the zone's variable is not needed. Once
     * successfully setup it is not called again unless a subsequent teardown() has occurred.
     * Tecplot calls setup() to notify the service of pending subzone queries. After a successful
     * call to setup() the service is responsible for handling calls to find node and cell subzones
     * and various min/max values.
     * No other queries to this service are made before a call to setup() with the exception of the
     * destroy() method.
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     */
    virtual void setup() = 0;

    /**
     * Notifies the service when queries for this zone's variable through this interface are not
     * currently needed. This method will never get called if the service was not setup nor will it
     * necessarily be called before calling destroy(). Tecplot calls teardown() to notify the
     * service that variable subzone queries are not currently needed and the service is free to
     * return the state of the service to the point prior to the call to setup().
     */
    virtual void teardown() = 0;

    /**
     * Notifies the service when queries for this zone's variable through this interface are no
     * longer needed. No other requests for variable subzone data are made for this zone's variable
     * after this call. Tecplot can call destroy() at any time after registration to notify the
     * service that the variable managing the service is being destroyed.
     * @sa ~VarSubzoneQueryInterface()
     */
    virtual void destroy() = 0;

    /**
     * Finds the cell subzones at the specified value.
     * @param value
     *     The value at which the subzones are desired.
     * @param cellSubzones
     *     An array into which the resulting cell subzones are placed.
     * @pre cellSubzones.empty()
     * @post cellSubzones is sorted
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     */
    virtual void findCellSubzones(
        double                                            value,
        tecplot::RawArray<ItemAddress64::SubzoneAddress>& cellSubzones) = 0;

    /**
     * Finds the node subzones within the specified value.
     * @param value
     *     The value at which the subzones are desired.
     * @param nodeSubzones
     *     An array into which the resulting node subzones are placed.
     * @pre nodeSubzones.empty()
     * @post nodeSubzones is sorted
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     */
    virtual void findNodeSubzones(
        double                                            value,
        tecplot::RawArray<ItemAddress64::SubzoneAddress>& nodeSubzones) = 0;

    /**
     * Returns all the zone partition min/max values for this variable.
     * @note
     *     The min and max values are stored as alternating pairs of values, one pair for each zone
     *     partition.
     * @param minMaxValues
     *     An array into which the resulting min/max values are placed.
     * @pre minMaxValues.empty()
     * @post minMaxValues.size() == 2*numPartitions
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     */
    virtual void partitionMinMaxes(tecplot::RawArray<double>& minMaxValues) = 0;

    /**
     * Returns all the cell subzone min/max values for this variable's zone partition.
     * @note
     *     The min and max values are stored as alternating pairs of values, one pair for each
     *     zone partition.
     * @param partition
     *     Zone partition for which the cell subzone min/max values are desired.
     * @param minMaxValues
     *     An array into which the resulting min/max values are placed.
     * @pre minMaxValues.empty()
     * @post minMaxValues.size() == 2*numSubzones
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     */
    virtual void cellSubzoneMinMaxes(
        tecplot::ItemAddress64::Partition_t partition,
        tecplot::RawArray<double>&          minMaxValues) = 0;

    /**
     * Returns all the node subzone min/max values for this variable's zone partition.
     * @note
     *     The min and max values are stored as alternating pairs of values, one pair for each
     *     zone partition.
     * @param partition
     *     Zone partition for which the node subzone min/max values are desired.
     * @param minMaxValues
     *     An array into which the resulting min/max values are placed.
     * @pre minMaxValues.empty()
     * @post minMaxValues.size() == 2*numSubzones
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     */
    virtual void nodeSubzoneMinMaxes(
        tecplot::ItemAddress64::Partition_t partition,
        tecplot::RawArray<double>&          minMaxValues) = 0;

protected:
    /**
     * Service provider destructor. Tecplot makes no assumptions about the service provider's class
     * structure (composition or multiple interface inheritance) and therefore does not call the
     * destructor. Consequently, the service provider is responsible for calling its own destructor
     * some time after Tecplot calls destroy(). Typically loaders implement and register a single
     * object instance for each variable subzone service, in which case the last instruction in the
     * destroy() method is to delete itself: "delete this;"
     */
    virtual ~VarSubzoneQueryInterface(); // see implementation notes in exportedInterfaces.cpp
};

}}
