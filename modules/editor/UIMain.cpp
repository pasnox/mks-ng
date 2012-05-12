#include "UIMain.h"
#include "ui_UIMain.h"
#include "StackedDocumentModel.h"
#include "Document.h"
#include "Menu.h"

#include <FreshGui/pActionsMenuBar>
#include <FreshGui/pActionsModel>

#include <QEvent>
#include <QDebug>

UIMain::UIMain( QWidget* parent )
    : pMainWindow( parent ),
        ui( new Ui_UIMain )
{
    ui->setupUi( this );
    ui->sdtbDocument->setStackedDocument( ui->sdDocuments );
    ui->sdtvDocuments->setStackedDocument( ui->sdDocuments );
    
#ifndef Q_OS_MAC
    ui->sdtbDocument->insertWidget( ui->sdtbDocument->actions().first(), menuBar() );
#endif

    pActionsModel* model = menuBar()->model();
    model->addMenu( MENU_FILE, tr( "File" ) );
    QAction* aOpen = model->addAction( MENU_FILE_OPEN, tr( "Open..." ), QIcon::fromTheme( "project-open" ) );
    QAction* aOpenPlainText = model->addAction( MENU_FILE_OPEN_PLAIN_TEXT, tr( "Open plain text..." ), QIcon::fromTheme( "document-open" ) );
    model->addSeparator( MENU_FILE_SEPARATOR_1 );
    QAction* aReload = model->addAction( MENU_FILE_RELOAD, tr( "Reload..." ), QIcon::fromTheme( "view-refresh" ) );
    model->addSeparator( MENU_FILE_SEPARATOR_2 );
    QAction* aSave = model->addAction( MENU_FILE_SAVE, tr( "Save..." ), QIcon::fromTheme( "document-save" ) );
    QAction* aSaveAll = model->addAction( MENU_FILE_SAVE_ALL, tr( "Save all..." ), QIcon::fromTheme( "document-save-all" ) );
    QAction* aBackup = model->addAction( MENU_FILE_BACKUP, tr( "Backup..." ), QIcon::fromTheme( "document-save-as" ) );
    model->addSeparator( MENU_FILE_SEPARATOR_3 );
    QAction* aClose = model->addAction( MENU_FILE_CLOSE, tr( "Close..." ), QIcon::fromTheme( "document-close" ) );
    QAction* aCloseAll = model->addAction( MENU_FILE_CLOSE_ALL, tr( "Close all..." ), QIcon::fromTheme( "document-close-all" ) );
    model->addSeparator( MENU_FILE_SEPARATOR_4 );
    QAction* aQuit = model->addAction( MENU_FILE_QUIT, tr( "Quit..." ), QIcon::fromTheme( "window-close" ) );
    
    connect( aOpen, &QAction::triggered, this, &UIMain::actionOpenTriggered );
    connect( aOpenPlainText, &QAction::triggered, this, &UIMain::actionOpenPlainTextTriggered );
    connect( aReload, &QAction::triggered, this, &UIMain::actionReloadTriggered );
    connect( aSave, &QAction::triggered, this, &UIMain::actionSaveTriggered );
    connect( aSaveAll, &QAction::triggered, this, &UIMain::actionSaveAllTriggered );
    connect( aBackup, &QAction::triggered, this, &UIMain::actionBackupTriggered );
    connect( aClose, &QAction::triggered, this, &UIMain::actionCloseTriggered );
    connect( aCloseAll, &QAction::triggered, this, &UIMain::actionCloseAllTriggered );
    connect( aQuit, &QAction::triggered, this, &UIMain::actionQuitTriggered );
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

void UIMain::open( const QStringList& filePaths, const QString& encoding )
{
    Q_UNUSED( filePaths );
    Q_UNUSED( encoding );
    // Reserved for futur use
}

void UIMain::openPlainText( const QStringList& filePaths, const QString& encoding )
{
    /*foreach ( const QString& filePath, filePaths ) {
        Document* document = ui->sdDocuments->createDocument();
        
        if ( !document->open( file ) ) {
            document->setProperty( Document::Text, file );
        }
        
        ui->sdDocuments->addDocument( document );
    }*/
}

void UIMain::changeEvent( QEvent* event )
{
    QMainWindow::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}

void UIMain::actionOpenTriggered()
{
}

void UIMain::actionOpenPlainTextTriggered()
{
}

void UIMain::actionReloadTriggered()
{
}

void UIMain::actionSaveTriggered()
{
}

void UIMain::actionSaveAllTriggered()
{
}

void UIMain::actionBackupTriggered()
{
}

void UIMain::actionCloseTriggered()
{
}

void UIMain::actionCloseAllTriggered()
{
}

void UIMain::actionQuitTriggered()
{
}
