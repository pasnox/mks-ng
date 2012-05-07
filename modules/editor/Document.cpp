#include "Document.h"

#include <QEvent>
#include <QMetaObject>

QHash<QString, const QMetaObject*> Document::mAbstractors;

Document::Document( QWidget* parent )
    : BaseWidget( parent )
{
#if USE_MDI_AREA == 1
    setWindowFlags( Qt::FramelessWindowHint );
#else
#endif
}

Document::~Document()
{
}

void Document::registerDocumentAbstraction( const QMetaObject* meta )
{
    Q_ASSERT( meta );
    mAbstractors[ QString::fromLocal8Bit( meta->className() ) ] = meta;
}

void Document::unregisterDocumentAbstraction( const QMetaObject* meta )
{
    Q_ASSERT( meta );
    mAbstractors.remove( QString::fromLocal8Bit( meta->className() ) );
}

QStringList Document::documentAbstractionKeys()
{
    return mAbstractors.keys();
}

Document* Document::createDocument( const QString& key, QWidget* parent )
{
    const QMetaObject* meta = mAbstractors.value( key );
    Q_ASSERT( meta );
    return qobject_cast<Document*>( meta->newInstance( Q_ARG( QWidget*, parent ) ) );
}

void Document::retranslateUi()
{
    // do your custom retranslate here
}

void Document::changeEvent( QEvent* event )
{
    QWidget::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}
