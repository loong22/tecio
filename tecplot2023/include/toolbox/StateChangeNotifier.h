/*
 *****************************************************************
 *****************************************************************
 *******                                                  ********
 *******     (C) Copyright 1989-2007  by Tecplot Inc.     ********
 *******             All Rights Reserved.                 ********
 *******                                                  ********
 *****************************************************************
 *****************************************************************
 */
#pragma once

#if defined MSWIN
#pragma once
#endif

#include <set>
#include "ClassMacros.h"
#include "MiscMacros.h"
#include "StateChangeNotifierInterface.h"
#include "StateChangeEventInterface.h"
#include "Set.h"
#include "StringList.h"
#include "tptoolbox_Exports.h"

namespace tecplot
{
namespace toolbox
{

/**
 * Handles State Change Notification events and passes those events
 * on to registered listeners. State Change Listeners must be
 * of type StateChangeListenerInterface.
 */
class tptoolbox_API StateChangeNotifier : public StateChangeNotifierInterface
{
public:
    StateChangeNotifier(StateChangeMode_e stateChangeMode = StateChangeMode_v100);
    virtual ~StateChangeNotifier();

    /**
     */
    void addListener(StateChangeListenerInterface* listener);

    /**
     */
    void removeListener(StateChangeListenerInterface* listener);

    /**
     */
    bool isListening(StateChangeListenerInterface* listener);

    /**
     */
    bool setMode(StateChangeMode_e mode);
    /**
     */
    StateChangeMode_e mode();

    /**
     * Inform Tecplot of a state change event.
     *
     * There are certain circumstances in which you will have to inform
     * Tecplot of your changes. You application must sent state changes
     * in the following circumstances:
     *
     @verbatim
     +-------------------------+-------------------------------+-------------------+
     |Circumstance             | StateChange                   | Supplemental      |
     |                         |                               | Information       |
     +-------------------------+-------------------------------+-------------------+
     |After a variable         |                               |                   |
     |has been added and       | StateChange_VarsAdded         | None              |
     |modified                 |                               |                   |
     +-------------------------+-------------------------------+-------------------+
     |                         |                               | varSet (required) |
     | After a variable        | StateChange_VarsAltered       | zoneSet           |
     | has been modified       |                               | index (if only one|
     |                         |                               | value was altered)|
     +-------------------------+-------------------------------+-------------------+
     |After the node map       |                               |                   |
     |has been altered         | StateChange_NodeMapsAltered   | zoneSet (required)|
     +-------------------------+-------------------------------+-------------------+
     |After a zone has         |                               |                   |
     |been added and the       | StateChange_ZonesAdded        | zoneSet (required)|
     |field data has been      |                               |                   |
     |modified                 |                               |                   |
     +-------------------------+-------------------------------+-------------------+
     |After adding, removing   |                               |                   |
     |or modifying one or      | StateChange_Text              | None              |
     |more text elements       |                               |                   |
     +-------------------------+-------------------------------+-------------------+
     |After adding, removing   |                               |                   |
     |or modifying one or      | StateChange_Geom              | None              |
     |more geometry elements   |                               |                   |
     +-------------------------+-------------------------------+-------------------+
     |Add-On Developers only:                                                      |
     +-------------------------+-------------------------------+-------------------+
     |After launch or dismissal| StateChange_ModalDialogLaunch |                   |
     |of a modal dialog        | StateChange_ModalDialogDismiss| None              |
     |(Windows only)           |                               |                   |
     +-------------------------+-------------------------------+-------------------+
     @endverbatim
     *
     * @param stateChange
     *   The state change which is to be sent to Tecplot
     *
     * @param zoneSet
     *   Set of affected zones (if required)
     *
     * @param varSet
     *   Set of affected variables (if required)
     *
     * @param index
     *   Index value of the value changed (if required)
     */
    static void sendStateChange(StateChange_e stateChange,
                                const Set&    zoneSet = Set(),
                                const Set&    varSet  = Set(),
                                LgIndex_t     index   = TECUTILBADID);
protected:
    /**
     * This method is called by the Tecplot SDK whenever a state change
     * event occurs. Override this method if you wish to do something
     * special with the state change notification. For instance you
     * might create a custom StateChangeNotifier that only notifies of
     * StateChange_Style notifications. Be sure to call notifyOfStateChanged
     * to notifiy the listeners.
     *
     * @param stateChangeEvent
     *   The state change event information that corresponds to
     *   the current state change
     */
    virtual void onStateChanged(const StateChangeEventInterface& stateChangeEvent);

