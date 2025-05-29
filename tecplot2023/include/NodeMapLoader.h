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
 * Node map loader registered with Tecplot for loading classic FE connectivity on demand.
 */
class tecutildataio_utils_API NodeMapLoader
{
public:
    ~NodeMapLoader() = default;

    /**
     * Functor responsible for loading the element node values into the FE Classic zone nodemap's
     * raw dataBuffer. This functor is called asynchronously. Tecplot locking is performed but it is
     * the client's responsibility to acquire a lock if synchronization is required.
     * @param filename
     *     Filename for the connectivity given at registration.
     * @param offsetDataType
     *     Specifies the word size for the node values to be stored in dataBuffer.
     * @param dataBuffer
     *     Raw data buffer to receive the dataType'ed values.
     * @throws std::bad_alloc if sufficient resource were not available to supply the values.
     * @throws tecplot::IOException if the file could not be opened for reading.
     * @throws tecplot::FileFormatException if while parsing the file for the values the format was found incorrect.
     */
    using LoadFEClassicValues_t = std::function<
        void(boost::filesystem::path const& filename,
             OffsetDataType_e               offsetDataType,
             void*                          dataBuffer)>;

    /**
     * Functor responsible for loading the element node values into the FE Mixed zone's nodemap.
     * This functor is called asynchronously. Tecplot locking is performed but it is the client's
     * responsibility to acquire a lock if synchronization is required.
     * @param filename
     *     Filename for the connectivity given at registration.
     * @param nodeMap
     *     Nodemap to receive the connectivity for the FE Mixed zone.
     * @throws std::bad_alloc if sufficient resource were not available to supply the values.
     * @throws tecplot::IOException if the file could not be opened for reading.
     * @throws tecplot::FileFormatException if while parsing the file for the values the format was found incorrect.
     */
    using LoadFEMixedValues_t = std::function<
        void(boost::filesystem::path const& filename,
             NodeMap_pa                     nodeMap)>;

    /**
     * Functor responsible for unloading the element node map resources. Subsequent load requests
     * can be made. This functor is called asynchronously. Tecplot locking is performed but it is
     * the client's responsibility to acquire a lock if synchronization is required.
     */
    using UnloadValues_t = std::function<void()>;

    /**
     * Functor responsible for getting a single element node value at the specified offset. This
     * functor is used for node map value load-on-demand. This functor is called asynchronously,
     * and should be written in such a way that no synchronization locking is required so that data
     * access is high performance.
     * @param nodeOffset
     *     Zero based offset for the nodal value of interest.
     * @return
     *     Zero based nodal value at the specified offset.
     */
    using GetValue_t = std::function<NodeMap_t(LgIndex_t nodeOffset)>;

    /**
     * Registers a connectivity loader strategy for the classic FE connectivity. This action is
     * performed without synchronization for the loader, therefore the loader must ensure that the
     * operations performed can be done asynchronously without creating a race condition or it must
     * provide appropriate synchronization as needed. See TecUtilDataNodeCustomLOD() for additional
     * details about responsibilities of each function.
     * @param addOnID
     *     Add on identifier.
     * @param dataFormatName
     *     This short name describes the format for error messages. For example, "CONVERGE HDF5",
     *     "Exodus", "Plot3D", etc.
     * @param filename
     *     File containing the connectivity.
     * @param zone
     *     Tecplot zone to receive the connectivity loader.
     * @param loadValues
     *     Functor called when the node map values are to be loaded.
     * @param unloadValues
     *     Functor called when the node map values are to be unloaded.
     * @throws std::bad_alloc if sufficient resources were not available to register the service
     */
    static void registerFEClassicStrategy(
        AddOn_pa                                       addOnID,
        std::shared_ptr<std::string const>             dataFormatName,
        std::shared_ptr<boost::filesystem::path const> filename,
        EntIndex_t                                     zone,
        LoadFEClassicValues_t const&                   loadValues,
        UnloadValues_t const&                          unloadValues);

    /**
     * Registers a connectivity loader strategy for the FE Mixed connectivity. This action is
     * performed without synchronization for the loader, therefore the loader must ensure that the
     * operations performed can be done asynchronously without creating a race condition or it must
     * provide appropriate synchronization as needed. See TecUtilDataNodeCustomLOD() for additional
     * details about responsibilities of each function.
     * @param addOnID
     *     Add on identifier.
     * @param dataFormatName
     *     This short name describes the format for error messages. For example, "CONVERGE HDF5",
     *     "Exodus", "Plot3D", etc.
     * @param filename
     *     File containing the connectivity.
     * @param zone
     *     Tecplot zone to receive the connectivity loader.
     * @param loadValues
     *     Functor called when the node map values are to be loaded.
     * @param unloadValues
     *     Functor called when the node map values are to be unloaded.
     * @throws std::bad_alloc if sufficient resources were not available to register the service
     */
    static void registerFEMixedStrategy(
        AddOn_pa                                       addOnID,
        std::shared_ptr<std::string const>             dataFormatName,
        std::shared_ptr<boost::filesystem::path const> filename,
        EntIndex_t                                     zone,
        LoadFEMixedValues_t const&                     loadValues,
        UnloadValues_t const&                          unloadValues);

private:
    static Boolean_t loadValuesAdapter(NodeMap_pa nodeMap);
    static Boolean_t unloadValuesAdapter(NodeMap_pa nodeMap);
    static void cleanupValuesAdapter(NodeMap_pa nodeMap);

    NodeMapLoader(
        AddOn_pa                                       addOnID,
        std::shared_ptr<std::string const>             dataFormatName,
        std::shared_ptr<boost::filesystem::path const> filename,
        LoadFEClassicValues_t const&                   loadFEClassicValues,
        LoadFEMixedValues_t const&                     loadFEMixedValues,
        UnloadValues_t const&                          unloadValues);

    AddOn_pa const                                       m_addOnID;
    std::shared_ptr<std::string const> const             m_dataFormatName;
    std::shared_ptr<boost::filesystem::path const> const m_filename;
    LoadFEClassicValues_t const                          m_loadFEClassicValues;
    LoadFEMixedValues_t const                            m_loadFEMixedValues;
    UnloadValues_t const                                 m_unloadValues;

    UNCOPYABLE_CLASS(NodeMapLoader);
};

}
