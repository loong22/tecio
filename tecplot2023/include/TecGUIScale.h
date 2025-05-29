#pragma  once

#include "ThirdPartyHeadersBegin.h"
#  include <QSlider>
#  include <string>
#  include <boost/scoped_ptr.hpp>
#include "ThirdPartyHeadersEnd.h"

#include "TECADDON.h"
#include "tecgui_Exports.h"

#include "ClassMacros.h"

namespace tecplot { namespace tecgui {

class TecGUIScale
    : public QSlider
{
    Q_OBJECT
public:

    TecGUIScale(
        GUIInt_t 	            parentDialogID,
        GUIInt_t 	            x,
        GUIInt_t 	            y,
        GUIInt_t 	            width,
        GUIInt_t 	            height,
        GUIInt_t 	            scaleMin,
        GUIInt_t 	            scaleMax,
        GUIInt_t 	            decimalPrecision,
        TecGUIIntCallback_pf 	valueChangedCallback,
        TecGUIIntCallback_pf 	dragValueChangedCallback	 
        );

    virtual ~TecGUIScale();
    
    void showNumericDisplay(bool show);
    class QLabel& buddy() const;

private slots:
    void onValueChanged(int value);

    // For the drag callback, we use the actionTriggered
    // signal instead of the sliderMoved signal. 
    // This allows the unit tests to fake a drag event
    // by calling scale.triggerAction().
    void onActionTriggered(int action);

private:
    struct Impl;
    boost::scoped_ptr<Impl> m_impl;

    UNCOPYABLE_CLASS(TecGUIScale);
};

}}
