#define ENGINE_INTERNAL_EXPORT // force LINKTOADDON to export insead of import
#include "TecUtil.h"
#include "tp360addonuiaction_Exports.h"

#if defined LINKTOADDON
#undef LINKTOADDON
#endif

#define LINKTOADDON EXTERNC tp360addonuiaction_API

typedef void (*DynamicMenuCallback_pf)();

/**
 * Adds a simple menu option to the end of the parent menu in Tecplot's main menu.
 *
 * @param MenuPath
 *     This is used to specify where you want your menu option placed in Tecplot's menu structure.
 *     Use the newline character ('\\n') to create options in sub-menu. Each sub-menu may have a
 *     mnemonic specified by putting a '&' in front of the desired letter. The mnemonic is used only
 *     if the sub-menu does not already exist, and thus is created by this call.  (A mnemonic is a
 *     keyboard short-cut to access the menu.)
 *
 * @param MenuLabel
 *     Text to put on the menu option. You may specify a mnemonic for the option by putting a '&' in
 *     front of the desired letter. If you want to specify a '&' in the menu option itself, use
 *     "&&". (A mnemonic is a keyboard short-cut to access the menu.) You may specify an accelerator
 *     key by separating it from the MenuLabel with a tab ('\\t'). An accelerator key is a key that
 *     can be used from within Tecplot to perform the function without going through the menu
 *     system. Accelerators should be of the form "Ctrl+L" or "Shift+L" or "Alt+L" or a combination
 *     such as "Ctrl+Shift+L" where "L" can be any letter or number.
 *
 * @param Mnemonic
 *     If set to TECUTILAUTOMNEMONIC or '\\0' MenuLabel is passed to the menu system as-is otherwise
 *     Mnemonic must be an alpha or numeric character. If the Mnemonic character is found in the
 *     menu label it is used as the mnemonic.
 *
 * @param MenuOptionCallback
 *     Function you create which will be called when the new menu option is selected. See
 *     DynamicMenuCallback_pf.
 *
 * @return
 *     Returns TRUE if the menu option was successfully installed.
 *
 * <FortranSyntax>
 *    INTEGER*4 FUNCTION TecUtilMenuAddOption(
 *   &                   MenuPath,
 *   &                   MenuLabel,
 *   &                   Mnemonic,
 *   &                   MenuOptionCallback)
 *    CHARACTER*(*)   MenuPath
 *    CHARACTER*(*)   MenuLabel
 *    CHARACTER*(*)   Mnemonic
 *    EXTERNAL        MenuOptionCallback
 * </FortranSyntax>
 *
 * <PythonSyntax>
 * </PythonSyntax>
 *
 * Add an option to the Tools menu in Tecplot named "Banana". When selected,
 * have Tecplot call the function BananaCallback.
 *
 * First create the BananaCallback function:
 *
 * @code
 *   void BananaCallback(void)
 *   {
 *      // code executed when "Banana" is selected in the
 *      // "Tools" menu.
 *   }
 * @endcode
 *
 * In the initialization code for the add-on add:
 *
 * @code
 *    IsOk = TecUtilMenuAddOption("Tools",
 *                                "&Banana",
 *                                TECUTILAUTOMNEMONIC,
 *                                BananaCallback);
 * @endcode
 *
 * To put the menu item in a sub-menu of Tools named "Fruit" use:
 *
 * @code
 *   IsOk = TecUtilMenuAddOption("Tools\n&Fruit", "&Banana", TECUTILAUTOMNEMONIC, BananaCallback);
 * @endcode
 *
 * The sub-menu "Fruit" of "Tools" will be created if necessary, and if it is created, it will be
 * given a mnemonic of 'F'.
 *
 * To install an accelerator of Shift+B for this option, use:
 *
 * @code
 *   IsOk = TecUtilMenuAddOption("Tools\n&Fruit", "&Banana\tShift+B", TECUTILAUTOMNEMONIC, BananaCallback);
 * @endcode
 *
 * The user can then invoke the BananaCallback without using the menu system. Hitting Shift+B in the
 * work area of Tecplot will call BananaCallback.
 *
 * @ingroup UserInterface
 * @sa TecUtilMenuInsertOption() for more control over where the menu option is placed.
 *
 * #internalattributes exclude_sdkdoc, exclude_tcl
 */
LINKTOADDON Boolean_t TecUtilMenuAddOption(
    const char*            MenuPath,
    const char*            MenuLabel,
    char                   Mnemonic,
    DynamicMenuCallback_pf MenuOptionCallback);

