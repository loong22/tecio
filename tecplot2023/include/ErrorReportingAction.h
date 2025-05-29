#pragma once

#include "ThirdPartyHeadersBegin.h"
    #include <functional>
    #include <memory>
    #include <string>
#include "ThirdPartyHeadersEnd.h"

#include "ClassMacros.h"
#include "TecUtil.h"
#include "tecutildataio_utils_Exports.h"

namespace tecplot::tecutildataio_utils {

/**
 * Simple class to encapsulate perform an injected loader action that throw exceptions. Any
 * exceptions are posted as an error message and a false return value is returned from the action.
 */
class tecutildataio_utils_API ErrorReportingAction
{
public:
    /**
     * Creates an error reporting action wrapper around the specified action to perform.
     * @param addOnID
     *     Add-on requesting the action.
     * @param dataFormatName
     *     Data format name used for constructing generic error messages when error details aren't
     *     available in the exception.
     * @param errorDetail
     *     Optional error message detail providing more information about what
     *     action was being performed on failure.
     * @param action
     *     Action to perform when then function call operator is executed.
     */
    ErrorReportingAction(
        AddOn_pa                     addOnID,
        std::string const&           dataFormatName,
        std::string const&           errorDetail,
        std::function<void()> const& action);

    /**
     */
    ~ErrorReportingAction() = default;

    /**
     * Executes action after first locking the loader and Tecplot, transcribing any exceptions into
     * a posted error message and a return value.
     * @return
     *     true if no exceptions occurred, false otherwise
     */
    bool operator()() const;

private:
    AddOn_pa const              m_addOnID;
    std::string const           m_dataFormatName;
    std::string const           m_errorDetail;
    std::function<void()> const m_action;
    UNCOPYABLE_CLASS(ErrorReportingAction);
};

}
