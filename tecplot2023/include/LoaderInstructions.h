#pragma once

#include "ThirdPartyHeadersBegin.h"
    #include <boost/filesystem/path.hpp>
    #include <map>
    #include <memory>
    #include <string>
    #include <vector>
#include "ThirdPartyHeadersEnd.h"

#include "ClassMacros.h"
#include "InvalidInstructionsException.h"
#include "StringList.h"
#include "TecUtil.h"
#include "tecutildataio_utils_Exports.h"


namespace tecplot::tecutildataio_utils {

auto const trueTokens  = std::vector<std::string> {{ "True",  "T", "Yes", "Y" }};
auto const falseTokens = std::vector<std::string> {{ "False", "F", "No",  "N" }};

/**
 * Convenience function for parsing an argument value as a Boolean.
 * @param addonName
 *     Name of the add-on used for error messaging.
 * @param argValue
 *     Argument value expected to be a Boolean.
 * @return
 *     true or false, depending on the Boolean argument value.
 * @throws
 *     tecplot::InvalidInstructionsException if the argument value is not a recognized
 *     Boolean value.
 */
tecutildataio_utils_API bool parseBoolean(
    std::string const& addonName,
    std::string const& argValue);

/**
 * Convenience function for retrieving the valid enumeration options from the mapping as a comma
 * separated string.
 * @tparam E
 *     Enumeration
 * @param enumNameToValue
 *     Mapping of valid enum names to their values.
 */
template <typename E>
std::string validEnumOptions(std::map<std::string,E> const& enumNameToValue)
{
    std::string validOptions;
    std::for_each(std::begin(enumNameToValue), std::end(enumNameToValue),
                  [&validOptions](auto const& option) {
                      if (!validOptions.empty())
                          validOptions.append(", ");
                      validOptions.append(option.first);
                  });
    return validOptions;
}

/**
 * Convenience function for parsing an argument value as an enum.
 * @param addonName
 *     Name of the add-on used for error messaging.
 * @param argValue
 *     Argument value expected to be a enumeration
 * @param enumNameToValue
 *     Mapping of valid enum names to their values.
 * @return
 *     The enumeration value of the argument.
 * @throws
 *     tecplot::InvalidInstructionsException if the argument value is not a recognized
 *     enumeration value.
 */
template <typename E>
E parseEnum(
    std::string const&             addonName,
    std::string const&             argValue,
    std::map<std::string,E> const& enumNameToValue)
{
    auto const enumValue = enumNameToValue.find(argValue);
    if (enumValue == std::end(enumNameToValue))
        throw tecutilutil::InvalidInstructionsException(
            addonName,
            "Unrecognized enum value, "+argValue+"."
            "\nValid enum values: "+validEnumOptions(enumNameToValue));

    return enumValue->second;
}

/**
 * Convenience function for converting a string list of filename to a vector of shared filesystem
 * paths. The paths are constructed in their native OS preferred representation, even if they are
 * received in a generic format, so that error messaging is sensible on the target platform. In
 * other words if C:/Users/Blah is passed on Windows it will be stored as C:\Users\Blah.
 * @param filenames
 *     Filenames list.
 * @return
 *     Vector of shared pointers to native OS preferred file path representations of the filenames
 *     in the list.
 */
tecutildataio_utils_API std::vector<std::shared_ptr<boost::filesystem::path const>> to_filesystem_paths(
    tecplot::toolbox::StringList const& filenames);

/**
 * Holds onto the instructions while setting up the callbacks.
 */
class tecutildataio_utils_API LoaderInstructions
{
public:
    /**
     * For parsing and serializing advanced instructions, inject an instance of the advanced
     * argument processors to the LoaderInstructions constructor.
     */
    class AdvancedArgProcessor
    {
    public:
        /**
         * Parses a single advanced loader option and returns the index to the next argument name
         * in the string list.
         * @param instructions
         *     Complete loader instruction list.
         * @param argNameBaseIndex
         *     Base index of the argument name for the name/value(s) pair within the instruction list.
         * @return
         *     Base index of the argument name for the next name/value(s) pair, or one past the end
         *     of the list if there are no more instructions.
         * @throws std::bad_alloc if there are insufficient resources
         * @throws InvalidInstructionsException
         *     if the instruction name is not a recognized advanced loader option or the loader
         *     option's value is not formatted correctly
         */
        virtual LgIndex_t parseArg(
            tecplot::toolbox::StringList const& instructions,
            LgIndex_t                           argNameBaseIndex) = 0;

