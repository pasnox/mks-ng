#include "SourceHighlightQtCodeEditor.h"
#include "SourceHighlightQtDocument.h"

#include <QDir>
#include <QStringList>
#if defined( HAS_QT_5 )
#include <QMimeType>
#endif
#include <QDebug>

SourceHighlightQtCodeEditor::SourceHighlightQtCodeEditor( QObject* parent )
    : CodeEditorAbstractor( parent ), mDataWatcher( new SourceHighlightQtDataWatcher( this ) )
{
#if defined( HAS_QT_5 )
    connect( mDataWatcher, &SourceHighlightQtDataWatcher::filesChanged, this, &SourceHighlightQtCodeEditor::dataWatcher_filesChanged );
#else
    connect( mDataWatcher, SIGNAL( filesChanged( SourceHighlightQtDataWatcher::Type ) ), this, SLOT( dataWatcher_filesChanged( SourceHighlightQtDataWatcher::Type ) ) );
#endif
}

SourceHighlightQtCodeEditor::~SourceHighlightQtCodeEditor()
{
}

QIcon SourceHighlightQtCodeEditor::iconForLanguage( const QString& language ) const
{
    const QString name = language;
    static QHash<QString, QString> mimeTypes;
    
    if ( mimeTypes.isEmpty() ) {
        // specials
        mimeTypes[ "default" ] = "";
        mimeTypes[ "langdef" ] = "";
        mimeTypes[ "nohilite" ] = "";
        mimeTypes[ "outlang" ] = "";
        mimeTypes[ "style" ] = "";
        //
        mimeTypes[ "ada" ] = "text/x-adasrc";
        mimeTypes[ "applescript" ] = "";
        mimeTypes[ "asm" ] = "text/x-nasm";
        mimeTypes[ "awk" ] = "application/x-awk";
        mimeTypes[ "bat" ] = "application/x-shellscript";
        mimeTypes[ "bib" ] = "text/x-bibtex";
        mimeTypes[ "bison" ] = "";
        mimeTypes[ "c_*" ] = "text/x-csrc"; // c_comment, c_string
        mimeTypes[ "c" ] = "text/x-csrc";
        mimeTypes[ "caml" ] = "text/x-ocaml";
        mimeTypes[ "changelog" ] = "text/x-changelog";
        mimeTypes[ "clike_vardeclaration" ] = "text/x-csrc";
        mimeTypes[ "clipper" ] = "";
        mimeTypes[ "cobol" ] = "text/x-cobol";
        mimeTypes[ "conf" ] = "text/x-ini";
        mimeTypes[ "cpp" ] = "text/x-c++src";
        mimeTypes[ "csharp" ] = "text/x-csharp";
        mimeTypes[ "css" ] = "text/css";
        mimeTypes[ "d" ] = "text/x-dsrc";
        mimeTypes[ "desktop" ] = "application/x-desktop";
        mimeTypes[ "diff" ] = "text/x-patch";
        mimeTypes[ "erlang" ] = "text/x-erlang";
        mimeTypes[ "errors" ] = "";
        mimeTypes[ "extreme_*" ] = ""; // extreme_comment, extreme_comment2, extreme_comment3, extreme_comment4
        mimeTypes[ "*fortran" ] = "text/x-fortran"; // fixed-fortran, fortran
        mimeTypes[ "flex" ] = "";
        mimeTypes[ "function" ] = "";
        mimeTypes[ "glsl" ] = "application/x-glsl";
        mimeTypes[ "haskell" ] = "text/x-haskell";
        mimeTypes[ "haskell_literate" ] = "text/x-literate-haskell";
        mimeTypes[ "haxe" ] = "text/x-haxe";
        mimeTypes[ "html*" ] = "text/html"; // html_simple, html
        mimeTypes[ "islisp" ] = "text/x-emacs-lisp";
        mimeTypes[ "java" ] = "text/x-java";
        mimeTypes[ "javalog" ] = "text/x-java";
        mimeTypes[ "javascript" ] = "application/javascript";
        mimeTypes[ "key_string" ] = "";
        mimeTypes[ "latex" ] = "text/x-tex";
        mimeTypes[ "ldap" ] = "inode/vnd.kde.service.ldap";
        mimeTypes[ "lilypond" ] = "text/x-lilypond";
        mimeTypes[ "lisp" ] = "text/x-emacs-lisp";
        mimeTypes[ "log" ] = "text/x-log";
        mimeTypes[ "logtalk" ] = "text/x-logtalk";
        mimeTypes[ "lsm" ] = "";
        mimeTypes[ "lua" ] = "text/x-lua";
        mimeTypes[ "m4" ] = "application/x-m4";
        mimeTypes[ "makefile" ] = "text/x-makefile";
        mimeTypes[ "manifest" ] = "text/cache-manifest";
        mimeTypes[ "number" ] = "";
        mimeTypes[ "opa" ] = "text/x-opa";
        mimeTypes[ "oz" ] = "application/x-oz-application";
        mimeTypes[ "pascal" ] = "text/x-pascal";
        mimeTypes[ "pc" ] = "";
        mimeTypes[ "perl" ] = "application/x-perl";
        mimeTypes[ "php" ] = "application/x-php";
        mimeTypes[ "po" ] = "text/x-gettext-translation";
        mimeTypes[ "postscript" ] = "application/postscript";
        mimeTypes[ "prolog" ] = "text/x-prolog";
        mimeTypes[ "properties" ] = "text/x-ini";
        mimeTypes[ "proto" ] = "application/x-protobuf"; // not official
        mimeTypes[ "python" ] = "text/x-python";
        mimeTypes[ "ruby" ] = "application/x-ruby";
        mimeTypes[ "scala" ] = "text/x-scala";
        mimeTypes[ "scheme" ] = "text/x-scheme";
        mimeTypes[ "script_comment" ] = "";
        mimeTypes[ "sh" ] = "application/x-shellscript";
        mimeTypes[ "slang" ] = "";
        mimeTypes[ "sml" ] = "application/x-standardml";
        mimeTypes[ "spec" ] = "text/x-rpm-spec";
        mimeTypes[ "sql" ] = "text/x-sql";
        mimeTypes[ "symbols" ] = "";
        mimeTypes[ "tcl" ] = "text/x-tcl";
        mimeTypes[ "texinfo" ] = "text/x-texinfo";
        mimeTypes[ "tml*" ] = ""; // tml_formatting_all, tml_formatting, tml_glue, tml_macrolinks, tml_macros, tml_macros1, tml_macros2, tml_macrosdelayed1, tml_macrosdelayed2, tml_macrosdelayed3, tml_macrotokens, tml
        mimeTypes[ "todo" ] = "";
        mimeTypes[ "upc" ] = "";
        mimeTypes[ "url" ] = "";
        mimeTypes[ "vala" ] = "text/x-vala";
        mimeTypes[ "vbscript" ] = "text/x-vba"; // not sure
        mimeTypes[ "xml" ] = "application/xml";
        mimeTypes[ "xorg" ] = "";
    }
    
    //
    QString type = mLanguageMimeTypeCache.value( name, "-" );
    
    if ( type == "-" ) {
        type = mimeTypes.value( name );
        
        if ( type.isEmpty() ) {
            foreach ( const QString& key, mimeTypes.keys() ) {
                if ( QDir::match( key, name ) ) {
                    type = mimeTypes.value( key );
                    break;
                }
            }
        }
        
        mLanguageMimeTypeCache[ name ] = type;
    }
    
    QString icon = QString( type ).replace( "/", "-" );
    
    if ( !QIcon::hasThemeIcon( icon ) ) {
        icon = "text-plain";
    }
    
    return QIcon::fromTheme( icon );
}

