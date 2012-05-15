#include "StackedDocumentCloseQuery.h"
#include "ui_StackedDocumentCloseQuery.h"
#include "StackedDocumentCloseQueryModel.h"
#include "StackedDocument.h"
#include "UIMain.h"

#include <QEvent>

StackedDocumentCloseQuery::StackedDocumentCloseQuery( const QList<Document*>& documents, UIMain* window )
    : QDialog( window ),
        ui( new Ui_StackedDocumentCloseQuery ),
        mModel( new StackedDocumentCloseQueryModel( documents, this ) ),
        mWindow( window )
{
    Q_ASSERT( !documents.isEmpty() );
    Q_ASSERT( window );
    ui->setupUi( this );
    ui->tvDocuments->setModel( mModel );
}

StackedDocumentCloseQuery::~StackedDocumentCloseQuery()
{
    delete ui;
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
    const QDialogButtonBox::StandardButton type = ui->dbbButtons->standardButton( button );
    
    switch ( type ) {
        case QDialogButtonBox::SaveAll: {
            const QList<Document*> checkedDocuments = mModel->checkedDocumentList();
            
            foreach ( Document* document, checkedDocuments ) {
                mWindow->saveDocument( document );
            }
            
            accept();
            break;
        }
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
