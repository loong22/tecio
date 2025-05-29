#pragma  once

#include "ThirdPartyHeadersBegin.h"
#  include <QListWidget>
#  include <boost/scoped_ptr.hpp>
#include "ThirdPartyHeadersEnd.h"

#include "TECADDON.h"
#include "tecgui_Exports.h"

#include "ClassMacros.h"

namespace tecplot { namespace tecgui {

class TecGUIList
    : public QListWidget
{
    Q_OBJECT
public:

    TecGUIList(
        GUIInt_t 	parentDialogID,
        GUIInt_t 	x,
        GUIInt_t 	y,
        GUIInt_t 	width,
        GUIInt_t 	height,
        Boolean_t 	isMultiSelection,
        TecGUIIntCallback_pf 	valueChangedCallback);

    virtual ~TecGUIList();

private slots:
    void onItemSelectedChanged();

private:
    struct Impl;
    boost::scoped_ptr<Impl> m_impl;

    UNCOPYABLE_CLASS(TecGUIList);
};

}}