QString SourceHighlightQtCodeEditor::languageForMimeType( const QString& type ) const
{
    static QHash<QString, QString> mimeTypes;
    
    if ( mimeTypes.isEmpty() ) {
        // specials
        /*mimeTypes[ "" ] = "default";
        mimeTypes[ "" ] = "langdef";
        mimeTypes[ "" ] = "nohilite";
        mimeTypes[ "" ] = "outlang";
        mimeTypes[ "" ] = "style";*/
        //
        mimeTypes[ "text/x-adasrc" ] = "ada";
        //mimeTypes[ "" ] = "applescript";
        mimeTypes[ "text/x-nasm" ] = "asm";
        mimeTypes[ "application/x-awk" ] = "awk";
        mimeTypes[ "application/x-shellscript" ] = "bat";
        mimeTypes[ "text/x-bibtex" ] = "bib";
        mimeTypes[ "" ] = "bison";
        mimeTypes[ "text/x-chdr" ] = "c"; // c_comment, c_string
        mimeTypes[ "text/x-csrc" ] = "c"; // c_comment, c_string, clike_vardeclaration
        mimeTypes[ "text/x-objchdr" ] = "c";
        mimeTypes[ "text/x-objcsrc" ] = "c";
        mimeTypes[ "text/x-ocaml" ] = "caml";
        mimeTypes[ "text/x-changelog" ] = "changelog";
        //mimeTypes[ "" ] = "clipper";
        mimeTypes[ "text/x-cobol" ] = "cobol";
        mimeTypes[ "text/x-ini" ] = "conf";
        mimeTypes[ "text/x-c++hdr" ] = "cpp";
        mimeTypes[ "text/x-c++src" ] = "cpp";
        mimeTypes[ "text/x-csharp" ] = "csharp";
        mimeTypes[ "text/css" ] = "css";
        mimeTypes[ "text/x-dsrc" ] = "d";
        mimeTypes[ "application/x-desktop" ] = "desktop";
        mimeTypes[ "text/x-patch" ] = "diff";
        mimeTypes[ "text/x-erlang" ] = "erlang";
        //mimeTypes[ "" ] = "errors";
        //mimeTypes[ "" ] = "extreme_*"; // extreme_comment, extreme_comment2, extreme_comment3, extreme_comment4
        mimeTypes[ "text/x-fortran" ] = "*fortran"; // fixed-fortran, fortran
        //mimeTypes[ "" ] = "flex";
        //mimeTypes[ "" ] = "function";
        mimeTypes[ "application/x-glsl" ] = "glsl";
        mimeTypes[ "text/x-haskell" ] = "haskell";
        mimeTypes[ "text/x-literate-haskell" ] = "haskell_literate";
        mimeTypes[ "text/x-haxe" ] = "haxe";
        mimeTypes[ "text/html" ] = "html"; // html_simple, html
        mimeTypes[ "text/x-emacs-lisp" ] = "islisp";
        mimeTypes[ "text/x-java" ] = "java";
        mimeTypes[ "text/x-java" ] = "javalog";
        mimeTypes[ "application/javascript" ] = "javascript";
        //mimeTypes[ "" ] = "key_string";
        mimeTypes[ "text/x-tex" ] = "latex";
        mimeTypes[ "inode/vnd.kde.service.ldap" ] = "ldap";
        mimeTypes[ "text/x-lilypond" ] = "lilypond";
        mimeTypes[ "text/x-emacs-lisp" ] = "lisp";
        mimeTypes[ "text/x-log" ] = "log";
        mimeTypes[ "text/x-logtalk" ] = "logtalk";
        //mimeTypes[ "" ] = "lsm";
        mimeTypes[ "text/x-lua" ] = "lua";
        mimeTypes[ "application/x-m4" ] = "m4";
        mimeTypes[ "text/x-makefile" ] = "makefile";
        mimeTypes[ "text/cache-manifest" ] = "manifest";
        //mimeTypes[ "" ] = "number";
        mimeTypes[ "text/x-opa" ] = "opa";
        mimeTypes[ "application/x-oz-application" ] = "oz";
        mimeTypes[ "text/x-pascal" ] = "pascal";
        //mimeTypes[ "" ] = "pc";
        mimeTypes[ "application/x-perl" ] = "perl";
        mimeTypes[ "application/x-php" ] = "php";
        mimeTypes[ "text/x-gettext-translation" ] = "po";
        mimeTypes[ "application/postscript" ] = "postscript";
        mimeTypes[ "text/x-prolog" ] = "prolog";
        mimeTypes[ "text/x-ini" ] = "properties";
        mimeTypes[ "application/x-protobuf" ] = "proto"; // not official
        mimeTypes[ "text/x-python" ] = "python";
        mimeTypes[ "application/x-ruby" ] = "ruby";
        mimeTypes[ "text/x-scala" ] = "scala";
        mimeTypes[ "text/x-scheme" ] = "scheme";
        //mimeTypes[ "" ] = "script_comment";
        mimeTypes[ "application/x-shellscript" ] = "sh";
        //mimeTypes[ "" ] = "slang";
        mimeTypes[ "application/x-standardml" ] = "sml";
        mimeTypes[ "text/x-rpm-spec" ] = "spec";
        mimeTypes[ "text/x-sql" ] = "sql";
        //mimeTypes[ "" ] = "symbols";
        mimeTypes[ "text/x-tcl" ] = "tcl";
        mimeTypes[ "text/x-texinfo" ] = "texinfo";
        //mimeTypes[ "" ] = "tml*"; // tml_formatting_all, tml_formatting, tml_glue, tml_macrolinks, tml_macros, tml_macros1, tml_macros2, tml_macrosdelayed1, tml_macrosdelayed2, tml_macrosdelayed3, tml_macrotokens, tml
        //mimeTypes[ "" ] = "todo";
        //mimeTypes[ "" ] = "upc";
        //mimeTypes[ "" ] = "url";
        mimeTypes[ "text/x-vala" ] = "vala";
        mimeTypes[ "text/x-vba" ] = "vbscript"; // not sure
        mimeTypes[ "application/xml" ] = "xml";
        //mimeTypes[ "" ] = "xorg";
    }
    
    return mimeTypes.value( type );
}

QStringList SourceHighlightQtCodeEditor::supportedLanguages() const
{
    return mDataWatcher->files( SourceHighlightQtDataWatcher::Languages );
}

QStringList SourceHighlightQtCodeEditor::supportedStyles() const
{
    return mDataWatcher->files( SourceHighlightQtDataWatcher::Styles );
}

Document* SourceHighlightQtCodeEditor::createDocument( QWidget* parent ) const
{
    return new SourceHighlightQtDocument( this, parent );
}

void SourceHighlightQtCodeEditor::dataWatcher_filesChanged( SourceHighlightQtDataWatcher::Type type )
{
    switch ( type ) {
        case SourceHighlightQtDataWatcher::Languages:
            emit supportedLanguagesChanged( supportedLanguages() );
            break;
        case SourceHighlightQtDataWatcher::OutputLanguages:
            break;
        case SourceHighlightQtDataWatcher::Styles:
            emit supportedStylesChanged( supportedStyles() );
            break;
    }
}
