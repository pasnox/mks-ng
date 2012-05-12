#ifndef CODEEDITORABSTRACTOR_H
#define CODEEDITORABSTRACTOR_H

#include "BaseAbstractor.h"
#include "Document.h"
#include "Abstractors.h"

#include <QIcon>
#include <QMimeDatabase>
#include <QDebug>

class QStringList;

class CodeEditorAbstractor : public BaseAbstractor
{
    Q_OBJECT

public:
    CodeEditorAbstractor( QObject* parent = 0 )
        : BaseAbstractor( parent ) {
    }

    QIcon iconForState( Document::StateHints state ) const {
        if ( state & Document::ExternallyDeleted ) {
            return iconFromTheme( "archive-remove" );
        }
        else if ( state & Document::ExternallyModified ) {
            return iconFromTheme( "document-properties" );
        }
        else if ( state & Document::Modified ) {
            return iconFromTheme( "document-save" );
        }
        
        return QIcon();
    }

    QIcon iconForFileName( const QString& fileName ) const {
        const QList<QMimeType> types = Abstractors::mimeDatabase().mimeTypesForFileName( fileName );
        
        foreach ( const QMimeType& type, types ) {
            const QIcon icon = iconFromTheme( mimeTypeIconName( type ) );
            
            if ( !icon.isNull() ) {
                return icon;
            }
        }
        
        return iconFromTheme( defaultMimeTypeIconName() );
    }

    QIcon iconForLanguage( const QString& language ) const {
        const QMimeType type = mimeTypeForLanguage( language );
        return iconFromTheme( mimeTypeIconName( type ) );
    }

    QIcon iconForContent( const QString& content ) const {
        const QMimeType type = Abstractors::mimeDatabase().mimeTypeForData( content.toLocal8Bit() );
        return iconFromTheme( mimeTypeIconName( type ) );
    }
    
    virtual QString languageForFileName( const QString& fileName ) const {
        const QMimeType type = Abstractors::mimeDatabase().mimeTypesForFileName( fileName ).value( 0 );
        return languageForMimeType( type );
    }
    
    virtual QStringList supportedLanguages() const = 0;
    virtual QStringList supportedStyles() const = 0;
    virtual Document* createDocument( QWidget* parent = 0 ) const = 0;

protected:
    QString defaultMimeTypeIconName() const {
        return "text-x-generic";
    }
    
    QString mimeTypeIconName( const QMimeType& type ) const {
        QString name;
        
        if ( type.isValid() ) {
            name = type.iconName();
            
            if ( name.isEmpty() ) {
                name = type.genericIconName();
            }
        }
        
        if ( name.isEmpty() ) {
            name = defaultMimeTypeIconName();
        }
        
        return name;
    }
    
    QIcon iconFromTheme( const QString& name ) const {
        return QIcon::hasThemeIcon( name ) ? QIcon::fromTheme( name ) : QIcon::fromTheme( defaultMimeTypeIconName() );
    }
    
    virtual QMimeType mimeTypeForLanguage( const QString& language ) const = 0;
    virtual QString languageForMimeType( const QMimeType& type ) const = 0;

signals:
    void supportedLanguagesChanged( const QStringList& languages );
    void supportedStylesChanged( const QStringList& styles );
};

#endif // CODEEDITORABSTRACTOR_H
