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
#ifndef TECPLOT_TOOLBOX_STATECHANGELISTENERINTERFACE_H
#define TECPLOT_TOOLBOX_STATECHANGELISTENERINTERFACE_H

#include "tptoolbox.h"
#include "tptoolbox_Exports.h"

namespace tecplot { namespace toolbox {
    class StateChangeEventInterface;
    class StateChangeNotifierInterface;
}}

namespace tecplot
{
namespace toolbox
{

/**
 * @deprecated
 *   Use signal-based listening instead.
 * Interface for listening to State Change Notification events.
 * State Change Listeners must register with StateChangeNotifier.
 */
class tptoolbox_API StateChangeListenerInterface
{
public:
    virtual void stateChanged(const tecplot::toolbox::StateChangeEventInterface& stateChangeEvent,
                              tecplot::toolbox::StateChangeNotifierInterface&    source) = 0;
    virtual ~StateChangeListenerInterface() {}
};

}
}

#endif
