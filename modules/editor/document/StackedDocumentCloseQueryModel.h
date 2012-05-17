#ifndef STACKEDDOCUMENTCLOSEQUERYMODEL_H
#define STACKEDDOCUMENTCLOSEQUERYMODEL_H

#include <QAbstractListModel>
#include "Document.h"

class StackedDocumentCloseQueryModel : public QAbstractListModel
{
    Q_OBJECT
    
public:
    StackedDocumentCloseQueryModel( const DocumentList& documents, QObject* parent = 0 );
    virtual ~StackedDocumentCloseQueryModel();
    
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
    virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );
    
    DocumentList checkedDocumentList() const;

protected:
    DocumentList mDocuments;
    QHash<Document*, bool> mDocumentsState;
};

#endif // STACKEDDOCUMENTCLOSEQUERYMODEL_H
