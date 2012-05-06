###########################################################################################
##      Created using Monkey Studio IDE v1.9.0.1 (1.9.0.1)
##
##  Author    : Lorenzo Bettini
##  Project   : source-highlite
##  FileName  : source-highlite.pro
##  Date      : 2012-05-06T12:30:18
##  License   : GPL
##  Comment   : Creating using Monkey Studio RAD
##  Home Page : http://www.gnu.org/software/src-highlite
##
##  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

include( config.pri )
include( auto_file_h.pri )
include( ../3rdparty/src_highlite.git/defines.pri )

TARGET_NAME = source-highlite
initializeProject( lib, $${TARGET_NAME}, $${BUILD_MODE}, $${BUILD_PATH}/$${TARGET_NAME}, $${BUILD_TARGET_PATH}, shared )

# defines.pri does not declare variables, only defines. Let declare variables from defines.
#for( define, DEFINES ) {
#    # evaluate the define
#    eval( $$define )
#    # get variable name
#    name = $$define
#    name = $$replace( name, "=.*", $${Q_NULL} )
#    # remove quotes from values
#    $$name = $$replace( $$name, $${Q_QUOTE}, $${Q_NULL} )
#}

#autoGenerateFile( source-highlite-config.h.in, ../3rdparty/src_highlite.git/lib/srchilite/config.h )

#DEFINES *= HAVE_CONFIG_H

#CONFIG += create_prl

INCLUDEPATH = ../3rdparty/src_highlite.git/lib/srchilite
DEPENDPATH = $${INCLUDEPATH}

