#pragma once
#include "TecGUIBitmapButtonAbstract.h"

namespace tecplot { namespace tecgui {

class TecGUIBitmapButton
    : public tecplot::tecgui::TecGUIBitmapButtonAbstract
{
    Q_OBJECT

public:
    TecGUIBitmapButton(
        GUIInt_t              parentId,
        GUIInt_t              x,
        GUIInt_t              y,
        GUIInt_t              buttonWidth,
        GUIInt_t              buttonHeight,
        GUIInt_t              bitmapWidth,
        GUIInt_t              bitmapHeight,
        char const*            bitmapData,
        bool                   useTransparentColor,
        GUIInt_t              transparentR,
        GUIInt_t              transparentG,
        GUIInt_t              transparentB,
        TecGUIVoidCallback_pf  buttonCallback);

    virtual ~TecGUIBitmapButton();

private slots:
    void buttonClicked();

private:
    TecGUIVoidCallback_pf  m_buttonCallback;

    UNCOPYABLE_CLASS(TecGUIBitmapButton);
};

}}
