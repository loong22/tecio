#define ENGINE_INTERNAL_EXPORT // force LINKTOADDON to export insead of import
#include "TecUtil.h"
#include "tp360addonuiaction_Exports.h"

#if defined LINKTOADDON
#undef LINKTOADDON
#endif

#define LINKTOADDON EXTERNC tp360addonuiaction_API

/**
 * This enumeration is to avoid tight coupling with Qt.  If we were to use
 * Qt::DockWidgetArea here we'd need to include <QDockWidget> which would
 * force all addons to become dependent on Qt.
 */
enum DockLocation_e
{
    DockLocation_Left,
    DockLocation_Right,
    DockLocation_Top,
    DockLocation_Bottom,
    END_DockLocation_e,
    DockLocation_Invalid
};

/**
 * Register a QDockWidget with the application.  It's best to call this function from within
 * InitTecAddOn in your add-on.
 *
 * @param dockWidget
 *   This must be of type QDockWidget. You must also give this widget a unique
 *   object name, otherwise the UI state may not be saved/restored correctly. See
 *   QDockWidget::setObjectName().
 *
 * @param defaultLocation 
 *   This is the default location of the dock widget.  Tecplot 360 saves
 *   the UI state, so this will may be ignored if the dock widget has been used before.
 *
 * @param menuPath 
 *   UNUSED.  In the future this will be the menu location for the menu option
 *   that shows/hides the dock widget.  Currently the dock widget menu item will be appended
 *   to the View menu.
 *
 * @param menuLabel
 *   The name that you want displayed on the menu option which shows/hides the dock widget.  Use
 *   ampersand to include a mnemonic character.  For example: "My D&ock Widget".  This makes 'o'
 *   the keyboard shortcut.
 *
 * @code
 *    QDockWidget* myDockWidget = new QDockWidget();
 *    myDockWidget->setObjectName("Unique Dock Widget Name"));
 *    myDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
 *    myDockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
 *    myDockWidget->setFloating(false);
 *    myDockWidget->setWindowTitle("My Dock Widget");
 *
 *    ... Add controls to your Dock Widget or create your dock widget in QDesigner ...
 *
 *    TecUtilMainWindowAddDockWidget(
 *       *myDockWidget,
 *       DockLocation_Right,
 *       "",
 *       "My &Dock Widget");
 * @endcode
 */
class QDockWidget;
LINKTOADDON void TecUtilMainWindowAddDockWidget(QDockWidget& dockWidget,
                                                        DockLocation_e defaultLocation,
                                                        const char *menuPath,
                                                        const char *menuLabel);
