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

#pragma once

#include "ThirdPartyHeadersBegin.h"
#  include <list>
#  include <string>
#include "ThirdPartyHeadersEnd.h"

#include "tptoolbox.h"
#include "tptoolbox_Exports.h"

/*
 * Baffled as to why we need this include instead of a forward declaration -bdp.
 */
#include "OutOfMemoryException.h"

namespace tecplot { namespace toolbox {

/**
 * StringList maintains a list of strings. This class should be
 * used with TecUtil functions that use the StringList_pa type.
 */
class tptoolbox_API StringList
{
public:
    /**
     * Creates a StringList from the contents of an STL list
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the StringList could not be allocated or expanded.
     */
    static StringList fromStdList(std::list<std::string> const& stdList);

    /**
     * Create an empty StringList object
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the StringList could not be allocated.
     */
    StringList();

    /**
     * Create a StringList object
     *
     * @param stringList
     *   A valid StringList_pa which contains zero or more items.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the StringList could not be allocated.
     *
     * @sa
     *   TecUtilStringListDealloc
     */
    explicit StringList(StringList_pa stringList);

    /**
     * Create a StringList object
     *
     * @param stringList
     *   A valid StringList object which contains zero or more items.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the StringList could not be allocated.
     */
    StringList(const StringList& stringList);

    /**
     * Create a StringList object from a simple C array of strings with the last entry containing a
     * NULL pointer.
     *
     * @param list
     *   Pointer to an array of simple C strings terminated by a NULL pointer.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the StringList could not be allocated.
     */
    StringList(char const* const* list);

    /**
     * Create a StringList object
     *
     * @param first
     *   The first of any number of strings. The final string must be NULL
     *   to signify the final string.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the StringList could not be allocated.
     */
    explicit StringList(const char* first,
                        ...);

    /**
     * Constructs a StringList object from two iterators identifying the begin() and end() of a
     * container of strings.
     */
    template <typename InputIt>
    StringList(
        InputIt first,
        InputIt last)
        : m_stringList(NULL)
    {
        try
        {
            alloc();
            for (InputIt it = first; it != last; ++it)
                append(*it);
        }
        catch (OutOfMemoryException const&)
        {
            dealloc();
            throw;
        }
    }

    virtual ~StringList(); // the StringList_pa will be deallocated in the destructor

public:
    virtual bool operator==(const StringList& rhs) const;
    virtual bool operator!=(const StringList& rhs) const;

    /**
     * Append the contents of the supplied StringList to
     * this StringList.
     *
     * @param stringList
     *   stringList may contain zero or more items.
     *
     * @return
     *   @c true if the contents of stringList were appended,
     *   @c false otherwise.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the StringList could not be expanded.
     */
    virtual void append(const StringList& stringList);

    /**
     * Append a string to the string list.
     *
     * @param strValue
     *   String to append. This value may be NULL or have zero length.
     *
     * @return
     *   @c true if the string was appended, @c false otherwise
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the StringList could not be expanded.
     */
    virtual void append(const char* strValue);
    virtual inline void append(std::string strValue)
    {
        append(strValue.c_str());
    }

    /**
     * Remove all contents of the string list
     */
    virtual void clear();

    /**
     * Copy the contents of the specified string list to
     * this string list
     *
     * @param stringList
     *   The string list you want to copy. The string list may
     *   contain zero or more items.
     *
     * @return
     *   @c true if the items were copied, @c false otherwise.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the StringList could not be expanded.
     */
    virtual void assign(const StringList& stringList);

    /**
     * Split the supplied string according to the delimiters
     * and populate the StringList with the results.
     *
     * @param strValue
     *   The delimited string
     *
     * @param delimiters
     *   A set of zero or more characters representing the possible
     *   delimiters.
     *
     * @param discardEmptyStrings
     *   Set to 'true' if you do not want empty strings included in the resulting
     *   StringList. Empty strings may be created when consecutive delimiters
     *   exist.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the StringList could not be expanded.
     *
     * @sa
     *   StrUtil::split
     */
    virtual void assign(const char* strValue,
                        const char* delimiters          = ",",
                        bool        discardEmptyStrings = false);
    virtual inline void assign(std::string strValue,
                               std::string delimiters          = ",",
                               bool        discardEmptyStrings = false)
    {
        assign(strValue.c_str(), delimiters.c_str(), discardEmptyStrings);
    }

    /**
     * Insert a copy of the string into the nth position of the string list.
     * The string list is shifted and expanded to accomodate the new string.
     *
     * @param index
     *   The position at which to place the new string. index must be >= 1
     *   and <= StringList::size().
     *
     * @param strValue
     *   The string to insert into the string list. This value may be NULL.
     *
     * @return
     *   @c true if the string was inserted, @c false otherwise.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the StringList could not be expanded.
     */
    virtual void insert(LgIndex_t   index,
                        const char* strValue);
    virtual inline void insert(LgIndex_t   index,
                               std::string strValue)
    {
        insert(index, strValue.c_str());
    }

