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

QT *= widgets network xml

INCLUDEPATH = \
    $$PWD/../3rdparty/src_highlite.git/lib/srchilite \
    $$PWD/../3rdparty/src_highlite_qt.git/lib/srchiliteqt \
    $$getFolders( $$PWD/../3rdparty/fresh.git/src )

DEPENDPATH = $${INCLUDEPATH}

LIBS *= -L$${BUILD_TARGET_PATH}
LIBS *= \
    -lsource-highlite \
    -lsource-highlite-qt \
    -lfresh
