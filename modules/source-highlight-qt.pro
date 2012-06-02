###########################################################################################
##      Created using Monkey Studio IDE v1.9.0.1 (1.9.0.1)
##
##  Author    : Lorenzo Bettini
##  Project   : source-highlight-qt
##  FileName  : source-highlight-qt.pro
##  Date      : 2012-05-06T12:30:18
##  License   : GPL
##  Comment   : Creating using Monkey Studio RAD
##  Home Page : http://srchiliteqt.sourceforge.net
##
##  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

include( config.pri )
include( ../3rdparty/source-highlight-qt.git/defines.pri )

TARGET_NAME = source-highlight-qt
initializeProject( lib, $${TARGET_NAME}, $${BUILD_MODE}, $${BUILD_PATH}/$${TARGET_NAME}, $${BUILD_TARGET_PATH}, shared )

#CONFIG += create_prl
greaterThan(QT_MAJOR_VERSION, 4):QT *= widgets

INCLUDEPATH = \
    ../3rdparty/source-highlight.git/lib \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt

DEPENDPATH = $${INCLUDEPATH}

FORMS *=  \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/LanguageElemColorForm.ui \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/SourceHighlightSettingsDialog.ui \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/ColorDialog.ui \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/MainColorForm.ui \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/SourceHighlightSettingsPage.ui

HEADERS *=  \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/TextFormatterFactory.h \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/SourceHighlightSettingsDialog.h \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/SourceHighlightExceptionBox.h \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/Qt4SourceHighlightStyleGenerator.h \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/StyleComboBox.h \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/Qt4TextFormatterFactory.h \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/MainColorForm.h \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/GNUSyntaxHighlighter.h \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/Qt4SyntaxHighlighter.h \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/SourceHighlightSettingsPage.h \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/ColorDialog.h \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/LanguageElemColorForm.h \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/TextFormatter.h \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/ParagraphMap.h \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/QtColorMap.h \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/TextEditHighlighted.h \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/LanguageComboBox.h \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/Qt4TextFormatter.h \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/OutputFormatComboBox.h \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/HighlightStateData.h

SOURCES *=  \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/SourceHighlightExceptionBox.cpp \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/TextFormatter.cpp \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/SourceHighlightSettingsDialog.cpp \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/QtColorMap.cpp \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/LanguageElemColorForm.cpp \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/Qt4SyntaxHighlighter.cpp \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/GNUSyntaxHighlighter.cpp \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/MainColorForm.cpp \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/OutputFormatComboBox.cpp \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/ColorDialog.cpp \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/LanguageComboBox.cpp \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/Qt4TextFormatterFactory.cpp \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/TextFormatterFactory.cpp \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/Qt4TextFormatter.cpp \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/TextEditHighlighted.cpp \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/Qt4SourceHighlightStyleGenerator.cpp \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/SourceHighlightSettingsPage.cpp \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/StyleComboBox.cpp \
    ../3rdparty/source-highlight-qt.git/lib/srchiliteqt/ParagraphMap.cpp