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
#ifndef TECPLOT_TOOLBOX_STYLEVALUE_H
#define TECPLOT_TOOLBOX_STYLEVALUE_H

#if defined MSWIN
#pragma once
#endif

#include <string>
#include "tptoolbox.h"
#include "ArgList.h"
#include "RawArray.h"
#include "Set.h"
#include "StringList.h"
#include "tptoolbox_Exports.h"

/*
 * Have to use defines instead of class constants because of the IBM.
 * See notes below (look for IBM below)
 */
#define STYLEVALUE_NO_OFFSET        ((int32_t)-1)
#define STYLEVALUE_NO_ASSIGNOP      AssignOp_Invalid
#define STYLEVALUE_MAX_PARAMETERS   7

namespace tecplot { namespace toolbox {

/**
 * An object referring to a node in the Tecplot style hierarchy, which can
 * be used to retrieve and modify style settings.
 *
 * The Tecplot style hierarchy is a collection of settings that define the
 * behavior of the Tecplot Engine and the appearance of plots. Some of the style
 * values are global state; others are specific to a page, a frame, or a plot type.
 * Only leaf nodes in the hierarchy contain actual settings. Non-leaf nodes are
 * used to group related settings.
 *
 * Nodes in the style hierarchy are named. To indicate a specific value, you
 * name all the nodes from the root to the leaf that contains the value. For example,
 * to set the color of the gridlines for the Y axis in a 3D plot, you specify
 * THREEDAXIS, then YDETAIL, then GRIDLINES, and finally COLOR.
 *
 * The <a href="../sv/index.html" target="_top">Style Hierarchy Reference</a>
 * is a browseable version of the Tecplot style hierarchy that can help you
 * determine the path to a setting you need.
 *
 * @note
 *   In your application, you specify names in the form of constants, not strings.
 *   The constants used to specify style node names all begin with SV, such
 *   as SV_THREEDAXIS.
 *
 * The StyleValue class in the Tecplot Toolbox is a convenient and flexible way
 * to work with the style hierarchy in your application. A StyleValue object
 * can refer to any node in the hierarchy: the root, a specific leaf that stores a
 * setting you want to access, or an intermediate node that refers to a family of
 * related settings.
 *
 * You specify the node to which you want a given StyleValue object to refer at
 * its construction.  For example, the code below creates five StyleValue objects
 * that refer to various nodes in the path that leads to the Y-axis gridline color
 * for a 3D plot (as seen earlier).
 *
 * @code
 *   // Declare root StyleValue with access to the entire style hierarchy
 *   StyleValue style1();
 *
 *   // Declare StyleValue with access to the THREEDAXIS subtree
 *   StyleValue style2(SV_THREEDAXIS);
 *
 *   // Declare StyleValue objects with access to smaller subtrees
 *   StyleValue style3(SV_THREEDAXIS, SV_YDETAIL);
 *   StyleValue style4(SV_THREEDAXIS, SV_YDETAIL, SV_GRIDLINES);
 *
 *   // Declare StyleValue object to access a specific setting
 *   StyleValue style5(SV_THREEDAXIS, SV_YDETAIL, SV_GRIDLINES, SV_COLOR);
 * @endcode
 *
 * The StyleValue object's get() and set() methods may be used to specify the
 * path to the desired setting and to retrieve or modify its value.  The path
 * specified when instantiating the object is combined with the path specified
 * in the get() or set() call to obtain the full path of the desired setting.  The
 * examples below all get the same gridline color setting, specifying only the
 * part of the path not already specified at instantiaton of a given StyleValue
 * object.
 *
 * @code
 *   // Get the Y-axis gridline color using each of the above objects
 *   ColorIndex_t result;
 *
 *   // Must specify full SV path when using style1 since it refers to the root
 *   style1.get(&result, SV_THREEDAXIS, SV_YDETAIL, SV_GRIDLINES, SV_COLOR);
 *
 *   // Specify partial SV paths when using style2-style4
 *   style2.get(&result, SV_YDETAIL, SV_GRIDLINES, SV_COLOR);
 *   style3.get(&result, SV_GRIDLINES, SV_COLOR);
 *   style4.get(&result, SV_COLOR);
 *
 *   // style5 already refers directly to the desired leaf
 *   style5.get(&result);
 * @endcode
 *
 * Some nodes in the style hierarchy contain values for a group of similar
 * Tecplot objects. For example, the GLOBALCONTOUR subtree holds settings for
 * each contour group in a frame, while the SLICEATTRIBUTES subtree holds settings
 * for each slice group. When using styles in such subtrees, you must indicate
 * a specific contour group, slice group, or other Tecplot object in the call to
 * the set() or get() method.  Only one index may be specified in each set() or
 * get() call. To retrieve or apply such a setting for all of the Tecplot objects
 * in a group, you must use multiple calls.
 *
 * The FIELDMAP and LINEMAP subtrees allow you to specify a group of fieldmaps or
 * linemaps when you set a value using a Set object. The value is set for all of
 * the specified fieldmaps or linemaps. If you do not provide a Set object, the
 * setting is applied to all fieldmaps or linemaps. Retrieving a value in these
 * subtrees still requires that you specify a single fieldmap or linemap using
 * an index.
 *
 * For page- or frame-specific settings, the get() and set() methods normally
 * operate on the page or frame that is current when the method is invoked.
 * You can create a StyleValue object that always operates on a particular page
 * or frame by providing the unique ID of that page or frame to an alternate
 * SetValue constructor. In the Style Hierarchy Reference, settings that
 * allow a unique page or frame ID to be provided to the constructor have been
 * conveniently separated into FrameStyle and PageStyle categories.
 *
 */
class tptoolbox_API StyleValue
{
public:
    /**
     * Create a StyleValue instance that represents a node in the Tecplot style
     * value hierarchy. The instance may then be used to retrieve or modify style
     * values for the current page and frame using the get() and set() methods.
     *
     * @param P1
     *   P1 through P7 are SV constants that represent a path through the Tecplot
     *   style hierarchy, as explained in the detailed description above.
     *   You may provide zero or more SV constants to the constructor. The instance
     *   will then refer to that point in the style hierarchy, and you need
     *   only specify the path from there when calling the set() and get() methods.
     *
     * @param P2 See P1.
     * @param P3 See P1.
     * @param P4 See P1.
     * @param P5 See P1.
     * @param P6 See P1.
     * @param P7 See P1.
     */
    StyleValue(
        char const* P1 = NULL,
        char const* P2 = NULL,
        char const* P3 = NULL,
        char const* P4 = NULL,
        char const* P5 = NULL,
        char const* P6 = NULL,
        char const* P7 = NULL);

