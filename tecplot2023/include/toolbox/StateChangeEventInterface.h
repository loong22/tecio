#pragma once

#if defined MSWIN
#pragma once
#endif

#include "ThirdPartyHeadersBegin.h"
#  include <string>
#include "ThirdPartyHeadersEnd.h"

#include "TecUtil.h"
#include "tptoolbox_Exports.h"

namespace tecplot { namespace toolbox {
    class Set;
    class StringList;
}}

namespace tecplot { namespace toolbox {

/**
 * StateChangeEventInterface provides methods for investigating information
 * regarding a state change event. StateChangeEventInterface is passed
 * to registered StateChangeListeners. A StateChangeListener must derive
 * from StateChangeListenerInterface and register with StateChangeNotifier
 * (or tecplot::sdk::integration::StateChangeEventObservable - SDK only).
 */
class tptoolbox_API StateChangeEventInterface
{
public:
    virtual ~StateChangeEventInterface() {}

    virtual const StateChangeEventInterface* clone() const = 0;

    /**
     * Check to see if the state change event has an
     * associated index value.
     */
    virtual bool hasIndex() const = 0;

    /**
     * Check to see if the state change event has an
     * associated variable set.
     */
    virtual bool hasVarSet() const = 0;

    /**
     * Check to see if the state change event has an
     * associated zone set.
     */
    virtual bool hasZoneSet() const = 0;

    /**
     * Check to see if the state change event has an
     * associated enumerated value.
     */
    virtual bool hasArbEnum() const = 0;

    /**
     * Check to see if the state change event has an
     * associated line map number.
     */
    virtual bool hasMap() const = 0;

    /**
     * Check to see if the state change event has an
     * associated zone number.
     */
    virtual bool hasZone() const = 0;

    /**
     * Check to see if the state change event has an
     * associated variable number.
     */
    virtual bool hasVar() const = 0;

    /**
     * Check to see if the state change event has an
     * associated name.
     */
    virtual bool hasName() const = 0;

    /**
     * Check to see if the state change event has an
     * associated page unique ID.
     */
    virtual bool hasPageUniqueID() const = 0;

    /**
     * Check to see if the state change event has an
     * associated frame unique ID.
     */
    virtual bool hasFrameUniqueID() const = 0;

    /**
     * Check to see if the state change event has an
     * associated data set unique ID.
     */
    virtual bool hasDataSetUniqueID() const = 0;

    /**
     * Check to see if the state change event has an
     * associated unique ID.
     */
    virtual bool hasUniqueID() const = 0;

    /**
     * Check to see if the state change event has an
     * associated line map set.
     */
    virtual bool hasLineMapSet() const = 0;

    /**
     * Check to see if the state change event has an
     * associated field map set.
     */
    virtual bool hasFieldMapSet() const = 0;

    /**
     * Check to see if the state change event has
     * associated style parameters.
     */
    virtual bool hasStyleParams() const = 0;

    /**
     * Check to see if the state change event has an
     * associated offset 1.
     */
    virtual bool hasOffset1() const = 0;

    /**
     * Check to see if the state change event has an
     * associated offset 2.
     */
    virtual bool hasOffset2() const = 0;


    /**
     * Get the state change associated with this event. StateChange
     * is always included and therefore there is no hasStateChange
     * method.
     */
    virtual StateChange_e getStateChange() const = 0;

    /**
     * Get the index value associated with the state change event.
     @verbatim
     +-------------------------+-------------------------------------+
     | Availability            | Meaning                             |
     +-------------------------+-------------------------------------+
     | StateChange_QuitTecplot | Tecplot Exit Code                   |
     +-------------------------+-------------------------------------+
     | StateChange_VarsAltered | Node index of the value changed, if |
     |                         | only a single index was altered.    |
     +-------------------------+-------------------------------------+
     @endverbatim
     * @pre
     *   hasIndex() must return true to call this method
     */
    virtual LgIndex_t getIndex() const = 0;

