#include "StackedDocument.h"
#include "StackedDocumentModel.h"
#include "Document.h"

#include <QEvent>
#include <QDebug>

StackedDocument::StackedDocument( QWidget* parent )
    : QStackedWidget( parent ), mModel( new StackedDocumentModel( this ) )
{
    connect( this, &QStackedWidget::currentChanged, this, &StackedDocument::_q_currentChanged );
    connect( this, &QStackedWidget::widgetRemoved, this, &StackedDocument::_q_widgetRemoved );
}

StackedDocument::~StackedDocument()
{
}

QString StackedDocument::documentAbstractionKey() const
{
    return mDocumentAbstractionKey;
}

void StackedDocument::setDocumentAbstractionKey( const QString& key )
{
    mDocumentAbstractionKey = key;
}

StackedDocumentModel* StackedDocument::model() const
{
    return mModel;
}

Document* StackedDocument::createDocument() const
{
    return Document::createDocument( mDocumentAbstractionKey );
}

int	StackedDocument::addDocument( Document* document )
{
    return insertDocument( -1, document );
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
    
    const bool locked = blockSignals( true );
    inIndex = QStackedWidget::insertWidget( index, document );
    blockSignals( locked );
    _q_documentInserted( inIndex, document );
    
    handleDocument( document );
    
    if ( count() == 1 ) {
        _q_currentChanged( inIndex );
    }
    
    return inIndex;
}

void StackedDocument::removeDocument( Document* document )
{
    const int index = QStackedWidget::indexOf( document );
    
    if ( index != -1 ) {
        emit documentIndexAboutToBeRemoved( index );
        emit documentAboutToBeRemoved( document );
        
        unhandleDocument( document );
        QStackedWidget::removeWidget( document );
    }
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

void StackedDocument::handleDocument( Document* document )
{
    connect( document, &Document::propertyChanged, this, &StackedDocument::_q_documentPropertyChanged );
    connect( document, &Document::propertiesChanged, this, &StackedDocument::_q_documentPropertiesChanged );
}

void StackedDocument::unhandleDocument( Document* document )
{
    disconnect( document, &Document::propertyChanged, this, &StackedDocument::_q_documentPropertyChanged );
    disconnect( document, &Document::propertiesChanged, this, &StackedDocument::_q_documentPropertiesChanged );
}

void StackedDocument::_q_documentInserted( int index, Document* document )
{
    if ( index != -1 && document ) {
        emit documentIndexInserted( index );
        emit documentInserted( document );
    }
}

void StackedDocument::_q_documentPropertyChanged( int property )
{
    Document* document = qobject_cast<Document*>( sender() );
    emit documentIndexPropertyChanged( property, indexOf( document ) );
    emit documentPropertyChanged( property, document );
}

void StackedDocument::_q_documentPropertiesChanged()
{
    Document* document = qobject_cast<Document*>( sender() );
    emit documentIndexPropertiesChanged( indexOf( document ) );
    emit documentPropertiesChanged( document );
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