    /**
     * Alternate constructor for the StyleValue class creates an object that
     * refers to the styles of a specified page or frame.
     *
     * @param pageOrFrameUniqueID
     *   Unique ID of the target page or frame.
     *
     * @param P1
     *   P1 through P7 are SV constants that represent a path through the Tecplot
     *   style hierarchy, as explained in the detailed description above.
     *   You may provide zero or more SV constants to the constructor. The instance
     *   will then refer to that point in the style hierarchy, and you need
     *   only specify the path from there when calling the set() and get() methods.
     *
     * @param P2 See P1.
     * @param P3 See P1.
     * @param P4 See P1.
     * @param P5 See P1.
     * @param P6 See P1.
     * @param P7 See P1.
     */
    StyleValue(
        UniqueID_t  pageOrFrameUniqueID,
        char const* P1 = NULL,
        char const* P2 = NULL,
        char const* P3 = NULL,
        char const* P4 = NULL,
        char const* P5 = NULL,
        char const* P6 = NULL,
        char const* P7 = NULL);

    /**
     * Alternate constructor that uses a StringList rather than a series of
     * SV constants to specify a style node.
     *
     * @param paramList
     *   A StringList containing zero or more style node names. (Do not use the
     *   SV prefix.)
     */
    StyleValue(StringList const& paramList);

    /**
     * Alternate constructor that uses a StringList and refers to the styles of a
     * specified page or frame.
     *
     * @param pageOrFrameUniqueID
     *   Unique ID of the target page or frame.
     *
     * @param paramList
     *   A StringList containing zero or more style node names. (Do not use the
     *   SV prefix.)
     */
    StyleValue(
        UniqueID_t        pageOrFrameUniqueID,
        StringList const& paramList);

    /**
     */
    StyleValue(StyleValue const& other);
    StyleValue& operator=(StyleValue const& other);

    /**
     */
    virtual ~StyleValue();

    /**
     * Checks a set() return code for an error condition.
     *
     * @param SVRC
     *   A return code from a call to a StyleValue::set() method.
     *
     * @return
     *   @c true if SVRC is @ref SetValue_Ok or @ref SetValue_DuplicateValue, or
     *   @c false otherwise.
     */
    static bool returnCodeOk(SetValueReturnCode_e SVRC);

    /**
     * Check a get() return code for an error condition.
     *
     * @param GVRC
     *   A return code from a call to a StyleValue::get() method.
     *
     * @return
     *   @c true if GVRC is @ref GetValue_Ok, or @c false otherwise.
     */
    static bool returnCodeOk(GetValueReturnCode_e GVRC);

    /**
     * Sets a Tecplot style value.
     *
     * @note
     *   The Tecplot Engine must be locked when using this method.
     *
     * @param value
     *   The value to which the specified style node will be set. Supported
     *   types for this parameter include, but are not limited to, @ref LgIndex_t,
     *   @ref EntIndex_t, double, char*, @ref Set, @ref ArbParam_t, @ref ColorIndex_t,
     *   and various enumerated types.
     *
     * @param aP1
     *   Parameters aP1 through aP7 specify a full or partial path to the desired
     *   node in the style hierarchy using SV constants. This path is appended
     *   to the path (if any) passed to the constructor at instantiation to obtain
     *   the full path to the desired style node.
     *
     * @param aP2 See aP1.
     * @param aP3 See aP1.
     * @param aP4 See aP1.
     * @param aP5 See aP1.
     * @param aP6 See aP1.
     * @param aP7 See aP1.
     *
     * @return
     *   SetValue_Ok: The command was formed correctly and the value was set.
     *   SetValue_DuplicateValue: The command was formed correctly and the value
     *                            was already the value provided.
     *   Other values indicate various errors.
     *
     * @code
     *   // Set the frame position to (2.5, 1.2)
     *   StyleValue framePos(SV_FRAMELAYOUT, SV_XYPOS);
     *   framePos.set(2.5, SV_X);
     *   framePos.set(1.2, SV_Y);
     * @endcode
     */
    template <typename T>
    SetValueReturnCode_e set(
        T           value,
        char const* aP1 = NULL,
        char const* aP2 = NULL,
        char const* aP3 = NULL,
        char const* aP4 = NULL,
        char const* aP5 = NULL,
        char const* aP6 = NULL,
        char const* aP7 = NULL) const
    {
        ArbParam_t macroIDsArray[STYLEVALUE_MAX_PARAMETERS];
        tecplot::RawArray<ArbParam_t> macroIDs(macroIDsArray, STYLEVALUE_MAX_PARAMETERS);
        initMacroIDsfromStyleValueNames(aP1,aP2,aP3,aP4,aP5,aP6,aP7,macroIDs);
        return setValue(value, macroIDs, NULL, STYLEVALUE_NO_OFFSET, STYLEVALUE_NO_OFFSET,
                        STYLEVALUE_NO_ASSIGNOP);
    }

