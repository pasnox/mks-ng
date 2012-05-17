#ifndef STACKEDDOCUMENTMODEL_H
#define STACKEDDOCUMENTMODEL_H

#include <QAbstractListModel>

class StackedDocument;

class StackedDocumentModel : public QAbstractListModel
{
    Q_OBJECT
    
public:
    StackedDocumentModel( StackedDocument* stackedDocument = 0 );
    virtual ~StackedDocumentModel();
    
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
    virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );
    
    StackedDocument* stackedDocument() const;
    void setStackedDocument( StackedDocument* stackedDocument );

protected:
    StackedDocument* mStacker;

protected slots:
    void _q_documentIndexInserted( int index );
    void _q_documentIndexPropertiesChanged( int index );
    void _q_currentDocumentIndexChanged( int index );
    void _q_documentIndexRemoved( int index );
};

#endif // STACKEDDOCUMENTMODEL_H
