#pragma once

#define ENGINE_INTERNAL_EXPORT // force LINKTOADDON to export insead of import
#include "TecUtil.h"
#include "tp360addonuiaction_Exports.h"

#if defined LINKTOADDON
#undef LINKTOADDON
#endif

#define LINKTOADDON EXTERNC tp360addonuiaction_API

LINKTOADDON void TecUtilHelp(
	const char * helpFileOrURL,
	Boolean_t goToID,
	int helpID);
