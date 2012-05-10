###########################################################################################
##      Created using Monkey Studio IDE v1.9.0.1 (1.9.0.1)
##
##  Author    : The Monkey Studio Team
##  Project   : build
##  FileName  : build.pro
##  Date      : 2012-05-06T12:18:03
##  License   : GPL
##  Comment   : Creating using Monkey Studio RAD
##  Home Page : http://monkeystudio.org
##
##  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

XUP.QT_VERSION = 5.0.0-alpha
XUP.OTHERS_PLATFORM_TARGET_DEFAULT = bin/Linux/editor_debug

include( modules/config.pri )

TEMPLATE = subdirs
CONFIG *= ordered
SUBDIRS *=  \
    modules/modules.pro