#ifndef STACKEDDOCUMENTCLOSEQUERY_H
#define STACKEDDOCUMENTCLOSEQUERY_H

#include <QDialog>

class Ui_StackedDocumentCloseQuery;
class QEvent;
class QAbstractButton;

class StackedDocumentCloseQueryModel;
class Document;
class UIMain;

class StackedDocumentCloseQuery : public QDialog
{
    Q_OBJECT

public:
    StackedDocumentCloseQuery( const QList<Document*>& documents, UIMain* window );
    virtual ~StackedDocumentCloseQuery();

public slots:
    virtual void retranslateUi();
    
protected:
    Ui_StackedDocumentCloseQuery* ui;
    StackedDocumentCloseQueryModel* mModel;
    UIMain* mWindow;
    
    virtual void changeEvent( QEvent* event );

protected slots:
    void on_dbbButtons_clicked( QAbstractButton* button );
};

#endif // STACKEDDOCUMENTCLOSEQUERY_H
