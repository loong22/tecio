#pragma once

#include "ItemAddress.h"
#include "RawArray.h"
#include "tpsdkbase_Exports.h"

namespace tecplot { namespace szl {

/**
 * SZL variable data query service interface registered with Tecplot. The query service is
 * responsible for loading the requested information or throwing an appropriate exception as
 * documented.
 */
class tpsdkbase_API VarValueQueryInterface
{
public:
    /**
     * Notifies the service prior to asking for any SZL data for this zone's variable. This
     * method may never get called if the zone's variable data is not needed. Once successfully
     * setup it is not called again unless a subsequent teardown() has occurred. Tecplot calls
     * setup() to notify the service of pending queries about the variable and its data. After a
     * successful call to setup() the service is responsible for handling calls to checkout/in
     * subzones, convert addresses, and retrieve field data information. No other queries to this
     * service are made before a call to setup() with the exception of the destroy(), loader(),
     * isCalculated(), and valueRange() methods.
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     */
    virtual void setup() = 0;

    /**
     * Notifies the service when the SZL data for this zone's variable is not currently needed.
     * This method will never get called if the service was not setup nor will it necessarily be
     * called before calling destroy(). Tecplot calls teardown() to notify the service that the
     * variable data is not currently needed and the service is free to return the state of the
     * service to the point prior to the call to setup().
     */
    virtual void teardown() = 0;

    /**
     * Notifies the service when the SZL data for this zone's variable is no longer needed.
     * No other queries to this interface are made after this call. Tecplot can call destroy() at
     * any time after registration to notify the service that the variable managing the service is
     * being destroyed.
     * @sa ~VarValueQueryInterface()
     */
    virtual void destroy() = 0;

    /**
     * Returns a reference to the loader that created this query service. Tecplot can call loader()
     * at any time after registration and before destroy() is called to acquire the interface for
     * communicating with the loader.
     * @return
     *     Reference to the loader responsible for creating this query service. The interface is
     *     used to group batch requests for dispatching to the correct loader.
     */
    virtual class LoaderInterface& loader() = 0;

    /**
     * Indicates if the variable services is calculated. This method may be called at any time after
     * registration.
     * @return
     *     true if this service is calculated, otherwise false
     */
    virtual bool isCalculated() = 0;

    /**
     * Returns the variable's min/max value range. Tecplot can call valueRange() at any time after
     * the service has been registered and before destroy() is called.
     * @note:
     *     This method does not throw any exceptions so the expectation is that the values are
     *     available without performing any I/O operations.
     * @param minValue, maxValue
     *     Minimum and maximum values for the variable.
     * @post minValue <= maxValue
     */
    virtual void valueRange(
        double& minValue,
        double& maxValue) = 0;

    /**
     * Notifies the query service that the subzones' variable data are needed. It is important to
     * checkout any subzone that needs to be accessed even if some are currently checked out
     * otherwise it could be checked in by another thread while still in use. Tecplot calls
     * checkout() from code that has been made SZL addressing aware.
     * @param subzones
     *     Subzones for which the variable data are needed.
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     * @throws tecplot::InterruptedException if the process was interrupted
     */
    virtual void checkout(tecplot::RawArray<tecplot::ItemAddress64::SubzoneAddress> const& subzones) = 0;

    /**
     * Notifies the query service that the subzones' variable data are no longer needed. All
     * subzones checked out with a call to checkout() must be checked in with a symmetric call to
     * checkin() when no longer needed. Tecplot calls checkin() from code that has been made SZL
     * addressing aware.
     * @param subzones
     *     Subzones for which the variable data are no longer needed.
     */
    virtual void checkin(tecplot::RawArray<tecplot::ItemAddress64::SubzoneAddress> const& subzones) = 0;

    /**
     * Notifies the query service that the variable data for all subzones is needed. It is important
     * to checkout any subzone that needs to be accessed even if some other subzones are currently
     * checked out otherwise they could be checked in by another thread while still in use. Tecplot
     * calls checkoutAll() from code that is not SZL addressing aware or for one reason or another
     * cannot utilize SZL addressing.
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     * @throws tecplot::InterruptedException if the process was interrupted
     */
    virtual void checkoutAll() = 0;

    /**
     * Notifies the query service that variable data of all subzones checked out with checkoutAll()
     * are no longer needed. Tecplot may not call checkinAll() however if it doesn't, a call to
     * checkoutAll() will not be made until a future call to checkinAll() or teardown()/setup() is
     * made. Tecplot calls checkinAll() from code that is SZL addressing aware but for one reason
     * or another cannot utilize SZL addressing.
     */
    virtual void checkinAll() = 0;

    /**
     * Notifies the query service that all the variable's data is needed in a uniform addressable
     * format. The service is responsible for assigning its values into the pre-allocated uniform
     * target field data.
     * @see TecUtilDataValueSetByRef(), TecUtilDataValueArraySetByRef(),
     *      TecUtilDataValueRefGetSetFunc()
     * @param targetFD
     *     Pointer to the field data to receive the variable data.
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     * @throws tecplot::InterruptedException if the process was interrupted
     */
    virtual void copyToUniform(struct FieldData_s* targetFD) = 0;

    /**
     * Returns the field value for the item using direct access to the values maintained by the
     * addressed item's subzone.
     * @param itemAddress
     *     Index/offset address within the subzone or uniform address for which the field value
     *     is desired.
     * @return
     *     Field value at the specified item address.
     */
    virtual double fieldValue(tecplot::ItemAddress64 const& itemAddress) = 0;

    /**
     * Returns the field values for the items using direct access to the values maintained by the
     * addressed items' subzones.
     * @param itemAddresses
     *     Array of index/offset addresses within the subzones or uniform addresses for which the
     *     field values are desired.
     * @param values
     *     Array into which the resulting field values for the specified item addresses are placed.
     * @throws std::bad_alloc if sufficient memory is not available
     * @pre !itemAddresses.empty()
     * @pre values.empty();
     * @post values.size() == itemAddresses.size()
     */
    virtual void fieldValues(
        tecplot::RawArray<tecplot::ItemAddress64> const& itemAddresses,
        tecplot::RawArray<double>&                       values) = 0;

protected:
    /**
     * Service provider destructor. Tecplot makes no assumptions about the service provider's class
     * structure (composition or multiple interface inheritance) and therefore does not call the
     * destructor. Consequently, the service provider is responsible for calling its own destructor
     * some time after Tecplot calls destroy(). Typically loaders implement and register a single
     * object instance for each zone/var service, in which case the last instruction in the
     * destroy() method is to delete itself: "delete this;"
     */
    virtual ~VarValueQueryInterface(); // see implementation notes in exportedInterfaces.cpp
};

}}
