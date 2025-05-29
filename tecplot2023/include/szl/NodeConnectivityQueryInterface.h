#pragma once

#include "ItemAddress.h"
#include "RawArray.h"
#include "tpsdkbase_Exports.h"

namespace tecplot { namespace szl {

/**
 * SZL node connectivity query service interface registered with Tecplot. The query service is
 * responsible for loading the requested information or throwing an appropriate exception as
 * documented.
 */
class tpsdkbase_API NodeConnectivityQueryInterface
{
public:
    /**
     * Notifies the service prior to asking for any SZL node connectivity for this zone. This
     * method may never get called if the zone's connectivity is not needed. Once successfully
     * setup it is not called again unless a subsequent teardown() has occurred. Tecplot calls
     * setup() to notify the service of pending connectivity queries. After a successful call to
     * setup() the service is responsible for handling calls to checkout/in subzones, convert
     * addresses, and retrieve connectivity information. No other queries to this service are made
     * before a call to setup() with the exception of the destroy() method.
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     */
    virtual void setup() = 0;

    /**
     * Notifies the service when the SZL node connectivity for this zone is not currently needed.
     * This method will never get called if the service was not setup nor will it necessarily be
     * called before calling destroy(). Tecplot calls teardown() to notify the service that the
     * connectivity data is not currently needed and the service is free to return the state of the
     * service to the point prior to the call to setup().
     */
    virtual void teardown() = 0;

    /**
     * Notifies the service when the SZL node connectivity for this zone is no longer needed.
     * No other requests for node connectivity data are made for this zone after this call. Tecplot
     * can call destroy() at any time after registration to notify the service that the zone
     * managing the service is being destroyed.
     * @sa ~NodeConnectivityQueryInterface()
     */
    virtual void destroy() = 0;

    /**
     * Notifies the query service that the node subzones' connectivity and referenced cell subzone
     * indices need to be loaded. It is important to checkout any subzone that needs to be accessed
     * even if some are currently checked out otherwise it could be checked in by another thread
     * while still in use. Tecplot calls checkout() from code that has been made SZL addressing
     * aware.
     * @param nodeSubzones
     *     Node subzones for which the connectivity and referenced cell subzone indices are needed.
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     * @throws tecplot::InterruptedException if the process was interrupted
     */
    virtual void checkout(tecplot::RawArray<tecplot::ItemAddress64::SubzoneAddress> const& nodeSubzones) = 0;

    /**
     * Notifies the query service that the node subzones' connectivity and referenced cell subzone
     * indices are no longer needed. All subzones checked out with a call to checkout() must be
     * checked in with a symmetric call to checkin() when no longer needed. Tecplot calls checkin()
     * from code that has been made SZL addressing aware.
     * @param nodeSubzones
     *     Node subzones for which the connectivity and referenced cell subzone indices are no
     *     longer needed.
     */
    virtual void checkin(tecplot::RawArray<tecplot::ItemAddress64::SubzoneAddress> const& nodeSubzones) = 0;

    /**
     * Notifies the query service that the node connectivity data and referenced cell subzone
     * indices for all node subzones is needed. It is important to checkout any subzone that needs
     * to be accessed even if some other subzones are currently checked out otherwise they could be
     * checked in by another thread while still in use. Tecplot calls checkoutAll() from code that
     * is not SZL addressing aware or for one reason or another cannot utilize SZL addressing.
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     * @throws tecplot::InterruptedException if the process was interrupted
     */
    virtual void checkoutAll() = 0;

    /**
     * Notifies the query service that node connectivity data and referenced cell subzone indices of
     * all subzones checked out with checkoutAll() are no longer needed. Tecplot may not call
     * checkinAll() however if it doesn't, a call to checkoutAll() will not be made until a future
     * call to checkinAll() or teardown()/setup() is made. Tecplot calls checkinAll() from code that
     * is SZL addressing aware but for one reason or another cannot utilize SZL addressing.
     */
    virtual void checkinAll() = 0;

    /**
     * Translates the Sizzle address to a uniform address.
     * @param itemAddress
     *     Sizzle address needing translating.
     * @return
     *     Uniform address translation of the Sizzle address.
     * @pre itemAddress is a Sizzle address
     * @post result is a uniform address
     */
    virtual tecplot::ItemAddress64 toUniformAddress(tecplot::ItemAddress64 const& itemAddress) = 0;

    /**
     * Translates the uniform address to a Sizzle address.
     * @note
     *     Conversion from a uniform offset to a Sizzle address can be more expensive than from
     *     a Sizzle address to a uniform offset so use this method sparingly.
     * @param itemAddress
     *     Uniform item address needing translating to its Sizzle address.
     * @return
     *     Sizzle address translation of the uniform offset.
     * @post result is a Sizzle address
     */
    virtual tecplot::ItemAddress64 toSzlAddress(tecplot::ItemAddress64 const& itemAddress) = 0;

    /**
     * Returns the node subzone's referenced cell subzones.
     * @param nodeSubzone
     *     Node subzone for which with referenced cell subzones are needed.
     * @param cellSubzones
     *     An array into which the resulting cell subzones are placed.
     * @pre cellSubzones.empty()
     * @post cellSubzones is sorted
     * @throws std::bad_alloc if sufficient memory is not available
     */
    virtual void referencedCellSubzones(
        tecplot::ItemAddress64::SubzoneAddress const&              nodeSubzone,
        tecplot::RawArray<tecplot::ItemAddress64::SubzoneAddress>& cellSubzones) = 0;

    /**
     * Returns the number of node subzones comprising the zone.
     */
    virtual tecplot::ItemAddress64::SubzoneIndex_t numSubzones() = 0;

    /**
     * Returns the number of nodes that the specified node subzone contains. The node subzone's node
     * addresses can then be constructed from the node subzone index and a logic value between zero
     * and the size()-1.
     */
    virtual tecplot::ItemAddress64::ItemOffset_t size(tecplot::ItemAddress64::SubzoneAddress const& nodeSubzone) = 0;

protected:
    /**
     * Service provider destructor. Tecplot makes no assumptions about the service provider's class
     * structure (composition or multiple interface inheritance) and therefore does not call the
     * destructor. Consequently, the service provider is responsible for calling its own destructor
     * some time after Tecplot calls destroy(). Typically loaders implement and register a single
     * object instance for each zone's connectivity service, in which case the last instruction in
     * the destroy() method is to delete itself: "delete this;"
     */
    virtual ~NodeConnectivityQueryInterface(); // see implementation notes in exportedInterfaces.cpp
};

}}
