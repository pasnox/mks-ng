#include "StackedDocumentCloseQuery.h"
#include "ui_StackedDocumentCloseQuery.h"
#include "StackedDocumentCloseQueryModel.h"
#include "StackedDocument.h"

#include <QEvent>

StackedDocumentCloseQuery::StackedDocumentCloseQuery( const QList<Document*>& documents, QWidget* parent )
    : QDialog( parent ),
        ui( new Ui_StackedDocumentCloseQuery ),
        mModel( new StackedDocumentCloseQueryModel( documents, this ) ),
        mResult( QDialogButtonBox::NoButton )
{
    Q_ASSERT( !documents.isEmpty() );
    ui->setupUi( this );
    ui->tvDocuments->setModel( mModel );
}

StackedDocumentCloseQuery::~StackedDocumentCloseQuery()
{
    delete ui;
}

QList<Document*> StackedDocumentCloseQuery::checkedDocuments() const
{
    return mResult == QDialogButtonBox::SaveAll ? mModel->checkedDocumentList() : QList<Document*>();
}

void StackedDocumentCloseQuery::retranslateUi()
{
    ui->retranslateUi( this );
    // do your custom retranslate here
}

void StackedDocumentCloseQuery::changeEvent( QEvent* event )
{
    QDialog::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}

void StackedDocumentCloseQuery::on_dbbButtons_clicked( QAbstractButton* button )
{
    mResult = ui->dbbButtons->standardButton( button );
    
    switch ( mResult ) {
        case QDialogButtonBox::SaveAll:
            accept();
            break;
        case QDialogButtonBox::Cancel:
            reject();
            break;
        case QDialogButtonBox::Discard:
            accept();
            break;
        default:
            Q_ASSERT( 0 );
            break;
    }
}
