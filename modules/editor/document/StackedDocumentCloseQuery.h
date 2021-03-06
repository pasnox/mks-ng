#ifndef STACKEDDOCUMENTCLOSEQUERY_H
#define STACKEDDOCUMENTCLOSEQUERY_H

#include <QDialog>
#include <QDialogButtonBox>

#include "Document.h"

class Ui_StackedDocumentCloseQuery;
class QEvent;
class QAbstractButton;

class StackedDocumentCloseQueryModel;

class StackedDocumentCloseQuery : public QDialog
{
    Q_OBJECT

public:
    StackedDocumentCloseQuery( const DocumentList& documents, QWidget* parent );
    virtual ~StackedDocumentCloseQuery();
    
    DocumentList checkedDocuments() const;

public slots:
    virtual void retranslateUi();
    
protected:
    Ui_StackedDocumentCloseQuery* ui;
    StackedDocumentCloseQueryModel* mModel;
    QDialogButtonBox::StandardButton mResult;
    
    virtual void changeEvent( QEvent* event );

protected slots:
    void on_dbbButtons_clicked( QAbstractButton* button );
};

#endif // STACKEDDOCUMENTCLOSEQUERY_H
