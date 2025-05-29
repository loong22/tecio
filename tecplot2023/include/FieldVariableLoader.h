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
 * Field data loader that is registered with Tecplot for loading variables on demand.
 */
class tecutildataio_utils_API FieldVariableLoader
{
public:
    ~FieldVariableLoader() = default;

    /**
     * Functor responsible for loading the field values into the dataBuffer. This functor is called
     * asynchronously. Tecplot locking is performed but it is the client's responsibility to acquire
     * a lock if synchronization is required.
     * @param filename
     *     Filename for the field variable given at registration.
     * @param dataType
     *     Datatype of the variable.
     * @param numValues
     *     Number of values that should be loaded into the databuffer.
     * @param dataBuffer
     *     Data buffer to receive numValues of dataType'ed values.
     * @throws std::bad_alloc if sufficient resource were not available to supply the values.
     * @throws tecplot::IOException if the file could not be opened for reading.
     * @throws tecplot::FileFormatException if while parsing the file for the values the format was found incorrect.
     */
    using LoadValues_t = std::function<
        void(boost::filesystem::path const& filename,
             FieldDataType_e                dataType,
             LgIndex_t                      numValues,
             void*                          dataBuffer)>;

    /**
     * Functor responsible for unloading the field variable resources. Subsequent load requests
     * can be made. This functor is called asynchronously. Tecplot locking is performed but it is
     * the client's responsibility to acquire a lock if synchronization is required.
     */
    using UnloadValues_t = std::function<void()>;

    /**
     * Functor responsible for getting a single field value at the specified offset. This functor is
     * used for field value load-on-demand. This functor is called asynchronously, and should be
     * written in such a way that no synchronization locking is required so that data access is high
     * performance.
     * @param valueOffset
     *     Zero based offset for the value of interest.
     * @return
     *     The field value at the specified offset.
     */
    using GetValue_t = std::function<double(LgIndex_t valueOffset)>;

    /**
     * Registers a field data loader strategy for the variable. This action is performed without
     * synchronization for the loader, therefore the loader must ensure that the operations
     * performed can be done asynchronously without creating a race condition or it must provide
     * appropriate synchronization as needed. See TecUtilDataValueCustomLOD() for additional details
     * about responsibilities of each function.
     * @param addOnID
     *     Add on identifier.
     * @param dataFormatName
     *     This short name describes the format for error messages. For example, "CONVERGE HDF5",
     *     "Exodus", "Plot3D", etc.
     * @param filename
     *     File containing the variable.
     * @param zone
     *     Tecplot zone to receive the field data loader.
     * @param var
     *     Tecplot variable to receive the field data loader.
     * @param loadValues
     *     Functor responsible for loading the specific data.
     * @param unloadValues
     *     Functor called when the field values are to be unloaded. The default is nullptr,
     *     indicating the client is not interested in being notified about unload requests.
     * @param getValue
     *     Functor called to get a specific field value from the variable. The default is nullptr,
     *     indicating that Tecplot is responsible for the storage of, and access to, the stored node
     *     map data. See TecUtilDataValueCustomValueLOD() for details.
     * @throws std::bad_alloc if sufficient resources were not available to register the service
     */
    static void registerStrategy(
        AddOn_pa                                       addOnID,
        std::shared_ptr<std::string const>             dataFormatName,
        std::shared_ptr<boost::filesystem::path const> filename,
        EntIndex_t                                     zone,
        EntIndex_t                                     var,
        LoadValues_t const&                            loadValues,
        UnloadValues_t const&                          unloadValues = nullptr,
        GetValue_t const&                              getValue = nullptr);

private:
    static Boolean_t loadAdapter(FieldData_pa fieldData);
    static Boolean_t unloadAdapter(FieldData_pa fieldData);
    static void cleanupAdapter(FieldData_pa fieldData);
    static double getValueAdapter(
        FieldData_pa fieldData,
        LgIndex_t  valueOffset);

    FieldVariableLoader(
        AddOn_pa                                       addOnID,
        std::shared_ptr<std::string const>             dataFormatName,
        std::shared_ptr<boost::filesystem::path const> filename,
        LoadValues_t const&                            loadValues,
        UnloadValues_t const&                          unloadValues,
        GetValue_t const&                              getValue);

    AddOn_pa const                                       m_addOnID;
    std::shared_ptr<std::string const> const             m_dataFormatName;
    std::shared_ptr<boost::filesystem::path const> const m_filename;
    LoadValues_t const                                   m_loadValues;
    UnloadValues_t const                                 m_unloadValues;
    GetValue_t const                                     m_getValue;

    UNCOPYABLE_CLASS(FieldVariableLoader);
};

}
