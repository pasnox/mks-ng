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
        mimeTypes[ "applescript" ] = QString::null;
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
#if defined( HAS_QT_5 )
    /*const QStringList languages = supportedLanguages();
    
    foreach ( const QString& language, languages ) {
        const QString name = language.section( '.', 0, 0 );
        
        if ( type.suffixes().contains( name ) ) {
            return language;
        }
        
        if ( type.name().contains( "hdr" ) ) {
            const QString srcName = type.name().replace( "hdr", "src" );
            const QMimeType srcType = Abstractors::mimeDatabase().mimeTypeForName( srcName );
            
            if ( srcType.suffixes().contains( name ) ) {
                return language;
            }
        }
    }*/
#else
    Q_UNUSED( type );
#endif

    return QString::null;
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
