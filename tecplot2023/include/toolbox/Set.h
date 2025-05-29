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
#ifndef TECPLOT_TOOLBOX_SET_H
#define TECPLOT_TOOLBOX_SET_H

#if defined MSWIN
#pragma once
#endif

#include "ThirdPartyHeadersBegin.h"
#  include <string>
#  include <set>
#  include <iosfwd>
#include "ThirdPartyHeadersEnd.h"

#include "tptoolbox.h"
#include "tptoolbox_Exports.h"

namespace tecplot
{
namespace toolbox
{

/**
 * Set should be used with TecUtil functions that use
 * the Set_pa type. Some TecUtil functions allocate a
 * Set_pa in which case you will need to use TecUtilSetDealloc
 * to free the memory.
 */
class tptoolbox_API Set
{
public:
    /**
     * Create a Set object.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the Set could not be allocated.
     */
    Set();

    /**
     * Create a Set object.
     *
     * @param member
     *   The value with which to seed the Set object.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the Set could not be allocated.
     */
    explicit Set(SetIndex_t member);

    /**
     * Create a Set object.
     *
     * @param strValue
     *   strValue must have the form: "[s[,s][,s]...]"
     *   where s can be one of:  'n' or 'n-m' or 'n-m:l'
     *   And where: n >= 1
     *              m > n
     *              l >= 1 (l is a skip factor)
     *   e.g.
     *     "[1,4,7]"      - 1,4,7
     *     "[1-5]"        - 1,2,3,4,5
     *     "[1,4,7-13:2]" - 1,4,7,9,11,13
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the Set could not be allocated.
     *
     * @throw InvalidArgumentException
     *   InvalidArgumentException is thrown if strValue does not conform to
     *   the specified rules.
     *
     * @sa
     *   Set::assign, Set::toString
     */
    explicit Set(std::string strValue);

    /**
     * Create a Set object from a std::set<int>
     *
     * @param stdSet
     *    std::set<int> object
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the Set could not be allocated.
     *
     * @throw InvalidArgumentException
     *   InvalidArgumentException is thrown if stdSet contains items that are
     *   not valid set members. See isValidMember() for more info.
     *
     * @sa
     *   Set::assign, Set::toString
     */
    explicit Set(std::set<int> const& stdSet);

    /**
     * Create a Set object by making a copy of the given set.  The client is responsible for
     * deallocating the Set_pa.  
     *
     * @param set
     *   A preallocated Set_pa with which, to populate the Set object.
     *   If this parameter is NULL an empty Set will be created.
     *
     * @code
     *   Set_pa zoneSet;
     *   TecUtilZoneGetEnabled(&zoneSet);
     *   Set enabledZones(zoneSet);
     *   TecUtilSetDealloc(&zoneSet);
     *   ... do something with enabledZones ...
     * @endcode
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the Set could not be allocated.
     *
     * @sa
     *   TecUtilZoneGetEnabled, TecUtilZoneGetActive, Set::assign(Set_pa)
     */
    explicit Set(Set_pa set);

    /**
     * Takes ownership of the given set and will deallocate it on destruction.
     *
     * @param set
     *   A preallocated Set_pa which becomes managed by the Set object.
     *
     * @code
     *   Set_pa zoneSet;
     *   TecUtilZoneGetEnabled(&zoneSet);
     *   Set enabledZones(Set::Take(zoneSet));
     *   ... do something with enabledZones ...
     *   ... Don't call TecUtilSetDealloc(&zoneSet)!!!...
     * @endcode
     *
     * @sa
     *   TecUtilZoneGetEnabled, TecUtilZoneGetActive
     */
    static Set Take(Set_pa set);

    /**
     * Create a Set object.
     *
     * @param set
     *   A Set object you want to copy
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the Set could not be allocated.
     */
    Set(const Set& set);

    virtual ~Set();

    /**
     * Query if the set contains the same members as the specified set.
     *
     * @param rhs
     *   The set on the right hand side to compare with
     *
     * @return
     *   @c true if the set contains exactly the same members as the specified
     *   set. @c false otherwise.
     *
     * @sa
     *   isEqual
     */
    virtual bool operator==(const Set& rhs) const;

    /**
     * Query if the set is different from the specified set.
     *
     * @param rhs
     *   The set on the right hand side to compare with
     *
     * @return
     *   @c true if the sets are different. @c false otherwise.
     *
     * @sa
     *   isEqual
     */
    virtual bool operator!=(const Set& rhs) const;


    /**
     * Assign a single member to the Set
     *
     * @param rhs
     *   A SetIndex_t which is greater than or equal to 1.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the Set could not be expanded.
     */
    virtual Set& operator=(SetIndex_t rhs);

    /**
     * Copy the members of one set to the set
     *
     * @param rhs
     *   A set object that contains zero or more members
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the Set could not be expanded.
     */
    virtual Set& operator=(const Set& rhs);

