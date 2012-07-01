#ifndef QODEEDITCODEEDITOR_H
#define QODEEDITCODEEDITOR_H

#include "CodeEditorAbstractor.h"
#include "QodeEditDataWatcher.h"

class QodeEditCodeEditor : public CodeEditorAbstractor
{
    Q_OBJECT
    
public:
    Q_INVOKABLE QodeEditCodeEditor( QObject* parent = 0 );
    virtual ~QodeEditCodeEditor();
    
    virtual QIcon iconForLanguage( const QString& language ) const;;
    virtual QString languageForMimeType( const QString& type ) const;
    virtual QStringList supportedLanguages() const;
    virtual QStringList supportedStyles() const;
    virtual Document* createDocument( QWidget* parent = 0 ) const;

protected:
    QodeEditDataWatcher* mDataWatcher;
    mutable QHash<QString, QString> mLanguageMimeTypeCache;

protected slots:
    void dataWatcher_filesChanged( QodeEditDataWatcher::Type type );
};

#endif // QODEEDITCODEEDITOR_H
