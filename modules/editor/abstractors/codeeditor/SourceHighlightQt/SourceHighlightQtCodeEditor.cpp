#include "SourceHighlightQtCodeEditor.h"
#include "SourceHighlightQtDocument.h"

#include <QStringList>
#include <QMimeType>

SourceHighlightQtCodeEditor::SourceHighlightQtCodeEditor( QObject* parent )
    : CodeEditorAbstractor( parent ), mDataWatcher( new SourceHighlightQtDataWatcher( this ) )
{
    connect( mDataWatcher, &SourceHighlightQtDataWatcher::filesChanged, this, &SourceHighlightQtCodeEditor::dataWatcher_filesChanged );
}

SourceHighlightQtCodeEditor::~SourceHighlightQtCodeEditor()
{
}

QStringList SourceHighlightQtCodeEditor::supportedLanguages()
{
    return mDataWatcher->files( SourceHighlightQtDataWatcher::Languages );
}

QStringList SourceHighlightQtCodeEditor::supportedStyles()
{
    return mDataWatcher->files( SourceHighlightQtDataWatcher::Styles );
}

Document* SourceHighlightQtCodeEditor::createDocument( QWidget* parent )
{
    return new SourceHighlightQtDocument( this, parent );
}

QMimeType SourceHighlightQtCodeEditor::mimeTypeForLanguage( const QString& language ) const
{
    return QMimeType();
}

QString SourceHighlightQtCodeEditor::languageForMimeType( const QMimeType& type ) const
{
    return QString::null;
}

void SourceHighlightQtCodeEditor::dataWatcher_filesChanged( SourceHighlightQtDataWatcher::Type type )
{
    switch ( type ) {
        case SourceHighlightQtDataWatcher::Languages:
            emit supportedLanguagesChanged();
            break;
        case SourceHighlightQtDataWatcher::OutputLanguages:
            break;
        case SourceHighlightQtDataWatcher::Styles:
            emit supportedStylesChanged();
            break;
    }
}
