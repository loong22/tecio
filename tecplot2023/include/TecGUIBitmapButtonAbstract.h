#pragma once

#include "ThirdPartyHeadersBegin.h"
#  include <QPushButton>
#  include <boost/scoped_ptr.hpp>
#include "ThirdPartyHeadersEnd.h"

#include "ClassMacros.h"
#include "TecGUIGlobal.h"
#include "tecgui_Exports.h"

namespace tecplot { namespace tecgui {

class TecGUIBitmapButtonAbstract
    : public QPushButton
{
public:
    TecGUIBitmapButtonAbstract(
        GUIInt_t   parentId,
        GUIInt_t   x,
        GUIInt_t   y,
        GUIInt_t   buttonWidth,
        GUIInt_t   buttonHeight,
        GUIInt_t   bitmapWidth,
        GUIInt_t   bitmapHeight,
        char const* bitmapData,
        bool        useTransparentColor,
        GUIInt_t   transparentR,
        GUIInt_t   transparentG,
        GUIInt_t   transparentB);

    virtual ~TecGUIBitmapButtonAbstract();

private:

    struct Impl;
    boost::scoped_ptr<Impl> m_impl;

    UNCOPYABLE_CLASS(TecGUIBitmapButtonAbstract);
};
}}
