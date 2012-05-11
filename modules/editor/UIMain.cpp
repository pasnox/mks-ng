#include "UIMain.h"
#include "ui_UIMain.h"
#include "StackedDocumentModel.h"
#include "Document.h"

#include <QEvent>
#include <QDebug>

UIMain::UIMain( QWidget* parent )
    : QMainWindow( parent ), ui( new Ui_UIMain )
{
    ui->setupUi( this );
    ui->sdtbDocument->setStackedDocument( ui->sdDocuments );
    ui->sdtvDocuments->setStackedDocument( ui->sdDocuments );
    
    const QStringList files = QStringList()
        << "/home/pasnox/Developpement/C++/Qt5/mks-ng/modules/editor/UIMain.h"
        << "/home/pasnox/Developpement/C++/Qt5/mks-ng/modules/editor/UIMain.cpp"
        << "#include <Test.h>"
    ;
    
    foreach ( const QString& file, files ) {
        Document* document = ui->sdDocuments->createDocument();
        if ( !document->open( file ) ) {
            document->setProperty( Document::Text, file );
        }
        ui->sdDocuments->addDocument( document );
    }
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
