#include "TecUtil.h"
#include "tecquery_Exports.h"

#if defined LINKTOADDON
  #undef LINKTOADDON
#endif

#define LINKTOADDON EXTERNC tecquery_API


/*
 * No namespace (yet).   All "TecUtil" functions are currently not in a namespace
 */


/**
 * Launch a dialog that prompts the user for a color or for a multi-color
 * setting.
 *
 * @since
 *   10.0-3-129
 *
 * @par Note:
 *   This function cannot be called when Tecplot is running in batch mode.
 *
 * @param AllowMultiColor
 *   Set to TRUE if you wish to allow the user to select RGB or multi-color.
 *
 * @param Color
 *   The returned color value. If AllowMultiColor is FALSE then this is in the
 *   range between Black_C and Custom56_C (See GLOBAL.h). If AllowMultiColor
 *   is TRUE then the following constants may also be returned:
 *
 * @verbatim
     MultiColor_C             The user selected the first "MultiColor" option
                              which implies they want the object colored by
                              the contour variable used by contour group 1.

     MultiColor2_C            The user selected the "C2" button
                              which implies they want the object colored by
                              the contour variable used by contour group 2.

     MultiColor3_C            The user selected the "C3" button
                              which implies they want the object colored by
                              the contour variable used by contour group 3.

     MultiColor4_C            The user selected the "C4" button
                              which implies they want the object colored by
                              the contour variable used by contour group 4.

     MultiColor5_C            The user selected the "C5" button
                              which implies they want the object colored by
                              the contour variable used by contour group 5.

     MultiColor6_C            The user selected the "C6" button
                              which implies they want the object colored by
                              the contour variable used by contour group 6.

     MultiColor7_C            The user selected the "C7" button
                              which implies they want the object colored by
                              the contour variable used by contour group 7.

     MultiColor8_C            The user selected the "C8" button
                              which implies they want the object colored by
                              the contour variable used by contour group 8.

     RGBColor_C               The user selected the "RGB" button
                              which implies they want the object colored by
                              RGB.
   @endverbatim
 *
 * @return
 *   TRUE if successful, FALSE if the user pressed the "Cancel" button in the
 *   dialog.
 *
 * <FortranSyntax>
 *    INTEGER*4 FUNCTION TecUtilDialogGetColor(
 *   &                   AllowMultiColor,
 *   &                   Color)
 *    INTEGER*4       AllowMultiColor
 *    INTEGER*4       Color
 * </FortranSyntax>
 *
 * <PythonSyntax>
 * </PythonSyntax>
 *
 * Prompt the user for a basic color (no multi-color options):
 *
 * @code
 *   ColorIndex_t Color;
 *   if (TecUtilDialogGetColor(FALSE, // AllowMultiColor
 *                             &Color))
 *     {
 *       // Do something with Color.
 *     }
 * @endcode
 *
 * @sa TecUtilMacroIsBatchModeActive()
 *
 * @ingroup UserInterface
 *
 * #internalattributes exclude_sdkdoc
 */
LINKTOADDON Boolean_t TecUtilDialogGetColor(Boolean_t  AllowMultiColor,
                                                    TP_OUT     int32_t* Color);

