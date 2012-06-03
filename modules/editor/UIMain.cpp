#include "UIMain.h"
#include "ui_UIMain.h"
#include "StackedDocumentModel.h"
#include "Document.h"
#include "Menu.h"
#include "main.h"
#include "StackedDocumentCloseQuery.h"
#include "SettingsNodeDialogBuilder.h"
#include "ApplicationSettingsDelegate.h"
#include "Abstractors.h"

#include <FreshGui/pQueuedMessageToolBar>
#include <FreshGui/pActionsMenuBar>
#include <FreshGui/pActionsModel>
#include <FreshGui/pFileDialog>
#include <FreshCore/pSettings>

#include <QCloseEvent>
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
    
    model->addMenu( MENU_EDIT, tr( "Edit" ) );
    QAction* aUndo = model->addAction( MENU_EDIT_UNDO, tr( "Undo" ), QIcon::fromTheme( "edit-undo" ) );
    QAction* aRedo = model->addAction( MENU_EDIT_REDO, tr( "Redo" ), QIcon::fromTheme( "edit-redo" ) );
    QAction* aCopy = model->addAction( MENU_EDIT_COPY, tr( "Copy" ), QIcon::fromTheme( "edit-copy" ) );
    QAction* aCut = model->addAction( MENU_EDIT_CUT, tr( "Cut" ), QIcon::fromTheme( "edit-cut" ) );
    QAction* aPaste = model->addAction( MENU_EDIT_PASTE, tr( "Paste" ), QIcon::fromTheme( "edit-paste" ) );
    QAction* aPreferences = model->addAction( MENU_EDIT_PREFERENCES, tr( "Preferences" ), QIcon::fromTheme( "preferences-other" ) );
    
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
    aQuit->setMenuRole( QAction::QuitRole );
    
    model->setDefaultShortcut( aUndo, QKeySequence::Undo );
    aUndo->setData( Document::Undo );
    model->setDefaultShortcut( aRedo, QKeySequence::Redo );
    aRedo->setData( Document::Redo );
    model->setDefaultShortcut( aCopy, QKeySequence::Copy );
    aCopy->setData( Document::Copy );
    model->setDefaultShortcut( aCut, QKeySequence::Cut );
    aCut->setData( Document::Cut );
    model->setDefaultShortcut( aPaste, QKeySequence::Paste );
    aPaste->setData( Document::Paste );
    model->setDefaultShortcut( aPreferences, QKeySequence( "Ctrl+Shift+P" ) );
    aPreferences->setMenuRole( QAction::PreferencesRole );
    
    if ( model->defaultShortcut( aSaveAs ).isEmpty() ) {
        const QString shortcut = QString( "Shift+%1" ).arg( model->defaultShortcut( aSave ).toString() );
        model->setDefaultShortcut( aSaveAs, QKeySequence( shortcut ) );
    }
    
#if defined( HAS_QT_5 )
    connect( this, &UIMain::preferencesChanged, ui->sdDocuments, &StackedDocument::applyPreferences );
    
    connect( aUndo, &QAction::triggered, this, &UIMain::documentActionTriggered );
    connect( aRedo, &QAction::triggered, this, &UIMain::documentActionTriggered );
    connect( aCopy, &QAction::triggered, this, &UIMain::documentActionTriggered );
    connect( aCut, &QAction::triggered, this, &UIMain::documentActionTriggered );
    connect( aPaste, &QAction::triggered, this, &UIMain::documentActionTriggered );
    connect( aPreferences, &QAction::triggered, this, &UIMain::actionPreferencesTriggered );
    
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
#else
    connect( this, SIGNAL( preferencesChanged() ), ui->sdDocuments, SLOT( applyPreferences() ) );
    
    connect( aUndo, SIGNAL( triggered() ), this, SLOT( documentActionTriggered() ) );
    connect( aRedo, SIGNAL( triggered() ), this, SLOT( documentActionTriggered() ) );
    connect( aCopy, SIGNAL( triggered() ), this, SLOT( documentActionTriggered() ) );
    connect( aCut, SIGNAL( triggered() ), this, SLOT( documentActionTriggered() ) );
    connect( aPaste, SIGNAL( triggered() ), this, SLOT( documentActionTriggered() ) );
    connect( aPreferences, SIGNAL( triggered() ), this, SLOT( actionPreferencesTriggered() ) );
    
    connect( aNewPlainText, SIGNAL( triggered() ), this, SLOT( actionNewPlainTextTriggered() ) );
    connect( aOpen, SIGNAL( triggered() ), this, SLOT( actionOpenTriggered() ) );
    connect( aOpenPlainText, SIGNAL( triggered() ), this, SLOT( actionOpenPlainTextTriggered() ) );
    connect( aReload, SIGNAL( triggered() ), this, SLOT( actionReloadTriggered() ) );
    connect( aSave, SIGNAL( triggered() ), this, SLOT( actionSaveTriggered() ) );
    connect( aSaveAs, SIGNAL( triggered() ), this, SLOT( actionSaveAsTriggered() ) );
    connect( aSaveAll, SIGNAL( triggered() ), this, SLOT( actionSaveAllTriggered() ) );
    connect( aClose, SIGNAL( triggered() ), this, SLOT( actionCloseTriggered() ) );
    connect( aCloseAll, SIGNAL( triggered() ), this, SLOT( actionCloseAllTriggered() ) );
    connect( aQuit, SIGNAL( triggered() ), this, SLOT( actionQuitTriggered() ) );
