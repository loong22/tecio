#pragma once

#include "ItemAddress.h"
#include "RawArray.h"
#include "tpsdkbase_Exports.h"

namespace tecplot { namespace szl {

/**
 * SZL cell connectivity query service interface registered with Tecplot. The query service is
 * responsible for loading the requested information or throwing an appropriate exception as
 * documented.
 */
class tpsdkbase_API CellConnectivityQueryInterface
{
public:
    /**
     * Type for indexing the nodes of a cell.
     */
    using CellNodeIndex_t = int32_t;

    /**
     * Type for indexing the faces of a cell.
     */
    using FaceIndex_t = int32_t;

    /**
     * Notifies the service prior to asking for any SZL cell connectivity for this zone. This
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
     * Notifies the service when the SZL cell connectivity for this zone is not currently needed.
     * This method will never get called if the service was not setup nor will it necessarily be
     * called before calling destroy(). Tecplot calls teardown() to notify the service that the
     * connectivity data is not currently needed and the service is free to return the state of the
     * service to the point prior to the call to setup().
     */
    virtual void teardown() = 0;

    /**
     * Notifies the service when the SZL cell connectivity for this zone is no longer needed.
     * After this call no other requests for cell connectivity data are made for this zone. Tecplot
     * can call destroy() at any time after registration to notify the service that the zone
     * managing the service is being destroyed.
     * @sa ~CellConnectivityQueryInterface()
     */
    virtual void destroy() = 0;

    /**
     * Notifies the query service that the cell subzones' cell definitions and, if requested, the
     * user defined face neighbor connectivity need to be loaded. It is important to checkout any
     * subzone that needs to be accessed even if some are currently checked out otherwise it could
     * be checked in by another thread while still in use. Tecplot calls checkout() from code that
     * has been made subzone addressing aware.
     * @param includeFaceNeighbors
     *     Indicates if the user defined face neighbors should be loaded along with the cell
     *     definitions.
     * @param cellSubzones
     *     Cell subzones for which the connectivity and referenced node subzone indices are needed.
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     * @throws tecplot::InterruptedException if the process was interrupted
     */
    virtual void checkout(
        bool                                                             includeFaceNeighbors,
        tecplot::RawArray<tecplot::ItemAddress64::SubzoneAddress> const& cellSubzones) = 0;

    /**
     * Notifies the query service that the cell subzones' connectivity and referenced node subzone
     * indices are no longer needed. All subzones checked out with a call to checkout() must be
     * checked in with a symmetric call to checkin() when no longer needed. Tecplot calls checkin()
     * from code that has been made SZL addressing aware.
     * @param cellSubzones
     *     Cell subzones for which the connectivity and referenced node subzone indices are no
     *     longer needed.
     */
    virtual void checkin(tecplot::RawArray<tecplot::ItemAddress64::SubzoneAddress> const& cellSubzones) = 0;

    /**
     * Notifies the query service that the cell definitions and, if requested, the user defined face
     * neighbor connectivity for all cell subzones is needed. It is important to checkout any
     * subzone that needs to be accessed even if some other subzones are currently checked out
     * otherwise they could be checked in by another thread while still in use. Tecplot calls
     * checkoutAll() from code that is not subzone addressing aware or for one reason or another
     * cannot utilize subzone addressing.
     * @param includeFaceNeighbors
     *     Indicates if the user defined face neighbors should be loaded along with the cell
     *     definitions.
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     * @throws tecplot::InterruptedException if the process was interrupted
     */
    virtual void checkoutAll(bool includeFaceNeighbors) = 0;

    /**
     * Notifies the query service that cell connectivity data and referenced node subzone indices of
     * all subzones checked out with checkoutAll() are no longer needed. Tecplot may not call
     * checkinAll() however if it doesn't, a call to checkoutAll() will not be made until a future
     * call to checkinAll() or teardown()/setup() is made. Tecplot calls checkinAll() from code that
     * is SZL addressing aware but for one reason or another cannot utilize SZL addressing.
     */
    virtual void checkinAll() = 0;

    /**
     * Notifies the query service that all the cell's cell-to-node definition data is needed in a
     * uniform addressable format. The service is responsible for assigning its values into the
     * pre-allocated uniform target cell-to-node map.
     * @see TecUtilDataNodeSetByRef(), TecUtilDataNodeArraySetByRef(), TecUtilDataNodeArraySetByRef64(),
     * @param targetNM
     *     Pointer to the cell-to-node map to receive the cells' cell-to-node definition data.
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     * @throws tecplot::InterruptedException if the process was interrupted
     */
    virtual void copyToUniform(class ElemToNodeMap* targetNM) = 0;

    /**
     * Translates the Sizzle address to a uniform address.
     * @param itemAddress
     *     Sizzle address needing translating.
     * @return
     *     Uniform offset translation of the Sizzle address.
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
     * Returns the cell subzone's referenced node subzones.
     * @param cellSubzone
     *     Cell subzone for which with referenced node subzones are needed.
     * @param nodeSubzones
     *     An array into which the resulting node subzones are placed.
     * @pre nodeSubzones.empty()
     * @post nodeSubzones is sorted
     * @throws std::bad_alloc if sufficient memory is not available
     */
    virtual void referencedNodeSubzones(
        tecplot::ItemAddress64::SubzoneAddress const&              cellSubzone,
        tecplot::RawArray<tecplot::ItemAddress64::SubzoneAddress>& nodeSubzones) = 0;