    /**
     * Sets a Tecplot style value for a setting in a group.
     *
     * @note
     *   The Tecplot Engine must be locked when using this method.
     *
     * @param value
     *   The value to which the specified style node will be set. Supported
     *   types for this parameter include, but are not limited to, @ref LgIndex_t,
     *   @ref EntIndex_t, double, char*, @ref Set, @ref ArbParam_t, @ref ColorIndex_t,
     *   and various enumerated types.
     *
     * @param offset1
     *   An offset or index used to specify which object or group the setting
     *   should be applied to.
     *
     * @param aP1
     *   Parameters aP1 through aP7 specify a full or partial path to the desired
     *   node in the style hierarchy using SV constants. This path is appended
     *   to the path (if any) passed to the constructor at instantiation to obtain
     *   the full path to the desired style node.
     *
     * @param aP2 See aP1.
     * @param aP3 See aP1.
     * @param aP4 See aP1.
     * @param aP5 See aP1.
     * @param aP6 See aP1.
     * @param aP7 See aP1.
     *
     * @return
     *   SetValue_Ok: The command was formed correctly and the value was set.
     *   SetValue_DuplicateValue: The command was formed correctly and the value
     *                            was already the value provided.
     *   Other values indicate various errors.
     *
     * @code
     *   // Set the Slice contour type to Lines & Flood for slice group 2
     *   StyleValue sliceStyle(SV_SLICEATTRIBUTES);
     *   sliceStyle.set(ContourType_Overlay, 2, SV_CONTOUR, SV_CONTOURTYPE);
     * @endcode
     */
    template <typename T>
    SetValueReturnCode_e set(
        T           value,
        LgIndex_t   offset1,
        char const* aP1 = NULL,
        char const* aP2 = NULL,
        char const* aP3 = NULL,
        char const* aP4 = NULL,
        char const* aP5 = NULL,
        char const* aP6 = NULL,
        char const* aP7 = NULL) const
    {
        ArbParam_t macroIDsArray[STYLEVALUE_MAX_PARAMETERS];
        tecplot::RawArray<ArbParam_t> macroIDs(macroIDsArray, STYLEVALUE_MAX_PARAMETERS);
        initMacroIDsfromStyleValueNames(aP1,aP2,aP3,aP4,aP5,aP6,aP7,macroIDs);
        return setValue(value, macroIDs, NULL, offset1, STYLEVALUE_NO_OFFSET,
                        STYLEVALUE_NO_ASSIGNOP);
    }

    /**
     * Sets a Tecplot style value, optionally modifying the original value or
     * converting units.
     *
     * @note
     *   The Tecplot Engine must be locked when using this method.
     *
     * @param value
     *   The value which will be used to modify the specified style setting.
     *   Supported types for this parameter include, but are not limited to,
     *   @ref LgIndex_t, @ref EntIndex_t, double, char*, @ref Set, @ref ArbParam_t,
     *   @ref ColorIndex_t, and various enumerated types.
     *
     * @param assignOp
     *   The assignment operator for changing the specified style node. May be set
     *   to replace the original setting (AssignOp_Equals), to perform a simple
     *   calculation on the original value, or to convert the specified value from
     *   various units. When the value is a set, AssignOp_MinusEquals and
     *   AssignOp_PlusEquals can be used to add or remove items from the set.
     *
     * @param aP1
     *   Parameters aP1 through aP7 specify a full or partial path to the desired
     *   node in the style hierarchy using SV constants. This path is appended
     *   to the path (if any) passed to the constructor at instantiation to obtain
     *   the full path to the desired style node.
     *
     * @param aP2 See aP1.
     * @param aP3 See aP1.
     * @param aP4 See aP1.
     * @param aP5 See aP1.
     * @param aP6 See aP1.
     * @param aP7 See aP1.
     *
     * @return
     *   SetValue_Ok: The command was formed correctly and the value was set.
     *   SetValue_DuplicateValue: The command was formed correctly and the value
     *                            was already the value provided.
     *   Other values indicate various errors.
     *
     * @code
     *   // Remove field map 3 from the set of active field maps
     *   StyleValue activeFieldMaps(SV_ACTIVEFIELDMAPS);
     *   activeFieldMaps.set(Set(3), AssignOp_MinusEquals);
     *
     *   // Double the current frame width
     *   StyleValue frameLayout(SV_FRAMELAYOUT);
     *   frameLayout.set(2.0, AssignOp_TimesEquals, SV_WIDTH);
     * @endcode
     */
    template <typename T>
    SetValueReturnCode_e set(
        T           value,
        AssignOp_e  assignOp,
        char const* aP1 = NULL,
        char const* aP2 = NULL,
        char const* aP3 = NULL,
        char const* aP4 = NULL,
        char const* aP5 = NULL,
        char const* aP6 = NULL,
        char const* aP7 = NULL) const
    {
        ArbParam_t macroIDsArray[STYLEVALUE_MAX_PARAMETERS];
        tecplot::RawArray<ArbParam_t> macroIDs(macroIDsArray, STYLEVALUE_MAX_PARAMETERS);
        initMacroIDsfromStyleValueNames(aP1,aP2,aP3,aP4,aP5,aP6,aP7,macroIDs);
        return setValue(value, macroIDs, NULL, STYLEVALUE_NO_OFFSET, STYLEVALUE_NO_OFFSET,
                        assignOp);
    }

