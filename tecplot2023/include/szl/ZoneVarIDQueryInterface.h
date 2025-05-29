#pragma once

#include "ThirdPartyHeadersBegin.h"
    #include <memory>
    #include <string>
#include "ThirdPartyHeadersEnd.h"

#include "ItemAddress.h"
#include "RawArray.h"
#include "tpsdkbase_Exports.h"

namespace tecplot { namespace eqn {
class ExprInterface;
}}

namespace tecplot { namespace szl {

/**
 * SZL zone variable ID query service interface registered with Tecplot. The query service is
 * responsible for creating calculated variables and other miscellaneous tasks for a zone and a set
 * of variable IDs.
 */
class tpsdkbase_API ZoneVarIDQueryInterface
{
public:
    /**
     * Notifies the service when it is no longer needed. After this call no other requests for
     * calculated variables are made for this zone. Tecplot can call destroy() at any time after
     * registration to notify the service that the zone managing the service is being destroyed.
     * @sa ~ZoneVarIDQueryInterface()
     */
    virtual void destroy() = 0;

    /**
     * Create a duplicate of the zone variable ID query service for use in a new Tecplot zone. This
     * is typically performed when the Tecplot Engine duplicates a zone, initially sharing all
     * variables and the connectivity.
     * @return
     *     Duplicate of the zone variable ID query ready to be used as a new zone service beginning
     *     as a duplicate of this zone.
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     */
    virtual ZoneVarIDQueryInterface* duplicate() = 0;

    /**
     * Create the variable query services for the calculated variable.
     * @param varID
     *     Unique ID for the variable. It is the same across all zones of the dataset.
     * @param nativeValueLocation
     *     The desired native value location for the calculated variable.
     * @param fieldDataType
     *     The desired type for the calculated variable.
     * @param expression
     *     Calculated variable expression.
     * @param cellCenteredVarQuery
     *     Resulting SZL cell centered variable data query service responsible for loading the
     *     requested information for specified subzones. See VarValueQueryInterface's setup(),
     *     teardown(), and destroy() methods for life cycle details.
     * @param nodalVarQuery
     *     Resulting SZL nodal variable data query service responsible for loading the requested
     *     information for specified subzones. See VarValueQueryInterface's setup(), teardown(), and
     *     destroy() methods for life cycle details.
     * @param varSubzoneQuery
     *     Resulting SZL query service responsible for performing various queries for the zone's
     *     variable.  See VarQueryInterface's setup(), teardown(), and destroy() methods for life
     *     cycle details.
     * @throws std::bad_alloc if sufficient memory is not available
     */
    virtual void createCalcVar(
        UniqueID_t                          varID,
        ValueLocation_e                     nativeValueLocation,
        FieldDataType_e                     fieldDataType,
        std::shared_ptr<eqn::ExprInterface> expression,
        class VarValueQueryInterface*&      cellCenteredVarQuery,
        class VarValueQueryInterface*&      nodalVarQuery,
        class VarSubzoneQueryInterface*&    varSubzoneQuery) = 0;

    /**
     * Indicates if the variable identified by the unique ID exists in the loader's dataset.
     * @param varID
     *     Unique ID for the variable. It is the same across all zones of the dataset.
     * @return
     *     true if the variable exists, false otherwise
     */
    virtual bool varExists(UniqueID_t varID) = 0;

    /**
     * Find the cell subzones whose min/maxes encompass all the variable values.
     *
     * @param varValues
     *     An array of variable values corresponding to the supplied array of variable IDs.
     * @param varIDs
     *     An array of variable unique IDs corresponding to the supplied array of variable values.
     * @param cellSubzones
     *     An empty, reallocable array capable of receiving the intersection of the subzones returned
     *     from each value/query pair.
     * @pre varValues.size() == varIDs.size()
     * @pre cellSubzones.empty()
     * @post cellSubzones is sorted
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     */
    virtual void findCellSubzones(
        tecplot::RawArray<double> const&                           varValues,
        tecplot::RawArray<UniqueID_t> const&                       varIDs,
        tecplot::RawArray<tecplot::ItemAddress64::SubzoneAddress>& cellSubzones) = 0;

    /**
     * Find the node subzones whose min/maxes encompass all the variable values.
     *
     * @param varValues
     *     An array of variable values corresponding to the supplied array of variable IDs.
     * @param varIDs
     *     An array of variable unique IDs corresponding to the supplied array of variable values.
     * @param nodeSubzones
     *     An empty, reallocable array capable of receiving the intersection of the subzones returned
     *     from each value/query pair.
     * @pre varValues.size() == varIDs.size()
     * @pre nodeSubzones.empty()
     * @post nodeSubzones is sorted
     * @throws std::bad_alloc if sufficient memory is not available
     * @throws tecplot::IOException if an I/O error occurs
     */
    virtual void findNodeSubzones(
        tecplot::RawArray<double> const&                           varValues,
        tecplot::RawArray<UniqueID_t> const&                       varIDs,
        tecplot::RawArray<tecplot::ItemAddress64::SubzoneAddress>& nodeSubzones) = 0;

protected:
    /**
     * Service provider destructor. Tecplot makes no assumptions about the service provider's class
     * structure (composition or multiple interface inheritance) and therefore does not call the
     * destructor. Consequently, the service provider is responsible for calling its own destructor
     * some time after Tecplot calls destroy(). Typically loaders implement and register a single
     * object instance for each zone, in which case the last instruction in the destroy() method is
     * to delete itself: "delete this;"
     */
    virtual ~ZoneVarIDQueryInterface(); // see implementation notes in exportedInterfaces.cpp
};

}}
