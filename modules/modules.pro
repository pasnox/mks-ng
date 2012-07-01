###########################################################################################
##      Created using Monkey Studio IDE v1.9.0.1 (1.9.0.1)
##
##  Author    : The Monkey Studio Team
##  Project   : modules
##  FileName  : modules.pro
##  Date      : 2012-05-06T12:19:35
##  License   : GPL
##  Comment   : Creating using Monkey Studio RAD
##  Home Page : http://monkeystudio.org
##
##  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

include( config.pri )

TEMPLATE = subdirs
CONFIG *= ordered
SUBDIRS *= \
    qodeedit.pro \
    source-highlight.pro \
    source-highlight-qt.pro \
    fresh.pro \
    editor/editor.pro