#pragma once

#include "ThirdPartyHeadersBegin.h"
    #include <boost/filesystem/path.hpp>
    #include <functional>
    #include <memory>
    #include <string>
    #include <vector>
#include "ThirdPartyHeadersEnd.h"

#include "ClassMacros.h"
#include "TecUtil.h"
#include "tecutildataio_utils_Exports.h"

namespace tecplot::toolbox {
class ArgList;
}

namespace tecplot::tecutildataio_utils {

/**
 * Zone metrics loader that is registered with Tecplot for loading zone metrics on demand.
 */
class tecutildataio_utils_API ZoneMetricsLoader
{
public:
    ~ZoneMetricsLoader() = default;

    /**
     * Functor responsible for loading the deferred metrics for the zone with which it was
     * registered. This is an optional functor registered with the TecUtilDataSetAddZoneX() if one
     * or more pieces of the zone metrics are not known at the time the zone is added to the
     * dataset. The functor must do whatever work is necessary to fill in the remaining metrics that
     * are relevant to the zone type. For ordered zones only the number of I, J, and K points are
     * relevant. For classic FE zones only the number of I and J points are relevant, and for
     * polytope zone all six values are potentially relevant.
     *
     * This functor may never be called by Tecplot if the metrics for the zone with which it is
     * registered are never requested. At most it will be called a single time for the zone.
     *
     * @param numIPts
     *     Pointer to the number of I points in the zone. The input value will either be -1 or a
     *     positive value greater than zero. A value of -1 indicates that the number of I points was
     *     not known at the time of registration and the functor is responsible for setting the
     *     value upon a successful return from this functor. A positive value indicates that the
     *     zone was registered with a specified number of I points and it must remain unchanged. For
     *     ordered data this is simply the number of points in the I dimension. For classic FE and
     *     polytope data this is the number of unique nodes in the zone.
     * @param numJPts
     *     Pointer to the number of J points in the zone. The input value will either be -1 or a
     *     positive value greater than zero. A value of -1 indicates that the number of J points was
     *     not known at the time of registration and the functor is responsible for setting the
     *     value upon a successful return from this functor. A positive value indicates that the
     *     zone was registered with a specified number of J points and it must remain unchanged. For
     *     ordered data this is simply the number of points in the J dimension. For classic FE and
     *     polytope data this is the number of elements in the zone.
     * @param numKPts
     *     Pointer to the number of K points in the zone. The input value will either be -1 or a
     *     positive value. A value of -1 indicates that the number of K points was not known at the
     *     time of registration and the functor is responsible for setting the value upon a
     *     successful return from this functor. A positive value indicates that the zone was
     *     registered with a specified number of K points, or it was determined based on its classic
     *     FE zone type, and it must remain unchanged. For ordered data this is simply the number of
     *     points in the K dimension. For classic FE data this is the number of nodes per element
     *     and it is set by Tecplot based on the zone type. For polytope data this is the number of
     *     unique faces in the zone.
     * @param numFaceNodes
     *     Pointer to the total number of face nodes for the unique faces in the polytope zone or
     *     nullptr for all other zone types. For polytope zones the input value will either be -1 or
     *     a positive value. A value of -1 indicates that the number of number face nodes for the
     *     unique faces was not known at the time of registration and the functor is responsible for
     *     setting the value upon a successful return from this functor. A positive value indicates
     *     that the zone was registered with a specified number of face nodes for the unique faces
     *     and it must remain unchanged.
     * @param numFaceBndryFaces
     *     Pointer to the total number of boundary faces in the polytope zone or nullptr for all
     *     other zone types. For polytope zones the input value will either be -1 or a positive
     *     value. A value of -1 indicates that the number of number boundary faces was not known at
     *     the time of registration and the functor is responsible for setting the value upon a
     *     successful return from this functor. A positive value indicates that the zone was
     *     registered with a specified number of boundary face and it must remain unchanged.
     * @param numFaceBndryConns
     *     Pointer to the total number of boundary connected elements or element/zone pairs in the
     *     polytope or nullptr for all other zone types. For polytope zones the input value will
     *     either be -1 or a positive value. A value of -1 indicates that the number of number
     *     boundary faces was not known at the time of registration and the functor is responsible
     *     for setting the value upon a successful return from this functor. A positive value
     *     indicates that the zone was registered with a specified number of boundary face
     *     connections and it must remain unchanged.
     *
     * For example, if a polygonal zone is comprised of two triangle polygons that share a common
     * face, numIPts would be 4, numJPts would be 2, numKPts would be 5, and numFaceNodes would be
     * 5*2=10, not 4.
     *
     * @throws std::bad_alloc if sufficient resource were not available to supply the values.
     * @throws tecplot::IOException if the file could not be opened for reading.
     * @throws tecplot::FileFormatException if while parsing the file for the values the format was found incorrect.
     * @throws tecplot::InterruptedException if the process was interrupted.
     */
    using LoadMetrics_t = std::function<void(ZoneMetrics_pa zoneMetrics)>;

