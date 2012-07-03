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
initializeProject( lib, $${TARGET_NAME}, $${BUILD_MODE}, $${BUILD_PATH}/$${TARGET_NAME}, $${BUILD_TARGET_PATH}, shared, "0.1.0" )

QT *= xml
greaterThan(QT_MAJOR_VERSION, 4):QT *= widgets

INCLUDEPATH *= $$getFolders( ../3rdparty/qodeedit.git/src )

DEPENDPATH *= $${INCLUDEPATH}

RESOURCES *= ../3rdparty/qodeedit.git/resources/qodeedit.qrc

FORMS *= 

HEADERS *=  \
    ../3rdparty/qodeedit.git/src/QodeEdit.h

SOURCES *=  \
    ../3rdparty/qodeedit.git/src/QodeEdit.cpp