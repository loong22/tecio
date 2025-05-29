#pragma  once

#include "ThirdPartyHeadersBegin.h"
#  include <QComboBox>
#  include <string>
#  include <boost/scoped_ptr.hpp>
#include "ThirdPartyHeadersEnd.h"

#include "TECADDON.h"
#include "tecgui_Exports.h"

#include "ClassMacros.h"

namespace tecplot { namespace tecgui {

class TecGUIOptionMenu
    : public QComboBox
{
    Q_OBJECT
public:

    TecGUIOptionMenu(
        GUIInt_t 	            parentDialogID,
        GUIInt_t 	            x,
        GUIInt_t 	            y,
        GUIInt_t 	            width,
        GUIInt_t 	            height,
        std::string const&      optionList,
        TecGUIIntCallback_pf 	valueChangedCallback);

    virtual ~TecGUIOptionMenu();

private slots:
    void onCurrentIndexChanged(int index);

private:
    struct Impl;
    boost::scoped_ptr<Impl> m_impl;

    UNCOPYABLE_CLASS(TecGUIOptionMenu);
};

}}
