#include "CodeEditorMimeType.h"
#include "CodeEditorAbstractor.h"

#include <QFileInfo>

#if defined( HAS_QT_5 )
#include <QMimeDatabase>
static QMimeDatabase mMimeDB;
#else
#include <freedesktopmime.h>
static QFreeDesktopMime* mMimeDB = 0;
#endif

CodeEditorMimeType::CodeEditorMimeType( const CodeEditorAbstractor* codeEditorAbstractor )
    : mCodeEditorAbstractor( codeEditorAbstractor )
{
    Q_ASSERT( mCodeEditorAbstractor );
#if !defined( HAS_QT_5 )
    mMimeDB = new QFreeDesktopMime();
#endif
}

CodeEditorMimeType::~CodeEditorMimeType()
{
#if !defined( HAS_QT_5 )
    delete mMimeDB;
    mMimeDB = 0;
#endif
}

QString CodeEditorMimeType::mimeTypeForFileName( const QString& fileName ) const
{
    QStringList types;
    
#if defined( HAS_QT_5 )
    foreach ( const QMimeType& type, mMimeDB.mimeTypesForFileName( fileName ) ) {
        types << type.name();
    }
#else
    types << mMimeDB->fromFileName( fileName );
#endif

    #warning fix me, if many types, try to detect the best
    return types.value( 0 );
}

QString CodeEditorMimeType::mimeTypeForContent( const QByteArray& content ) const
{
#if defined( HAS_QT_5 )
    return mMimeDB.mimeTypeForData( content ).name();
#else
    Q_UNUSED( content );
    return QString::null;
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
    
#if defined( HAS_QT_5 )
    const QStringList types = QStringList( type ) << mMimeDB.mimeTypeForName( type ).parentMimeTypes();
    
    foreach ( const QString& type, types ) {
        const QString name = mimeIcons.value( type );
        
        if ( !name.isEmpty() ) {
            return name;
        }
    }
#else
#endif
    
    return mimeIcons.value( type, "text-x-generic" );
}

QString CodeEditorMimeType::mimeTypeIconName( const QString& type ) const
{
    QString name;
    
#if defined( HAS_QT_5 )
    const QStringList types = QStringList( type ) << mMimeDB.mimeTypeForName( type ).parentMimeTypes();
    
    foreach ( const QString& _type, types ) {
        const QMimeType mimeType = mMimeDB.mimeTypeForName( _type );
        
        if ( mimeType.isValid() ) {
            name = mimeType.iconName();
            
            if ( name.isEmpty() ) {
                name = mimeType.genericIconName();
            }
        }
        
        if ( !name.isEmpty() ) {
            break;
        }
    }
#else
    const QStringList types = QStringList( type ) << mMimeDB->subClassOf( type );
    
    foreach ( const QString& _type, types ) {
        name = QString( _type ).replace( "/", "-" );
        
        if ( !QIcon::hasThemeIcon( name ) ) {
            name = mMimeDB->genericIconName( _type );
            
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

QIcon CodeEditorMimeType::iconFromTheme( const QString& name, const QString& type ) const
{
    return QIcon::hasThemeIcon( name ) ? QIcon::fromTheme( name ) : QIcon::fromTheme( defaultMimeTypeIconName( type ) );
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
    return iconFromTheme( mimeTypeIconName( type ), type );
}