    /**
     * Returns the number of cell subzones comprising the zone.
     */
    virtual tecplot::ItemAddress64::SubzoneIndex_t numSubzones() = 0;

    /**
     * Returns the number of cells that the specified cell subzone contains. The cell subzone's cell
     * addresses can then be constructed from the cell subzone index and a logic value between zero
     * and the size()-1.
     */
    virtual tecplot::ItemAddress64::ItemOffset_t size(tecplot::ItemAddress64::SubzoneAddress const& cellSubzone) = 0;

    /**
     * Returns the number of corner nodes for the specified cell. Within a subzone, all cells have
     * the same number of corner nodes.
     * @param
     *     Index/offset address within the cell subzone or a uniform address for which the corner
     *     node count is needed.
     * @return
     *     Number of corner nodes for each cell in the cell's subzone.
     */
    virtual size_t cornerNodeCount(tecplot::ItemAddress64 const& cellAddress) = 0;

    /**
     * Returns the number of nodes for the specified cell. Within a subzone, all cells have the same
     * number of nodes.
     * @param
     *     Index/offset address within the cell subzone or a uniform address for which the node
     *     count is needed.
     * @return
     *     Number of nodes for each cell in the cell's subzone.
     */
    virtual size_t nodeCount(tecplot::ItemAddress64 const& cellAddress) = 0;

    /**
     * Returns the number of faces for the specified cell. Within a subzone, all cells have the same
     * number of faces.
     * @param
     *     Index/offset address within the cell subzone or a uniform address for which the node
     *     count is needed.
     * @return
     *     Number of faces for each cell in the cell's subzone.
     */
    virtual size_t faceCount(tecplot::ItemAddress64 const& cellAddress) = 0;

    /**
     * Returns the corner node addresses for the cell using direct access to the connectivity
     * maintained by the addressed cell subzone. The corner node addresses can then be used for
     * direct access to the node values.
     * @param cellAddress
     *     Index/offset address within the cell subzone or a uniform address for which the corner
     *     node addresses are desired.
     * @param nodeAddresses
     *     Array into which the resulting cell's corner node addresses are placed. The addressing of
     *     the nodes returned is the same as that of the cellAddress.
     * @pre nodeAddresses.empty()
     * @throws std::bad_alloc if sufficient memory is not available
     */
    virtual void cornerNodeAddresses(
        tecplot::ItemAddress64 const&              cellAddress,
        tecplot::RawArray<tecplot::ItemAddress64>& nodeAddresses) = 0;

    /**
     * Returns the node address for the cell corner using direct access to the connectivity
     * maintained by the addressed cell subzone. The corner node address can then be used for direct
     * access to the node value.
     * @param cellAddress
     *     Index/offset address within the cell subzone or a uniform address for which the corner
     *     node address is desired.
     * @param cellNodeIndex
     *     Indicates which corner node within the cell is desired.
     * @return
     *     Address of the specified cell's corner node. The addressing of the node returned is the
     *     same as that of the cellAddress.
     */
    virtual tecplot::ItemAddress64 cornerNodeAddress(
        tecplot::ItemAddress64 const& cellAddress,
        CellNodeIndex_t               cellNodeIndex) = 0;

    /**
     * Returns the natural node addresses for the cell using direct access to the connectivity
     * maintained by the addressed cell subzone. The natural node addresses can then be used for
     * direct access to the node values.
     * @param cellAddress
     *     Index/offset address within the cell subzone or a uniform address for which the natural
     *     node addresses are desired.
     * @param nodeAddresses
     *     Array into which the resulting cell's natural node addresses are placed. The addressing
     *     of the nodes returned is the same as that of the cellAddress.
     * @pre nodeAddresses.empty()
     * @throws std::bad_alloc if sufficient memory is not available
     */
    virtual void nodeAddresses(
        tecplot::ItemAddress64 const&              cellAddress,
        tecplot::RawArray<tecplot::ItemAddress64>& nodeAddresses) = 0;

    /**
     * Returns the node address for the natural node using direct access to the connectivity
     * maintained by the addressed cell subzone. The natural node address can then be used for
     * direct access to the node value.
     * @param cellAddress
     *     Index/offset address within the cell subzone or a uniform address for which the node
     *     address is desired.
     * @param cellNodeIndex
     *     Indicates which natural node within the cell is desired.
     * @return
     *     Address of the specified cell's natural node. The addressing of the node returned is the
     *     same as that of the cellAddress.
     */
    virtual tecplot::ItemAddress64 nodeAddress(
        tecplot::ItemAddress64 const& cellAddress,
        CellNodeIndex_t               cellNodeIndex) = 0;