#endif
    
    on_sdDocuments_currentDocumentChanged( 0 );
}

UIMain::~UIMain()
{
    delete ui;
}

QIcon UIMain::currentWindowIcon() const
{
    const Document* document = ui->sdDocuments->currentDocument();
    return document ? document->property( Document::Decoration ).value<QIcon>() : QApplication::windowIcon();
}

QString UIMain::currentWindowTitle() const
{
    const Document* document = ui->sdDocuments->currentDocument();
    const QString documentTitle = document ? document->property( Document::Title ).toString() : QString::null;
    
    return document
        ? QString( "%4[*] - %1 v%2 (%3)" ).arg( APPLICATION_NAME ).arg( APPLICATION_VERSION ).arg( APPLICATION_VERSION_LONG ).arg( documentTitle )
        : QString( "%1 v%2 (%3)[*]" ).arg( APPLICATION_NAME ).arg( APPLICATION_VERSION ).arg( APPLICATION_VERSION_LONG )
    ;
}

void UIMain::retranslateUi()
{
    ui->retranslateUi( this );
    // do your custom retranslate here
}

void UIMain::saveState()
{
    pMainWindow::saveState();
    Abstractors::applicationSettings().save( settings() );
}

void UIMain::restoreState()
{
    Abstractors::applicationSettings().load( settings() );
    pMainWindow::restoreState();
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
    const QHash<Document*, QString> documents = openedDocuments();
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
    delete document;
    return true;
}

void UIMain::closeEvent( QCloseEvent* event )
{
    if ( actionCloseAllTriggered() ) {
    }
    else {
        event->ignore();
        return;
    }
    
    pMainWindow::closeEvent( event );
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
    const QString filePath = document ? document->property( Document::FilePath ).toString() : QString::null;
    return document && !filePath.isEmpty() ? QFileInfo( filePath ).absolutePath() : QDir::currentPath();
}

QString UIMain::cleanFilePath( const QString& filePath ) const
{
    return QDir::cleanPath( QDir::toNativeSeparators( filePath ) );
}

QHash<Document*, QString> UIMain::openedDocuments( bool modifiedOnly ) const
{
    QHash<Document*, QString> documents;
    
    for ( int i = 0; i < ui->sdDocuments->count(); i++ ) {
        Document* document = ui->sdDocuments->document( i );
        const QString filePath = document->property( Document::FilePath ).toString();
        const bool fileModified = document->property( Document::State ).toInt() & Document::Modified;
        
        if ( ( !modifiedOnly ) || ( modifiedOnly && fileModified ) ) {
            documents[ document ] = cleanFilePath( filePath );
        }
    }
    
    return documents;
}

Document* UIMain::documentForFilePath( const QString& filePath, const QHash<Document*, QString>& openedDocuments ) const
{
    QHash<Document*, QString> documents = openedDocuments;
    
    if ( documents.isEmpty() ) {
        documents = this->openedDocuments();
    }
    
    return openedDocuments.key( cleanFilePath( filePath ) );
}

void UIMain::showError( const QString& text, QObject* buddy )
{
    Q_UNUSED( buddy ); // api for buddy not yet ready
    appendMessage( text );
}

void UIMain::showDocumentError( Document* document )
{
    const QString text = document->property( Document::LastError ).toString();
    
    if ( !text.isEmpty() ) {
        showError( text, document );
    }
}

pFileDialogResult UIMain::requestUserOpenFileNames( const QString& text, const QString& path, const QString& filters, const QString& encoding )
{
    const QString textCodec = encoding.isEmpty() ? Abstractors::applicationSettings().editor.documents.codec.value().toString() : encoding;
    return pFileDialog::getOpenFileNames( this, text, path, filters, true, true, textCodec );
}

pFileDialogResult UIMain::requestUserSaveFileName( const QString& text, const QString& path, const QString& filters, const QString& encoding )
{
    const QString textCodec = encoding.isEmpty() ? Abstractors::applicationSettings().editor.documents.codec.value().toString() : encoding;
    return pFileDialog::getSaveFileName( this, text, path, filters, true, textCodec );
}

bool UIMain::requestUserConfirmation( const QString& message )
{
    return QMessageBox::question( this, QString::null, message, QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes;
}

void UIMain::on_sdDocuments_currentDocumentChanged( Document* document )
{
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::Decoration );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::Title );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::FilePath );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::State );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::UndoAvailable );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::RedoAvailable );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::CursorPosition );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::Eol );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::Indent );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::TabWidth );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::IndentWidth );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::SelectionStart );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::SelectionEnd );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::SelectionLength );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::SelectedText );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::Text );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::Ruler );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::Language );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::Style );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::LineCount );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::NewFile );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::LastError );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::TextEncoding );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::CopyAvailable );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::CutAvailable );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::PasteAvailable );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::ReadOnly );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::InitialText );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::LineWrap );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::LineNumberMargin );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::FoldMargin );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::SymbolMargin );
    on_sdDocuments_currentDocumentPropertyChanged( document, Document::ChangeMargin );
}

