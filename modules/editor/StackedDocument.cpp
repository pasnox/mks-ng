#include "StackedDocument.h"
#include "StackedDocumentModel.h"
#include "Document.h"

#include <QEvent>

StackedDocument::StackedDocument( QWidget* parent )
    : QStackedWidget( parent ), mModel( new StackedDocumentModel( this ) )
{
    connect( this, &QStackedWidget::currentChanged, this, &StackedDocument::_q_currentChanged );
    connect( this, &QStackedWidget::widgetRemoved, this, &StackedDocument::_q_widgetRemoved );
    
    const QString key = Document::documentAbstractionKeys().first();
    addDocument( Document::createDocument( key ) );
    addDocument( Document::createDocument( key ) );
    addDocument( Document::createDocument( key ) );
    addDocument( Document::createDocument( key ) );
    addDocument( Document::createDocument( key ) );
}

StackedDocument::~StackedDocument()
{
}

StackedDocumentModel* StackedDocument::model() const
{
    return mModel;
}

int	StackedDocument::addDocument( Document* document )
{
    int inIndex = indexOf( document );
    
    if ( inIndex != -1 ) {
        return inIndex;
    }
    
    inIndex = QStackedWidget::addWidget( document );
    _q_documentInserted( inIndex, document );
    return inIndex;
}

Document* StackedDocument::currentDocument() const
{
    return qobject_cast<Document*>( QStackedWidget::currentWidget() );
}

int StackedDocument::indexOf( Document* document ) const
{
    return QStackedWidget::indexOf( document );
}

int StackedDocument::insertDocument( int index, Document* document )
{
    int inIndex = indexOf( document );
    
    if ( inIndex != -1 ) {
        return inIndex;
    }
    
    inIndex = QStackedWidget::insertWidget( index, document );
    _q_documentInserted( index, document );
    return inIndex;
}

void StackedDocument::removeDocument( Document* document )
{
    const int index = QStackedWidget::indexOf( document );
    
    if ( index != -1 ) {
        emit documentIndexAboutToBeRemoved( index );
        emit documentAboutToBeRemoved( document );
    }
    
    QStackedWidget::removeWidget( document );
}

Document* StackedDocument::document( int index ) const
{
    return qobject_cast<Document*>( QStackedWidget::widget( index ) );
}

void StackedDocument::retranslateUi()
{
    // do your custom retranslate here
}

void StackedDocument::setCurrentDocument( Document* document )
{
    QStackedWidget::setCurrentWidget( document );
}

void StackedDocument::changeEvent( QEvent* event )
{
    QStackedWidget::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}

void StackedDocument::_q_documentInserted( int index, Document* document )
{
    if ( index != -1 && document ) {
        emit documentIndexInserted( index );
        emit documentInserted( document );
    }
}

void StackedDocument::_q_currentChanged( int index )
{
    emit currentDocumentIndexChanged( index );
    emit currentDocumentChanged( document( index ) );
}

void StackedDocument::_q_widgetRemoved( int index )
{
    emit documentIndexRemoved( index );
    emit documentRemoved( document( index ) );
}
