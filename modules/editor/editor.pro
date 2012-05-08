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

FORMS *=  \
    UIMain.ui

HEADERS *=  \
    UIMain.h \
    document/Document.h \
    document/StackedDocument.h \
    document/StackedDocumentModel.h \
    document/StackedDocumentTreeView.h \
    document/abstractions/SourceHighlightQt/SourceHighlightQtDocument.h

SOURCES *=  \
    main.cpp \
    UIMain.cpp \
    document/Document.cpp \
    document/StackedDocument.cpp \
    document/StackedDocumentModel.cpp \
    document/StackedDocumentTreeView.cpp \
    document/abstractions/SourceHighlightQt/SourceHighlightQtDocument.cpp