    /**
     * Sets a Tecplot style value in a group, optionally modifying the original
     * value or converting units.
     *
     * @note
     *   The Tecplot Engine must be locked when using this method.
     *
     * @param value
     *   The value which will be used to modify the specified style setting.
     *   Supported types for this parameter include, but are not limited to,
     *   @ref LgIndex_t, @ref EntIndex_t, double, char*, @ref Set, @ref ArbParam_t,
     *   @ref ColorIndex_t, and various enumerated types.
     *
     * @param offset1
     *   An offset or index used to specify which object or group the setting
     *   should be applied to.
     *
     * @param assignOp
     *   The assignment operator for changing the specified style node. May be set
     *   to replace the original setting (AssignOp_Equals), to perform a simple
     *   calculation on the original value, or to convert the specified value from
     *   various units. When the value is a set, AssignOp_MinusEquals and
     *   AssignOp_PlusEquals can be used to add or remove items from the set.
     *
     * @param aP1
     *   Parameters aP1 through aP7 specify a full or partial path to the desired
     *   node in the style hierarchy using SV constants. This path is appended
     *   to the path (if any) passed to the constructor at instantiation to obtain
     *   the full path to the desired style node.
     *
     * @param aP2 See aP1.
     * @param aP3 See aP1.
     * @param aP4 See aP1.
     * @param aP5 See aP1.
     * @param aP6 See aP1.
     * @param aP7 See aP1.
     *
     * @return
     *   SetValue_Ok: The command was formed correctly and the value was set.
     *   SetValue_DuplicateValue: The command was formed correctly and the value
     *                            was already the value provided.
     *   Other values indicate various errors.
     */
    template <typename T>
    SetValueReturnCode_e set(
        T           value,
        LgIndex_t   offset1,
        AssignOp_e  assignOp,
        char const* aP1 = NULL,
        char const* aP2 = NULL,
        char const* aP3 = NULL,
        char const* aP4 = NULL,
        char const* aP5 = NULL,
        char const* aP6 = NULL,
        char const* aP7 = NULL) const
    {
        ArbParam_t macroIDsArray[STYLEVALUE_MAX_PARAMETERS];
        tecplot::RawArray<ArbParam_t> macroIDs(macroIDsArray, STYLEVALUE_MAX_PARAMETERS);
        initMacroIDsfromStyleValueNames(aP1,aP2,aP3,aP4,aP5,aP6,aP7,macroIDs);
        return setValue(value, macroIDs, NULL, offset1, STYLEVALUE_NO_OFFSET, assignOp);
    }

    /**
     * Sets a Tecplot style value in a group.
     *
     * @note
     *   The Tecplot Engine must be locked when using this method.
     *
     * @param value
     *   The value to which the specified style node will be set. Supported
     *   types for this parameter include, but are not limited to, @ref LgIndex_t,
     *   @ref EntIndex_t, double, char*, @ref Set, @ref ArbParam_t, @ref ColorIndex_t,
     *   and various enumerated types.
     *
     * @param offset1
     *   An offset or index used to specify which object or group the setting
     *   should be applied to.
     *
     * @param offset2
     *   A second offset or index used to specify a subgroup.  Currently, only
     *   SV_COLORMAPOVERRIDE requires a second offset.
     *
     * @param aP1
     *   Parameters aP1 through aP7 specify a full or partial path to the desired
     *   node in the style hierarchy using SV constants. This path is appended
     *   to the path (if any) passed to the constructor at instantiation to obtain
     *   the full path to the desired style node.
     *
     * @param aP2 See aP1.
     * @param aP3 See aP1.
     * @param aP4 See aP1.
     * @param aP5 See aP1.
     * @param aP6 See aP1.
     * @param aP7 See aP1.
     *
     * @return
     *   SetValue_Ok: The command was formed correctly and the value was set.
     *   SetValue_DuplicateValue: The command was formed correctly and the value
     *                            was already the value provided.
     *   Other values indicate various errors.
     */
    template <typename T>
    SetValueReturnCode_e set(
        T           value,
        LgIndex_t   offset1,
        LgIndex_t   offset2,
        char const* aP1 = NULL,
        char const* aP2 = NULL,
        char const* aP3 = NULL,
        char const* aP4 = NULL,
        char const* aP5 = NULL,
        char const* aP6 = NULL,
        char const* aP7 = NULL) const
    {
        ArbParam_t macroIDsArray[STYLEVALUE_MAX_PARAMETERS];
        tecplot::RawArray<ArbParam_t> macroIDs(macroIDsArray, STYLEVALUE_MAX_PARAMETERS);
        initMacroIDsfromStyleValueNames(aP1,aP2,aP3,aP4,aP5,aP6,aP7,macroIDs);
        return setValue(value, macroIDs, NULL, offset1, offset2, STYLEVALUE_NO_ASSIGNOP);
    }

    /**
     * Sets a Tecplot style value in a group, optionally modifying the original
     * value or converting units.
     *
     * @note
     *   The Tecplot Engine must be locked when using this method.
     *
     * @param value
     *   The value which will be used to modify the specified style setting.
     *   Supported types for this parameter include, but are not limited to,
     *   @ref LgIndex_t, @ref EntIndex_t, double, char*, @ref Set, @ref ArbParam_t,
     *   @ref ColorIndex_t, and various enumerated types.
     *
     * @param offset1
     *   An offset or index used to specify which object or group the setting
     *   should be applied to.
     *
     * @param offset2
     *   A second offset or index used to specify a subgroup.  Currently, only
     *   SV_COLORMAPOVERRIDE requires a second offset.
     *
     * @param assignOp
     *   The assignment operator for changing the specified style node. May be set
     *   to replace the original setting (AssignOp_Equals), to perform a simple
     *   calculation on the original value, or to convert the specified value from
     *   various units. When the value is a set, AssignOp_MinusEquals and
     *   AssignOp_PlusEquals can be used to add or remove items from the set.
     *
     * @param aP1
     *   Parameters aP1 through aP7 specify a full or partial path to the desired
     *   node in the style hierarchy using SV constants. This path is appended
     *   to the path (if any) passed to the constructor at instantiation to obtain
     *   the full path to the desired style node.
     *
     * @param aP2 See aP1.
     * @param aP3 See aP1.
     * @param aP4 See aP1.
     * @param aP5 See aP1.
     * @param aP6 See aP1.
     * @param aP7 See aP1.
     *
     * @return
     *   SetValue_Ok: The command was formed correctly and the value was set.
     *   SetValue_DuplicateValue: The command was formed correctly and the value
     *                            was already the value provided.
     *   Other values indicate various errors.
     */
    template <typename T>
    SetValueReturnCode_e set(
        T           value,
        LgIndex_t   offset1,
        LgIndex_t   offset2,
        AssignOp_e  assignOp,
        char const* aP1 = NULL,
        char const* aP2 = NULL,
        char const* aP3 = NULL,
        char const* aP4 = NULL,
        char const* aP5 = NULL,
        char const* aP6 = NULL,
        char const* aP7 = NULL) const
    {
        ArbParam_t macroIDsArray[STYLEVALUE_MAX_PARAMETERS];
        tecplot::RawArray<ArbParam_t> macroIDs(macroIDsArray, STYLEVALUE_MAX_PARAMETERS);
        initMacroIDsfromStyleValueNames(aP1,aP2,aP3,aP4,aP5,aP6,aP7,macroIDs);
        return setValue(value, macroIDs, NULL, offset1, offset2, assignOp);
    }

