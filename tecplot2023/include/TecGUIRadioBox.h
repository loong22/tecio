#pragma  once

#include "ThirdPartyHeadersBegin.h"
#  include <QButtonGroup>
#  include <string>
#  include <vector>
#  include <boost/scoped_ptr.hpp>
#include "ThirdPartyHeadersEnd.h"

#include "TECADDON.h"
#include "tecgui_Exports.h"

#include "ClassMacros.h"

namespace tecplot { namespace tecgui {

class TecGUIRadioBox
    : public QButtonGroup
{
    Q_OBJECT
    static int const NUM_LABELS = 5;

public:
    TecGUIRadioBox(
        GUIInt_t 	              parentDialogID,
        GUIInt_t 	              x,
        GUIInt_t 	              y,
        GUIInt_t 	              width,
        GUIInt_t 	              height,
        std::vector<std::string>& labelArray,
        TecGUIIntCallback_pf      valueChangedCallback	 
        );

    virtual ~TecGUIRadioBox();

private slots:
    void onButtonClicked (int id);

private:
    struct Impl;
    boost::scoped_ptr<Impl> m_impl;

    UNCOPYABLE_CLASS(TecGUIRadioBox);
};

}}