    /**
     * Assign set members from a string representation of
     * a set.
     *
     * @param rhs
     *   As string that represents a valid set. See Set::assign.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the Set could not be expanded.
     *
     * @throw InvalidArgumentException
     *   InvalidArgumentException is thrown if strValue does not conform to
     *   the specified rules.
     *
     * @sa
     *   Set::assign, Set::toString
     */
    virtual Set& operator=(const std::string rhs);

    /**
     * Add a member to the set
     *
     * @param rhs
     *   Member to add to the set. Member must be greater than or equal to 1.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the Set could not be expanded.
     */
    virtual Set& operator+=(SetIndex_t rhs);

    /**
     * Add the members of one set to the set
     *
     * @param rhs
     *   The set which to add to the current set.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the Set could not be expanded.
     */
    virtual Set& operator+=(const Set& rhs);

    /**
     * Add set members from a string representation of
     * a set.
     *
     * @param rhs
     *   As string that represents a valid set. See Set::assign.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the Set could not be expanded.
     *
     * @throw InvalidArgumentException
     *   InvalidArgumentException is thrown if strValue does not conform to
     *   the specified rules.
     *
     * @sa
     *   Set::assign, Set::toString
     */
    virtual Set& operator+=(const std::string rhs);

    /**
     * Remove a member from the set
     *
     * @param member
     *   Member to remove from the set.
     */
    virtual Set& operator-=(SetIndex_t member);

    /**
     * Remove the members of one set from the set
     *
     * @param rhs
     *   The set identifying the members to remove from the current set.
     */
    virtual Set& operator-=(const Set& rhs);

    /**
     * Remove set members from a string representation of
     * a set.
     *
     * @param rhs
     *   As string that represents a valid set. See Set::assign.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if there is not enough memory
     *   to perform this operation.
     *
     * @throw InvalidArgumentException
     *   InvalidArgumentException is thrown if strValue does not conform to
     *   the specified rules.
     *
     * @sa
     *   Set::assign, Set::toString
     */
    virtual Set& operator-=(const std::string rhs);

    /**
     * Add a member to the set
     *
     * @param member
     *   Member to add to the set. Member must be greater than or equal to 1.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the Set could not be expanded.
     */
    virtual void add(SetIndex_t member);

    /**
     * Add the members of one set to the set
     *
     * @param set
     *   The set which to add to the current set.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the Set could not be expanded.
     */
    virtual void add(const Set& set);

    /**
     * Remove a member from the set
     *
     * @param member
     *   Member to remove from the set.
     */
    virtual void remove(SetIndex_t member);

    /**
     * Remove the members of one set from the set
     *
     * @param set
     *   The set which to remove the members from the current set.
     */
    virtual void remove(const Set& set);

    /**
     * Remove all members from the set
     */
    virtual void clear();

    /**
     * Releases ownership of it's current Set_pa and takes ownership of the supplied Set_pa
     * 
     * @param set
     *   The Set_pa which should now be owned by this instance
     *
     * @sa Set::Take()
     */
    virtual void reset(Set_pa set);

    /**
     * Copy the members of one set to the set
     *
     * @param set
     *   A set object that contains zero or more members
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the Set could not be expanded.
     */
    virtual void assign(const Set& set);
    virtual void assign(Set_pa set);

    /**
     * Assign set members from a string representation of
     * a set.
     *
     * @param strValue
     *   strValue must have the form: "[s[,s][,s]...]"
     *   where s can be one of:  'n' or 'n-m' or 'n-m:l'
     *   And where: n >= 1
     *              m > n
     *              l >= 1 (l is a skip factor)
     *   e.g.
     *     "[1,4,7]"      - 1,4,7
     *     "[1-5]"        - 1,2,3,4,5
     *     "[1,4,7-13:2]" - 1,4,7,9,11,13
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the Set could not be expanded.
     *
     * @throw InvalidArgumentException
     *   InvalidArgumentException is thrown if strValue does not conform to
     *   the specified rules.
     *
     * @sa
     *   Set::toString
     */
    virtual void assign(std::string strValue);

    /**
     * Create a std::set<int> representation of the set contents.
     *
     * @param stdSet
     *   std set<int> to which the member of the set will be assigned.
     *
     * @sa
     *  Set::assign
     */
    virtual void toStdSet(std::set<int>& stdSet);

    /**
     * Create a string representation of the set contents. The resulting
     * string will be in the form as described by Set::assign(std::string strValue).
     *
     * @return
     *   A string representation of the set contents.
     *
     * @sa
     *   Set::assign
     */
    virtual std::string toString() const;

    /**
     * Get a reference to the Set_pa that is attached to
     * the Set object.
     *
     * @return
     *   A reference to the Set_pa owned by the Set object.
     *   This reference must not be deallocated.
     */
    virtual Set_pa getRef() const;

    /**
     * Get the member at the specified position in the set.
     *
     * @param position
     *   The position in the set. position must be >= 1.
     *
     * @return
     *   The member at the specified position. If a member does not exist at
     *   the specified position the return value is TECUTILSETNOTMEMBER
     *
     * @note
     *   Avoid using this method for looping over a Set.  Use Set::getNext() instead.  This method
     *   must iterate over the set to find the member, making iteration over a set using this method
     *   an N^2 operation.
     */
    virtual SetIndex_t get(SetIndex_t position) const;

