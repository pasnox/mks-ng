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

include( $$PWD/config.pri )

QT *= network xml
greaterThan(QT_MAJOR_VERSION, 4):QT *= widgets

INCLUDEPATH = \
    $$PWD/../3rdparty/document-properties-discover.git/src \
    $$PWD/../3rdparty/qfreedesktopmime.git/src \
    $$PWD/../3rdparty/source-highlight.git/lib \
    $$PWD/../3rdparty/source-highlight-qt.git/lib \
    $$getFolders( $$PWD/../3rdparty/fresh.git/src ) \
    $$PWD/../3rdparty/fresh.git/include

DEPENDPATH = $${INCLUDEPATH}

LIBS *= -L$${BUILD_TARGET_PATH}
LIBS *= \
    -l$$targetForMode( fresh, $${BUILD_MODE} ) \
    -l$$targetForMode( source-highlight-qt, $${BUILD_MODE} ) \
    -l$$targetForMode( source-highlight, $${BUILD_MODE} ) \
    -lboost_regex
    
