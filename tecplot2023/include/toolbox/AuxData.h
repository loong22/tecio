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
#ifndef TECPLOT_TOOLBOX_AUXDATA_H
#define TECPLOT_TOOLBOX_AUXDATA_H

#if defined MSWIN
#pragma once
#endif

#include <string>
#include "tptoolbox.h"
#include "tptoolbox_Exports.h"

namespace tecplot
{
namespace toolbox
{

class Set;

/**
 * The AuxData class may be used in place of TecUtilAuxData functions.
 */
class tptoolbox_API AuxData
{
public:
    /**
     * Create an AuxData object to manipulate AuxData
     *
     * @param auxDataLocation
     *   A valid AuxDataLocation_e must be passed.
     *
     * @param zoneOrMapOrVariable
     *   The Zone, Map, or Variable number associated with the
     *   auxDataLocation. This parameter is required if auxDataLocation
     *   is one of:
     *         AuxDataLocation_Zone
     *         AuxDataLocation_Var
     *         AuxDataLocation_LineMap
     *   Note that this object maintains the same zone, map, variable number
     *   regardless of whether new zones/maps/variables are added or deleted.
     *   So let's look at the following situation:
     *     1) You have a dataset with 3 zones.
     *     2) You create an AuxData object - AuxData(AuxDataLocation_Zone, 2);
     *     3) Zone 1 is deleted. The zone numbers are shifted so zone 2 becomes zone 1
     *        and zone 3 becomes zone 2.
     *     4) The AuxData object created in step 2 will continue to affect zone 2,
     *        which is what used to be zone 3.
     *
     * @sa
     *   AuxData::isValid
     */
    AuxData(const AuxDataLocation_e auxDataLocation,
            const EntIndex_t        zoneOrMapOrVariable = 1);
    virtual ~AuxData();

    /**
     * Determine if the AuxData object is valid. An AuxData object may
     * become invalid if the AuxDataLocation is Zone,LineMap, or Variable
     * and the assigned Zone, LineMap, or Variable no longer exists.
     * If the AuxDataLocation is DataSet, the object will be invalid for
     * frames without a dataset.
     *
     * @return
     *   @c true if the object is valid, @c false otherwise.
     */
    virtual bool isValid() const;

    /**
     * The AuxData_pa reference associated with this item.
     *
     * @return
     *   Reference to the auxiliary data associated with this object.
     */
    virtual AuxData_pa getRef() const;

    /**
     * Set an AuxData name value pair in Tecplot.
     *
     * @par Note:
     *   Tecplot must be locked when using this method.
     *
     * @param name
     *   Name of the AuxData. A valid name must start with an '_' or
     *   alpha character and may be followed by one or more '_', '.',
     *   alpha or digit characters.
     *
     * @param value
     *   String value to set for the item
     *
     * @param retain
     *   A flag specifying whether or not to retain this item on file export,
     *   such as to a Tecplot datafile.
     *
     * @return
     *   @c true if the value was set, @c false otherwise.
     */
    virtual bool set(std::string name,
                     const char* value,
                     bool        retain = true) const;
    virtual inline bool set(std::string name,
                            std::string value,
                            bool        retain = true) const
    {
        return set(name, value.c_str(), retain);
    }

    /**
     * Set an AuxData name value pair in Tecplot.
     *
     * @par Note:
     *   Tecplot must be locked when using this method.
     *
     * @par Note:
     *   AuxData can only handle strings, so the LgIndex_t value
     *   will be stored internally as a string.
     *
     * @param name
     *   Name of the AuxData. A valid name must start with an '_' or
     *   alpha character and may be followed by one or more '_', '.',
     *   alpha or digit characters.
     *
     * @param value
     *   LgIndex_t value to set for the item
     *
     * @param retain
     *   A flag specifying whether or not to retain this item on file export,
     *   such as to a Tecplot datafile.
     *
     * @return
     *   @c true if the value was set, @c false otherwise.
     */
    virtual bool set(std::string name,
                     LgIndex_t   value,
                     bool        retain = true) const;