    /**
     * Registers a zone metrics loader strategy with the argument list that will be passed by the
     * client to TecUtilDataSetAddZoneX(). This action is performed without synchronization for the
     * loader, therefore the loader must ensure that the operations performed can be done
     * asynchronously without creating a race condition or it must provide appropriate
     * synchronization as needed.
     * @param addOnID
     *     Add on identifier.
     * @param dataFormatName
     *     This short name describes the format for error messages. For example, "CONVERGE HDF5",
     *     "Exodus", "Plot3D", etc.
     * @param filename
     *     File containing the zone metrics.
     * @param zoneDimension
     *     Zone dimension: 1 for a linear zone, 2 for a surface zone, or 3 for a volume zone.
     * @param loadValues
     *     Functor called when the node map values are to be loaded.
     * @param unloadValues
     *     Functor called when the node map values are to be unloaded. The default is nullptr,
     *     indicating the client is not interested in being notified about unload requests.
     * @param argList
     *     A partially populated argument list to which the registration of the zone metrics loader
     *     are assigned.
     * @throws std::bad_alloc if sufficient resources were not available to register the service
     */
    static void registerStrategy(
        AddOn_pa                                       addOnID,
        std::shared_ptr<std::string const>             dataFormatName,
        std::shared_ptr<boost::filesystem::path const> filename,
        int32_t                                        zoneDimension,
        LoadMetrics_t const&                           loadMetrics,
        tecplot::toolbox::ArgList&                     argList);

    /**
     * Registers a zone metrics loader strategy with the argument list that will be passed by the
     * client to TecUtilDataSetAddZoneX(). This action is performed without synchronization for the
     * loader, therefore the loader must ensure that the operations performed can be done
     * asynchronously without creating a race condition or it must provide appropriate
     * synchronization as needed.
     * @param addOnID
     *     Add on identifier.
     * @param dataFormatName
     *     This short name describes the format for error messages. For example, "CONVERGE HDF5",
     *     "Exodus", "Plot3D", etc.
     * @param filename
     *     File containing the zone metrics.
     * @param zoneDimension
     *     Zone dimension: 1 for a linear zone, 2 for a surface zone, or 3 for a volume zone.
     * @param loadValues
     *     Functor called when the node map values are to be loaded.
     * @param unloadValues
     *     Functor called when the node map values are to be unloaded. The default is nullptr,
     *     indicating the client is not interested in being notified about unload requests.
     * @param argList
     *     A partially populated argument list to which the registration of the zone metrics loader
     *     are assigned.
     * @throws std::bad_alloc if sufficient resources were not available to register the service
     */
    static void registerStrategy(
        AddOn_pa                                       addOnID,
        std::shared_ptr<std::string const>             dataFormatName,
        std::shared_ptr<boost::filesystem::path const> filename,
        int32_t                                        zoneDimension,
        LoadMetrics_t const&                           loadMetrics,
        ArgList_pa                                     argList);

private:
    static Boolean_t loadMetricsAdapter(
        ArbParam_t clientData,
        ZoneMetrics_pa zoneMetrics);
    static void cleanupMetricsAdapter(ArbParam_t clientData);

    ZoneMetricsLoader(
        AddOn_pa                                       addOnID,
        std::shared_ptr<std::string const>             dataFormatName,
        std::shared_ptr<boost::filesystem::path const> filename,
        LoadMetrics_t const&                           loadMetrics);

    AddOn_pa const                                       m_addOnID;
    std::shared_ptr<std::string const> const             m_dataFormatName;
    std::shared_ptr<boost::filesystem::path const> const m_filename;
    LoadMetrics_t const                                  m_loadMetrics;

    UNCOPYABLE_CLASS(ZoneMetricsLoader);
};

}