    /**
     * Get the variable set associated with the state change event.
     @verbatim
     +---------------------------------+-------------------------------------+
     | Availability                    | Meaning                             |
     +---------------------------------+-------------------------------------+
     | StateChange_VarsAdded           | Set of added variables              |
     +---------------------------------+-------------------------------------+
     | StateChange_VarsDeleted         | Set of deleted zones                |
     +---------------------------------+-------------------------------------+
     | StateChange_VarsAltered         | Set of variables which were altered |
     +---------------------------------+-------------------------------------+
     @endverbatim
     * @pre
     *   hasVarSet() must return true to call this method
     */
    virtual const Set& getVarSet() const = 0;

    /**
     * Get the zone set associated with the state change event.
     @verbatim
     +---------------------------------+-------------------------------------+
     | Availability                    | Meaning                             |
     +---------------------------------+-------------------------------------+
     | StateChange_NodeMapsAltered     | Set of zones in which the node map  |
     |                                 | was altered                         |
     +---------------------------------+-------------------------------------+
     | StateChange_ZonesDeleted        | Set of deleted zones                |
     +---------------------------------+-------------------------------------+
     | StateChange_ZonesAdded          | Set of added zones                  |
     +---------------------------------+-------------------------------------+
     | StateChange_VarsAltered         | Set of zones in which the variables |
     |                                 | were altered                        |
     +---------------------------------+-------------------------------------+
     | StateChange_Style when P1 is:   |                                     |
     |(deprecated) SV_FIELD            | Set of zones that were changed      |
     |(deprecated) SV_ACTIVEFIELDZONES | Set of zones that are now active    |
     +---------------------------------+-------------------------------------+
     @endverbatim
     * @pre
     *   hasZoneSet() must return true to call this method
     *
     * @sa
     *   hasZoneSet, getStyleParam, getStyleParams, getLineMapSet
     */
    virtual const Set& getZoneSet() const = 0;

    /**
     * Get the line map set associated with the state change event.
     @verbatim
     +-------------------------------+--------------------------------------+
     | Availability                  | Meaning                              |
     +-------------------------------+--------------------------------------+
     | StateChange_Style when P1 is: |                                      |
     |        SV_LINEMAP             | Set of line maps that were changed   |
     |        SV_ACTIVELINEMAPS      | Set of line maps that are now active |
     +-------------------------------+--------------------------------------+
     @endverbatim
     * @pre
     *   hasLineMapSet() must return true to call this method
     *
     * @sa
     *   hasLineMapSet, getStyleParam, getStyleParams, getLineMapSet
     */
    virtual const Set& getLineMapSet() const  = 0;

    /**
     * Get the field map set associated with the state change event.
     @verbatim
     +-------------------------------+---------------------------------------+
     | Availability                  | Meaning                               |
     +-------------------------------+---------------------------------------+
     | StateChange_Style when P1 is: |                                       |
     |        SV_FIELDMAP            | Set of field maps that were changed   |
     |        SV_ACTIVEFIELDMAPS     | Set of field maps that are now active |
     +-------------------------------+---------------------------------------+
     @endverbatim
     * @pre
     *   hasFieldMapSet() must return true to call this method
     *
     * @sa
     *   hasFieldMapSet, getStyleParam, getStyleParams, getLineMapSet
     */
    virtual const Set& getFieldMapSet() const = 0;

    /**
     * Get the arbirary enumerated value associated
     * with the state change event.
     @verbatim
     +-----------------------------+---------------------+
     | Availability                | Meaning             |
     +-----------------------------+---------------------+
     | StateChange_Streamtrace     | StreamtraceAction_e |
     +-----------------------------+---------------------+
     | StateChange_VariableLockOn  | VarLockMode_e       |
     +-----------------------------+---------------------+
     | StateChange_MouseModeUpdate | MouseButtonMode_e   |
     +-----------------------------+---------------------+
     | StateChange_View            | View_e              |
     +-----------------------------+---------------------+
     @endverbatim
     * @pre
     *   hasArbEnum() must return true to call this method
     */
    virtual int32_t getArbEnum() const = 0;

