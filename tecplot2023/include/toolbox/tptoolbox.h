#pragma once
/*
 *****************************************************************
 *****************************************************************
 *******                                                  ********
 *******     (C) Copyright 1989-2007  by Tecplot Inc.     ********
 *******             All Rights Reserved.                 ********
 *******                                                  ********
 *****************************************************************
 *****************************************************************
 */

#include "tptoolbox_Exports.h"

#if defined WIN32
/*
 * warning C4996: 'foo': This function or variable may be unsafe. Consider using foo_s instead.
 */
#pragma warning(disable: 4996)

/*
 * warning C4786: 'Some STL template class' : identifier was truncated to
 * '255' characters in the debug information
 */
#pragma warning(disable: 4786)

/*
 * class 'std::set<>,class std::allocator<> >' needs to have dll-interface
 */
#pragma warning (disable : 4251)

/*
 * "C++ exception specification ignored except to indicate a function is not __declspec(nothrow)"
 * Indeed, in the current implementation of the compiler, there is no check if the method will throw
 * only the exceptions that it claims it throws. The 'throw' qualifier is added to methods for purely
 * documentational purposes.
 */
#pragma warning (disable : 4290)
#endif

#include "TecUtil.h"

namespace tecplot { namespace toolbox {} }
namespace tbx = tecplot::toolbox;