    /**
     * Set an AuxData name value pair in Tecplot.
     *
     * @par Note:
     *   Tecplot must be locked when using this method.
     *
     * @par Note:
     *   AuxData can only handle strings, so the LgIndex_t value
     *   will be stored internally as a string.
     *
     * @param name
     *   Name of the AuxData. A valid name must start with an '_' or
     *   alpha character and may be followed by one or more '_', '.',
     *   alpha or digit characters.
     *
     * @param value
     *   int value to set for the item
     *
     * @param retain
     *   A flag specifying whether or not to retain this item on file export,
     *   such as to a Tecplot datafile.
     *
     * @return
     *   @c true if the value was set, @c false otherwise.
     */
    virtual bool set(std::string name,
                     int         value,
                     bool        retain = true) const;

    /**
     * Set an AuxData name value pair in Tecplot.
     *
     * @par Note:
     *   Tecplot must be locked when using this method.
     *
     * @par Note:
     *   AuxData can only handle strings, so the double value
     *   will be stored internally as a string. Some precision
     *   may be lost.
     *
     * @param name
     *   Name of the AuxData. A valid name must start with an '_' or
     *   alpha character and may be followed by one or more '_', '.',
     *   alpha or digit characters.
     *
     * @param value
     *   Double value to set for the item
     *
     * @param retain
     *   A flag specifying whether or not to retain this item on file export,
     *   such as to a Tecplot datafile.
     *
     * @return
     *   @c true if the value was set, @c false otherwise.
     */
    virtual bool set(std::string name,
                     double      value,
                     bool        retain = true) const;

    /**
     * Set an AuxData double value with formatting in Tecplot.
     *
     * @par Note:
     *   Tecplot must be locked when using this method.
     *
     * @par Note:
     *   AuxData can only handle strings, so the double value
     *   will be stored internally as a string.
     *
     * @param name
     *   Name of the AuxData. A valid name must start with an '_' or
     *   alpha character and may be followed by one or more '_', '.',
     *   alpha or digit characters.
     *
     * @param value
     *   Double value to set for the item
     *
     * @param format
     *   Format string to apply to the double to string conversion. Can be used
     *   to set a custom precision value.
     *
     * @param retain
     *   A flag specifying whether or not to retain this item on file export,
     *   such as to a Tecplot datafile.
     *
     * @return
     *   @c true if the value was set, @c false otherwise.
     */
    virtual bool set(std::string name,
                     double      value,
                     std::string format,
                     bool        retain = true) const;

    /**
     * Set an AuxData name value pair in Tecplot.
     *
     * @par Note:
     *   Tecplot must be locked when using this method.
     *
     * @par Note:
     *   AuxData can only handle strings, so the bool value
     *   will be stored internally as a string. The string values used
     *   are "true" and "false".
     *
     * @param name
     *   Name of the AuxData. A valid name must start with an '_' or
     *   alpha character and may be followed by one or more '_', '.',
     *   alpha or digit characters.
     *
     * @param value
     *   LgIndex_t value to set for the item
     *
     * @param retain
     *   A flag specifying whether or not to retain this item on file export,
     *   such as to a Tecplot datafile.
     *
     * @return
     *   @c true if the value was set, @c false otherwise.
     */
    virtual bool set(std::string name,
                     bool        value,
                     bool        retain = true) const;

    /**
     * Set an AuxData name value pair in Tecplot.
     *
     * @par Note:
     *   Tecplot must be locked when using this method.
     *
     * @par Note:
     *   AuxData can only handle strings, so the Set value
     *   will be stored internally as a string. See Set::toString
     *
     * @param name
     *   Name of the AuxData. A valid name must start with an '_' or
     *   alpha character and may be followed by one or more '_', '.',
     *   alpha or digit characters.
     *
     * @param value
     *   Set_t value to set for the item
     *
     * @param retain
     *   A flag specifying whether or not to retain this item on file export,
     *   such as to a Tecplot datafile.
     *
     * @return
     *   @c true if the value was set, @c false otherwise.
     */
    virtual bool set(std::string name,
                     const Set&  value,
                     bool        retain = true) const;

