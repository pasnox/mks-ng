#include "StackedDocumentModel.h"
#include "StackedDocument.h"
#include "Document.h"

#include <QDebug>

StackedDocumentModel::StackedDocumentModel( StackedDocument* stackedDocument )
    : QAbstractListModel( stackedDocument ), mStacker( 0 )
{
    setStackedDocument( stackedDocument );
}

StackedDocumentModel::~StackedDocumentModel()
{
    setStackedDocument( 0 );
}

StackedDocument* StackedDocumentModel::stackedDocument() const
{
    return mStacker;
}

void StackedDocumentModel::setStackedDocument( StackedDocument* stackedDocument )
{
    if ( mStacker == stackedDocument ) {
        return;
    }
    
    if ( mStacker ) {
        disconnect( mStacker.data(), &StackedDocument::documentIndexInserted, this, &StackedDocumentModel::_q_documentIndexInserted );
        disconnect( mStacker.data(), &StackedDocument::documentIndexPropertiesChanged, this, &StackedDocumentModel::_q_documentIndexPropertiesChanged );
        disconnect( mStacker.data(), &StackedDocument::currentDocumentIndexChanged, this, &StackedDocumentModel::_q_currentDocumentIndexChanged );
        disconnect( mStacker.data(), &StackedDocument::documentIndexRemoved, this, &StackedDocumentModel::_q_documentIndexRemoved );
    }
    
    mStacker = stackedDocument;
    
    if ( mStacker ) {
        connect( mStacker.data(), &StackedDocument::documentIndexInserted, this, &StackedDocumentModel::_q_documentIndexInserted );
        connect( mStacker.data(), &StackedDocument::documentIndexPropertiesChanged, this, &StackedDocumentModel::_q_documentIndexPropertiesChanged );
        connect( mStacker.data(), &StackedDocument::currentDocumentIndexChanged, this, &StackedDocumentModel::_q_currentDocumentIndexChanged );
        connect( mStacker.data(), &StackedDocument::documentIndexRemoved, this, &StackedDocumentModel::_q_documentIndexRemoved );
    }
    
    reset();
}

int StackedDocumentModel::rowCount( const QModelIndex& parent ) const
{
    return parent == QModelIndex() ? ( mStacker ? mStacker->count() : 0 ) : 0;
}

QVariant StackedDocumentModel::data( const QModelIndex& index, int role ) const
{
    if ( index == QModelIndex() || index.row() < 0 || index.row() >= rowCount( index.parent() ) || index.column() != 0 ) {
        return QVariant();
    }
    
    Document* document = mStacker->document( index.row() );
    
    switch ( role ) {
        case Qt::DecorationRole:
            return document->property( Document::Decoration );
        case Qt::DisplayRole:
            return document->property( Document::Title ).toString().replace( "[*]", QString::null );
        case Qt::ToolTipRole:
            return document->property( Document::FilePath );
        case Qt::CheckStateRole:
            return document->property( Document::ReadOnly ).toBool() ? Qt::Checked : Qt::Unchecked;
    }
    
    return QVariant();
}

Qt::ItemFlags StackedDocumentModel::flags( const QModelIndex& index ) const
{
    Qt::ItemFlags flags = QAbstractListModel::flags( index );
    
    if ( index.isValid() ) {
        flags |= Qt::ItemIsUserCheckable;
    }
    
    return flags;
}

bool StackedDocumentModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
    if ( index == QModelIndex() || index.row() < 0 || index.row() >= rowCount( index.parent() ) || index.column() != 0 ) {
        return QAbstractListModel::setData( index, value, role );
    }
    
    Document* document = mStacker->document( index.row() );
    
    switch ( role ) {
        case Qt::CheckStateRole:
            document->setProperty( Document::ReadOnly, value.toInt() == Qt::Checked );
            break;
        default:
            return QAbstractListModel::setData( index, value, role );
    }
    
    emit dataChanged( index, index );
    
    return true;
}

void StackedDocumentModel::_q_documentIndexInserted( int index )
{
    beginInsertRows( QModelIndex(), index, index );
    endInsertRows();
}

void StackedDocumentModel::_q_documentIndexPropertiesChanged( int index )
{
    const QModelIndex idx = QAbstractListModel::index( index, 0, QModelIndex() );
    
    if ( idx.isValid() ) {
        emit dataChanged( idx, idx );
    }
}

void StackedDocumentModel::_q_currentDocumentIndexChanged( int index )
{
    _q_documentIndexPropertiesChanged( index );
}

void StackedDocumentModel::_q_documentIndexRemoved( int index )
{
    beginRemoveRows( QModelIndex(), index, index );
    endRemoveRows();
}
