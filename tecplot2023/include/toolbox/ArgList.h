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
#ifndef TECPLOT_TOOLBOX_ARGLIST_H
#define TECPLOT_TOOLBOX_ARGLIST_H

#if defined MSWIN
#pragma once
#endif

#include <string>
#include <list>
#include <vector>
#include "tptoolbox.h"
#include "tptoolbox_Exports.h"

namespace tecplot { namespace toolbox {
class Set;
class StringList;
}}

namespace tecplot
{
namespace toolbox
{

/**
 * ArgList should be used with TecUtil functions
 * that take the ArgList_pa type.
 */
class tptoolbox_API ArgList
{
public:
    /**
     * Create an empty ArgList object.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the ArgList could not be allocated.
     */
    ArgList();
    virtual ~ArgList();

    /**
     * Append an ArbParam_t to an argument list.
     *
     * @param name
     *   Parameter name. Use the SV_constants listed with the extended function.
     *
     * @param value
     *   ArbParam_t associated with the parameter name.
     *
     * @return
     *   Returns @c true if append is successful, @c false otherwise.
     */
    virtual bool appendArbParam(std::string name,
                                ArbParam_t  value);

    /**
     * Append an ArbParam_t pointer to an argument list.
     *
     * @param name
     *   Parameter name. Use the SV_constants listed with the extended function.
     *
     * @param value
     *   ArbParam_t pointer associated with the parameter name.
     *
     * @return
     *   Returns @c true if append is successful, @c false otherwise.
     */
    virtual bool appendArbParamPtr(std::string name,
                                   ArbParam_t* value);

    /**
     * Append a 64 bit signed integer array to an argument list.
     *
     * @param name
     *   Parameter name. Use the SV_constants listed with the extended function.
     *
     * @param int64Array
     *   Array of data associated with the parameter name.
     *
     * @return
     *   Returns @c true if append is successful, @c false otherwise.
     */
    virtual bool appendInt64Array(std::string    name,
                                  const int64_t* int64Array);

    /**
     * Append an array to an argument list.
     *
     * @param name
     *   Parameter name. Use the SV_constants listed with the extended function.
     *
     * @param data
     *   Array of data associated with the parameter name.
     *
     * @return
     *   Returns @c true if append is successful, @c false otherwise.
     */
    virtual bool appendArray(std::string name,
                             const void* data);

    /**
     * Append an array to an argument list.
     *
     * @param name
     *   Parameter name. Use the SV_constants listed with the extended function.
     *
     * @param data
     *   Array of data associated with the parameter name.
     *
     * @return
     *   Returns @c true if append is successful, @c false otherwise.
     */
    template<typename T>
    bool appendArray(std::string name,
                     const T*    data);

    /**
     * Append a vector to an argument list. Can be used in place of appendArray.
     *
     * @param name
     *   Parameter name. Use the SV_constants listed with the extended function.
     *
     * @param data
     *   Vector of data associated with the parameter name.
     *   Tecplot does not make a copy of the vector so the client 
     *   must maintain the vector until the ArgList is cleared or deallocated 
     *   at which point the vector can be deallocated.
     *
     * @return
     *   Returns @c true if append is successful, @c false otherwise.
     */
    template<typename T>
    bool appendArray(std::string           name,
                     const std::vector<T>& data);

    /**
     * Append a double to an argument list.
     *
     * @param name
     *   Parameter name. Use the SV_constants listed with the extended function.
     *
     * @param value
     *   Value associated with the parameter name.
     *
     * @return
     *   Returns @c true if append is successful, @c false otherwise.
     */
    virtual bool appendDouble(std::string name,
                              double      value);

    /**
     * Append a double pointer to an argument list.
     *
     * @param name
     *   Parameter name. Use the SV_constants listed with the extended function.
     *
     * @param value
     *   Double pointer associated with the parameter name.
     *
     * @return
     *   Returns @c true if append is successful, @c false otherwise.
     */
    virtual bool appendDoublePtr(std::string name,
                                 double*     value);

    /**
     * Append a function to an argument list.
     *
     * @param name
     *   Parameter name. Use the SV_constants listed with the extended function.
     *
     * @param function
     *   Function pointer associated with the parameter name.
     *
     * @return
     *   Returns @c true if append is successful, @c false otherwise.
     */
    virtual bool appendFunction(std::string name,
                                const void* function);

    /**
     * Append an integer value to an argument list.
     *
     * @param name
     *   Parameter name. Use the SV_constants listed with the extended function.
     *
     * @param value
     *   Value associated with the parameter name.
     *
     * @return
     *   Returns @c true if append is successful, @c false otherwise.
     */
    virtual bool appendInt(std::string name,
                           LgIndex_t   value);

    /**
     * Append a set to an argument list.
     *
     * @param name
     *   Parameter name. Use the SV_constants listed with the extended function.
     *
     * @param set
     *   Set associated with the parameter name.
     *
     * @return
     *   Returns @c true if append is successful, @c false otherwise.
     */
    virtual bool appendSet(std::string name,
                           const Set&  set);

    /**
     * Append a string to an argument list.
     *
     * @param name
     *   Parameter name. Use the SV_constants listed with the extended function.
     *
     * @param value
     *   String value associated with the parameter name.
     *
     * @return
     *   Returns @c true if append is successful, @c false otherwise.
     */
    virtual bool appendString(std::string name,
                              std::string value);

    /**
     * Append a named string list to an argument list.
     *
     * @param name
     *   Parameter name. Use the SV_constants listed with the extended function.
     *
     * @param stringList
     *   String List associated with the parameter name.
     *
     * @return
     *   Returns @c true if append is successful, @c false otherwise.
     */
    virtual bool appendStringList(std::string       name,
                                  const StringList& stringList);

    /**
     * Removes all entries in the argument list.
     */
    virtual void clear();

    /**
     * Get the ArgList_pa which is managed by this object. The ArgList_pa
     * type is used with many TecUtil functions. ArgList_pa must not be deallocated.
     *
     * @return
     *   Returns a reference to the ArgList_pa type.
     */
    virtual ArgList_pa getRef() const;
private:
    ArgList_pa            m_argList;
    std::list<StringList> m_stringLists;
    std::list<Set>        m_sets;

    ArgList(const ArgList&);
    ArgList& operator=(const ArgList&);
};

/**
 */
template<typename T>
bool ArgList::appendArray(std::string name,
                          const T*    data)
{
    REQUIRE(!name.empty());
    REQUIRE(VALID_REF(data));

    Boolean_t result = TecUtilArgListAppendArray(m_argList,
                                                 name.c_str(),
                                                 reinterpret_cast<const void*>(data));

    ENSURE(VALID_BOOLEAN(result));
    return (result == TRUE);
}

/**
 */
template<typename T>
bool ArgList::appendArray(std::string           name,
                          const std::vector<T>& data)
{
    REQUIRE(!name.empty());
    REQUIRE(!data.empty());

    Boolean_t result = TecUtilArgListAppendArray(m_argList,
                                                 name.c_str(),
                                                 reinterpret_cast<const void*>(&data[0]));

    ENSURE(VALID_BOOLEAN(result));
    return (result == TRUE);
}

}
}

#endif
