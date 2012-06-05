#include "CodeEditorMimeType.h"
#include "CodeEditorAbstractor.h"

#include <QFileInfo>

#if defined( HAS_QT_5 )
#include <QMimeDatabase>
static QMimeDatabase mMimeDB;
#else
#include <freedesktopmime.h>
static QFreeDesktopMime mMimeDB;
#endif

CodeEditorMimeType::CodeEditorMimeType( const CodeEditorAbstractor* codeEditorAbstractor )
    : mCodeEditorAbstractor( codeEditorAbstractor )
{
    Q_ASSERT( mCodeEditorAbstractor );
}

CodeEditorMimeType::~CodeEditorMimeType()
{
}

QString CodeEditorMimeType::mimeTypeForFileName( const QString& fileName ) const
{
#if defined( HAS_QT_5 )
    return mMimeDB.mimeTypeForFile( fileName ).name();
#else
    return mMimeDB.fromFileName( fileName );
#endif
}

QString CodeEditorMimeType::mimeTypeForContent( const QByteArray& content ) const
{
#if defined( HAS_QT_5 )
    return mMimeDB.mimeTypeForData( content ).name();
#else
    return mMimeDB.fromData( content );
#endif
}

QString CodeEditorMimeType::languageForMimeType( const QString& type ) const
{
    return mCodeEditorAbstractor->languageForMimeType( type );
}

QString CodeEditorMimeType::languageForFileName( const QString& fileName ) const
{
    return languageForMimeType( mimeTypeForFileName( fileName ) );
}

QString CodeEditorMimeType::defaultMimeTypeIconName( const QString& type ) const
{
    static QHash<QString, QString> mimeIcons;
    
    if ( mimeIcons.isEmpty() ) {
        mimeIcons[ "text/plain" ] = "text-x-generic";
    }
    
    return mimeIcons.value( type, "text-x-generic" );
}

QString CodeEditorMimeType::mimeTypeIconName( const QString& type ) const
{
    QString name;
    
#if defined( HAS_QT_5 )
    const QStringList types = QStringList( type ) << mMimeDB.mimeTypeForName( type ).parentMimeTypes();
    
    foreach ( const QString& _type, types ) {
        const QMimeType mimeType = mMimeDB.mimeTypeForName( _type );
        name = mimeType.iconName();
        
        if ( !QIcon::hasThemeIcon( name ) ) {
            name = mimeType.genericIconName();
            
            if ( !QIcon::hasThemeIcon( name ) ) {
                name.clear();
            }
        }
        
        if ( !name.isEmpty() ) {
            break;
        }
    }
#else
    const QStringList types = QStringList( type ) << mMimeDB.subClassOf( type );
    
    foreach ( const QString& _type, types ) {
        name = mMimeDB.iconName( _type );
        
        if ( !QIcon::hasThemeIcon( name ) ) {
            name = mMimeDB.genericIconName( _type );
            
            if ( !QIcon::hasThemeIcon( name ) ) {
                name.clear();
            }
        }
        
        if ( !name.isEmpty() ) {
            break;
        }
    }
#endif

    if ( name.isEmpty() ) {
        name = defaultMimeTypeIconName( type );
    }
    
    return name;
}

QIcon CodeEditorMimeType::iconForState( Document::StateHints state ) const
{
    if ( state & Document::ExternallyDeleted ) {
        return QIcon::fromTheme( "archive-remove" );
    }
    else if ( state & Document::ExternallyModified ) {
        return QIcon::fromTheme( "document-properties" );
    }
    else if ( state & Document::Modified ) {
        return QIcon::fromTheme( "document-save" );
    }
    
    return QIcon();
}

QIcon CodeEditorMimeType::iconForLanguage( const QString& language ) const
{
    return mCodeEditorAbstractor->iconForLanguage( language );
}

QIcon CodeEditorMimeType::iconForFileName( const QString& fileName ) const
{
    const QString type = mimeTypeForFileName( fileName );
    return QIcon::fromTheme( mimeTypeIconName( type ) );
}

QIcon CodeEditorMimeType::iconForContent( const QByteArray& content ) const
{
    const QString type = mimeTypeForContent( content );
    const QString name = mimeTypeIconName( type );
    return QIcon::hasThemeIcon( name ) ? QIcon::fromTheme( name ) : QIcon::fromTheme( defaultMimeTypeIconName( type ) );
}
