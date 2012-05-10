#include "UIMain.h"
#include "ui_UIMain.h"
#include "StackedDocumentModel.h"
#include "Document.h"
#include "CodeEditorAbstractor.h"

#include <QEvent>
#include <QDebug>

#include "SourceHighlightQtDocument.h"
#include "SourceHighlightQtDataWatcher.h"

UIMain::UIMain( QWidget* parent )
    : QMainWindow( parent ), ui( new Ui_UIMain )
{
    ui->setupUi( this );
    ui->sdtvDocuments->setStackedDocument( ui->sdDocuments );
    
    CodeEditorAbstractor* cea = ui->sdDocuments->codeEditorAbstractor();
    
    ui->cbLanguages->addItems( cea->supportedLanguages() );
    ui->cbStyles->addItems( cea->supportedStyles() );
    
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

void UIMain::on_cbLanguages_activated( const QString& language )
{
    Document* document = ui->sdDocuments->currentDocument();
    
    if ( document ) {
        document->setProperty( Document::Language, language );
    }
}

void UIMain::on_cbStyles_activated( const QString& style )
{
    Document* document = ui->sdDocuments->currentDocument();
    
    if ( document ) {
        document->setProperty( Document::Style, style );
    }
}
