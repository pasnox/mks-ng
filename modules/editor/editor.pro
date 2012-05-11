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

DEPENDPATH = $${INCLUDEPATH}

RESOURCES *= resources/resources.qrc

FORMS *=  \
    UIMain.ui \
    document/DocumentIndentationPopup.ui

HEADERS *=  \
    UIMain.h \
    document/Document.h \
    document/StackedDocument.h \
    document/StackedDocumentModel.h \
    document/StackedDocumentTreeView.h \
    document/StackedDocumentToolBar.h \
    document/DocumentEol.h \
    document/DocumentIndentation.h \
    document/DocumentIndentationPopup.h \
    document/DocumentPosition.h \
    abstractors/Abstractors.h \
    abstractors/BaseAbstractor.h \
    abstractors/CodeEditorAbstractor.h \
    abstractors/codeeditor/SourceHighlightQt/SourceHighlightQtCodeEditor.h \
    abstractors/codeeditor/SourceHighlightQt/SourceHighlightQtDocument.h \
    abstractors/codeeditor/SourceHighlightQt/SourceHighlightQtDataWatcher.h

SOURCES *= main.cpp \
    UIMain.cpp \
    document/Document.cpp \
    document/StackedDocument.cpp \
    document/StackedDocumentModel.cpp \
    document/StackedDocumentTreeView.cpp \
    document/StackedDocumentToolBar.cpp \
    document/DocumentEol.cpp \
    document/DocumentIndentation.cpp \
    document/DocumentIndentationPopup.cpp \
    document/DocumentPosition.cpp \
    abstractors/Abstractors.cpp \
    abstractors/codeeditor/SourceHighlightQt/SourceHighlightQtCodeEditor.cpp \
    abstractors/codeeditor/SourceHighlightQt/SourceHighlightQtDocument.cpp \
    abstractors/codeeditor/SourceHighlightQt/SourceHighlightQtDataWatcher.cpp