void UIMain::on_sdDocuments_currentDocumentPropertyChanged( Document* document, int property )
{
    const pActionsModel* model = menuBar()->model();
    
    switch ( property ) {
        case Document::Decoration:
            setWindowIcon( currentWindowIcon() );
            return;
        case Document::Title:
            setWindowTitle( currentWindowTitle() );
            return;
        case Document::State: {
            const int documentCount = ui->sdDocuments->count();
            const bool documentModified = document ? document->property( property ).toInt() & Document::Modified : false;
            setWindowModified( documentModified );
            setWindowIcon( currentWindowIcon() );
            model->action( MENU_FILE_RELOAD )->setEnabled( document );
            model->action( MENU_FILE_SAVE )->setEnabled( documentModified );
            model->action( MENU_FILE_SAVE_AS )->setEnabled( document );
            model->action( MENU_FILE_SAVE_ALL )->setEnabled( documentModified || documentCount > 0 );
            model->action( MENU_FILE_CLOSE )->setEnabled( document );
            model->action( MENU_FILE_CLOSE_ALL )->setEnabled( document || documentCount > 0 );
            return;
        }
        case Document::UndoAvailable: {
            const bool undoAvailable = document ? document->property( property ).toBool() : false;
            model->action( MENU_EDIT_UNDO )->setEnabled( undoAvailable );
            return;
        }
        case Document::RedoAvailable: {
            const bool redoAvailable = document ? document->property( property ).toBool() : false;
            model->action( MENU_EDIT_REDO )->setEnabled( redoAvailable );
            return;
        }
        case Document::CopyAvailable: {
            const bool copyAvailable = document ? document->property( property ).toBool() : false;
            model->action( MENU_EDIT_COPY )->setEnabled( copyAvailable );
            return;
        }
        case Document::CutAvailable: {
            const bool cutAvailable = document ? document->property( property ).toBool() : false;
            model->action( MENU_EDIT_CUT )->setEnabled( cutAvailable );
            return;
        }
        case Document::PasteAvailable: {
            const bool pasteAvailable = document ? document->property( property ).toBool() : false;
            model->action( MENU_EDIT_PASTE )->setEnabled( pasteAvailable );
            return;
        }
        case Document::LastError: // for debugging
            /*if ( document ) {
                showDocumentError( document );
            }*/
            return;
        
        // these properties does not (yet) have main window interest, just ignore them.
        case Document::FilePath:
        case Document::CursorPosition:
        case Document::Eol:
        case Document::Indent:
        case Document::TabWidth:
        case Document::IndentWidth:
        case Document::SelectionStart:
        case Document::SelectionEnd:
        case Document::SelectionLength:
        case Document::SelectedText:
        case Document::Text:
        case Document::Ruler:
        case Document::Language:
        case Document::Style:
        case Document::LineCount:
        case Document::NewFile:
        case Document::TextEncoding:
        case Document::ReadOnly:
        case Document::InitialText:
        case Document::LineWrap:
        case Document::LineNumberMargin:
        case Document::FoldMargin:
        case Document::SymbolMargin:
        case Document::ChangeMargin:
            break;
    }
}

void UIMain::documentActionTriggered()
{
    Document* document = ui->sdDocuments->currentDocument();
    QAction* action = qobject_cast<QAction*>( sender() );
    document->triggerAction( action->data().toInt() );
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
    const QHash<Document*, QString> modifiedDocuments = openedDocuments( true );
    
    if ( !modifiedDocuments.isEmpty() ) {
        StackedDocumentCloseQuery dlg( modifiedDocuments.keys(), this );
        
        if ( dlg.exec() == QDialog::Rejected ) {
            return false;
        }
        
        const DocumentList checkedDocuments = dlg.checkedDocuments();
        
        foreach ( Document* document, checkedDocuments ) {
            saveDocument( document );
        }
    }
    
    for ( int i = ui->sdDocuments->count() -1; i >= 0; i-- ) {
        Document* document = ui->sdDocuments->document( i );
        ui->sdDocuments->removeDocument( document );
        document->deleteLater();
    }
    
    return true;
}

bool UIMain::actionQuitTriggered()
{
    return close();
}

void UIMain::actionPreferencesTriggered()
{
    ApplicationSettingsDelegate delegate;
    SettingsNodeDialogBuilder dlg( this );
    
    dlg.setDelegate( &delegate );
    
    if ( !dlg.build( Abstractors::applicationSettings() ) ) {
        showError( tr( "Can't build the preferences dialog." ), this );
        return;
    }
    
#if defined( HAS_QT_5 )
    connect( &dlg, &SettingsNodeDialogBuilder::applyClicked, this, &UIMain::preferencesChanged );
#else
    connect( &dlg, SIGNAL( applyClicked() ), this, SIGNAL( preferencesChanged() ) );
#endif
    
    dlg.exec();
}