HEADERS *= ../3rdparty/src_highlite.git/lib/srchilite/formattermanager.h \
    ../3rdparty/src_highlite.git/lib/srchilite/langelemsprinter.h \
    ../3rdparty/src_highlite.git/lib/srchilite/tostringcollection.h \
    ../3rdparty/src_highlite.git/lib/srchilite/stringlistlangelem.h \
    ../3rdparty/src_highlite.git/lib/srchilite/srcuntabifier.h \
    ../3rdparty/src_highlite.git/lib/srchilite/textstyle.h \
    ../3rdparty/src_highlite.git/lib/srchilite/refposition.h \
    ../3rdparty/src_highlite.git/lib/srchilite/highlighteventlistener.h \
    ../3rdparty/src_highlite.git/lib/srchilite/namedsubexpslangelem.h \
    ../3rdparty/src_highlite.git/lib/srchilite/highlightstatebuilder.h \
    ../3rdparty/src_highlite.git/lib/srchilite/chartranslator.h \
    ../3rdparty/src_highlite.git/lib/srchilite/textstyles.h \
    ../3rdparty/src_highlite.git/lib/srchilite/highlightrule.h \
    ../3rdparty/src_highlite.git/lib/srchilite/sourcehighlightutils.h \
    ../3rdparty/src_highlite.git/lib/srchilite/ctagsmanager.h \
    ../3rdparty/src_highlite.git/lib/srchilite/linenumgenerator.h \
    ../3rdparty/src_highlite.git/lib/srchilite/langdefparserfun.h \
    ../3rdparty/src_highlite.git/lib/srchilite/parserinfo.h \
    ../3rdparty/src_highlite.git/lib/srchilite/ctagscollector.h \
    ../3rdparty/src_highlite.git/lib/srchilite/langelemsprinter.hpp \
    ../3rdparty/src_highlite.git/lib/srchilite/langelems.h \
    ../3rdparty/src_highlite.git/lib/srchilite/colormap.h \
    ../3rdparty/src_highlite.git/lib/srchilite/parserexception.h \
    ../3rdparty/src_highlite.git/lib/srchilite/textstyleformattercollection.h \
    ../3rdparty/src_highlite.git/lib/srchilite/formatter.h \
    ../3rdparty/src_highlite.git/lib/srchilite/highlightstate.h \
    ../3rdparty/src_highlite.git/lib/srchilite/highlightstateprinter.h \
    ../3rdparty/src_highlite.git/lib/srchilite/highlighttoken.h \
    ../3rdparty/src_highlite.git/lib/srchilite/parsestyles.h \
    ../3rdparty/src_highlite.git/lib/srchilite/regexrulefactory.h \
    ../3rdparty/src_highlite.git/lib/srchilite/languageinfer.h \
    ../3rdparty/src_highlite.git/lib/srchilite/regexhighlightrule.h \
    ../3rdparty/src_highlite.git/lib/srchilite/highlightbuilderexception.h \
    ../3rdparty/src_highlite.git/lib/srchilite/preformatter.h \
    ../3rdparty/src_highlite.git/lib/srchilite/textstyleformatter.h \
    ../3rdparty/src_highlite.git/lib/srchilite/textstylebuilder.h \
    ../3rdparty/src_highlite.git/lib/srchilite/styleparser.h \
    ../3rdparty/src_highlite.git/lib/srchilite/bufferedoutput.h \
    ../3rdparty/src_highlite.git/lib/srchilite/fileutil.h \
    ../3rdparty/src_highlite.git/lib/srchilite/outlangdefparserfun.h \
    ../3rdparty/src_highlite.git/lib/srchilite/docgenerator.h \
    ../3rdparty/src_highlite.git/lib/srchilite/stopwatch.h \
    ../3rdparty/src_highlite.git/lib/srchilite/settings.h \
    ../3rdparty/src_highlite.git/lib/srchilite/ioexception.h \
    ../3rdparty/src_highlite.git/lib/srchilite/stringtable.h \
    ../3rdparty/src_highlite.git/lib/srchilite/parsestruct.h \
    ../3rdparty/src_highlite.git/lib/srchilite/stylekey.h \
    ../3rdparty/src_highlite.git/lib/srchilite/verbosity.h \
    ../3rdparty/src_highlite.git/lib/srchilite/vardefinitions.h \
    ../3rdparty/src_highlite.git/lib/srchilite/regexranges.h \
    ../3rdparty/src_highlite.git/lib/srchilite/linebuffer.h \
    ../3rdparty/src_highlite.git/lib/srchilite/lineranges.h \
    ../3rdparty/src_highlite.git/lib/srchilite/readtags.h \
    ../3rdparty/src_highlite.git/lib/srchilite/delimitedlangelem.h \
    ../3rdparty/src_highlite.git/lib/srchilite/sourcefilehighlighter.h \
    ../3rdparty/src_highlite.git/lib/srchilite/colors.h \
    ../3rdparty/src_highlite.git/lib/srchilite/ctagsformatter.h \
    ../3rdparty/src_highlite.git/lib/srchilite/matchingparameters.h \
    ../3rdparty/src_highlite.git/lib/srchilite/stylefileparser.h \
    ../3rdparty/src_highlite.git/lib/srchilite/sourcehighlight.h \
    ../3rdparty/src_highlite.git/lib/srchilite/langdefparser.h \
    ../3rdparty/src_highlite.git/lib/srchilite/highlightrulefactory.h \
    ../3rdparty/src_highlite.git/lib/srchilite/doctemplate.h \
    ../3rdparty/src_highlite.git/lib/srchilite/instances.h \
    ../3rdparty/src_highlite.git/lib/srchilite/stylecssparser.h \
    ../3rdparty/src_highlite.git/lib/srchilite/formatterparams.h \
    ../3rdparty/src_highlite.git/lib/srchilite/versions.h \
    ../3rdparty/src_highlite.git/lib/srchilite/sourcehighlighter.h \
    ../3rdparty/src_highlite.git/lib/srchilite/textstyleformatterfactory.h \
    ../3rdparty/src_highlite.git/lib/srchilite/eventgenerator.h \
    ../3rdparty/src_highlite.git/lib/srchilite/fileinfo.h \
    ../3rdparty/src_highlite.git/lib/srchilite/statelangelem.h \
    ../3rdparty/src_highlite.git/lib/srchilite/langelem.h \
    ../3rdparty/src_highlite.git/lib/srchilite/regexpreprocessor.h \
    ../3rdparty/src_highlite.git/lib/srchilite/formatterfactory.h \
    ../3rdparty/src_highlite.git/lib/srchilite/langmap.h \
    ../3rdparty/src_highlite.git/lib/srchilite/debuglistener.h \
    ../3rdparty/src_highlite.git/lib/srchilite/keys.h \
    ../3rdparty/src_highlite.git/lib/srchilite/outlangdefparser.h \
    ../3rdparty/src_highlite.git/lib/srchilite/substfun.h \
    ../3rdparty/src_highlite.git/lib/srchilite/statestartlangelem.h \
    ../3rdparty/src_highlite.git/lib/srchilite/stringdef.h \
    ../3rdparty/src_highlite.git/lib/srchilite/wordtokenizer.h \
    ../3rdparty/src_highlite.git/lib/srchilite/outlangdefscanner.h \
    ../3rdparty/src_highlite.git/lib/srchilite/langdefscanner.h \
    ../3rdparty/src_highlite.git/lib/srchilite/highlightevent.h \
    ../3rdparty/src_highlite.git/lib/srchilite/highlightstatebuilder.hpp \
    ../3rdparty/src_highlite.git/lib/srchilite/langdefmanager.h \
    ../3rdparty/src_highlite.git/lib/srchilite/utils.h