/**
 * Adds a simple menu option just after the menu option with the BeforeMenuLabel label.
 *
 * @param MenuPath
 *     This is used to specify where you want your menu option placed in Tecplot's menu structure.
 *     Use the newline character ('\\n') to create options in sub-menu. Each sub-menu may have a
 *     mnemonic specified by putting a '&' in front of the desired letter. The mnemonic is used only
 *     if the sub-menu does not already exist, and thus is created by this call.  (A mnemonic is a
 *     keyboard short-cut to access the menu.)
 *
 * @param MenuLabel
 *     Text to put on the menu option. You may specify a mnemonic for the option by putting a '&' in
 *     front of the desired letter. If you want to specify a '&' in the menu option itself, use
 *     "&&". (A mnemonic is a keyboard short-cut to access the menu.) You may specify an accelerator
 *     key by separating it from the MenuLabel with a tab ('\\t'). An accelerator key is a key that
 *     can be used from within Tecplot to perform the function without going through the menu
 *     system. Accelerators should be of the form "Ctrl+L" or "Shift+L" or "Alt+L" or a combination
 *     such as "Ctrl+Shift+L" where "L" can be any letter or number.
 *
 * @param BeforeMenuLabel
 *     An already existing menu option label before which this menu option should be inserted. If not
 *     found the option is added to the end of the menu. Pass NULL or an empty string, "", to add
 *     the menu option to the end of the menu or use TecUtilMenuAddOption().
 *
 * @param Mnemonic
 *     If set to TECUTILAUTOMNEMONIC or '\\0' MenuLabel is passed to the menu system as-is otherwise
 *     Mnemonic must be an alpha or numeric character. If the Mnemonic character is found in the
 *     menu label it is used as the mnemonic.
 *
 * @return
 *     Returns TRUE if the menu option was successfully installed.
 *
 * <FortranSyntax>
 *    INTEGER*4 FUNCTION TecUtilMenuInsertOption(
 *   &                   MenuPath,
 *   &                   MenuLabel,
 *   &                   BeforeMenuLabel,
 *   &                   Mnemonic,
 *   &                   MenuOptionCallback)
 *    CHARACTER*(*)   MenuPath
 *    CHARACTER*(*)   MenuLabel
 *    CHARACTER*(*)   BeforeMenuLabel
 *    CHARACTER*(*)   Mnemonic
 *    EXTERNAL        MenuOptionCallback
 * </FortranSyntax>
 *
 * <PythonSyntax>
 * </PythonSyntax>
 *
 * Add an option to the Tools menu in Tecplot named "Banana" after the "Apple" menu option. When
 * selected, have Tecplot call the function BananaCallback.
 *
 * First create the BananaCallback function:
 *
 * @code
 *   void BananaCallback(void)
 *   {
 *      // code executed when "Banana" is selected in the
 *      // "Tools" menu.
 *   }
 * @endcode
 *
 * In the initialization code for the add-on add:
 *
 * @code
 *    IsOk = TecUtilMenuInsertOption("Tools",
 *                                   "&Banana",
 *                                   "Apple",
 *                                   TECUTILAUTOMNEMONIC,
 *                                   BananaCallback);
 * @endcode
 *
 * To put the menu item in a sub-menu of Tools named "Fruit" after the "Apple" menu option use:
 *
 * @code
 *   IsOk = TecUtilMenuInsertOption("Tools\n&Fruit", "&Banana", "Apple", TECUTILAUTOMNEMONIC, BananaCallback);
 * @endcode
 *
 * The sub-menu "Fruit" of "Tools" will be created if necessary, and if it is created, it will be
 * given a mnemonic of 'F'.
 *
 * To install an accelerator of Shift+B for this option, use:
 *
 * @code
 *   IsOk = TecUtilMenuInsertOption("Tools\n&Fruit", "&Banana\tShift+B", "Apple", TECUTILAUTOMNEMONIC, BananaCallback);
 * @endcode
 *
 * The user can then invoke the BananaCallback without using the menu system. Hitting Shift+B in the
 * work area of Tecplot will call BananaCallback.
 *
 * @ingroup UserInterface
 * @sa TecUtilMenuAddOption() to simply add menu options to the end of the menu.
 *
 * #internalattributes exclude_sdkdoc, exclude_tcl
 */
LINKTOADDON Boolean_t TecUtilMenuInsertOption(
    const char*            MenuPath,
    const char*            MenuLabel,
    const char*            BeforeMenuLabel,
    char                   Mnemonic,
    DynamicMenuCallback_pf MenuOptionCallback);

/**
 * Adds menu separator to the end of the specified parent menu.
 *
 * @param MenuPath
 *   Specify the path to where the separator should be placed.
 *
 * @return
 *   Returns TRUE if the menu separator was successfully added, FALSE otherwise.
 *
 * <FortranSyntax>
 *    INTEGER*4 FUNCTION TecUtilMenuAddSeparator(MenuPath)
 *    CHARACTER*(*) MenuPath
 * </FortranSyntax>
 *
 * <PythonSyntax>
 * </PythonSyntax>
 *
 * @ingroup UserInterface
 *
 * #internalattributes exclude_sdkdoc
 */
LINKTOADDON Boolean_t TecUtilMenuAddSeparator(const char* MenuPath);

/**
 * Sets the menu sensitivity.
 *
 * @param MenuPath
 *   Specify the path to where the separator should be placed.
 *
 * @param MenuLabel
 *   Menu text of the menu who's sensitivity should be updated.
 *
 * @param IsSensitive
 *   TRUE if the item is sensitive, FALSE otherwise.
 */
LINKTOADDON void TecUtilMenuSetSensitivity(
    const char* MenuPath,
    const char* MenuLabel,
    Boolean_t   IsSensitive);