    /**
     * Get AuxData from Tecplot, by name.
     *
     * @param name
     *   Name of the AuxData item you wish to get. Non case-sensitive.
     *
     * @param value
     *   Pointer to one of the types supported by AuxData::getAt.
     *   These are: std::string*, double*, LgIndex_t*, bool*, Set*
     *
     * @param retain
     *   A bool pointer to retrieve the retain value. You may pass NULL
     *   if you do not wish to receive this value.
     *
     * @return
     *   @c true if the item could be found, @c false otherwise.
     */
    template <typename T>
    inline bool get(std::string name,
                    T*          value,
                    bool*       retain = NULL) const
    {
        REQUIRE(VALID_NON_ZERO_LEN_STR(name.c_str()));
        REQUIRE(VALID_REF(value));
        REQUIRE(VALID_REF_OR_NULL(retain));
        // NOTE: It would be more efficient to call TecUtilAuxDataGetItemByName()
        //       directly, but by going through these calls we consolodate
        //       code into one location, everything goes through getItemByIndex().
        //
        //       Even though we have an inefficiency here, the probability that
        //       getting auxdata is a bottleneck is pretty slim.
        int32_t index;
        bool result = getIndex(name, &index);
        if (result)
            get(index,
                value,
                NULL, // name
                retain);
        return result;
    }

    /**
     * Get AuxData from Tecplot, by index.
     *
     * @param index
     *   Index of the AuxData item you wish to get. index must be greater than
     *   or equal to 1 and less than or equal to AuxData::size()
     *
     * @param value
     *   A std::string pointer to receive a string representation of the value.
     *
     * @param name
     *   A std::string pointer to retrieve AuxData name associated with the index.
     *   You may pass NULL if you do not wish to receive this value.
     *
     * @param retain
     *   A bool pointer to retrieve the retain value. You may pass NULL
     *   if you do not wish to receive this value.
     */
    virtual void get(int32_t      index,
                     std::string* value,
                     std::string* name   = NULL,
                     bool*        retain = NULL) const;

    /**
     * Get AuxData from Tecplot, by index.
     *
     * @param index
     *   Index of the AuxData item you wish to get. index must be greater than
     *   or equal to 1 and less than or equal to AuxData::size()
     *
     * @param value
     *   A LgIndex_t pointer to receive the value. Note that AuxData is maintained
     *   as a string so the LgIndex_t value will be converted from a string.
     *
     * @param name
     *   A std::string pointer to retrieve AuxData name associated with the index.
     *   You may pass NULL if you do not wish to receive this value. If the
     *   conversion is not successful value will be zero.
     *
     * @param retain
     *   A bool pointer to retrieve the retain value. You may pass NULL
     *   if you do not wish to receive this value.
     */
    virtual void get(int32_t      index,
                     LgIndex_t*   value,
                     std::string* name   = NULL,
                     bool*        retain = NULL) const;

    /**
     * Get AuxData from Tecplot, by index.
     *
     * @param index
     *   Index of the AuxData item you wish to get. index must be greater than
     *   or equal to 1 and less than or equal to AuxData::size()
     *
     * @param value
     *   An int pointer to receive the value. Note that AuxData is maintained
     *   as a string so the int value will be converted from a string.
     *
     * @param name
     *   A std::string pointer to retrieve AuxData name associated with the index.
     *   You may pass NULL if you do not wish to receive this value. If the
     *   conversion is not successful value will be zero.
     *
     * @param retain
     *   A bool pointer to retrieve the retain value. You may pass NULL
     *   if you do not wish to receive this value.
     */
    virtual void get(int32_t      index,
                     int*         value,
                     std::string* name   = NULL,
                     bool*        retain = NULL) const;

    /**
     * Get AuxData from Tecplot, by index.
     *
     * @param index
     *   Index of the AuxData item you wish to get. index must be greater than
     *   or equal to 1 and less than or equal to AuxData::size()
     *
     * @param value
     *   A double pointer to receive the value. Note that AuxData is maintained
     *   as a string so the double value will be converted from a string. If the
     *   conversion is not successful value will be zero.
     *
     * @param name
     *   A std::string pointer to retrieve AuxData name associated with the index.
     *   You may pass NULL if you do not wish to receive this value.
     *
     * @param retain
     *   A bool pointer to retrieve the retain value. You may pass NULL
     *   if you do not wish to receive this value.
     */
    virtual void get(int32_t      index,
                     double*      value,
                     std::string* name   = NULL,
                     bool*        retain = NULL) const;

