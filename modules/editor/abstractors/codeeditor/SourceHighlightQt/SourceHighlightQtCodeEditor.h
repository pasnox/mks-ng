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
    
    virtual QIcon iconForLanguage( const QString& language ) const;;
    virtual QString languageForMimeType( const QString& type ) const;
    virtual QStringList supportedLanguages() const;
    virtual QStringList supportedStyles() const;
    virtual Document* createDocument( QWidget* parent = 0 ) const;

protected:
    SourceHighlightQtDataWatcher* mDataWatcher;
    mutable QHash<QString, QString> mLanguageMimeTypeCache;

protected slots:
    void dataWatcher_filesChanged( SourceHighlightQtDataWatcher::Type type );
};

#endif // SOURCEHIGHLIGHTQTCODEEDITOR_H
