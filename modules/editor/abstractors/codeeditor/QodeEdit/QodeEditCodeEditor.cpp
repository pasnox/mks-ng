#include "QodeEditCodeEditor.h"
#include "QodeEditDocument.h"

#include <QDir>
#include <QStringList>
#if defined( HAS_QT_5 )
#include <QMimeType>
#endif
#include <QDebug>

QodeEditCodeEditor::QodeEditCodeEditor( QObject* parent )
    : CodeEditorAbstractor( parent ), mDataWatcher( new QodeEditDataWatcher( this ) )
{
#if defined( HAS_QT_5 )
    connect( mDataWatcher, &QodeEditDataWatcher::filesChanged, this, &QodeEditCodeEditor::dataWatcher_filesChanged );
#else
    connect( mDataWatcher, SIGNAL( filesChanged( QodeEditDataWatcher::Type ) ), this, SLOT( dataWatcher_filesChanged( QodeEditDataWatcher::Type ) ) );
#endif
}

QodeEditCodeEditor::~QodeEditCodeEditor()
{
}

QIcon QodeEditCodeEditor::iconForLanguage( const QString& language ) const
{
    return QIcon::fromTheme( "text-plain" );
}

QString QodeEditCodeEditor::languageForMimeType( const QString& type ) const
{
    return QString::null;
}

QStringList QodeEditCodeEditor::supportedLanguages() const
{
    return mDataWatcher->files( QodeEditDataWatcher::Languages );
}

QStringList QodeEditCodeEditor::supportedStyles() const
{
    return mDataWatcher->files( QodeEditDataWatcher::Styles );
}

Document* QodeEditCodeEditor::createDocument( QWidget* parent ) const
{
    return new QodeEditDocument( parent );
}

void QodeEditCodeEditor::dataWatcher_filesChanged( QodeEditDataWatcher::Type type )
{
    switch ( type ) {
        case QodeEditDataWatcher::Languages:
            emit supportedLanguagesChanged( supportedLanguages() );
            break;
        case QodeEditDataWatcher::Styles:
            emit supportedStylesChanged( supportedStyles() );
            break;
    }
}
