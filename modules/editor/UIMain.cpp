#include "UIMain.h"
#include "ui_UIMain.h"

#include <QEvent>

UIMain::UIMain( QWidget* parent )
    : QMainWindow( parent ), ui( new Ui_UIMain )
{
    ui->setupUi( this );
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