    /**
     * Notify the listeners of the state change event.
     *
     * @param stateChangeEvent
     *   The state change event information that corresponds to
     *   the state change
     */
    void notifyOfStateChanged(const StateChangeEventInterface& stateChangeEvent);


private:
    /**
     * Override these methods if you want to provide locking.
     * @note
     *     Add-on developers should use TecUtilLockStart and TecUtilLockFinish APIs while SDK
     *     application developers should use the tecplot::sdk::AppLock convenience class in
     *     tpsdkintegrationmanager library.
     */
    virtual void lockStart(bool shutdownImplicitRecording = false) { UNUSED(shutdownImplicitRecording); }
    virtual void lockFinish() {}

    static void staticStateChangeCallback(StateChange_e stateChange,
                                                  ArbParam_t    clientData);

    /**
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the StateChangeCallback could
     *   not be registered with the Tecplot Engine.
     */
    void addStateChangeCallback();
    void removeStateChangeCallback();
    bool m_stateChangeCallbackIsRegistered;
    StateChangeMode_e m_stateChangeMode;

    typedef std::set<StateChangeListenerInterface*> ListenerSet_t;
    ListenerSet_t m_listeners;

    virtual void signalStateChangedConnected() const;

    UNCOPYABLE_CLASS(StateChangeNotifier);

private:
    /**
     * The timing of StateChangeEvent construction is very
     * specific, so we make this an inner class so it can't
     * be instantiated by just anyone at any time.
     *
     * This must be instantiated when the Tecplot SDK sends
     * its state change out to the StateChangeNotifier
     */
    class StateChangeEvent : public StateChangeEventInterface
    {
    public:
        StateChangeEvent(StateChange_e stateChange);
        ~StateChangeEvent();
    public:
        const StateChangeEventInterface* clone() const;

        bool hasIndex() const;
        bool hasVarSet() const;
        bool hasZoneSet() const;
        bool hasLineMapSet() const;
        bool hasFieldMapSet() const;
        bool hasArbEnum() const;
        bool hasStyleParams() const;
        bool hasMap() const;
        bool hasZone() const;
        bool hasVar() const;
        bool hasName() const;
        bool hasPageUniqueID() const;
        bool hasFrameUniqueID() const;
        bool hasDataSetUniqueID() const;
        bool hasUniqueID() const;
        bool hasOffset1() const;
        bool hasOffset2() const;

        StateChange_e     getStateChange() const;
        LgIndex_t         getIndex() const;
        const Set&        getVarSet() const;
        const Set&        getZoneSet() const;
        const Set&        getLineMapSet() const;
        const Set&        getFieldMapSet() const;
        int32_t           getArbEnum() const;
        const StringList& getStyleParams() const;
        std::string       getStyleParam(int param) const;
        EntIndex_t        getMap() const;
        EntIndex_t        getZone() const;
        EntIndex_t        getVar() const;
        std::string       getName() const;
        UniqueID_t        getPageUniqueID() const;
        UniqueID_t        getFrameUniqueID() const;
        UniqueID_t        getDataSetUniqueID() const;
        UniqueID_t        getUniqueID() const;
        LgIndex_t         getOffset1() const;
        LgIndex_t         getOffset2() const;

    private:
        StateChange_e  m_stateChange;
        LgIndex_t      m_index;
        Set            m_varSet;
        Set            m_zoneSet;
        Set            m_lineMapSet;
        Set            m_fieldMapSet;
        int32_t        m_arbEnum;
        StringList     m_styleParams;
        EntIndex_t     m_map;
        EntIndex_t     m_zone;
        EntIndex_t     m_var;
        std::string    m_name;
        UniqueID_t     m_pageUniqueID;
        UniqueID_t     m_frameUniqueID;
        UniqueID_t     m_dataSetUniqueID;
        UniqueID_t     m_uniqueID;
        LgIndex_t      m_offset1;
        LgIndex_t      m_offset2;
    };
};

}
}