    /**
     * If not already up-to-date, generate the cell subzones' internal face neighbors. Internal face
     * neighbors are those that share common face nodes within a face node. Connections between
     * subzones, zones, and zone boundaries are not updated by this request. If a subzone contains
     * user defined face neighbors they are assigned when checked out and are not modified when
     * generating internal face neighbors.
     * @param cellSubzones
     *     Cell subzones needing their face neighbors up-to-date.
     * @pre cellSubzones have been checked out
     * @throws std::bad_alloc if sufficient memory is not available
     */
    virtual void updateInternalFaceNeighbors(tecplot::RawArray<tecplot::ItemAddress64::SubzoneAddress> const& cellSubzones) = 0;

    /**
     * Returns the cell addresses that are neighbors of the cell's face. If the neighbor is not yet
     * know, as is the case initially at a subzone boundary, a single uniform address,
     * UNKNOWN_FACE_NEIGHBOR, is returned. For cell faces with definitive boundaries, a single
     * uniform address, NO_NEIGHBORING_ELEMENT, is returned. If the neighbors are cells, non-uniform
     * cell addresses are returned. The zone neighbors array contains the Tecplot zone neighbors
     * associated with each returned cell face neighbor or NO_NEIGHBORING_ZONE indicating that a
     * cell face isn't connected to another zone. The loader is responsible for mapping between its
     * zone numbering and Tecplot's, therefore it must consider variable add/delete state changes
     * and update the mapping accordingly.
     * @param cellAddress
     *     Cell address for the face neighbor query.
     * @param cellFace
     *     Cell face of interest.
     * @param cellNeighbors
     *     Array of cell face neighbors containing the results of the query.
     * @param zoneNeighbors
     *     Array of zone neighbors containing the results of the query.
     * @param isFaceCompletelyObscured
     *     Indicates if the neighbors completely obscure the face.
     * @pre 0 <= cellFace < [cell type's max face count: 4 or 8]
     * @pre cellNeighbors.empty()
     * @pre zoneNeighbors.empty()
     * @post cellNeighbors.size() >= 1
     * @post cellNeighbors.size() == zoneNeighbors.size()
     * @post cellNeighbors contains UNKNOWN_FACE_NEIGHBOR  -> cellNeighbors.size() == 1
     * @post cellNeighbors contains NO_NEIGHBORING_ELEMENT -> cellNeighbors.size() == 1
     */
    virtual void faceNeighbors(
        tecplot::ItemAddress64 const&              cellAddress,
        FaceIndex_t                                cellFace,
        tecplot::RawArray<tecplot::ItemAddress64>& cellNeighbors,
        tecplot::RawArray<EntIndex_t>&             zoneNeighbors,
        bool&                                      isFaceCompletelyObscured) = 0;

    /**
     * Assigns the cell face neighbors. If the neighbor is not yet known, as is the case initially
     * at a subzone boundary, a single uniform address, UNKNOWN_FACE_NEIGHBOR, should be
     * assigned.  If the face is a definitive boundary, a single uniform address,
     * NO_NEIGHBORING_ELEMENT, should be assigned. If the neighbors are cells they should be
     * non-uniform cell addresses. The zone neighbors array contains the Tecplot zone neighbors
     * associated with each supplied cell face neighbor or NO_NEIGHBORING_ZONE indicating that a
     * cell face isn't connected to another zone. The loader is responsible for mapping between its
     * zone numbering and Tecplot's, therefore it must consider variable add/delete state changes
     * and update the mapping accordingly.
     * @param cellAddress
     *     Cell address for the face neighbor assignment.
     * @param cellFace
     *     Cell face to which the neighbors are assigned.
     * @param cellNeighbors
     *     Array of cell face neighbors to assign.
     * @param zoneNeighbors
     *     Array of zone neighbors to assign.
     * @param isFaceCompletelyObscured
     *     Indicates if the neighbors completely obscure the face.
     * @pre 0 <= cellFace < [cell type's max face count: 4 or 8]
     * @pre cellNeighbors.size() >= 1
     * @pre cellNeighbors.size() == zoneNeighbors.size()
     * @pre cellNeighbors contains UNKNOWN_FACE_NEIGHBOR -> cellNeighbors.size() == 1
     * @pre cellNeighbors contains NO_NEIGHBORING_ELEMENT -> cellNeighbors.size() == 1
     * @throws std::bad_alloc if sufficient memory is not available
     */
    virtual void setFaceNeighbors(
        tecplot::ItemAddress64 const&                    cellAddress,
        FaceIndex_t                                      cellFace,
        tecplot::RawArray<tecplot::ItemAddress64> const& cellNeighbors,
        tecplot::RawArray<EntIndex_t> const&             zoneNeighbors,
        bool                                             isFaceCompletelyObscured) = 0;

protected:
    /**
     * Service provider destructor. Tecplot makes no assumptions about the service provider's class
     * structure (composition or multiple interface inheritance) and therefore does not call the
     * destructor. Consequently, the service provider is responsible for calling its own destructor
     * some time after Tecplot calls destroy(). Typically loaders implement and register a single
     * object instance for each zone's connectivity service, in which case the last instruction in
     * the destroy() method is to delete itself: "delete this;"
     */
    virtual ~CellConnectivityQueryInterface(); // see implementation notes in exportedInterfaces.cpp
};

}}
