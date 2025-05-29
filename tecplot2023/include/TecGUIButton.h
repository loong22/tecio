#pragma  once

#include "ThirdPartyHeadersBegin.h"
#  include <QPushButton>
#  include <QString>
#  include <string>
#  include <boost/scoped_ptr.hpp>
#include "ThirdPartyHeadersEnd.h"

#include "TECADDON.h"
#include "tecgui_Exports.h"

#include "ClassMacros.h"

namespace tecplot { namespace tecgui {

class TecGUIButton
    : public QPushButton
{
    Q_OBJECT
public:

    TecGUIButton(
        GUIInt_t               parentDialogId,
        GUIInt_t               x,
        GUIInt_t               y,
        GUIInt_t               width,
        GUIInt_t               height,
        std::string const&      labelString,
        TecGUIVoidCallback_pf   buttonCallback
        );

    virtual ~TecGUIButton();

private slots:
        void buttonClicked();

private:
    struct Impl;
    boost::scoped_ptr<Impl> m_impl;

    UNCOPYABLE_CLASS(TecGUIButton);
};

}}