    /**
     * Sets a Tecplot style value for a set, such as for a set of fieldmaps or
     * linemaps.
     *
     * @note
     *   The Tecplot Engine must be locked when using this method.
     *
     * @param value
     *   The value to which the specified style node will be set. Supported
     *   types for this parameter include, but are not limited to, @ref LgIndex_t,
     *   @ref EntIndex_t, double, char*, @ref Set, @ref ArbParam_t, @ref ColorIndex_t,
     *   and various enumerated types.
     *
     * @param objectSet
     *   A Set that specifies the items to operate on. This is typically used
     *   with SV_FIELDMAP and SV_LINEMAP styles.
     *
     * @param aP1
     *   Parameters aP1 through aP7 specify a full or partial path to the desired
     *   node in the style hierarchy using SV constants. This path is appended
     *   to the path (if any) passed to the constructor at instantiation to obtain
     *   the full path to the desired style node.
     *
     * @param aP2 See aP1.
     * @param aP3 See aP1.
     * @param aP4 See aP1.
     * @param aP5 See aP1.
     * @param aP6 See aP1.
     * @param aP7 See aP1.
     *
     * @return
     *   SetValue_Ok: The command was formed correctly and the value was set.
     *   SetValue_DuplicateValue: The command was formed correctly and the value
     *                            was already the value provided.
     *   Other values indicate various errors.
     *
     * @code
     *   // Set the mesh color to Red and the line thickness to 1.2
     *   // for field maps 1 through 5
     *   Set fieldMaps;
     *   fieldMaps.assign("[1-5]");
     *
     *   StyleValue fieldMapMesh(SV_FIELDMAP, SV_MESH);
     *   fieldMapMesh.set(Red_C, fieldMaps, SV_COLOR);
     *   fieldMapMesh.set(1.2, fieldMaps, SV_LINETHICKNESS);
     * @endcode
     */
    template <typename T>
    SetValueReturnCode_e set(
        T           value,
        Set const&  objectSet,
        char const* aP1 = NULL,
        char const* aP2 = NULL,
        char const* aP3 = NULL,
        char const* aP4 = NULL,
        char const* aP5 = NULL,
        char const* aP6 = NULL,
        char const* aP7 = NULL) const
    {
        ArbParam_t macroIDsArray[STYLEVALUE_MAX_PARAMETERS];
        tecplot::RawArray<ArbParam_t> macroIDs(macroIDsArray, STYLEVALUE_MAX_PARAMETERS);
        initMacroIDsfromStyleValueNames(aP1,aP2,aP3,aP4,aP5,aP6,aP7,macroIDs);
        return setValue(value, macroIDs, &objectSet, STYLEVALUE_NO_OFFSET, STYLEVALUE_NO_OFFSET,
                        STYLEVALUE_NO_ASSIGNOP);
    }

    /**
     * Sets a Tecplot style value for a set, such as for a set of fieldmaps or
     * linemaps.
     *
     * @note
     *   The Tecplot Engine must be locked when using this method.
     *
     * @param value
     *   The value which will be used to modify the specified style setting.
     *   Supported types for this parameter include, but are not limited to,
     *   @ref LgIndex_t, @ref EntIndex_t, double, char*, @ref Set, @ref ArbParam_t,
     *   @ref ColorIndex_t, and various enumerated types.
     *
     * @param objectSet
     *   A Set that specifies the items to operate on. This is typically used
     *   with SV_FIELDMAP and SV_LINEMAP styles.
     *
     * @param assignOp
     *   The assignment operator for changing the specified style node. May be set
     *   to replace the original setting (AssignOp_Equals), to perform a simple
     *   calculation on the original value, or to convert the specified value from
     *   various units. When the value is a set, AssignOp_MinusEquals and
     *   AssignOp_PlusEquals can be used to add or remove items from the set.
     *
     * @param aP1
     *   Parameters aP1 through aP7 specify a full or partial path to the desired
     *   node in the style hierarchy using SV constants. This path is appended
     *   to the path (if any) passed to the constructor at instantiation to obtain
     *   the full path to the desired style node.
     *
     * @param aP2 See aP1.
     * @param aP3 See aP1.
     * @param aP4 See aP1.
     * @param aP5 See aP1.
     * @param aP6 See aP1.
     * @param aP7 See aP1.
     *
     * @return
     *   SetValue_Ok: The command was formed correctly and the value was set.
     *   SetValue_DuplicateValue: The command was formed correctly and the value
     *                            was already the value provided.
     *   Other values indicate various errors.
     *
     * @code
     *   // Set mesh line thickness to half its current width for fieldmaps 1 through 5
     *   Set fieldMaps;
     *   fieldMaps.assign("[1-5]");
     *   StyleValue fieldMapMesh(SV_FIELDMAP, SV_MESH);
     *   fieldMapMesh.set(2, fieldMaps, AssignOp_DivideEquals, SV_LINETHICKNESS);
     * @endcode
     */
    template <typename T>
    SetValueReturnCode_e set(
        T value,
        Set const&  objectSet,
        AssignOp_e  assignOp,
        char const* aP1 = NULL,
        char const* aP2 = NULL,
        char const* aP3 = NULL,
        char const* aP4 = NULL,
        char const* aP5 = NULL,
        char const* aP6 = NULL,
        char const* aP7 = NULL) const
    {
        ArbParam_t macroIDsArray[STYLEVALUE_MAX_PARAMETERS];
        tecplot::RawArray<ArbParam_t> macroIDs(macroIDsArray, STYLEVALUE_MAX_PARAMETERS);
        initMacroIDsfromStyleValueNames(aP1,aP2,aP3,aP4,aP5,aP6,aP7,macroIDs);
        return setValue(value, macroIDs, &objectSet, STYLEVALUE_NO_OFFSET, STYLEVALUE_NO_OFFSET,
                        assignOp);
    }


