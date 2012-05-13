#include "UIMain.h"
#include "ui_UIMain.h"
#include "StackedDocumentModel.h"
#include "Document.h"
#include "Menu.h"

#include <FreshGui/pQueuedMessageToolBar>
#include <FreshGui/pActionsMenuBar>
#include <FreshGui/pActionsModel>
#include <FreshGui/pFileDialog>

#include <QEvent>
#include <QDir>
#include <QMessageBox>
#include <QDebug>

UIMain::UIMain( QWidget* parent )
    : pMainWindow( parent ),
        ui( new Ui_UIMain )
{
    ui->setupUi( this );
    ui->sdtbDocument->setStackedDocument( ui->sdDocuments );
    ui->sdtvDocuments->setStackedDocument( ui->sdDocuments );
    
    removeToolBar( ui->sdtbDocument );
    insertToolBar( queuedMessageToolBar(), ui->sdtbDocument );
    insertToolBarBreak( queuedMessageToolBar() );
    ui->sdtbDocument->setVisible( true );
    
#ifndef Q_OS_MAC
    ui->sdtbDocument->insertWidget( ui->sdtbDocument->actions().first(), menuBar() );
#endif

    pActionsModel* model = menuBar()->model();
    model->addMenu( MENU_FILE, tr( "File" ) );
    QAction* aNewPlainText = model->addAction( MENU_FILE_NEW_PLAIN_TEXT, tr( "New plain text file" ), QIcon::fromTheme( "document-new" ) );
    QAction* aOpen = model->addAction( MENU_FILE_OPEN, tr( "Open..." ), QIcon::fromTheme( "project-open" ) );
    QAction* aOpenPlainText = model->addAction( MENU_FILE_OPEN_PLAIN_TEXT, tr( "Open plain text..." ), QIcon::fromTheme( "document-open" ) );
    model->addSeparator( MENU_FILE_SEPARATOR_1 );
    QAction* aReload = model->addAction( MENU_FILE_RELOAD, tr( "Reload..." ), QIcon::fromTheme( "view-refresh" ) );
    model->addSeparator( MENU_FILE_SEPARATOR_2 );
    QAction* aSave = model->addAction( MENU_FILE_SAVE, tr( "Save..." ), QIcon::fromTheme( "document-save" ) );
    QAction* aSaveAs = model->addAction( MENU_FILE_SAVE_AS, tr( "Save as..." ), QIcon::fromTheme( "document-save-as" ) );
    QAction* aSaveAll = model->addAction( MENU_FILE_SAVE_ALL, tr( "Save all..." ), QIcon::fromTheme( "document-save-all" ) );
    model->addSeparator( MENU_FILE_SEPARATOR_3 );
    QAction* aClose = model->addAction( MENU_FILE_CLOSE, tr( "Close..." ), QIcon::fromTheme( "document-close" ) );
    QAction* aCloseAll = model->addAction( MENU_FILE_CLOSE_ALL, tr( "Close all..." ), QIcon::fromTheme( "document-close-all" ) );
    model->addSeparator( MENU_FILE_SEPARATOR_4 );
    QAction* aQuit = model->addAction( MENU_FILE_QUIT, tr( "Quit..." ), QIcon::fromTheme( "window-close" ) );
    
    model->setDefaultShortcut( aNewPlainText, QKeySequence::New );
    //model->setDefaultShortcut( aOpen, QKeySequence::AddTab );
    model->setDefaultShortcut( aOpenPlainText, QKeySequence::Open );
    model->setDefaultShortcut( aReload, QKeySequence::Refresh );
    model->setDefaultShortcut( aSave, QKeySequence::Save );
    model->setDefaultShortcut( aSaveAs, QKeySequence::SaveAs );
    //model->setDefaultShortcut( aSaveAll, QKeySequence::SaveAll );
    model->setDefaultShortcut( aSaveAll, QKeySequence( "Ctrl+L" ) );
    model->setDefaultShortcut( aClose, QKeySequence::Close );
    //model->setDefaultShortcut( aCloseAll, QKeySequence::CloseAll );
    model->setDefaultShortcut( aQuit, QKeySequence::Quit );
    
    if ( model->defaultShortcut( aSaveAs ).isEmpty() ) {
        const QString shortcut = QString( "Shift+%1" ).arg( model->defaultShortcut( aSave ).toString() );
        model->setDefaultShortcut( aSaveAs, QKeySequence( shortcut ) );
    }
    
    connect( aNewPlainText, &QAction::triggered, this, &UIMain::actionNewPlainTextTriggered );
    connect( aOpen, &QAction::triggered, this, &UIMain::actionOpenTriggered );
    connect( aOpenPlainText, &QAction::triggered, this, &UIMain::actionOpenPlainTextTriggered );
    connect( aReload, &QAction::triggered, this, &UIMain::actionReloadTriggered );
    connect( aSave, &QAction::triggered, this, &UIMain::actionSaveTriggered );
    connect( aSaveAs, &QAction::triggered, this, &UIMain::actionSaveAsTriggered );
    connect( aSaveAll, &QAction::triggered, this, &UIMain::actionSaveAllTriggered );
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

bool UIMain::open( const QStringList& filePaths, const QString& encoding, bool readOnly )
{
    Q_UNUSED( filePaths );
    Q_UNUSED( encoding );
    Q_UNUSED( readOnly );
    // Reserved for futur use
    showError( tr( "UIMain::open: Not yet implemented. Currently only plain text files are handled." ), this );
    return false;
}

bool UIMain::openPlainText( const QStringList& filePaths, const QString& encoding, bool readOnly )
{
    const QHash<QString, Document*> documents = openedDocuments();
    Document* lastDocument = 0;
    bool ok = true;
    
    foreach ( QString filePath, filePaths ) {
        filePath = cleanFilePath( filePath );
        Document* document = documentForFilePath( filePath, documents );
        
        if ( document ) {
            document->setProperty( Document::TextEncoding, encoding );
            document->setProperty( Document::ReadOnly, readOnly );
            lastDocument = document;
            continue;
        }
        
        document = ui->sdDocuments->createDocument();
        
        if ( !document->open( filePath, encoding, readOnly ) ) {
            ok = false;
            showDocumentError( document );
            document->deleteLater();
            continue;
        }
        
        ui->sdDocuments->addDocument( document );
        lastDocument = document;
    }
    
    if ( lastDocument ) {
        ui->sdDocuments->setCurrentDocument( lastDocument );
    }
    
    return ok;
}

bool UIMain::saveDocument( Document* document, const QString& _filePath, const QString& _encoding )
{
    const bool modified = Document::StateHint( document->property( Document::State ).toInt() ) & Document::Modified;
    bool ok = true;
    
    if ( modified ) {
        QString encoding = _encoding.isEmpty() ? document->property( Document::TextEncoding ).toString() : _encoding;
        QString filePath = _filePath.isEmpty() ? document->property( Document::FilePath ).toString() : _filePath;
        
        if ( filePath.isEmpty() ) {
            const QString title = document->property( Document::Title ).toString();
            const QString text = tr( "Choose a file name for '%1'" ).arg( title );
            const QString filters;
            const pFileDialogResult result = requestUserSaveFileName( text, currentWorkingDirectory(), filters, encoding );
            
            encoding = result.textCodec();
            filePath = result.fileName();
        }
        
        if ( filePath.isEmpty() ) {
            return false;
        }
        
        ok = document->save( filePath, encoding );
        
        if ( !ok ) {
            showDocumentError( document );
        }
    }
    
    return ok;
}

bool UIMain::closeDocument( Document* document )
{
    const bool modified = Document::StateHint( document->property( Document::State ).toInt() ) & Document::Modified;
    
    if ( modified ) {
        const QString title = document->property( Document::Title ).toString();
        
        if ( requestUserConfirmation( tr( "The file '%1' has been modified, save it ?" ).arg( title ) ) ) {
            if ( !saveDocument( document ) ) {
                return false;
            }
        }
    }
    
    ui->sdDocuments->removeDocument( document );
    document->deleteLater();
    return true;
}

void UIMain::changeEvent( QEvent* event )
{
    pMainWindow::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}

QString UIMain::currentWorkingDirectory() const
{
    // this could evoluate when advanced things exists (projects, etc...)
    Document* document = ui->sdDocuments->currentDocument();
    return document ? QFileInfo( document->property( Document::FilePath ).toString() ).absolutePath() : QDir::currentPath();
}

QString UIMain::cleanFilePath( const QString& filePath ) const
{
    return QDir::cleanPath( QDir::toNativeSeparators( filePath ) );
}

QHash<QString, Document*> UIMain::openedDocuments() const
{
    QHash<QString, Document*> documents;
    
    for ( int i = 0; i < ui->sdDocuments->count(); i++ ) {
        Document* document = ui->sdDocuments->document( i );
        const QString filePath = document->property( Document::FilePath ).toString();
        
        if ( !filePath.isEmpty() ) {
            documents[ cleanFilePath( filePath ) ] = document;
        }
    }
    
    return documents;
}

Document* UIMain::documentForFilePath( const QString& filePath, const QHash<QString, Document*>& openedDocuments ) const
{
    QHash<QString, Document*> documents = openedDocuments;
    
    if ( documents.isEmpty() ) {
        documents = this->openedDocuments();
    }
    
    return openedDocuments.value( cleanFilePath( filePath ) );
}

void UIMain::showError( const QString& text, QObject* buddy )
{
    Q_UNUSED( buddy ); // api for buddy not yet ready
    appendMessage( text );
}

void UIMain::showDocumentError( Document* document )
{
    showError( document->property( Document::LastError ).toString(), document );
}

pFileDialogResult UIMain::requestUserOpenFileNames( const QString& text, const QString& path, const QString& filters, const QString& encoding )
{
    const QString textCodec = encoding.isEmpty() ? "UTF-8" : encoding; // to be updated to use application general settings
    return pFileDialog::getOpenFileNames( this, text, path, filters, true, true, textCodec );
}

pFileDialogResult UIMain::requestUserSaveFileName( const QString& text, const QString& path, const QString& filters, const QString& encoding )
{
    const QString textCodec = encoding.isEmpty() ? "UTF-8" : encoding; // to be updated to use application general settings
    return pFileDialog::getSaveFileName( this, text, path, filters, true, textCodec );
}

bool UIMain::requestUserConfirmation( const QString& message )
{
    return QMessageBox::question( this, QString::null, message, QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes;
}

bool UIMain::actionNewPlainTextTriggered()
{
    Document* document = ui->sdDocuments->createDocument();
    ui->sdDocuments->addDocument( document );
    ui->sdDocuments->setCurrentDocument( document );
    return true;
}

bool UIMain::actionOpenTriggered()
{
    const QString filters;
    const pFileDialogResult result = requestUserOpenFileNames( tr( "Select the files to open" ), currentWorkingDirectory(), filters );
    
    if ( !result.fileNames().isEmpty() ) {
        return open( result.fileNames(), result.textCodec(), result.readOnly() );
    }
    
    return false;
}

bool UIMain::actionOpenPlainTextTriggered()
{
    const QString filters;
    const pFileDialogResult result = requestUserOpenFileNames( tr( "Select the plain text files to open" ), currentWorkingDirectory(), filters );
    
    if ( !result.fileNames().isEmpty() ) {
        return openPlainText( result.fileNames(), result.textCodec(), result.readOnly() );
    }
    
    return false;
}

bool UIMain::actionReloadTriggered()
{
    Document* document = ui->sdDocuments->currentDocument();
    const bool modified = Document::StateHint( document->property( Document::State ).toInt() ) & Document::Modified;
    
    if ( modified ) {
        const QString title = document->property( Document::Title ).toString();
        
        if ( !requestUserConfirmation( tr( "The file '%1' has been modified, reload it anyway ?" ).arg( title ) ) ) {
            return false;
        }
    }
    
    if ( document->reload() ) {
        return true;
    }
    
    showDocumentError( document );
    return false;
}

bool UIMain::actionSaveTriggered()
{
    Document* document = ui->sdDocuments->currentDocument();
    return saveDocument( document );
}

bool UIMain::actionSaveAsTriggered()
{
    Document* document = ui->sdDocuments->currentDocument();
    const QString title = document->property( Document::Title ).toString();
    const QString text = tr( "Choose a new name for '%1'" ).arg( title );
    const QString filePath = document->property( Document::FilePath ).toString();
    const QString encoding = document->property( Document::TextEncoding ).toString();
    const QString filters;
    const pFileDialogResult result = requestUserSaveFileName( text, filePath, filters, encoding );
    
    //TODO: check result.fileName is not already opened.
    
    if ( result.fileName().isEmpty() ) {
        return false;
    }
    
    return saveDocument( document, result.fileName(), result.textCodec() );
}

bool UIMain::actionSaveAllTriggered()
{
    bool ok = true;
    
    for ( int i = ui->sdDocuments->count() -1; i >= 0; i-- ) {
        Document* document = ui->sdDocuments->document( i );
        
        if ( !saveDocument( document ) ) {
            ok = false;
        }
    }
    
    return ok;
}

bool UIMain::actionCloseTriggered()
{
    Document* document = ui->sdDocuments->currentDocument();
    return closeDocument( document );
}

bool UIMain::actionCloseAllTriggered()
{
    bool ok = true;
    
    for ( int i = ui->sdDocuments->count() -1; i >= 0; i-- ) {
        Document* document = ui->sdDocuments->document( i );
        
        if ( !closeDocument( document ) ) {
            ok = false;
        }
    }
    
    return ok;
}

bool UIMain::actionQuitTriggered()
{
    bool ok = actionSaveAllTriggered();
    
    if ( !ok ) {
        ok = requestUserConfirmation( tr( "Some files were not saved, exit anyway ?" ) );
    }
    
    if ( ok ) {
        qApp->quit();
    }
    
    return ok;
}
