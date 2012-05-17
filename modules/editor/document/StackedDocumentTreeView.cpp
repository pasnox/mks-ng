#include "StackedDocumentTreeView.h"
#include "StackedDocument.h"
#include "StackedDocumentModel.h"

#include <QEvent>
#include <QDebug>

StackedDocumentTreeView::StackedDocumentTreeView( QWidget* parent )
    : QTreeView( parent ),
        mStacker( 0 )
{
    setEditTriggers( QAbstractItemView::NoEditTriggers );
    setAlternatingRowColors( false );
    setVerticalScrollMode( QAbstractItemView::ScrollPerPixel );
    setRootIsDecorated( false );
    setUniformRowHeights( true );
    setHeaderHidden( true );
}

StackedDocumentTreeView::~StackedDocumentTreeView()
{
}

StackedDocument* StackedDocumentTreeView::stackedDocument() const
{
    return mStacker;
}

void StackedDocumentTreeView::setStackedDocument( StackedDocument* stackedDocument )
{
    if ( mStacker == stackedDocument ) {
        return;
    }
    
    if ( mStacker ) {
        disconnect( mStacker, &StackedDocument::currentDocumentIndexChanged, this, &StackedDocumentTreeView::_q_currentDocumentIndexChanged );
        disconnect( selectionModel(), &QItemSelectionModel::selectionChanged, this, &StackedDocumentTreeView::_q_selectionChanged );
    }
    
    mStacker = stackedDocument;
    setModel( mStacker ? mStacker->model() : 0 );
    
    if ( mStacker ) {
        connect( mStacker, &StackedDocument::currentDocumentIndexChanged, this, &StackedDocumentTreeView::_q_currentDocumentIndexChanged );
        connect( selectionModel(), &QItemSelectionModel::selectionChanged, this, &StackedDocumentTreeView::_q_selectionChanged );
        
        _q_currentDocumentIndexChanged( mStacker->currentIndex() );
    }
}

void StackedDocumentTreeView::retranslateUi()
{
    // do your custom retranslate here
}

void StackedDocumentTreeView::changeEvent( QEvent* event )
{
    QTreeView::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}

void StackedDocumentTreeView::updateCurrentIndex( const QModelIndex& index )
{
    setCurrentIndex( index );
    mStacker->setCurrentIndex( index.row() );
}

void StackedDocumentTreeView::_q_selectionChanged()
{
    const QModelIndex index = selectionModel()->selectedIndexes().value( 0 );
    updateCurrentIndex( index );
}

void StackedDocumentTreeView::_q_currentDocumentIndexChanged( int index )
{
    const QModelIndex idx = index >= 0 ? model()->index( index, 0 ) : QModelIndex();
    updateCurrentIndex( idx );
}