    /**
     * Get the list of style values associated
     * with the state change event.
     @verbatim
     +-------------------+-----------------------------------------+
     | Availability      | Meaning                                 |
     +-------------------+-----------------------------------------+
     | StateChange_Style | A partial or complete list of the style |
     |                   | hierarchy for the style that changed    |
     +-------------------+-----------------------------------------+
     @endverbatim
     * @pre
     *   hasStyleParams() must return true to call this method
     *
     * @sa
     *  hasStyleParams, StyleValue, StringList
     */
    virtual const StringList& getStyleParams() const = 0;

    /**
     * Get a single style value associated
     * with the state change event.
     *
     * @pre
     *   hasStyleParams() must return true to call this method
     *
     * @param paramIndex
     *   index into the StyleParam list. This value must be 1-5. If there
     *   is no style parameter associated with the given index, then an
     *   empty string is returned.
     *
     * @sa
     *   getStyleParams, hasStyleParams
     */
    virtual std::string getStyleParam(int paramIndex) const  = 0;

    /**
     * Get the line map number associated with the state change event.
     @verbatim
     +-----------------------------+---------------------+
     | Availability                | Meaning             |
     +-----------------------------+---------------------+
     | Reserved for future use                           |
     +-----------------------------+---------------------+
     @endverbatim
     * @pre
     *   hasMap() must return true to call this method
     */
    virtual EntIndex_t getMap() const = 0;

    /**
     * Get the zone number associated with the state change event.
     @verbatim
     +-----------------------------+---------------------+
     | Availability                | Meaning             |
     +-----------------------------+---------------------+
     | Reserved for future use                           |
     +-----------------------------+---------------------+
     @endverbatim
     * @pre
     *   hasZone() must return true to call this method
     */
    virtual EntIndex_t getZone() const = 0;

    /**
     * Get the variable number associated with the state change event.
     @verbatim
     +-----------------------------+------------------------------------+
     | Availability                | Meaning                            |
     +-----------------------------+------------------------------------+
     | StateChange_VariableLockOn  | Variable number which was locked   |
     +-----------------------------+------------------------------------+
     | StateChange_VariableLockOff | Variable number which was unlocked |
     +-----------------------------+------------------------------------+
     @endverbatim
     * @pre
     *   hasVar() must return true to call this method
     */
    virtual EntIndex_t getVar() const = 0;

    /**
     * Get the name associated with the state change event.
     @verbatim
     +-----------------------------+-------------------------------+
     | Availability                | Meaning                       |
     +-----------------------------+-------------------------------+
     | StateChange_VariableLockOn  | Name of the owner of the lock |
     +-----------------------------+-------------------------------+
     | StateChange_VariableLockOff | Name of the owner of the lock |
     +-----------------------------+-------------------------------+
     | StateChange_DataSetLockOn   | Name of the owner of the lock |
     +-----------------------------+-------------------------------+
     | StateChange_DataSetLockOff  | Name of the owner of the lock |
     +-----------------------------+-------------------------------+
     | StateChange_NewTopPage      | Name of the new top page      |
     +-----------------------------+-------------------------------+
     | StateChange_PageDeleted     | Name of the deleted page      |
     +-----------------------------+-------------------------------+
     @endverbatim
     * @pre
     *   hasName() must return true to call this method
     */
    virtual std::string getName() const = 0;