        /**
         * Serializes advanced arguments to the existing string list instructions.
         * @param instructions
         *     String list of loader instructions. The input instructions have the standard syntax
         *     and file names already added. The advanced loader instructions should be appended to
         *     the existing basic loader instructions.
         * @throws std::bad_alloc if there are insufficient resources
         */
        virtual void serializeArgs(tecplot::toolbox::StringList& instructions) const = 0;

        virtual ~AdvancedArgProcessor() = default;
    };

    /**
     * Constructs a loader instructions instance from the instruction string list, typically handed
     * to the loader via the registered load callback. Advanced argument parsing is performed by an
     * optionally injected advanced argument processor instance.
     * @param dataLoaderName
     *     Data loader name owning the instructions.
     * @param instructions
     *     Loader instructions, typically handed to the loader from Tecplot.
     * @param advancedArgProcessor
     *     Optional argument processor responsible for parsing and serializing the advanced loader
     *     options.
     * @throws std::bad_alloc if there are insufficient resources
     * @throws InvalidInstructionsException if instructions do not match the specification
     */
    LoaderInstructions(
        std::string const&                    dataLoaderName,
        tecplot::toolbox::StringList const&   instructions,
        std::shared_ptr<AdvancedArgProcessor> advancedArgProcessor = nullptr);

    /**
     * Constructs a loader instructions instance with the basic instructions and any advanced
     * options via an optionally injected advanced argument processor instance.
     * @param dataLoaderName
     *     Data loader name owning the instructions.
     * @param filenames
     *     One or more filename being processed by the loader.
     * @param advancedArgProcessor
     *     Optional argument processor responsible for parsing and serializing the advanced loader
     *     options.
     * @throws std::bad_alloc if there are insufficient resources
     */
    LoaderInstructions(
        std::string const&                                                 dataLoaderName,
        std::vector<std::shared_ptr<boost::filesystem::path const>> const& filenames,
        std::shared_ptr<AdvancedArgProcessor>                              advancedArgProcessor = nullptr);

    /**
     * Loader instructions are movable but not copyable.
     */
    LoaderInstructions(LoaderInstructions&&) = default;
    LoaderInstructions(LoaderInstructions const&) = delete;

    /**
     * Serializes the instructions so that they can be registered with Tecplot in the journal.
     * @return
     *     String list of serialized instructions conforming to the name/value(s) pair loader
     *     argument standard syntax.
     * @throws std::bad_alloc if there are insufficient resources
     */
    tecplot::toolbox::StringList serializeInstructions() const;

    /**
     * Returns the file names managed by the loader instructions.
     * @throws std::bad_alloc if there are insufficient resources
     */
    std::vector<std::shared_ptr<boost::filesystem::path const>> const& filenames() const;

    /**
     * Assigns a new set of filenames to the loader instructions. Loaders will use this API to
     * re-order the filenames, etc.
     * @param filenames
     *     One or more filename being processed by the loader.
     */
    void setFilenames(std::vector<std::shared_ptr<boost::filesystem::path const>> const& filenames);

private:
    std::string const                                           m_dataLoaderName;
    std::vector<std::shared_ptr<boost::filesystem::path const>> m_filenames;
    std::shared_ptr<AdvancedArgProcessor> const                 m_advancedArgProcessor;
};

}