    /**
     * Get AuxData from Tecplot, by index.
     *
     * @param index
     *   Index of the AuxData item you wish to get. index must be greater than
     *   or equal to 1 and less than or equal to AuxData::size()
     *
     * @param value
     *   A bool pointer to receive the value. Note that AuxData is maintained
     *   as a string so the bool value will be converted from a string. If the
     *   string does not represent a boolean, value will default to @c false. Supported
     *   strings that represent a @c true result are (not case-sensitive):
     *         "TRUE", "T", "1", "ON", "YES"
     *
     * @param name
     *   A std::string pointer to retrieve AuxData name associated with the index.
     *   You may pass NULL if you do not wish to receive this value.
     *
     * @param retain
     *   A bool pointer to retrieve the retain value. You may pass NULL
     *   if you do not wish to receive this value.
     */
    virtual void get(int32_t      index,
                     bool*        value,
                     std::string* name = NULL,
                     bool*        retain = NULL) const;

    /**
     * Get AuxData from Tecplot, by index.
     *
     * @param index
     *   Index of the AuxData item you wish to get. index must be greater than
     *   or equal to 1 and less than or equal to AuxData::size()
     *
     * @param value
     *   A Set pointer to receive the value. Note that AuxData is maintained
     *   as a string so the Set contents will be derived from a string. If the
     *   conversion is not successful Set will be empty.
     *
     * @param name
     *   A std::string pointer to retrieve AuxData name associated with the index.
     *   You may pass NULL if you do not wish to receive this value.
     *
     * @param retain
     *   A bool pointer to retrieve the retain value. You may pass NULL
     *   if you do not wish to receive this value.
     */
    virtual void get(int32_t      index,
                     Set*         value,
                     std::string* name   = NULL,
                     bool*        retain = NULL) const;

    /**
     * Delete an AuxData item from Tecplot at a particular index
     *
     * @par Note:
     *   Tecplot must be locked when using this method.
     *
     * @param index
     *   Index of the item you wish to delete. Index must be greater than or
     *   equal to 1 and less than or equal to AuxData::size()
     */
    virtual void remove(int32_t   index) const;

    /**
     * Delete an AuxData item from Tecplot with a particular name
     *
     * @par Note:
     *   Tecplot must be locked when using this method.
     *
     * @param name
     *   Name of the item you wish to delete. Non case-sensitive.
     *   Name must be a valid string with non-zero length.
     *
     * @return
     *   Returns @c true if an item was deleted, @c false otherwise.
     */
    virtual bool remove(std::string name)  const;

    /**
     * Get the index from Tecplot of a named AuxData item
     *
     * @param name
     *   Name of the item for which you wish to get the index. Non case-sensitive.
     *   Name must be a valid string with non-zero length.
     *
     * @param index
     *   int32_t pointer to receive the index value.
     *
     * @return
     *   Returns @c true if an item was deleted, @c false otherwise.
     */
    virtual bool getIndex(std::string name,
                          int32_t*    index) const;

    /**
     * Get the current number of auxiliary data items.
     *
     * @return
     *   Number of items maintain by the auxiliary data.
     */
    virtual int32_t size() const;

    /**
     * Get the auxiliary data location association.
     *
     * @return
     *   The location with which this object is associated.
     */
    AuxDataLocation_e getAuxDataLocation() const
    {
        return m_auxDataLocation;
    }

    /**
     * Get the zone, variable, line map, frame, or page association.
     *
     * @return
     *   The 1-based index of the zone, variable, line map, frame, or page
     *   with which this object is associated.
     */
    EntIndex_t getZoneOrMapOrVarIndex() const
    {
        return m_zoneOrMapOrVar;
    }

private:
    AuxData& operator=(const AuxData&);

private:
    const AuxDataLocation_e m_auxDataLocation;
    const EntIndex_t        m_zoneOrMapOrVar;
};

}
}

#endif
