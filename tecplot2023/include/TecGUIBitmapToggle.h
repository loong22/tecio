#pragma once
#include "TecGUIBitmapButtonAbstract.h"

namespace tecplot { namespace tecgui {

class TecGUIBitmapToggle
    : public TecGUIBitmapButtonAbstract
{
    Q_OBJECT

public:
    TecGUIBitmapToggle(
        GUIInt_t              parentId,
        GUIInt_t              x,
        GUIInt_t              y,
        GUIInt_t              buttonWidth,
        GUIInt_t              buttonHeight,
        GUIInt_t              bitmapWidth,
        GUIInt_t              bitmapHeight,
        char const*                     bitmapData,
        bool                            useTransparentColor,
        GUIInt_t              transparentR,
        GUIInt_t              transparentG,
        GUIInt_t              transparentB,
        TecGUIIntCallback_pf  buttonCallback);

    virtual ~TecGUIBitmapToggle();

private slots:
    void valueChangedCallback(bool isChecked);

private:
    TecGUIIntCallback_pf   m_buttonCallback;

    UNCOPYABLE_CLASS(TecGUIBitmapToggle);
};
}}