    /**
     * Get the page UniqueID associated with the state change event.
     @verbatim
     +--------------------------------------+------------------------------------------+
     | Availability                         | Unique ID Meaning                        |
     +--------------------------------------+------------------------------------------+
     | StateChange_FrameDeleted             | Page associated with the deleted frame.  |
     +--------------------------------------+------------------------------------------+
     | StateChange_PageDeleted              | Deleted page.                            |
     +--------------------------------------+------------------------------------------+
     | StateChange_NewTopPage               | New top page.                            |
     +--------------------------------------+------------------------------------------+
     | StateChange_Style                    | Page associated with the style change.   |
     +--------------------------------------+------------------------------------------+
     | StateChange_NewTopFrame (deprecated) | Page associated with the new top frame.  |
     +--------------------------------------+------------------------------------------+
     | StateChange_NewActiveFrame           | Page associated with the new top frame.  |
     +--------------------------------------+------------------------------------------+
     | StateChange_FrameOrderChanged        | Page associated with the new top frame.  |
     +--------------------------------------+------------------------------------------+
     | StateChange_ImageExported            | Page associated with the frame exported. |
     +--------------------------------------+------------------------------------------+
     | StateChange_AuxDataAdded             | Page who's auxiliary data was added.     |
     +--------------------------------------+------------------------------------------+
     | StateChange_AuxDataDeleted           | Page who's auxiliary data was deleted.   |
     +--------------------------------------+------------------------------------------+
     | StateChange_AuxDataAltered           | Page who's auxiliary data was altered.   |
     +--------------------------------------+------------------------------------------+
     @endverbatim
     * @pre
     *   hasPageUniqueID() must return true to call this method
     */
    virtual UniqueID_t getPageUniqueID() const = 0;

    /**
     * Get the frame UniqueID associated with the state change event.
     @verbatim
     +---------------------------------------+------------------------------------------+
     | Availability                          | Unique ID Meaning                        |
     +---------------------------------------+------------------------------------------+
     | StateChange_FrameDeleted              | Deleted frame.                           |
     +---------------------------------------+------------------------------------------+
     | StateChange_NewTopFrame (deprecated)  | New top frame.                           |
     +---------------------------------------+------------------------------------------+
     | StateChange_NewActiveFrame            | New active frame.                        |
     +---------------------------------------+------------------------------------------+
     | StateChange_ImageExported             | Frame exported.                          |
     +---------------------------------------+------------------------------------------+
     | StateChange_Style                     | Frame associated with the style change.  |
     +---------------------------------------+------------------------------------------+
     | StateChange_LineMapAssignment         | Frame associated with the style change.  |
     +---------------------------------------+------------------------------------------+
     | StateChange_ContourLevels             | Frame associated with the style change.  |
     +---------------------------------------+------------------------------------------+
     | StateChange_LineMapName               | Frame associated with the style change.  |
     +---------------------------------------+------------------------------------------+
     | StateChange_LineMapAddDeleteOrReorder | Frame associated with the style change.  |
     +---------------------------------------+------------------------------------------+
     | StateChange_View                      | Frame associated with the style change.  |
     +---------------------------------------+------------------------------------------+
     | StateChange_ContourVar                | Frame associated with the style change.  |
     +---------------------------------------+------------------------------------------+
     | StateChange_Streamtrace               | Frame associated with the style change.  |
     +---------------------------------------+------------------------------------------+
     | StateChange_NewAxisVariables          | Frame associated with the style change.  |
     +---------------------------------------+------------------------------------------+
     | StateChange_Text                      | Frame associated with the style change.  |
     +---------------------------------------+------------------------------------------+
     | StateChange_Geom                      | Frame associated with the style change.  |
     +---------------------------------------+------------------------------------------+
     | StateChange_AuxDataAdded              | Frame who's auxiliary data was added.    |
     +---------------------------------------+------------------------------------------+
     | StateChange_AuxDataDeleted            | Frame who's auxiliary data was deleted.  |
     +---------------------------------------+------------------------------------------+
     | StateChange_AuxDataAltered            | Frame who's auxiliary data was altered.  |
     +---------------------------------------+------------------------------------------+
     @endverbatim
     * @pre
     *   hasFrameUniqueID() must return true to call this method
     */
    virtual UniqueID_t getFrameUniqueID() const = 0;

