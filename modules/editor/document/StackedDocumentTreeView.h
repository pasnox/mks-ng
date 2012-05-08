#ifndef STACKEDDOCUMENTTREEVIEW_H
#define STACKEDDOCUMENTTREEVIEW_H

#include <QTreeView>
#include <QPointer>

class QEvent;

class StackedDocument;

class StackedDocumentTreeView : public QTreeView
{
    Q_OBJECT
    
public:
    StackedDocumentTreeView( QWidget* parent = 0 );
    virtual ~StackedDocumentTreeView();
    
    StackedDocument* stackedDocument() const;
    void setStackedDocument( StackedDocument* stackedDocument );

public slots:
    virtual void retranslateUi();
    
protected:
    QPointer<StackedDocument> mStacker;
    
    virtual void changeEvent( QEvent* event );
    void updateCurrentIndex( const QModelIndex& index );

protected slots:
    void _q_selectionChanged();
    void _q_currentDocumentIndexChanged( int index );
};

#endif // STACKEDDOCUMENTTREEVIEW_H
