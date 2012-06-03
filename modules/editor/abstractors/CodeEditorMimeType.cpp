#include "CodeEditorMimeType.h"
#include "CodeEditorAbstractor.h"

#include <QFileIconProvider>
#if defined( HAS_QT_5 )
#include <QMimeDatabase>
static QMimeDatabase mMimeDB;
#endif
static QFileIconProvider mIconDB;

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
    QStringList types;
    
#if defined( HAS_QT_5 )
    foreach ( const QMimeType& type, mMimeDB.mimeTypesForFileName( fileName ) ) {
        types << type.name();
    }
#else
    types << mIconDB.type( QFileInfo( fileName ) );
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
    
    foreach ( const QString& type, types ) {
        const QMimeType mimeType = mMimeDB.mimeTypeForName( type );
        
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
    name = QString( type ).replace( "/", "-" );
    
    if ( !QIcon::hasThemeIcon( name ) ) {
        name.clear();
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
#if defined( HAS_QT_5 )
    return QIcon::fromTheme( mimeTypeIconName( type ) );
#else
    QIcon icon = mIconDB.icon( QFileInfo( fileName ) );
    return icon.isNull() ? iconFromTheme( defaultMimeTypeIconName( type ), type ) : icon;
#endif
}

QIcon CodeEditorMimeType::iconForContent( const QByteArray& content ) const
{
    const QString type = mimeTypeForContent( content );
    return iconFromTheme( mimeTypeIconName( type ), type );
}