    /**
     * Place a copy of the specified string at the nth position in the string list.
     * If the position is beyond the end of the string list, the string list is
     * resized, so that the string references between the last item of the string
     * list in its original state and the last item of the string list in its new
     * state are assigned NULL. If the position is within the boundaries of the
     * original string list, the string at the specified position is replaced by
     * the new value.
     *
     * @param index
     *   The position at which to set the string. index must be >= 1.
     *
     * @param strValue
     *   The string to insert into the string list. This value may be NULL.
     *
     * @return
     *   @c true if the string value was set, @c false otherwise.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if insufficient memory is available.
     */
    virtual void replace(LgIndex_t   index,
                         const char* strValue);
    virtual inline void replace(LgIndex_t   index,
                                std::string strValue)
    {
        replace(index, strValue.c_str());
    }

    /**
     * Remove the string at the specified index. The members
     * following the removed item are shifted to fill the vacated
     * space.
     *
     * @param index
     *   Index at which you want to remove the string. index must be >= 1
     *   and <= StringList::size()
     */
    virtual void remove(LgIndex_t index);

    /**
     * Remove the specified number of strings beginning at the nth string. The
     * members following the items removed are shifted to fill the vacated space.
     *
     * @param index
     *   Start position in the string list. index must be >= 1 and <= StringList::size().
     *
     * @param count
     *   Number of items to remove from the string list. count must be >= 1,
     *   and less than or equal to the number of items remaining,
     *   including the string at the start position.
     */
    virtual void remove(LgIndex_t index,
                        LgIndex_t count);


    /**
     * Sorts the string list by repeatedly calling the 'Comparator' function until
     * the list is in order. Default parameters may be used to use the default
     * sorting algorithm.
     *
     * @param comparator
     *     Function called to compare two string list strings. Use NULL for the
     *     default sort. The default sorting handles NULL elements and uses the
     *     system's strcmp utility for comparing valid strings elements.
     *
     * @param clientData
     *     Contextual information that is passed along to the comparator function.
     *     Client data isn't used by the default comparator and can be passed any
     *     value. For specialized comparator functions the client data is used to
     *     hold contextual information so that global variable do not have to be
     *     used.
     */
    virtual void sort(const StringListStringComparator_pf comparator = NULL,
                      const ArbParam_t                    clientData = 0);


    /**
     * Count the number of strings currently maintained by the string list.
     *
     * @return
     *   The number of strings maintained by the string list.
     */
    virtual LgIndex_t size() const;

    /**
     * Return a reference to the nth string in a string list.
     *
     * @param index
     *   Position of string for which to obtain a reference. index must be >= 1 and <= StringList::size()
     *
     * @return
     *   Returns a REFERENCE to the string. DO NOT DEALLOCATE THIS REFERENCE. This value may be NULL.
     */
    virtual const char* getStringRef(LgIndex_t index) const;

    /**
     * Return a copy of the the nth string in a string list.
     *
     * @param index
     *   Position of string for which to obtain a copy. index must be >= 1 and <= StringList::size()
     *
     * @return
     *   Returns a copy of the string. An empty string is returned if the requested string is NULL.
     */
    virtual std::string get(LgIndex_t index) const;

    /**
     * Convert a StringList to a single string, using the specified delimiter.
     *
     * @param delimiters
     *   A series of zero or more characters to be used as a delimiter
     *   between the strings of a StringList.
     *
     * @return
     *   A single string which is the contatenation of all the strings
     *   in the StringList, separated by the supplied delimiter string.
     */
    virtual std::string toString(const char* delimiters) const;
    virtual inline std::string toString(std::string delimiters) const
    {
        return toString(delimiters.c_str());
    }

    /**
     * Get a reference to the StringList_pa maintained by the StringList object.
     * This type is required when calling certain TecUtil functions such as
     * TecUtilDataSetCreate. This reference must not be deallocated.
     *
     * @return
     *   A reference to the StringList_pa maintained by the StringList object.
     *   Do not deallocate this item.
     *
     * @sa
     *   TecUtilDataSetCreate, TecUtilReadDataSet
     */
    virtual StringList_pa getRef() const;

private:
    void alloc();
    void dealloc();

    StringList_pa m_stringList;

    /**
     * Hide the assignment operator so the compiler doesn't
     * create one for us.
     */
    StringList& operator = (const StringList& other);
};

}} // nameapce tecplot::toolbox
