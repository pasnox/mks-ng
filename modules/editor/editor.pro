###########################################################################################
##      Created using Monkey Studio IDE v1.9.0.1 (1.9.0.1)
##
##  Author    : The Monkey Studio Team
##  Project   : editor
##  FileName  : editor.pro
##  Date      : 2012-05-06T12:30:18
##  License   : GPL
##  Comment   : Creating using Monkey Studio RAD
##  Home Page : http://monkeystudio.org
##
##  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

include( ../modules.pri )

TARGET_NAME = editor
initializeProject( app, $${TARGET_NAME}, $${BUILD_MODE}, $${BUILD_PATH}/$${TARGET_NAME}, $${BUILD_TARGET_PATH}, "" )

INCLUDEPATH *= $$getFolders( . )

DEPENDPATH *= $${INCLUDEPATH}

RESOURCES *= resources/resources.qrc

FORMS *=  \
    settings/SettingsNodeDialogBuilder.ui \
    UIMain.ui \
    document/DocumentIndentationPopup.ui \
    document/DocumentPositionPopup.ui \
    document/StackedDocumentCloseQuery.ui

HEADERS *=  \
    ../../3rdparty/document-properties-discover.git/src/DocumentPropertiesDiscover.h \
    Menu.h \
    settings/SettingsNode.h \
    settings/SettingsNodeDialogBuilder.h \
    settings/SettingsNodeDialogBuilderDelegate.h \
    settings/ApplicationSettings.h \
    settings/ApplicationSettingsDelegate.h \
    settings/RadioGroup.h \
    settings/EolRadioGroup.h \
    settings/IndentationRadioGroup.h \
    settings/RulerRadioGroup.h \
    settings/WrapRadioGroup.h \
    UIMain.h \
    document/Document.h \
    document/DocumentLanguageModel.h \
    document/DocumentEol.h \
    document/DocumentIndentation.h \
    document/DocumentIndentationPopup.h \
    document/DocumentPosition.h \
    document/DocumentPositionPopup.h \
    document/StackedDocument.h \
    document/StackedDocumentModel.h \
    document/StackedDocumentTreeView.h \
    document/StackedDocumentToolBar.h \
    document/StackedDocumentCloseQueryModel.h \
    document/StackedDocumentCloseQuery.h \
    abstractors/CodeEditorMimeType.h \
    abstractors/Abstractors.h \
    abstractors/BaseAbstractor.h \
    abstractors/CodeEditorAbstractor.h \
    abstractors/codeeditor/SourceHighlightQt/SourceHighlightQtCodeEditor.h \
    abstractors/codeeditor/SourceHighlightQt/SourceHighlightQtDocument.h \
    abstractors/codeeditor/SourceHighlightQt/SourceHighlightQtDataWatcher.h

SOURCES *= main.cpp \
    ../../3rdparty/document-properties-discover.git/src/DocumentPropertiesDiscover.cpp \
    settings/SettingsNode.cpp \
    settings/SettingsNodeDialogBuilder.cpp \
    settings/SettingsNodeDialogBuilderDelegate.cpp \
    settings/ApplicationSettings.cpp \
    settings/ApplicationSettingsDelegate.cpp \
    settings/RadioGroup.cpp \
    settings/EolRadioGroup.cpp \
    settings/IndentationRadioGroup.cpp \
    settings/RulerRadioGroup.cpp \
    settings/WrapRadioGroup.cpp \
    UIMain.cpp \
    document/Document.cpp \
    document/DocumentLanguageModel.cpp \
    document/DocumentEol.cpp \
    document/DocumentIndentation.cpp \
    document/DocumentIndentationPopup.cpp \
    document/DocumentPosition.cpp \
    document/DocumentPositionPopup.cpp \
    document/StackedDocument.cpp \
    document/StackedDocumentModel.cpp \
    document/StackedDocumentTreeView.cpp \
    document/StackedDocumentToolBar.cpp \
    document/StackedDocumentCloseQueryModel.cpp \
    document/StackedDocumentCloseQuery.cpp \
    abstractors/CodeEditorMimeType.cpp \
    abstractors/Abstractors.cpp \
    abstractors/codeeditor/SourceHighlightQt/SourceHighlightQtCodeEditor.cpp \
    abstractors/codeeditor/SourceHighlightQt/SourceHighlightQtDocument.cpp \
    abstractors/codeeditor/SourceHighlightQt/SourceHighlightQtDataWatcher.cpp

!greaterThan(QT_MAJOR_VERSION, 4) {
    RESOURCES *= ../../3rdparty/qfreedesktopmime.tar/freedesktopmime.qrc
    HEADERS *= ../../3rdparty/qfreedesktopmime.tar/src/freedesktopmime.h
    SOURCES *= ../../3rdparty/qfreedesktopmime.tar/src/freedesktopmime.cpp
}