    /**
     * Get the number of members in the set.  To determine if a set is empty, prefer using 
     * Set::isEmpty.
     *
     * @return
     *   The count of the number of member in the set.
     *
     * @sa
     *   Set::isEmpty
     */
    virtual SetIndex_t size() const;

    /**
     * Get the next member in the set.
     *
     * @param member
     *   The member after which to return the next member. Use TECUTILSETNOTMEMBER
     *   to get the first member of the set.
     *
     * @return
     *   The member in the set following the specified member. If the specified member
     *   is not found or is the last member of the set, the return value is
     *   TECUTILSETNOTMEMBER
     *
     * @sa
     *   SET_FOREACH
     */
    virtual SetIndex_t getNext(SetIndex_t member) const;

    /**
     * Get the previous member in the set.
     *
     * @param member
     *   The member before which to return the next member. Use TECUTILSETNOTMEMBER
     *   to get the last member of the set.
     *
     * @return
     *   The member in the set preceeding the specified member. If the specified member
     *   is not found or is the first member of the set, the return value is
     *   TECUTILSETNOTMEMBER
     */
    virtual SetIndex_t getPrevious(SetIndex_t member) const;

    /**
     * Get the position of the member within the set
     *
     * @param member
     *   The member for which to get the position.
     *
     * @return
     *   The position of the specified member. If the member is not found,
     *   the return value will be TECUTILSETNOTMEMBER
     */
    virtual SetIndex_t getPosition(SetIndex_t member) const;

    /**
     * Calculate the intersection of two sets
     *
     * @param set1
     *   The first Set to use
     *
     * @param set2
     *   The second Set to use
     *
     * @return
     *   A Set containing the intersection of the two Sets.
     *
     * @throw OutOfMemoryException
     *   OutOfMemoryException is thrown if the resulting Set could
     *   not be created.
     */
    static Set intersection(const Set& set1, const Set& set2);

    /**
     * Query if the set has any members.
     *
     * @return
     *   Returns @c true if the set has no members, @c false if the set contains members
     */
    virtual bool isEmpty() const;

    /**
     * Query if the set contains the same member as the specified set.
     *
     * @param other
     *   The other set to compare with.
     *
     * @return
     *   @c true if the set contains exactly the same members as the specified
     *   set. @c false otherwise.
     */
    virtual bool isEqual(const Set& other) const;

    /**
     * Query if a member is a part of the set.
     *
     * @param member
     *   The member to look for in the set. member must be >= 1
     *
     * @return
     *   @c true if the member is contained in the set. @c false otherwise.
     */
    virtual bool isMember(SetIndex_t member) const;

    /**
     * Determine if a member number is valid
     *
     * @return
     *   @c true if the member number is valid, @c false otherwise.
     */
    virtual bool isValidMember(SetIndex_t member) const
    {
        return member >= 1;
    }

    /**
     * Determine if a position index is valid
     *
     * @return
     *   @c true if the position index is valid, @c false otherwise
     */
    virtual bool isValidPosition(SetIndex_t position) const
    {
        return position >= 1;
    }

private:
    /**
     */
    void alloc();

    /**
     */
    void dealloc();

    /**
     */
    void copy(Set_pa set);

    /**
     * This constructor is for Set::Take().  Since we already have a Set_pa constructor
     * we needed something to distinguish the two, hence the dummy enum
     */
    enum NoAlloc {DoNotAllocate};
    Set(Set_pa set, NoAlloc);

    Set_pa m_set;
};

/**
 *  Stream insertion operator
 */
tptoolbox_API std::ostream& operator<<(std::ostream& outputStream,
                                       const Set&    set);


/**
 * SetForEachHelper is declared within a single-shot for-loop below to ensure that it's in
 * a tight scope with the loop.  If we didn't do this we couldn't call two SET_FOREACH loops
 * in the same scope because we'd get double declaration of the SetForEachHelper variable.
 */
template <typename T>
struct SetForEachHelper
{
    SetIndex_t m_v;
    Set m_set;
    bool m_break;

    SetForEachHelper(Set const& set) 
        : m_v(TECUTILSETNOTMEMBER)
        , m_set(set)
        , m_break(false) 
    {}

    T getNext() 
    {
        m_v = m_set.getNext(m_v);
        return static_cast<T>(m_v);
    }

    bool doContinue() 
    {
        return m_v != TECUTILSETNOTMEMBER;
    }
};

/**
 * SET_FOREACH will iterate over the contents of a Set.  It will also typecast for you.
 *
 * @code
 *  EntIndex_t member; // Typecast is handled for you.
 *  SET_FOREACH(member, someSet)
 *  {
 *    // Do something with member
 *  }
 * @endcode
 */
#define SET_FOREACH(member,set) \
    for (tecplot::toolbox::SetForEachHelper<decltype(member)> helper(set); !helper.m_break; helper.m_break = true) \
        for ((member) = helper.getNext(); helper.doContinue(); (member) = helper.getNext())

}
}

#endif
