#include "StackedDocumentCloseQueryModel.h"

StackedDocumentCloseQueryModel::StackedDocumentCloseQueryModel( const DocumentList& documents, QObject* parent )
    : QAbstractListModel( parent )
{
    Q_ASSERT( !documents.isEmpty() );
    
    beginInsertRows( QModelIndex(), 0, documents.count() -1 );
    mDocuments = documents;
    endInsertRows();
}

StackedDocumentCloseQueryModel::~StackedDocumentCloseQueryModel()
{
}

DocumentList StackedDocumentCloseQueryModel::checkedDocumentList() const
{
    DocumentList documents;
    
    foreach ( Document* document, mDocuments ) {
        const bool checked = mDocumentsState.value( document, true );
        
        if ( checked ) {
            documents << document;
        }
    }
    
    return documents;
}

int StackedDocumentCloseQueryModel::rowCount( const QModelIndex& parent ) const
{
    return parent == QModelIndex() ? mDocuments.count() : 0;
}

QVariant StackedDocumentCloseQueryModel::data( const QModelIndex& index, int role ) const
{
    if ( index == QModelIndex() || index.row() < 0 || index.row() >= rowCount( index.parent() ) || index.column() != 0 ) {
        return QVariant();
    }
    
    Document* document = mDocuments[ index.row() ];
    
    switch ( role ) {
        case Qt::DecorationRole:
            return document->property( Document::Decoration );
        case Qt::DisplayRole:
            return document->property( Document::Title ).toString();
        case Qt::ToolTipRole:
            return document->property( Document::FilePath );
        case Qt::CheckStateRole:
            return mDocumentsState.value( document, true ) ? Qt::Checked : Qt::Unchecked;
    }
    
    return QVariant();
}

Qt::ItemFlags StackedDocumentCloseQueryModel::flags( const QModelIndex& index ) const
{
    Qt::ItemFlags flags = QAbstractListModel::flags( index );
    
    if ( index.isValid() ) {
        flags |= Qt::ItemIsUserCheckable;
    }
    
    return flags;
}

bool StackedDocumentCloseQueryModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
    if ( index == QModelIndex() || index.row() < 0 || index.row() >= rowCount( index.parent() ) || index.column() != 0 ) {
        return QAbstractListModel::setData( index, value, role );
    }
    
    Document* document = mDocuments[ index.row() ];
    
    switch ( role ) {
        case Qt::CheckStateRole:
            mDocumentsState[ document] = value.toInt() == Qt::Checked;
            break;
        default:
            return QAbstractListModel::setData( index, value, role );
    }
    
    emit dataChanged( index, index );
    
    return true;
}
