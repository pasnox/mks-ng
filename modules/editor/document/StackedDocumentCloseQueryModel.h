#ifndef STACKEDDOCUMENTCLOSEQUERYMODEL_H
#define STACKEDDOCUMENTCLOSEQUERYMODEL_H

#include <QAbstractListModel>
#include <QList>

class Document;

class StackedDocumentCloseQueryModel : public QAbstractListModel
{
    Q_OBJECT
    
public:
    StackedDocumentCloseQueryModel( const QList<Document*>& documents, QObject* parent = 0 );
    virtual ~StackedDocumentCloseQueryModel();
    
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
    virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );
    
    QList<Document*> checkedDocumentList() const;

protected:
    QList<Document*> mDocuments;
    QHash<Document*, bool> mDocumentsState;
};

#endif // STACKEDDOCUMENTCLOSEQUERYMODEL_H
