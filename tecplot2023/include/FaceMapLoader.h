#pragma once

#include "ThirdPartyHeadersBegin.h"
    #include <boost/filesystem/path.hpp>
    #include <functional>
    #include <memory>
    #include <string>
#include "ThirdPartyHeadersEnd.h"

#include "ClassMacros.h"
#include "TecUtil.h"
#include "tecutildataio_utils_Exports.h"

namespace tecplot::tecutildataio_utils {

/**
 * Face map loader registered with Tecplot for loading polytope connectivity on demand.
 */
class tecutildataio_utils_API FaceMapLoader
{
public:
    ~FaceMapLoader() = default;

    /**
     * Functor responsible for loading the face map connectivity. This functor is called
     * asynchronously. Tecplot locking is performed but it is the client's responsibility to acquire
     * a lock if synchronization is required.
     * @param filename
     *     Filename for the connectivity given at registration.
     * @param faceMap
     *     Polytope face map to receive the connectivity.
     * @throws std::bad_alloc if sufficient resource were not available to supply the values.
     * @throws tecplot::IOException if the file could not be opened for reading.
     * @throws tecplot::FileFormatException if while parsing the file for the values the format was found incorrect.
     */
    using LoadConnectivity_t = std::function<void(
        boost::filesystem::path const& filename,
        FaceMap_pa                     faceMap)>;

    /**
     * Functor responsible for unloading the resources needed to load the face map. Subsequent load
     * requests can be made. This functor is called asynchronously. Tecplot locking is performed but
     * it is the client's responsibility to acquire a lock if synchronization is required.
     */
    using UnloadConnectivity_t = std::function<void()>;

    /**
     * Registers a connectivity loader strategy for the polytope connectivity. This action is
     * performed without synchronization for the loader, therefore the loader must ensure that the
     * operations performed can be done asynchronously without creating a race condition or it must
     * provide appropriate synchronization as needed. See TecUtilDataFaceMapCustomLOD() for
     * additional details about responsibilities of each function.
     * @param addOnID
     *     Add on identifier.
     * @param dataFormatName
     *     This short name describes the format for error messages. For example, "CONVERGE HDF5",
     *     "Exodus", "Plot3D", etc.
     * @param filename
     *     File containing the connectivity.
     * @param zone
     *     Tecplot zone to receive the connectivity loader.
     * @param numFaces
     *     Total number of unique faces. If this value is not known at the time of registration and
     *     you registered a deferred zone metrics loader with with TecUtilDataSetAddZoneX's
     *     SV_ZONEMETRICSLOADCALLBACK, it should be set to -1 and it can be deferred until the zone
     *     metrics callback is executed.
     * @param numFaceNodes
     *     Total number of nodes for the unique faces. This is not the number of unique nodes which
     *     was given when the zone was added. For example if a face map defined two triangle
     *     polygons that share a common face, numFaces would be 5 and numFaceNodes would be 5*2=10,
     *     not 4. If this value is not known at the time of registration and you registered a
     *     deferred zone metrics loader with TecUtilDataSetAddZoneX's SV_ZONEMETRICSLOADCALLBACK, it
     *     should be set to -1 and it can be deferred until the zone metrics callback is executed.
     * @param numFaceBndryFaces
     *     Total number of faces boundary faces or zero if there are no connections. If this value
     *     is not known at the time of registration and you registered a deferred zone metrics
     *     loader with TecUtilDataSetAddZoneX's SV_ZONEMETRICSLOADCALLBACK, it should be set to -1
     *     and it can be deferred until the zone metrics callback is executed.
     * @param numFaceBndryConns
     *     Total number of boundary connected elements or element/zone pairs or zero if there are no
     *     connections. If this value is not known at the time of registration and you registered a
     *     deferred zone metrics loader with TecUtilDataSetAddZoneX's SV_ZONEMETRICSLOADCALLBACK, it
     *     should be set to -1 and can be deferred until the zone metrics callback is executed.
     * @param loadConnectivity
     *     Functor called when the face map connectivity is to be loaded.
     * @param unloadConnectivity
     *     Functor called when the face map connectivity is to be unloaded.
     * @throws std::bad_alloc if sufficient resources were not available to register the service
     */
    static void registerStrategy(
        AddOn_pa                                       addOnID,
        std::shared_ptr<std::string const>             dataFormatName,
        std::shared_ptr<boost::filesystem::path const> filename,
        EntIndex_t                                     zone,
        LgIndex_t                                      numFaces,
        LgIndex_t                                      numFaceNodes,
        LgIndex_t                                      numFaceBndryFaces,
        LgIndex_t                                      numFaceBndryConns,
        LoadConnectivity_t const&                      loadConnectivity,
        UnloadConnectivity_t const&                    unloadConnectivity);

private:
    static Boolean_t loadConnectivityAdapter(FaceMap_pa faceMap);
    static Boolean_t unloadConnectivityAdapter(FaceMap_pa faceMap);
    static void cleanupConnectivityAdapter(FaceMap_pa faceMap);

    FaceMapLoader(
        AddOn_pa                                       addOnID,
        std::shared_ptr<std::string const>             dataFormatName,
        std::shared_ptr<boost::filesystem::path const> filename,
        LoadConnectivity_t const&                      loadConnectivity,
        UnloadConnectivity_t const&                    unloadConnectivity);

    AddOn_pa const                                       m_addOnID;
    std::shared_ptr<std::string const> const             m_dataFormatName;
    std::shared_ptr<boost::filesystem::path const> const m_filename;
    LoadConnectivity_t const                             m_loadConnectivity;
    UnloadConnectivity_t const                           m_unloadConnectivity;

    UNCOPYABLE_CLASS(FaceMapLoader);
};

}
