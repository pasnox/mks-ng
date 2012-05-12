#include "SourceHighlightQtCodeEditor.h"
#include "SourceHighlightQtDocument.h"

#include <QStringList>
#include <QMimeType>
#include <QDebug>

SourceHighlightQtCodeEditor::SourceHighlightQtCodeEditor( QObject* parent )
    : CodeEditorAbstractor( parent ), mDataWatcher( new SourceHighlightQtDataWatcher( this ) )
{
    connect( mDataWatcher, &SourceHighlightQtDataWatcher::filesChanged, this, &SourceHighlightQtCodeEditor::dataWatcher_filesChanged );
}

SourceHighlightQtCodeEditor::~SourceHighlightQtCodeEditor()
{
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

QMimeType SourceHighlightQtCodeEditor::mimeTypeForLanguage( const QString& language ) const
{
    const QString name = language.section( '.', 0, 0 );
    static const QStringList masks = QStringList()
        << "text/x-%1"
        << "text/x-%1hdr"
        << "text/x-%1src"
        << "application/x-%1"
        << "text/%1"
        << "text/%1hdr"
        << "text/%1src"
        << "application/%1"
    ;
    const QMimeDatabase& mdb = Abstractors::mimeDatabase();
    const QList<QMimeType> types = mdb.mimeTypesForFileName( QString( "fake.%1" ).arg( name ) );
    QMimeType bestType;
    
    foreach ( const QMimeType& type, types ) {
        if ( QIcon::hasThemeIcon( type.iconName() ) ) {
            return type;
        }
        
        if ( QIcon::hasThemeIcon( type.genericIconName() ) ) {
            bestType = type;
        }
    }
    
    foreach ( QString mask, masks ) {
        const QMimeType type = mdb.mimeTypeForName( mask.arg( name ) );
        
        if ( QIcon::hasThemeIcon( type.iconName() ) ) {
            return type;
        }
        
        if ( QIcon::hasThemeIcon( type.genericIconName() ) ) {
            bestType = type;
        }
    }
    
    return bestType;
}

QString SourceHighlightQtCodeEditor::languageForMimeType( const QMimeType& type ) const
{
    const QStringList languages = supportedLanguages();
    
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
    }
    
    return "default.lang";
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