    /**
     * Gets a Tecplot style value.
     *
     * @param value
     *   A pointer or reference to the variable that will receive the retrieved
     *   style setting. Supported types for this variable include, but are not
     *   limited to, @ref LgIndex_t, @ref EntIndex_t, double, char*, @ref Set,
     *   @ref ArbParam_t, @ref ColorIndex_t, and various enumerated types.
     *
     * @param aP1
     *   Parameters aP1 through aP7 specify a full or partial path to the desired
     *   node in the style hierarchy using SV constants. This path is appended
     *   to the path (if any) passed to the constructor at instantiation to obtain
     *   the full path to the desired style node.
     *
     * @param aP2 See aP1.
     * @param aP3 See aP1.
     * @param aP4 See aP1.
     * @param aP5 See aP1.
     * @param aP6 See aP1.
     * @param aP7 See aP1.
     *
     * @return
     *   GetValue_Ok: the value was retrieved successfully.
     *   GetValue_ResultTypeError: the variable provided to receive the value was
     *                             of the wrong type.
     *
     * @code
     *   // Get the x position of the current frame.
     *
     *   StyleValue genericSV;
     *   double     xPos;
     *
     *   GetValueReturnCode_e GVRC = genericSV.get(&xPos, SV_FRAMELAYOUT, SV_XYPOS, SV_X);
     *   if ( genericSV.returnCodeOk(GVRC) )
     *     {
     *       // Do something with xPos
     *     }
     * @endcode
     */
    template <typename T>
    GetValueReturnCode_e get(
        T*          value,
        char const* aP1 = NULL,
        char const* aP2 = NULL,
        char const* aP3 = NULL,
        char const* aP4 = NULL,
        char const* aP5 = NULL,
        char const* aP6 = NULL,
        char const* aP7 = NULL) const
    {
        ArbParam_t macroIDsArray[STYLEVALUE_MAX_PARAMETERS];
        tecplot::RawArray<ArbParam_t> macroIDs(macroIDsArray, STYLEVALUE_MAX_PARAMETERS);
        initMacroIDsfromStyleValueNames(aP1,aP2,aP3,aP4,aP5,aP6,aP7,macroIDs);
        return getValue(value, STYLEVALUE_NO_OFFSET, STYLEVALUE_NO_OFFSET, macroIDs);
    }

    /**
     * Gets a Tecplot style value in a group.
     *
     * @param value
     *   A pointer or reference to the variable that will receive the retrieved
     *   style setting. Supported types for this variable include, but are not
     *   limited to, @ref LgIndex_t, @ref EntIndex_t, double, char*, @ref Set,
     *   @ref ArbParam_t, @ref ColorIndex_t, and various enumerated types.
     *
     * @param offset1
     *   An offset or index used to specify which object or group the setting
     *   should be applied to.
     *
     * @param aP1
     *   Parameters aP1 through aP7 specify a full or partial path to the desired
     *   node in the style hierarchy using SV constants. This path is appended
     *   to the path (if any) passed to the constructor at instantiation to obtain
     *   the full path to the desired style node.
     *
     * @param aP2 See aP1.
     * @param aP3 See aP1.
     * @param aP4 See aP1.
     * @param aP5 See aP1.
     * @param aP6 See aP1.
     * @param aP7 See aP1.
     *
     * @return
     *   GetValue_Ok: the value was retrieved successfully.
     *   GetValue_ResultTypeError: the variable provided to receive the value was
     *                             of the wrong type.
     *   Other values indicate various errors.
     *
     * @code
     *   // Get the font of the contour legend header for contour group 2.
     *   StyleValue contLegendHeaderText(SV_GLOBALCONTOUR,SV_LEGEND,SV_HEADERTEXTSHAPE);
     *   Font_e font;
     *   int    group = 1;
     *   GetValueReturnCode_e GVRC = contLegendHeaderText.get(&font, group, SV_FONT);
     *   if ( contLegendHeaderText.returnCodeOk(GVRC) )
     *     {
     *       // Do something with font
     *     }
     * @endcode
     */
    template <typename T>
    GetValueReturnCode_e get(
        T*          value,
        LgIndex_t   offset1,
        char const* aP1 = NULL,
        char const* aP2 = NULL,
        char const* aP3 = NULL,
        char const* aP4 = NULL,
        char const* aP5 = NULL,
        char const* aP6 = NULL,
        char const* aP7 = NULL) const
    {
        ArbParam_t macroIDsArray[STYLEVALUE_MAX_PARAMETERS];
        tecplot::RawArray<ArbParam_t> macroIDs(macroIDsArray, STYLEVALUE_MAX_PARAMETERS);
        initMacroIDsfromStyleValueNames(aP1,aP2,aP3,aP4,aP5,aP6,aP7,macroIDs);
        return getValue(value, offset1, STYLEVALUE_NO_OFFSET, macroIDs);
    }