SOURCES *= ../3rdparty/src_highlite.git/lib/srchilite/utils.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/wordtokenizer.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/highlightrule.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/ctagsmanager.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/langelems.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/ioexception.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/stopwatch.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/highlightstatebuilder_dbtab.cc \
    ../3rdparty/src_highlite.git/lib/srchilite/langelemsprinter.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/styleparser.cc \
    ../3rdparty/src_highlite.git/lib/srchilite/stylecssscanner.cc \
    ../3rdparty/src_highlite.git/lib/srchilite/yywrap.c \
    ../3rdparty/src_highlite.git/lib/srchilite/vardefinitions.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/parserexception.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/langdefmanager.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/namedsubexpslangelem.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/lineranges.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/statelangelem.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/regexpreprocessor.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/sourcehighlighter.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/fileinfo.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/regexranges.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/settings.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/preformatter.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/formatter.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/verbosity.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/docgenerator.cc \
    ../3rdparty/src_highlite.git/lib/srchilite/langdefscanner.cc \
    ../3rdparty/src_highlite.git/lib/srchilite/textstylebuilder.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/stringdef.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/readtags.c \
    ../3rdparty/src_highlite.git/lib/srchilite/highlightbuilderexception.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/srcuntabifier.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/ctagsformatter.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/regexhighlightrule.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/highlightstateprinter.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/languageinfer.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/outlangdefscanner.cc \
    ../3rdparty/src_highlite.git/lib/srchilite/textstyleformatterfactory.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/chartranslator.cc \
    ../3rdparty/src_highlite.git/lib/srchilite/fileutil.cc \
    ../3rdparty/src_highlite.git/lib/srchilite/stylescanner.cc \
    ../3rdparty/src_highlite.git/lib/srchilite/textstyle.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/linenumgenerator.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/sourcehighlight.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/stylefileparser.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/highlighttoken.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/formattermanager.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/outlangdefparser.cc \
    ../3rdparty/src_highlite.git/lib/srchilite/ctagscollector.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/versions.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/langelem.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/langelemsprinter_dbtab.cc \
    ../3rdparty/src_highlite.git/lib/srchilite/stringlistlangelem.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/sourcehighlightutils.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/highlightstatebuilder.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/doctemplate.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/highlightrulefactory.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/regexrulefactory.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/stringtable.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/instances.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/test_readtags_main.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/textstyleformatter.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/substfun.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/sourcefilehighlighter.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/langdefparser.cc \
    ../3rdparty/src_highlite.git/lib/srchilite/debuglistener.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/stylecssparser.cc \
    ../3rdparty/src_highlite.git/lib/srchilite/langmap.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/statestartlangelem.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/bufferedoutput.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/highlightstate.cpp \
    ../3rdparty/src_highlite.git/lib/srchilite/delimitedlangelem.cpp

#YACCSOURCES *= ../3rdparty/src_highlite.git/lib/srchilite/langdefparser.yy \
#    ../3rdparty/src_highlite.git/lib/srchilite/outlangdefparser.yy \
#    ../3rdparty/src_highlite.git/lib/srchilite/stylecssparser.yy \
#    ../3rdparty/src_highlite.git/lib/srchilite/styleparser.yy

#LEXSOURCES *= ../3rdparty/src_highlite.git/lib/srchilite/langdefscanner.ll \
#    ../3rdparty/src_highlite.git/lib/srchilite/stylescanner.ll \
#    ../3rdparty/src_highlite.git/lib/srchilite/outlangdefscanner.ll \
#    ../3rdparty/src_highlite.git/lib/srchilite/stylecssscanner.ll