    /**
     * Get the data set UniqueID associated with the state change event.
     @verbatim
     +-----------------------------+--------------------------------------------+
     | Availability                | Unique ID Meaning                          |
     +-----------------------------+--------------------------------------------+
     | StateChange_VarsAltered     | Data set changed.                          |
     +-----------------------------+--------------------------------------------+
     | StateChange_VarsAdded       | Data set changed.                          |
     +-----------------------------+--------------------------------------------+
     | StateChange_VarsDeleted     | Data set changed.                          |
     +-----------------------------+--------------------------------------------+
     | StateChange_ZonesDeleted    | Data set changed.                          |
     +-----------------------------+--------------------------------------------+
     | StateChange_ZonesAdded      | Data set changed.                          |
     +-----------------------------+--------------------------------------------+
     | StateChange_NodeMapsAltered | Data set changed.                          |
     +-----------------------------+--------------------------------------------+
     | StateChange_DataSetReset    | Data set changed.                          |
     +-----------------------------+--------------------------------------------+
     | StateChange_ZoneName        | Data set changed.                          |
     +-----------------------------+--------------------------------------------+
     | StateChange_VarName         | Data set changed.                          |
     +-----------------------------+--------------------------------------------+
     | StateChange_JournalReset    | Data set changed.                          |
     +-----------------------------+--------------------------------------------+
     | StateChange_DataSetLockOn   | Data set locked.                           |
     +-----------------------------+--------------------------------------------+
     | StateChange_DataSetLockOff  | Data set unlocked.                         |
     +-----------------------------+--------------------------------------------+
     | StateChange_VariableLockOn  | Data set who's variable was locked.        |
     +-----------------------------+--------------------------------------------+
     | StateChange_VariableLockOff | Data set who's variable was unlocked.      |
     +-----------------------------+--------------------------------------------+
     | StateChange_DataSetTitle    | Data set changed.                          |
     +-----------------------------+--------------------------------------------+
     | StateChange_AuxDataAdded    | Data set who's auxiliary data was added.   |
     +-----------------------------+--------------------------------------------+
     | StateChange_AuxDataDeleted  | Data set who's auxiliary data was deleted. |
     +-----------------------------+--------------------------------------------+
     | StateChange_AuxDataAltered  | Data set who's auxiliary data was altered. |
     +-----------------------------+--------------------------------------------+
     @endverbatim
     * @pre
     *   hasDataSetUniqueID() must return true to call this method
     */
    virtual UniqueID_t getDataSetUniqueID() const = 0;

    /**
     * Get the UniqueID associated with the state change event.
     @verbatim
     +--------------------------+--------------------------------+
     | Availability             | Meaning                        |
     +--------------------------+--------------------------------+
     | StateChange_FrameDeleted | UniqueID of the deleted frame. |
     +--------------------------+--------------------------------+
     | StateChange_PageDeleted  | UniqueID of the deleted page.  |
     +--------------------------+--------------------------------+
     | StateChange_NewTopPage   | UniqueID of the new top page.  |
     +--------------------------+--------------------------------+
     @endverbatim
     * @pre
     *   hasUniqueID() must return true to call this method
     */
    virtual UniqueID_t getUniqueID() const = 0;

    /**
     * Get the offset 1 associated with the state change event.
     @verbatim
     +-------------------+-----------------------------------------+
     | Availability      | Meaning                                 |
     +-------------------+-----------------------------------------+
     | StateChange_Style | Offset1 or group number for the style   |
     |                   | hierarchy returned by getStyleParams    |
     +-------------------+-----------------------------------------+
     @endverbatim
     * @pre
     *   hasOffset1() must return true to call this method
     *
     * @sa
     *  hasStyleParams, StyleValue
     */
    virtual LgIndex_t getOffset1() const = 0;

    /**
     * Get the offset 2 associated with the state change event.
     @verbatim
     +-------------------+-----------------------------------------+
     | Availability      | Meaning                                 |
     +-------------------+-----------------------------------------+
     | StateChange_Style | Offset2 or group number for the style   |
     |                   | hierarchy returned by getStyleParams    |
     +-------------------+-----------------------------------------+
     @endverbatim
     * @pre
     *   hasOffset2() must return true to call this method
     *
     * @sa
     *  hasStyleParams, StyleValue
     */
    virtual LgIndex_t getOffset2() const = 0;
};

}}