    /**
     * Gets a Tecplot style value in a group.
     *
     * @param value
     *   A pointer or reference to the variable that will receive the retrieved
     *   style setting. Supported types for this variable include, but are not
     *   limited to, @ref LgIndex_t, @ref EntIndex_t, double, char*, @ref Set,
     *   @ref ArbParam_t, @ref ColorIndex_t, and various enumerated types.
     *
     * @param offset1
     *   An offset or index used to specify which object or group the setting
     *   should be applied to.
     *
     * @param offset2
     *   A second offset or index used to specify a subgroup.  Currently, only
     *   SV_COLORMAPOVERRIDE requires a second offset.
     *
     * @param aP1
     *   Parameters aP1 through aP7 specify a full or partial path to the desired
     *   node in the style hierarchy using SV constants. This path is appended
     *   to the path (if any) passed to the constructor at instantiation to obtain
     *   the full path to the desired style node.
     *
     * @param aP2 See aP1.
     * @param aP3 See aP1.
     * @param aP4 See aP1.
     * @param aP5 See aP1.
     * @param aP6 See aP1.
     * @param aP7 See aP1.
     *
     * @return
     *   GetValue_Ok: the value was retrieved successfully.
     *   GetValue_ResultTypeError: the variable provided to receive the value was
     *                             of the wrong type.
     *   Other values indicate various errors.
     */
    template <typename T>
    GetValueReturnCode_e get(
        T*          value,
        LgIndex_t   offset1,
        LgIndex_t   offset2,
        char const* aP1 = NULL,
        char const* aP2 = NULL,
        char const* aP3 = NULL,
        char const* aP4 = NULL,
        char const* aP5 = NULL,
        char const* aP6 = NULL,
        char const* aP7 = NULL) const
    {
        ArbParam_t macroIDsArray[STYLEVALUE_MAX_PARAMETERS];
        tecplot::RawArray<ArbParam_t> macroIDs(macroIDsArray, STYLEVALUE_MAX_PARAMETERS);
        initMacroIDsfromStyleValueNames(aP1,aP2,aP3,aP4,aP5,aP6,aP7,macroIDs);
        return getValue(value, offset1, offset2, macroIDs);
    }

#if 0
    //
    // The IBM cannot handle class constants so we must use defines.
    //
    static const LgIndex_t  NO_OFFSET      = BAD_SET_VALUE;
    static const AssignOp_e NO_ASSIGNOP    = AssignOp_Invalid;
    static const int        MAX_PARAMETERS = 7;
#endif

protected:
    ////////////////////////////////////////
    // SetValue
    ////////////////////////////////////////

    /**
     * Set a Tecplot style value. This is the most low-level method for
     * setting styles with the StyleValue class.
     *
     * @note
     *   The Tecplot Engine must be locked when using this method.
     *   You may wish to override this method with one that handles
     *   locking for you and optionally asserts if the SetValueReturnCode
     *   is not SetValue_Ok or SetValue_DuplicateValue.
     *
     * @param argList
     *   The complete argument list that is passed to TecUtilStyleSetLowLevelX().
     *
     * @return
     *   SetValue_Ok: The command was formed correctly and the value was set.
     *   SetValue_DuplicateValue: The command was formed correctly and the value
     *                            was already the value provided.
     *   Other values indicate various errors.
     */
    virtual SetValueReturnCode_e styleSetLowLevel(ArgList const& argList) const;

    template <typename T>
    SetValueReturnCode_e setFinalValue(
        T        value,
        ArgList& argList) const
    {
        REQUIRE("value can be any value");
        REQUIRE("no way to check argList");
        (void)value; // ...eliminates warning of unused value parameter for certain template expansions of setFinalValue()

        argList.appendArbParam(SV_IVALUE, (ArbParam_t)value);
        /*
         * The user may have supplied a value that looks like an integer
         * but the SV_ params require a double, so pass the double just
         * in case.
         *
         * If we don't do this, then TecUtilStyleSetLowLevelX()
         * will default the DValue to zero and you'd most likely get back
         * SetValue_Ok or SetValue_DuplicateValue, which is incorrect.
         *
         * This is different from TecUtilStyleGetLowLevelX() which returns
         * GetValue_ResultTypeError if you pass the wrong type for
         * the specified parameters.
         *
         *    Example: StyleValue::set(1, SV_FRAMELAYOUT, SV_XYPOS, SV_X);
         *
         *    In this case Framelayout->XYPos->X requires a double, but
         *    the value '1' is an integer which puts us here. instead
         *    of in the double overload. If we only set the IValue, the
         *    Frame X-Position would get set to zero, because DValue defaults
         *    to zero in TecUtilStyleSetLowLevelX()
         *
         * NOTE: This behavior requires Tecplot 11.2-0-315 or later. Prior
         *       to that it was not allowed to set both the IValue and DValue.
         */

        // First typecast to ArbParam_t to handle pointer types that can't
        // be directly typecast to double (such as char*)
        argList.appendDouble(SV_DVALUE, (double)((ArbParam_t)value));
        return styleSetLowLevel(argList);
    }

    SetValueReturnCode_e setFinalValue(
        bool     value,
        ArgList& argList) const;
    SetValueReturnCode_e setFinalValue(
        double   value,
        ArgList& argList) const;
    SetValueReturnCode_e setFinalValue(
        Set_pa   value,
        ArgList& argList) const;
    SetValueReturnCode_e setFinalValue(
        Set const& value,
        ArgList&   argList) const;
    SetValueReturnCode_e setFinalValue(
        char const* value,
        ArgList&    argList) const;
    SetValueReturnCode_e setFinalValue(
        std::string const& value,
        ArgList&           argList) const;

