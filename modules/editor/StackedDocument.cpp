#include "StackedDocument.h"
#include "Document.h"

#include <QEvent>

StackedDocument::StackedDocument( QWidget* parent )
    : QStackedWidget( parent )
{
    connect( this, &QStackedWidget::currentChanged, this, &StackedDocument::_q_currentChanged );
    connect( this, &QStackedWidget::widgetRemoved, this, &StackedDocument::_q_widgetRemoved );
    
    addDocument( new Document( this ) );
    insertDocument( 0, new Document( this ) );
}

StackedDocument::~StackedDocument()
{
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
        emit documentAboutToBeRemoved( index );
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
        emit documentInserted( index );
        emit documentInserted( document );
    }
}

void StackedDocument::_q_currentChanged( int index )
{
    emit currentDocumentChanged( index );
    emit currentDocumentChanged( document( index ) );
}

void StackedDocument::_q_widgetRemoved( int index )
{
    emit documentRemoved( index );
    emit documentRemoved( document( index ) );
}
