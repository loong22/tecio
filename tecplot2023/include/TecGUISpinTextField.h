#pragma once

#include "ThirdPartyHeadersBegin.h"
#  include <QAbstractSpinBox>
#  include <QLineEdit>
#  include <boost/scoped_ptr.hpp>
#include "ThirdPartyHeadersEnd.h"

#include "TECADDON.h"
#include "tecgui_Exports.h"

#include "ClassMacros.h"

namespace tecplot { namespace tecgui {

class TecGUISpinTextField
    : public QAbstractSpinBox
{

public:

    TecGUISpinTextField(
        GUIInt_t parentDialogId,
        GUIInt_t x,
        GUIInt_t y,
        GUIInt_t width,
        GUIInt_t height,
        TecGUITextCallback_pf valueChangedCallback,
        TecGUIVoidCallback_pf buttonUpCallback,
        TecGUIVoidCallback_pf buttonDownCallback
        );

    virtual ~TecGUISpinTextField();

    QLineEdit& lineEdit() const;

protected:
    virtual StepEnabled stepEnabled() const;
    virtual void stepBy(int steps);


private:
    struct Impl;
    boost::scoped_ptr<Impl> m_impl;

    UNCOPYABLE_CLASS(TecGUISpinTextField);
};

}}