    template <typename T>
    SetValueReturnCode_e setValue(
        T const&                             value,
        tecplot::RawArray<ArbParam_t> const& extraMacroIDs,
        Set const*                           objectSet,
        LgIndex_t                            offset1,
        LgIndex_t                            offset2,
        AssignOp_e                           assignOp) const
    {
        REQUIRE("valid can be any value");
        REQUIRE(m_macroIDs.size() + extraMacroIDs.size() <= STYLEVALUE_MAX_PARAMETERS);
        REQUIRE(VALID_REF_OR_NULL(objectSet));
        REQUIRE(offset1 >= 1 || offset1 == STYLEVALUE_NO_OFFSET);
        REQUIRE(offset2 >= 1 || offset2 == STYLEVALUE_NO_OFFSET);
        REQUIRE(VALID_ENUM(assignOp, AssignOp_e) || assignOp == STYLEVALUE_NO_ASSIGNOP);
        // objectSet and offset1 are mutually exclusive, so both can't be valid values
        REQUIRE(IMPLICATION(VALID_REF(objectSet), offset1 == STYLEVALUE_NO_OFFSET));
        REQUIRE(IMPLICATION(offset1 >= 1, objectSet == NULL));

        ArbParam_t finalMacroIDsArray[STYLEVALUE_MAX_PARAMETERS+1]; // +1 for the required -1 array terminator
        tecplot::RawArray<ArbParam_t> finalMacroIDs(finalMacroIDsArray, STYLEVALUE_MAX_PARAMETERS+1);
        finalMacroIDs.append(m_macroIDs).append(extraMacroIDs).push_back(-1);

        ArgList argList;
        setupSetValueArgList(finalMacroIDs, objectSet, offset1, offset2, assignOp, argList);
        return setFinalValue(value, argList);
    }


    ////////////////////////////////////////
    // GetValue
    ////////////////////////////////////////

    /**
     * Gets a Tecplot style value.  This is the most low-level method for
     * setting styles with the StyleValue object.
     *
     * @note
     *  You may wish to override this method with one that asserts if the returned
     *  value is not GetValue_Ok.
     *
     * @param argList
     *   The complete argument list that is passed to TecUtilStyleGetLowLevelX().
     *
     * @return
     *   GetValue_Ok: the value was retrieved successfully.
     *   GetValue_ResultTypeError: the variable provided to receive the value was
     *                             of the wrong type.
     *   Other values indicate various errors.
     */
    virtual GetValueReturnCode_e styleGetLowLevel(ArgList const& argList) const;

    template <typename T>
    GetValueReturnCode_e getFinalValue(
        T*       value,
        ArgList& argList) const
    {
        REQUIRE(VALID_REF(value));
        REQUIRE("no way to check argList");

        ArbParam_t arbValue = 0;
        argList.appendArbParamPtr(SV_IVALUE, &arbValue);
        GetValueReturnCode_e GVRC = styleGetLowLevel(argList);
        /*
         * IMPORTANT:
         *     On error assign a default T() value so that client code that looks like this:
         *         Boolean_t show;
         *         auto gvrc{styleValue.get(&show, fieldMap, SV_SHOWISOSURFACES)};
         *         if (gvrc == GetValueReturnCode_Ok)
         *             ...do something with the 'show' flag
         *     doesn't cause compiler warnings indicating that 'show' may be uninitialized.
         */
        *value = returnCodeOk(GVRC)
            ? (T)arbValue
            : T();

        ENSURE(VALID_ENUM(GVRC, GetValueReturnCode_e));
        return GVRC;
    }

    GetValueReturnCode_e getFinalValue(
        bool*  value,
        ArgList& argList) const;
    GetValueReturnCode_e getFinalValue(
        double*  value,
        ArgList& argList) const;
    GetValueReturnCode_e getFinalValue(
        std::string* value,
        ArgList&     argList) const;
    GetValueReturnCode_e getFinalValue(
        char**   value,
        ArgList& argList) const;
    GetValueReturnCode_e getFinalValue(
        Set*     value,
        ArgList& argList) const;
    GetValueReturnCode_e getFinalValue(
        Set_pa*  value,
        ArgList& argList) const;

    template <typename T>
    GetValueReturnCode_e getValue(
        T*                                   value,
        LgIndex_t                            offset1,
        LgIndex_t                            offset2,
        tecplot::RawArray<ArbParam_t> const& extraMacroIDs) const
    {
        REQUIRE(VALID_REF(value));
        REQUIRE(offset1 >= 1 || offset1 == STYLEVALUE_NO_OFFSET);
        REQUIRE(offset2 >= 1 || offset2 == STYLEVALUE_NO_OFFSET);
        REQUIRE(m_macroIDs.size() + extraMacroIDs.size() <= STYLEVALUE_MAX_PARAMETERS);

        ArbParam_t finalMacroIDsArray[STYLEVALUE_MAX_PARAMETERS+1]; // +1 for the required -1 array terminator
        tecplot::RawArray<ArbParam_t> finalMacroIDs(finalMacroIDsArray, STYLEVALUE_MAX_PARAMETERS+1);
        finalMacroIDs.append(m_macroIDs).append(extraMacroIDs).push_back(-1);

        ArgList argList;
        setupGetValueArgList(finalMacroIDs, offset1, offset2, argList);
        return getFinalValue(value, argList);
    }

    ////////////////////////////////////////
    // ArgList handlers
    ////////////////////////////////////////

    void setupSetValueArgList(
        tecplot::RawArray<ArbParam_t> const& macroIDs,
        Set const*                           objectSet,
        LgIndex_t                            offset1,
        LgIndex_t                            offset2,
        AssignOp_e                           assignOp,
        ArgList&                             argList) const;

    void setupGetValueArgList(
        tecplot::RawArray<ArbParam_t> const& macroIDs,
        LgIndex_t                            offset1,
        LgIndex_t                            offset2,
        ArgList&                             argList) const;

private:
    void initMacroIDsfromStyleValueNames(
        StringList const&              stringList,
        tecplot::RawArray<ArbParam_t>& macroIDs) const;
    void initMacroIDsfromStyleValueNames(
        char const*                    p1,
        char const*                    p2,
        char const*                    p3,
        char const*                    p4,
        char const*                    p5,
        char const*                    p6,
        char const*                    p7,
        tecplot::RawArray<ArbParam_t>& macroIDs) const;

    UniqueID_t                    m_pageOrFrameUniqueID;
    ArbParam_t                    m_macroIDsArray[STYLEVALUE_MAX_PARAMETERS];
    tecplot::RawArray<ArbParam_t> m_macroIDs;

};

}}

#endif
