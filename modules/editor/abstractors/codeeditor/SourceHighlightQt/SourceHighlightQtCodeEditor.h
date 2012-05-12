#ifndef SOURCEHIGHLIGHTQTCODEEDITOR_H
#define SOURCEHIGHLIGHTQTCODEEDITOR_H

#include "CodeEditorAbstractor.h"
#include "SourceHighlightQtDataWatcher.h"

class SourceHighlightQtCodeEditor : public CodeEditorAbstractor
{
    Q_OBJECT
    
public:
    Q_INVOKABLE SourceHighlightQtCodeEditor( QObject* parent = 0 );
    virtual ~SourceHighlightQtCodeEditor();
    
    virtual QStringList supportedLanguages() const;
    virtual QStringList supportedStyles() const;
    virtual QString languageForFileName( const QString& fileName ) const;
    virtual Document* createDocument( QWidget* parent = 0 ) const;

protected:
    SourceHighlightQtDataWatcher* mDataWatcher;
    
    virtual QMimeType mimeTypeForLanguage( const QString& language ) const;
    virtual QString languageForMimeType( const QMimeType& type ) const;

protected slots:
    void dataWatcher_filesChanged( SourceHighlightQtDataWatcher::Type type );
};

#endif // SOURCEHIGHLIGHTQTCODEEDITOR_H
