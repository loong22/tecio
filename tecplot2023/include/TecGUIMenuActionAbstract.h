#pragma once

#include "ThirdPartyHeadersBegin.h"
#  include <QAction>
#  include <string>
#include "ThirdPartyHeadersEnd.h"

#include "TecGUIGlobal.h"
#include "tecgui_Exports.h"
#include "ClassMacros.h"

namespace tecplot { namespace tecgui {


class TecGUIMenuActionAbstract
    : public QAction
{
public:
    TecGUIMenuActionAbstract(
        GUIInt_t           parent,
        std::string const&  label);

    virtual ~TecGUIMenuActionAbstract();


private:
    UNCOPYABLE_CLASS(TecGUIMenuActionAbstract);
};
}}
