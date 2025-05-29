#pragma  once

#include "ThirdPartyHeadersBegin.h"
#  include <QPlainTextEdit>
#  include <QFocusEvent>
#  include <string>
#  include <boost/scoped_ptr.hpp>
#include "ThirdPartyHeadersEnd.h"

#include "TECADDON.h"
#include "tecgui_Exports.h"

#include "ClassMacros.h"

namespace tecplot { namespace tecgui {

class TecGUIText
    : public QPlainTextEdit
{
    Q_OBJECT
public:

    TecGUIText(
        GUIInt_t 	            parentDialogID,
        GUIInt_t 	            x,
        GUIInt_t 	            y,
        GUIInt_t 	            width,
        GUIInt_t 	            height,
        bool 	                isReadOnly,
        TecGUITextCallback_pf 	valueChangedCallback	
        );

    virtual ~TecGUIText();

private slots:
    void onTextChanged();

private:
    struct Impl;
    boost::scoped_ptr<Impl> m_impl;

    UNCOPYABLE_CLASS(TecGUIText);
};

}}
