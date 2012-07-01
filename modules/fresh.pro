###########################################################################################
##      Created using Monkey Studio IDE v1.9.0.1 (1.9.0.1)
##
##  Author    : The Monkey Studio Team
##  Project   : fresh
##  FileName  : fresh.pro
##  Date      : 2012-05-06T12:30:18
##  License   : GPL
##  Comment   : Creating using Monkey Studio RAD
##  Home Page : http://monkeystudio.org
##
##  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

include( config.pri )
include( ../3rdparty/fresh.git/defines.pri )

TARGET_NAME = fresh
initializeProject( lib, $${TARGET_NAME}, $${BUILD_MODE}, $${BUILD_PATH}/$${TARGET_NAME}, $${BUILD_TARGET_PATH}, shared )

QT *= network xml
greaterThan(QT_MAJOR_VERSION, 4):QT *= widgets

macx:LIBS *= "-framework CoreFoundation"

INCLUDEPATH *= $$getFolders( ../3rdparty/fresh.git/src )

DEPENDPATH *= $${INCLUDEPATH}

RESOURCES *= ../3rdparty/fresh.git/resources/fresh.qrc

FORMS *=  \
    ../3rdparty/fresh.git/src/gui/pTranslationDialog.ui \
    ../3rdparty/fresh.git/src/gui/gc-updatechecker/pUpdateCheckerDialog.ui \
    ../3rdparty/fresh.git/src/gui/actionmanager/pActionsShortcutEditor.ui \
    ../3rdparty/fresh.git/src/gui/environmentvariablemanager/pEnvironmentVariableEditor.ui \
    ../3rdparty/fresh.git/src/gui/environmentvariablemanager/pEnvironmentVariablesEditor.ui

HEADERS *=  \
    ../3rdparty/fresh.git/src/gui/pIconManager.h \
    ../3rdparty/fresh.git/src/core/pNetworkAccessManager.h \
    ../3rdparty/fresh.git/src/gui/pTranslationDialog.h \
    ../3rdparty/fresh.git/src/core/FreshExport.h \
    ../3rdparty/fresh.git/src/gui/pTreeComboBox.h \
    ../3rdparty/fresh.git/src/gui/dockmanager/pDockToolBarManager.h \
    ../3rdparty/fresh.git/src/gui/actionmanager/pActionsMenuBar.h \
    ../3rdparty/fresh.git/src/gui/pCheckComboBox.h \
    ../3rdparty/fresh.git/src/gui/pMainWindow.h \
    ../3rdparty/fresh.git/src/gui/gc-updatechecker/pUpdateCheckerDialog.h \
    ../3rdparty/fresh.git/src/gui/models/pRecursiveSortFilterProxyModel.h \
    ../3rdparty/fresh.git/src/gui/consolemanager/pConsoleCommand.h \
    ../3rdparty/fresh.git/src/gui/pPaypalButton.h \
    ../3rdparty/fresh.git/src/gui/pToolButton.h \
    ../3rdparty/fresh.git/src/core/pSettings.h \
    ../3rdparty/fresh.git/src/gui/listeditor/pPathListEditor.h \
    ../3rdparty/fresh.git/src/core/pGenericTableModel.h \
    ../3rdparty/fresh.git/src/gui/dockmanager/pDockToolBar.h \
    ../3rdparty/fresh.git/src/gui/environmentvariablemanager/pEnvironmentVariablesManager.h \
    ../3rdparty/fresh.git/src/core/pVersion.h \
    ../3rdparty/fresh.git/src/core/pGetOpt.h \
    ../3rdparty/fresh.git/src/gui/environmentvariablemanager/pEnvironmentVariablesModel.h \
    ../3rdparty/fresh.git/src/gui/queuedmessage/pQueuedMessageWidget.h \
    ../3rdparty/fresh.git/src/gui/stylemanager/pStylesActionGroup.h \
    ../3rdparty/fresh.git/src/gui/consolemanager/pConsole.h \
    ../3rdparty/fresh.git/src/gui/gc-updatechecker/pUpdateChecker.h \
    ../3rdparty/fresh.git/src/gui/pGuiUtils.h \
    ../3rdparty/fresh.git/src/gui/pColorButton.h \
    ../3rdparty/fresh.git/src/gui/pFontButton.h \
    ../3rdparty/fresh.git/src/gui/actionmanager/pActionsShortcutEditor.h \
    ../3rdparty/fresh.git/src/gui/actionmanager/pActionsModel.h \
    ../3rdparty/fresh.git/src/gui/pFileDialog.h \
    ../3rdparty/fresh.git/src/gui/pKeySequenceEdit.h \
    ../3rdparty/fresh.git/src/gui/environmentvariablemanager/pEnvironmentVariablesEditor.h \
    ../3rdparty/fresh.git/src/gui/environmentvariablemanager/pEnvironmentVariableEditor.h \
    ../3rdparty/fresh.git/src/gui/listeditor/pStringListEditor.h \
    ../3rdparty/fresh.git/src/core/pLocaleModel.h \
    ../3rdparty/fresh.git/src/core/pTranslationManager.h \
    ../3rdparty/fresh.git/src/gui/listeditor/pFileListEditor.h \
    ../3rdparty/fresh.git/src/gui/Gui.h \
    ../3rdparty/fresh.git/src/core/Fresh.h \
    ../3rdparty/fresh.git/src/gui/queuedmessage/pQueuedMessageToolBar.h \
    ../3rdparty/fresh.git/src/core/Core.h \
    ../3rdparty/fresh.git/src/core/pCoreUtils.h \
    ../3rdparty/fresh.git/src/gui/pLineEdit.h \
    ../3rdparty/fresh.git/src/gui/dockwidget/pDockWidget.h \
    ../3rdparty/fresh.git/src/gui/stylemanager/pStylesToolButton.h \
    ../3rdparty/fresh.git/src/gui/dockwidget/pDockWidgetTitleBar.h

