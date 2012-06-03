#ifndef CODEEDITORABSTRACTOR_H
#define CODEEDITORABSTRACTOR_H

#include "BaseAbstractor.h"
#include "Document.h"
#include "Abstractors.h"
#include "CodeEditorMimeType.h"

#include <QIcon>
#include <QDebug>

class QStringList;

class CodeEditorAbstractor : public BaseAbstractor
{
    Q_OBJECT

public:
    CodeEditorAbstractor( QObject* parent = 0 )
        : BaseAbstractor( parent ),
            mCodeEditorMimeType( CodeEditorMimeType( this ) )
    {
    }
    
    const CodeEditorMimeType& mimeTypeDB() const {
        return mCodeEditorMimeType;
    }
    
    virtual QIcon iconForLanguage( const QString& language ) const = 0;
    virtual QString languageForMimeType( const QString& type ) const = 0;
    virtual QStringList supportedLanguages() const = 0;
    virtual QStringList supportedStyles() const = 0;
    virtual Document* createDocument( QWidget* parent = 0 ) const = 0;

protected:
    CodeEditorMimeType mCodeEditorMimeType;

signals:
    void supportedLanguagesChanged( const QStringList& languages );
    void supportedStylesChanged( const QStringList& styles );
};

#endif // CODEEDITORABSTRACTOR_H
