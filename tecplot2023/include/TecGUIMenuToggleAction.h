#pragma once

#include "TecGUIMenuActionAbstract.h"
#include "TecGUIGlobal.h"
#include "tecgui_Exports.h"
#include "ClassMacros.h"

namespace tecplot { namespace tecgui {


class TecGUIMenuToggleAction
    : public TecGUIMenuActionAbstract
{
    Q_OBJECT

public:
    TecGUIMenuToggleAction(
        GUIInt_t                       parent,
        std::string const&              label,
        TecGUIIntCallback_pf	callback);

    virtual ~TecGUIMenuToggleAction();

private slots:
    void menuActionToggled(bool isChecked);

private:
    TecGUIIntCallback_pf  m_callback;
    UNCOPYABLE_CLASS(TecGUIMenuToggleAction);
};

}}
