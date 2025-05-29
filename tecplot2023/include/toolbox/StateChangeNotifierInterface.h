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
#ifndef TECPLOT_TOOLBOX_STATECHANGENOTIFIERINTERFACE_H
#define TECPLOT_TOOLBOX_STATECHANGENOTIFIERINTERFACE_H

#if defined MSWIN
#pragma once
#endif

#include "SignalDefinition.h"
#include "tptoolbox.h"
#include "tptoolbox_Exports.h"

namespace tecplot { namespace toolbox {
    class StateChangeEventInterface;
    class StateChangeListenerInterface;
}}

namespace tecplot
{
namespace toolbox
{

class tptoolbox_API StateChangeNotifierInterface
{
public:
    /**
     * @deprecated
     *   Use connectToStateChangedSignal instead.
     *
     * Register a listener with the StateChangeNotifier
     *
     * @par Note:
     *   Tecplot must be locked when calling this method. Override lockStart
     *   and lockFinish methods to provide locking as a convenience.
     *
     * @param listener
     *   Pointer to the listener class that has implemented the
     *   StateChangeListenerInterface
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if listener could not be added.
     */
    virtual void addListener(StateChangeListenerInterface* listener) = 0;

    /**
     * Register a listener with the StateChangeNotifier
     *
     * @par Note:
     *   Tecplot must be locked when calling this method. Override lockStart
     *   and lockFinish methods to provide locking as a convenience.
     */
    DEFINE_SIGNAL_1(StateChanged, tecplot::toolbox::StateChangeEventInterface const& stateChangeEvent);

    /**
     * @deprecated
     *   Use connectToStateChangedSignal instead.
     *
     * Remove a listener from the StateChangeNotifier
     *
     * @par Note:
     *   Tecplot must be locked when calling this method. Override lockStart
     *   and lockFinish methods to provide locking as a convenience.
     *
     * @param listener
     *   Pointer to the listener class that has implemented the
     *   StateChangeListenerInterface
     */
    virtual void removeListener(StateChangeListenerInterface* listener) = 0;

    /**
     * Checks to see if a particular listener is registered
     * with the StateChangeNotifier.
     *
     * @param listener
     *   Pointer to the listener class that has implemented the
     *   StateChangeListenerInterface
     *
     * @return
     *   true if the listener is registered, false otherwise.
     */
    virtual bool isListening(StateChangeListenerInterface* listener) = 0;

    /**
     * Sets the mode in which state changes are propagated to the registered listeners.
     * @par Note:
     *   Tecplot must be locked when calling this method. Override lockStart
     *   and lockFinish methods to provide locking as a convenience.
     * @param mode
     *   Mode you want state changes propagated to your listeners.
     *   Choose either StateChangeMode_v80, StateChangeMode_v100, or StateChangeMode_v113.
     *   See the section "Sending State Changes" in the "ADK User's Manual" for a complete
     *   description of the differences between these options.
     * @return
     *   Returns true is successful, false otherwise
     */
    virtual bool setMode(StateChangeMode_e mode) = 0;

    /**
     * @return
     *  The current mode for which the listeners are registerd to
     *  receive the state change notifications.
     */
    virtual StateChangeMode_e mode() = 0;

    virtual ~StateChangeNotifierInterface() {}
private:
    /**
     * Override these methods if you want to provide locking.
     * AddOn developers should use TecUtilLockStart and TecUtilLockFinish
     * SDK developers should use TecUtilParentLockStart and TecUtilParentLockFinish
     */
    virtual void lockStart(bool shutdownImplicitRecording) = 0;
    virtual void lockFinish() = 0;

};

}
}

#endif
