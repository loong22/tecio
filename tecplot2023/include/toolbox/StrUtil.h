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
#ifndef TECPLOT_TOOLBOX_STRUTIL_H
#define TECPLOT_TOOLBOX_STRUTIL_H

#if defined MSWIN
#pragma once
#endif

#include <string>
#include <vector>
#include "tptoolbox.h"
#include "tptoolbox_Exports.h"

namespace tecplot
{
namespace toolbox
{

class tptoolbox_API StrUtil
{
public:
    /**
     * Determine if the specified characater is whitespace or not.
     *
     * @param C
     *   Character to check.
     *
     * @return
     *   @c true if the character is considered whitespace, @c false otherwise.
     */
    static bool isWhiteSpace(const char C);

    /**
     * Remove leading whitespace from a string.
     *
     * @param str
     *   String which should have white space removed.
     *
     * @code
     *   std::string strValue("   \t   Hi Mom\n");
     *   cout << strValue.c_str();
     *   trimWhiteSpace(strValue);
     *   cout << strValue.c_str();
     *
     *   Output:
     *   "           Hi Mom"
     *   "Hi Mom"
     * @endcode
     */
    static void trim(std::string &str);

    /**
     * Find and remove a symbol for the beginning of a string.
     *
     * @param str
     *   String in which to search for the symbol
     *
     * @param symbol
     *   Character to search for
     *
     * @return
     *   @c true if symbol is the next non-whitespace character in the string.
     *   If the symbol is found, it will be removed from the front of the
     *   string. Whitespace is always trimmed from the front of the string
     *   regardless of the return value. @c false if the symbol is not the next
     *   non-whitespace character.
     *
     * @code
     *   std::string strValue("  [1,2,3]\n");
     *   cout << strValue.c_str();
     *   scanForSymbol(strValue, '!');
     *   cout << strValue.c_str();
     *   scanForSymbol(strValue, '[');
     *   cout << strValue.c_str();
     *
     *   Output:
     *   "  [1,2,3]"  // Original string
     *   "[1,2,3]"    // After looking for '!', the leading whitespace is trimmed
     *   "1,2,3]"     // Found '[' and removed it.
     * @endcode
     *
     * @sa
     *   StrUtil::scanForLgIndex
     */
    static bool scanForSymbol(std::string &str, const char symbol);

    /**
     * Find a LgIndex_t value at the beginning of a string.
     *
     * @param str
     *   String in which to search for a LgIndex_t value.
     *
     * @param value
     *   LgIndex_t pointer in which to stuff the result.
     *
     * @return
     *   @c true if the next non-whitspace series of characters in the string evaluate
     *   to a LgIndex_t. Whitespace is always trimmed from the front of the string.
     *   The string will be modified regardless of the return value. If this function
     *   returns @c false the string may be in an unknown state.
     *
     * @code
     *   std::string strValue("1,2,3\n");
     *   cout << strValue.c_str();
     *   LgIndex_t value;
     *   scanForLgIndex(strValue, &value);
     *   cout << strValue.c_str();
     *
     *   Output:
     *   "1,2,3"  // Original string
     *   ",2,3"   // After looking for the LgIndex_t value
     * @endcode
     */
    static bool scanForLgIndex(std::string &str, LgIndex_t *value);

    /**
     * Convert the supplied string to uppercase.
     *
     * @param str
     *   The string to be converted to uppercase.
     */
    static void toUpper(std::string &str);

    /**
     * Convert the supplied string to uppercase.
     *
     * @param str
     *   The string to be converted to uppercase.
     */
    static void toLower(std::string &str);

    /**
     * Splits a string around matches of the supplied delimiters
     *
     * @param str
     *   The delmited string to be split
     *
     * @param delimiters
     *   Zero or more characters to be used as delimiters. If the
     *   delimiter is a zero length string, the input string will
     *   not be split.
     *
     * @param discardEmptyStrings
     *   If true, empty strings will not be included in the result
     *   If false, empty strings may be included in the result
     *
     * @return
     *   Returns a vector of strings computed by splitting the
     *   supplied string around matches of the supplied delimiters.
     *
     * @code
     *   StrUtil::split("a,b,,c,", ",", false);
     *   // result will be: { "a", "b", "", "c", "" }
     *
     *   StrUtil::split("a,b,,c,", ",", true);
     *   // result will be: { "a", "b", "c" }
     *
     *   StrUtil::split("hi,mom;hi,\n;dad", ",\n;", false);
     *   // result will be: {"hi", "mom", "hi", "", "", "dad"}
     *
     *   StrUtil::split("hi,mom;hi,\n;dad", ",\n;", true);
     *   // result will be: {"hi", "mom", "hi", "dad"}
     * @endcode
     */
    static std::vector<std::string> split(std::string str,
                                          std::string delimiters = ",",
                                          bool        discardEmptyStrings = false);
};

}
}

#endif


