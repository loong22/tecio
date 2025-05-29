#pragma once

#include "ItemAddress.h"
#include "RawArray.h"
#include "tpsdkbase_Exports.h"

namespace tecplot { namespace szl {

/**
 * Provides a facility for batch loading and possibly other things in the future.
 */
class tpsdkbase_API LoaderInterface
{
public:
    /**
     * Notifies the loader that the variable value data from the subzones of each of the query
     * services are needed. It is important to checkout any subzone that need to be accessed even if
     * some are currently checked out by other threads otherwise it could be checked in while in
     * use by this thread.
     * @param varValueServices
     *     Variable value query services for which the subzone data are needed.
     * @param subzones
     *     Subzones for which the variable value data are needed.
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     * @throws tecplot::InterruptedException if the process was interrupted
     */
    virtual void checkout(
        tecplot::RawArray<class VarValueQueryInterface*> const& varValueServices,
        tecplot::RawArray<ItemAddress64::SubzoneAddress> const& subzones) = 0;

    /**
     * Notifies the loader that the variable value data from the subzones of each of the query
     * services are no longer needed.
     * @param varValueServices
     *     Variable value query services for which the subzone data are no longer needed.
     * @param subzones
     *     Subzones for which the variable value data are no longer needed.
     */
    virtual void checkin(
        tecplot::RawArray<class VarValueQueryInterface*> const& varValueServices,
        tecplot::RawArray<ItemAddress64::SubzoneAddress> const& subzones) = 0;

    virtual ~LoaderInterface(); // see implementation notes in exportedInterfaces.cpp
};

}}
