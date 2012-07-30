###########################################################################################
##      Created using Monkey Studio IDE v1.9.0.1 (1.9.0.1)
##
##  Author    : The Monkey Studio Team
##  Project   : qodeedit
##  FileName  : qodeedit.pro
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

TARGET_NAME = qodeedit
ABSOLUTE_DATA_DIR = $$PWD/share
initializeProject( lib, $${TARGET_NAME}, $${BUILD_MODE}, $${BUILD_PATH}/$${TARGET_NAME}, $${BUILD_LIBRARY_TARGET_PATH}, shared, "0.1.0" )
autoGenerateFile( "../3rdparty/qodeedit.git/main.h.in", "../3rdparty/qodeedit.git/src/main.h" )

QT *= xml
greaterThan(QT_MAJOR_VERSION, 4):QT *= widgets

INCLUDEPATH *= $$getFolders( ../3rdparty/qodeedit.git/src )

DEPENDPATH *= $${INCLUDEPATH}

RESOURCES *= ../3rdparty/qodeedit.git/resources/qodeedit.qrc

FORMS *= 

HEADERS *= ../3rdparty/qodeedit.git/src/QodeEdit.h \
    ../3rdparty/qodeedit.git/src/editor/CodeEditor.h \
    ../3rdparty/qodeedit.git/src/document/TextDocument.h \
    ../3rdparty/qodeedit.git/src/document/PlainTextDocumentLayout.h \
    ../3rdparty/qodeedit.git/src/document/TextBlockUserData.h \
    ../3rdparty/qodeedit.git/src/margins/MarginStacker.h \
    ../3rdparty/qodeedit.git/src/margins/AbstractMargin.h \
    ../3rdparty/qodeedit.git/src/margins/LineBookmarkMargin.h \
    ../3rdparty/qodeedit.git/src/margins/LineNumberMargin.h \
    ../3rdparty/qodeedit.git/src/margins/LineRevisionMargin.h \
    ../3rdparty/qodeedit.git/src/margins/LineSpacingMargin.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxItemData.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxRule.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxComment.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxKeywords.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxHighlighting.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxContext.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxGeneral.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxDocument.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxParser.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxList.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxFolding.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxEmptyLine.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxIndentation.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxSpellChecking.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxConfiguration.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxEncoding.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxFactory.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxModel.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxHighlighter.h \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxComboBox.h

SOURCES *= ../3rdparty/qodeedit.git/src/QodeEdit.cpp \
    ../3rdparty/qodeedit.git/src/editor/CodeEditor.cpp \
    ../3rdparty/qodeedit.git/src/document/TextDocument.cpp \
    ../3rdparty/qodeedit.git/src/document/PlainTextDocumentLayout.cpp \
    ../3rdparty/qodeedit.git/src/document/TextBlockUserData.cpp \
    ../3rdparty/qodeedit.git/src/margins/MarginStacker.cpp \
    ../3rdparty/qodeedit.git/src/margins/AbstractMargin.cpp \
    ../3rdparty/qodeedit.git/src/margins/LineBookmarkMargin.cpp \
    ../3rdparty/qodeedit.git/src/margins/LineNumberMargin.cpp \
    ../3rdparty/qodeedit.git/src/margins/LineRevisionMargin.cpp \
    ../3rdparty/qodeedit.git/src/margins/LineSpacingMargin.cpp \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxRule.cpp \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxContext.cpp \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxComment.cpp \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxDocument.cpp \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxGeneral.cpp \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxKeywords.cpp \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxHighlighting.cpp \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxItemData.cpp \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxParser.cpp \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxFolding.cpp \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxEmptyLine.cpp \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxIndentation.cpp \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxSpellChecking.cpp \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxConfiguration.cpp \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxEncoding.cpp \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxFactory.cpp \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxModel.cpp \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxHighlighter.cpp \
    ../3rdparty/qodeedit.git/src/syntax/SyntaxComboBox.cpp