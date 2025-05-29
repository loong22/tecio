#pragma once

#include "ItemAddress.h"
#include "RawArray.h"
#include "tpsdkbase_Exports.h"

namespace tecplot { namespace szl {

/**
 * SZL ordered zone query service interface registered with Tecplot. The query service is
 * responsible for reporting the query results or throwing an appropriate exception as documented.
 */
class tpsdkbase_API OrderedSubzoneQueryInterface
{
public:
    /**
     * Notifies the service prior to making any queries for this zone through this interface. This
     * method may never get called if the zone is not needed. Once successfully setup it is not
     * called again unless a subsequent teardown() has occurred. Tecplot calls setup() to notify the
     * service of pending subzone queries. After a successful call to setup() the service is
     * responsible for handling calls to find node and cell subzones. No other queries to this
     * service are made before a call to setup() with the exception of the destroy() method.
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     */
    virtual void setup() = 0;

    /**
     * Notifies the service when queries for this zone through this interface are not currently
     * needed. This method will never get called if the service was not setup nor will it
     * necessarily be called before calling destroy(). Tecplot calls teardown() to notify the
     * service that subzone queries are not currently needed and the service is free to return the
     * state of the service to the point prior to the call to setup().
     */
    virtual void teardown() = 0;

    /**
     * Notifies the service when queries for this zone through this interface are no longer needed.
     * No other requests for subzone data are made for this zone after this call. Tecplot can call
     * destroy() at any time after registration to notify the service that the zone managing the
     * service is being destroyed.
     * @sa ~OrderedSubzoneQueryInterface()
     */
    virtual void destroy() = 0;

    /**
     * Finds the cell subzones for the specified ordered zone's I, J, or K plane.
     * @param ijkPlane
     *     Plane of interest. The values can be IJKPlanes_I, IJKPlanes_J, or IJKPlanes_K.
     * @param planeIndex
     *     I, J, or K plane index of interest.
     * @param cellSubzones
     *     A raw array into which the resulting cell subzones are placed.
     * @pre cellSubzones.empty()
     * @post cellSubzones is sorted
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     */
    virtual void findIJKPlaneCellSubzones(
        IJKPlanes_e                                       ijkPlane,
        LgIndex_t                                         planeIndex,
        tecplot::RawArray<ItemAddress64::SubzoneAddress>& cellSubzones) = 0;

    /**
     * Finds the cell subzones for the specified ordered zone's I, J, or K line.
     * @param ijkLine
     *     Line of interest. The values can be IJKLines_I, IJKLines_J, or IJKLines_K.
     * @param mIndex, nIndex
     *     The I, J, K cross dimensions of the IJK line of interest. In other words, for an I line
     *     mIndex is J and nIndex is K.
     * @param cellSubzones
     *     A raw array into which the resulting cell subzones are placed.
     * @pre cellSubzones.empty()
     * @post cellSubzones is sorted
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     */
    virtual void findIJKLineCellSubzones(
        IJKLines_e                                        ijkLine,
        LgIndex_t                                         mIndex,
        LgIndex_t                                         nIndex,
        tecplot::RawArray<ItemAddress64::SubzoneAddress>& cellSubzones) = 0;

    /**
     * Finds the node subzones for the specified ordered zone's I, J, or K plane.
     * @param ijkPlane
     *     Plane of interest. The values can be IJKPlanes_I, IJKPlanes_J, or IJKPlanes_K.
     * @param planeIndex
     *     I, J, or K plane index of interest.
     * @param nodeSubzones
     *     A raw array into which the resulting node subzones are placed.
     * @pre nodeSubzones.empty()
     * @post nodeSubzones is sorted
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     */
    virtual void findIJKPlaneNodeSubzones(
        IJKPlanes_e                                       ijkPlane,
        LgIndex_t                                         planeIndex,
        tecplot::RawArray<ItemAddress64::SubzoneAddress>& nodeSubzones) = 0;

    /**
     * Finds the node subzones for the specified ordered zone's I, J, or K line.
     * @param ijkLine
     *     Line of interest. The values can be IJKLines_I, IJKLines_J, or IJKLines_K.
     * @param mIndex, nIndex
     *     The I, J, K cross dimensions of the IJK line of interest. In other words, for an I line
     *     mIndex is J and nIndex is K.
     * @param nodeSubzones
     *     A raw array into which the resulting node subzones are placed.
     * @pre nodeSubzones.empty()
     * @post nodeSubzones is sorted
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     */
    virtual void findIJKLineNodeSubzones(
        IJKLines_e                                        ijkLine,
        LgIndex_t                                         mIndex,
        LgIndex_t                                         nIndex,
        tecplot::RawArray<ItemAddress64::SubzoneAddress>& nodeSubzones) = 0;

protected:
    /**
     * Service provider destructor. Tecplot makes no assumptions about the service provider's class
     * structure (composition or multiple interface inheritance) and therefore does not call the
     * destructor. Consequently, the service provider is responsible for calling its own destructor
     * some time after Tecplot calls destroy(). Typically loaders implement and register a single
     * object instance for each subzone service, in which case the last instruction in the destroy()
     * method is to delete itself: "delete this;"
     */
    virtual ~OrderedSubzoneQueryInterface(); // see implementation notes in exportedInterfaces.cpp
};

}}
