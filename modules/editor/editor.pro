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
    StackedDocument.h \
    Document.h \
    StackedDocumentModel.h \
    abstractions/documents/SourceHighlightQtDocument.h \
    StackedDocumentTreeView.h

SOURCES *=  \
    UIMain.cpp \
    main.cpp \
    StackedDocument.cpp \
    Document.cpp \
    StackedDocumentModel.cpp \
    abstractions/documents/SourceHighlightQtDocument.cpp \
    StackedDocumentTreeView.cpp