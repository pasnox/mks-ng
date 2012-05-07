#include "UIMain.h"
#include "ui_UIMain.h"
#include "StackedDocumentModel.h"

#include <QEvent>

UIMain::UIMain( QWidget* parent )
    : QMainWindow( parent ), ui( new Ui_UIMain )
{
    ui->setupUi( this );
    ui->sdtvDocuments->setStackedDocument( ui->sdDocuments );
}

UIMain::~UIMain()
{
    delete ui;
}

void UIMain::retranslateUi()
{
    ui->retranslateUi( this );
    // do your custom retranslate here
}

void UIMain::changeEvent( QEvent* event )
{
    QMainWindow::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}
