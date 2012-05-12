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
    
    virtual QStringList supportedLanguages();
    virtual QStringList supportedStyles();
    virtual Document* createDocument( QWidget* parent = 0 );

protected:
    SourceHighlightQtDataWatcher* mDataWatcher;
    
    virtual QMimeType mimeTypeForLanguage( const QString& language ) const;
    virtual QString languageForMimeType( const QMimeType& type ) const;

protected slots:
    void dataWatcher_filesChanged( SourceHighlightQtDataWatcher::Type type );
};

#endif // SOURCEHIGHLIGHTQTCODEEDITOR_H
