#pragma once

#include "ThirdPartyHeadersBegin.h"
    #include <algorithm>
    #include <functional>
    #include <memory>
    #include <optional>
    #include <string>
    #include <unordered_map>
    #include <utility>
    #include <vector>
    #include <boost/filesystem/path.hpp>
#include "ThirdPartyHeadersEnd.h"

#include "TecUtil.h"
#include "tecutildataio_utils_Exports.h"

namespace tecplot::toolbox {
class ArgList;
class StringList;
}

namespace tecplot::ui {
class TaskProgress;
}

namespace tecplot::tecutildataio_utils {

class LoaderInstructions;

/**
 * Functor responsible for registering the zone load-on-demand strategies and other
 * miscellaneous zone data with the dataset.
 * @param registrationProgress
 *     Progress monitor.
 */
using ZoneDataRegistrar_t = std::function<
    void(std::unique_ptr<tecplot::ui::TaskProgress> registrationProgress)>;

/**
 * Functor responsible for creating the dataset and registering the zone and variable
 * load-on-demand strategies and other miscellaneous zone data with the dataset.
 */
using DataSetCreator_t = std::function<void()>;

/**
 * Functor responsible for getting the specific auxiliary data context.
 */
using GetAuxDataRef_t = std::function<AuxData_pa()>;

/**
 * Composes a standard error message given the message to display.
 */
tecutildataio_utils_API std::string composeErrorMessage(std::string const& message);

/**
 * Adds the initial plot type and auto strand assignment options to the V3 data argument list that
 * is handed off to TecUtilDataLoadFinishX() by the client.
 * @param dataLoadArgList
 *     Data loader argument list instructions for TecUtilDataLoadFinishX().
 * @param initialPlotType
 *     Preferred initial plot type. Default is PlotType_Automatic as is 360's data loader.
 * @param assignStrandIDs
 *     Indicates if Tecplot should assign strand ID's to those pending assignment. Default is TRUE
 *     as is 360's data loader.
 * @param addZonesToExistingStrands
 *     Indicates if Tecplot should add zones to existing strands with matching strand ID's. Default
 *     is FALSE as is 360's data loader.
 * @return
 *     true if the options were successfully added to the argument list, false otherwise
 */
tecutildataio_utils_API bool addV3LoaderSelectedOptions(
    tecplot::toolbox::ArgList& dataLoadArgList,
    PlotType_e                 initialPlotType = PlotType_Automatic,
    Boolean_t                  assignStrandIDs = TRUE,
    Boolean_t                  addZonesToExistingStrands = FALSE);

/**
 * Adds the initial plot type and auto strand assignment options to the V3 data argument list that
 * is handed off to TecUtilDataLoadFinishX() by the client.
 * @param dataLoadArgList
 *     Data loader argument list instructions for TecUtilDataLoadFinishX().
 * @param initialPlotType
 *     Preferred initial plot type. Default is PlotType_Automatic as is 360's data loader.
 * @param assignStrandIDs
 *     Indicates if Tecplot should assign strand ID's to those pending assignment. Default is TRUE
 *     as is 360's data loader.
 * @param addZonesToExistingStrands
 *     Indicates if Tecplot should add zones to existing strands with matching strand ID's. Default
 *     is FALSE as is 360's data loader.
 * @return
 *     true if the options were successfully added to the argument list, false otherwise
 */
tecutildataio_utils_API bool addV3LoaderSelectedOptions(
    ArgList_pa dataLoadArgList,
    PlotType_e initialPlotType = PlotType_Automatic,
    Boolean_t  assignStrandIDs = TRUE,
    Boolean_t  addZonesToExistingStrands = FALSE);

/**
 * Constructs the Tecplot dataset and registers the zone data. This is a helper function used by the
 * dataset creator functor passed to registerLoaderData(). This is mostly generic for all V3 loaders
 * that perform load-on-demand.
 * @param addOnName
 *     Name of the add-on.
 * @param dataFormatName
 *     This short name describes the format for error messages. For example, "CONVERGE HDF5",
 *     "Exodus", "Plot3D", etc.
 * @param tecplotVarNames
 *     String list of Tecplot variable names to register with the dataset.
 * @param createDataSetProgress
 *     Progress monitor.
 * @throws
 *     std::bad_alloc if sufficient resources are not available
 *     FileFormatException if unable to read the Exodus file
 */
tecutildataio_utils_API void registerDataset(
    std::string const&                         addOnName,
    std::string const&                         dataFormatName,
    tecplot::toolbox::StringList const&        tecplotVarNames,
    std::unique_ptr<tecplot::ui::TaskProgress> createDataSetProgress,
    ZoneDataRegistrar_t                        zoneDataRegistrar);

/**
 * Orchestrates the registering of a data loader that performs load-on-demand. This is mostly
 * generic for all V3 loaders that perform load-on-demand.
 * @param addOnID
 *     Add-on identifier.
 * @param addOnName
 *     Name of the add-on.
 * @param dataFormatName
 *     This short name describes the format for error messages. For example, "CONVERGE HDF5",
 *     "Exodus", "Plot3D", etc.
 * @param instructions
 *     Loader instructions.
 * @param invokedViaLoaderSelectedCallback
 *     Boolean Indicating if the registration is via the loader selected callback.
 * @param dataSetCreator
 *     Functor responsible for creating a dataset and registering the zone data and loader
 *     instructions with Tecplot. It is intended that the registerDataset() function be used
 *     to reduce code duplication.
 * @return
 *     true if the data loader was successfully registered, false otherwise
 */
tecutildataio_utils_API bool registerV3LoaderData(
    AddOn_pa                    addOnID,
    std::string const&          addOnName,
    std::string const&          dataFormatName,
    LoaderInstructions const&   instructions,
    bool                        invokedViaLoaderSelectedCallback,
    DataSetCreator_t const&     dataSetCreator);

/**
 * Indicates if the name is a valid auxiliary data name.
 * @param name
 *     Proposed auxiliary data name.
 * @return
 *     true if valid, false otherwise
 */
tecutildataio_utils_API bool isValidAuxDataName(std::string const& name);

/**
 * Each variable name in the supplied vector is searched for in the vector containing a union of the
 * variable names in a case insensitive manner. Variable names not found in the union are added to
 * the end of the union, preserving their original case.
 * @param varNames
 *     Vector of variable names to add to the union if not already present.
 * @param varNamesUnion
 *     Vector of variable names currently found as input, and a potentially larger set of variables
 *     that come from varNames as output if they were not already present in the input vector. New
 *     variables are added to the end of the union vector.
 */
tecutildataio_utils_API void addNewVariableMembers(
    std::vector<std::string> const& varNames,
    std::vector<std::string>&       varNamesUnion);

/**
 * Generalization for fetching a cached resource. If the resource is available in the cache it
 * returns it immediately, otherwise it creates the resource, caches it, and returns it.
 * @tparam KEY_TYPE
 *     Key type used for fetching the resource.
 * @tparam RESOURCE_TYPE
 *     Resource type to fetch.
 * @param resourceKey
 *     Key of the resource to fetch.
 * @param resourceCache
 *     A mapping of cached resources.
 * @param createResource
 *     Factory function for creating the resource if it isn't found in the cache.
 * @return
 *     Shared pointer to the resource either retrieved from the cache or created and inserted into
 *     the cache.
 */
template <typename KEY_TYPE, typename RESOURCE_TYPE>
std::shared_ptr<RESOURCE_TYPE> fetchResource(
    KEY_TYPE                                                       resourceKey,
    std::unordered_map<KEY_TYPE,std::weak_ptr<RESOURCE_TYPE>>&     resourceCache,
    std::function<std::shared_ptr<RESOURCE_TYPE>(KEY_TYPE)> const& createResource)
{
    auto& cachedResource = resourceCache[resourceKey];
    auto resource = cachedResource.lock();
    if (!resource)
    {
        resource = createResource(resourceKey);
        cachedResource = resource; // assign the shared pointer back to the weak reference for other's to acquire
    }
    return resource;
}

/**
 * Apply the sort permutation to the items and return the items in the permuted
 * ordered. The original values can be replaced quite easily with a call to
 * std::vector's swap() method as follows:
 *     applySortPermutation(myItems,sortedOffsets).swap(myItems);
 *
 * @param items
 *     Items to sort based on the permutation offsets.
 * @param permutationOffsets
 *     Offsets that define the sorted order, who's rule is defined elsewhere and
 *     was used to build the permutation offsets.
 * @return
 *     Items ordered by the permutation offsets.
 */
template <typename T>
std::vector<T> applySortPermutation(
    std::vector<T> const& items,
    std::vector<size_t>   permutationOffsets)
{
    REQUIRE(items.size() == permutationOffsets.size());

    std::vector<T> sortedItems;
    sortedItems.reserve(items.size());
    for (auto const& offset : permutationOffsets)
        sortedItems.push_back(items[offset]);

    return sortedItems;
}

/**
 * Convenience function covering TecUtilExtractTimesFromFileNames(). See the TecUtil documentation
 * for details.
 * @param filenames
 *     Vector of filename paths from which solution times, if embedded in the filenames should be
 *     extracted.
 * @return
 *     A pair of vectors. The first vector are the sorted solution times corresponding to the
 *     solution time embedded in the filenames and the second is the sorted permutation array used
 *     to sort this and other related vectors of information.
 */
tecutildataio_utils_API std::pair<std::vector<double>,std::vector<size_t>>  extractSolutionTimes(
    std::vector<std::shared_ptr<boost::filesystem::path const>> const& filenames);

/**
 * Gets the auxiliary data value and retention flag for the auxiliary data context returned from the
 * getAuxDataRef function.
 * @param name
 *     Auxiliary data name.
 * @param getAuxDataRef
 *     Returns the auxiliary data reference for the zone, dataset, layout, etc.
 * @return
 *     If the auxiliary data exists, return an optional value containing a pair, otherwise return an
 *     empty optional. The optional value contains a pair, where the first item in the pair is the
 *     auxiliary data string value, and the second item is its Boolean retention value.
 * @throws
 *     std::bad_alloc if sufficient resources are not available.
 */
tecutildataio_utils_API std::optional<std::pair<std::string,bool>> getAuxData(
    std::string const&     name,
    GetAuxDataRef_t const& getAuxDataRef);

/**
 * Adds the name/value auxiliary data for the auxiliary data context returned from the getAuxDataRef
 * function.
 * @param name
 *     Auxiliary data name.
 * @param value
 *     Auxiliary data value string.
 * @param getAuxDataRef
 *     Returns the auxiliary data reference for the zone, dataset, layout, etc.
 * @throws
 *     std::bad_alloc if sufficient resources are not available.
 */
tecutildataio_utils_API void addAuxData(
    std::string const&     name,
    std::string const&     value,
    GetAuxDataRef_t const& getAuxDataRef);

/**
 * Fetches the boundary connect zone's unique ID from the vector of Tecplot zone unique IDs for the
 * ones-based zone number. The unique ID's vector begins at the base Tecplot zone offset specified
 * in the parameter list. If a unique ID for the zone does not exist in the vector the unique IDs
 * for the zones between the last one in the vector to the desired zone are created.
 *
 * @param zone
 *     Ones-based zone number for which the unique ID is desired or TECUTIL_NO_NEIGHBORING_ZONE
 *     which indicates "self". When passing self, return zoneUniqueId. Note that only classic face
 *     neighbors have the concept of self referencing for global face neighbors. Polytope use a
 *     different mechanism.
 * @param baseTecplotZoneOffset
 *     The base zone at which the unique ID vector begins. The @ref zone number must be larger than
 *     the base TecplotZoneOffset.
 * @param zoneUniqueId
 *     The unique ID to use if zone is TECUTIL_NO_NEIGHBORING_ZONE, or self.
 * @param tecplotZoneUniqueIds
 *     Vector of unique IDs for the zones beginning at zone baseTecplotZoneOffset+1. If the zone in
 *     question is not found, the vector is expanded and the missing zone unique IDs are appended to
 *     the vector.
 * @throws std::bad_alloc if sufficient resources are not available to fulfill the request
 */
tecutildataio_utils_API UniqueID_t uniqueIdForBndryZone(
    EntIndex_t               zone,
    EntIndex_t               baseTecplotZoneOffset,
    UniqueID_t               zoneUniqueId,
    std::vector<UniqueID_t>& tecplotZoneUniqueIds);

}
