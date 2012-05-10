#ifndef CODEEDITORABSTRACTOR_H
#define CODEEDITORABSTRACTOR_H

#include "BaseAbstractor.h"
#include "Document.h"
#include "Abstractors.h"

#include <QIcon>
#include <QMimeDatabase>

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

    QIcon iconForFileName( const QString& fileName ) const {
        const QList<QMimeType> types = Abstractors::mimeDatabase().mimeTypesForFileName( fileName );
        
        foreach ( const QMimeType& type, types ) {
            const QIcon icon = QIcon::fromTheme( type.iconName() );
            
            if ( !icon.isNull() ) {
                return icon;
            }
        }
        
        return QIcon();
    }

    QIcon iconForLanguage( const QString& language ) const {
        const QMimeType type = mimeTypeForLanguage( language );
        return QIcon::fromTheme( type.iconName() );
    }

    QIcon iconForContent( const QString& content ) const {
        const QMimeType type = Abstractors::mimeDatabase().mimeTypeForData( content.toUtf8() );
        return QIcon::fromTheme( type.iconName() );
    }
    
    virtual QStringList supportedLanguages() = 0;
    virtual QStringList supportedStyles() = 0;
    virtual Document* createDocument( QWidget* parent = 0 ) = 0;
    virtual QMimeType mimeTypeForLanguage( const QString& language ) const = 0;
    virtual QString languageForMimeType( const QMimeType& type ) const = 0;

signals:
    void supportedLanguagesChanged();
    void supportedStylesChanged();
};

#endif // CODEEDITORABSTRACTOR_H