SOURCES *=  \
    ../3rdparty/fresh.git/src/core/pNetworkAccessManager.cpp \
    ../3rdparty/fresh.git/src/gui/queuedmessage/pQueuedMessageWidget.cpp \
    ../3rdparty/fresh.git/src/gui/pKeySequenceEdit.cpp \
    ../3rdparty/fresh.git/src/gui/models/pRecursiveSortFilterProxyModel.cpp \
    ../3rdparty/fresh.git/src/gui/stylemanager/pStylesActionGroup.cpp \
    ../3rdparty/fresh.git/src/core/pTranslationManager.cpp \
    ../3rdparty/fresh.git/src/gui/pPaypalButton.cpp \
    ../3rdparty/fresh.git/src/core/pCoreUtils.cpp \
    ../3rdparty/fresh.git/src/gui/pCheckComboBox.cpp \
    ../3rdparty/fresh.git/src/gui/listeditor/pPathListEditor.cpp \
    ../3rdparty/fresh.git/src/gui/environmentvariablemanager/pEnvironmentVariablesManager.cpp \
    ../3rdparty/fresh.git/src/gui/listeditor/pStringListEditor.cpp \
    ../3rdparty/fresh.git/src/core/pLocaleModel.cpp \
    ../3rdparty/fresh.git/src/gui/actionmanager/pActionsMenuBar.cpp \
    ../3rdparty/fresh.git/src/gui/environmentvariablemanager/pEnvironmentVariablesEditor.cpp \
    ../3rdparty/fresh.git/src/gui/pGuiUtils.cpp \
    ../3rdparty/fresh.git/src/gui/pMainWindow.cpp \
    ../3rdparty/fresh.git/src/gui/actionmanager/pActionsShortcutEditor.cpp \
    ../3rdparty/fresh.git/src/gui/listeditor/pFileListEditor.cpp \
    ../3rdparty/fresh.git/src/gui/pLineEdit.cpp \
    ../3rdparty/fresh.git/src/gui/pToolButton.cpp \
    ../3rdparty/fresh.git/src/gui/pFileDialog.cpp \
    ../3rdparty/fresh.git/src/gui/gc-updatechecker/pUpdateChecker.cpp \
    ../3rdparty/fresh.git/src/core/pSettings.cpp \
    ../3rdparty/fresh.git/src/gui/pTranslationDialog.cpp \
    ../3rdparty/fresh.git/src/gui/queuedmessage/pQueuedMessageToolBar.cpp \
    ../3rdparty/fresh.git/src/gui/stylemanager/pStylesToolButton.cpp \
    ../3rdparty/fresh.git/src/gui/actionmanager/pActionsModel.cpp \
    ../3rdparty/fresh.git/src/gui/environmentvariablemanager/pEnvironmentVariablesModel.cpp \
    ../3rdparty/fresh.git/src/gui/environmentvariablemanager/pEnvironmentVariableEditor.cpp \
    ../3rdparty/fresh.git/src/core/pGenericTableModel.cpp \
    ../3rdparty/fresh.git/src/gui/dockwidget/pDockWidgetTitleBar.cpp \
    ../3rdparty/fresh.git/src/gui/dockmanager/pDockToolBar.cpp \
    ../3rdparty/fresh.git/src/core/Fresh.cpp \
    ../3rdparty/fresh.git/src/gui/consolemanager/pConsoleCommand.cpp \
    ../3rdparty/fresh.git/src/gui/dockwidget/pDockWidget.cpp \
    ../3rdparty/fresh.git/src/gui/dockmanager/pDockToolBarManager.cpp \
    ../3rdparty/fresh.git/src/gui/pIconManager.cpp \
    ../3rdparty/fresh.git/src/gui/gc-updatechecker/pUpdateCheckerDialog.cpp \
    ../3rdparty/fresh.git/src/core/pGetOpt.cpp \
    ../3rdparty/fresh.git/src/gui/pTreeComboBox.cpp \
    ../3rdparty/fresh.git/src/core/pVersion.cpp \
    ../3rdparty/fresh.git/src/gui/pColorButton.cpp \
    ../3rdparty/fresh.git/src/gui/pFontButton.cpp \
    ../3rdparty/fresh.git/src/gui/consolemanager/pConsole.cpp

win32 {
    HEADERS *= ../3rdparty/fresh.git/src/core/pWinHelpers.h
}

macx {
    HEADERS *= ../3rdparty/fresh.git/src/core/pMacHelpers.h
    OBJECTIVE_SOURCES *= ../3rdparty/fresh.git/src/core/pMacHelpers.mm
}