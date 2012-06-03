#ifndef CODEEDITORMIMETYPE_H
#define CODEEDITORMIMETYPE_H

#include <QIcon>

#include "Document.h"

class CodeEditorAbstractor;

class CodeEditorMimeType
{
public:
    CodeEditorMimeType( const CodeEditorAbstractor* codeEditorAbstractor );
    virtual ~CodeEditorMimeType();
    
    QString mimeTypeForFileName( const QString& fileName ) const;
    QString mimeTypeForContent( const QByteArray& content ) const;
    
    QString languageForMimeType( const QString& type ) const;
    QString languageForFileName( const QString& fileName ) const;
    
    QString defaultMimeTypeIconName( const QString& type ) const;
    QString mimeTypeIconName( const QString& type ) const;
    
    QIcon iconFromTheme( const QString& name, const QString& type ) const;
    QIcon iconForState( Document::StateHints state ) const;
    QIcon iconForLanguage( const QString& language ) const;
    QIcon iconForFileName( const QString& fileName ) const;
    QIcon iconForContent( const QByteArray& content ) const;
    
protected:
    const CodeEditorAbstractor* mCodeEditorAbstractor;
};

#endif // CODEEDITORMIMETYPE_H
