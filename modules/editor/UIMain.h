#ifndef UIMAIN_H
#define UIMAIN_H

#include <FreshGui/pMainWindow>

class Ui_UIMain;
class QEvent;

class pFileDialogResult;

class Document;

class UIMain : public pMainWindow
{
    Q_OBJECT

public:
    UIMain( QWidget* parent = 0 );
    virtual ~UIMain();

public slots:
    virtual void retranslateUi();
    bool open( const QStringList& filePaths, const QString& encoding = QString::null, bool readOnly = false );
    bool openPlainText( const QStringList& filePaths, const QString& encoding = QString::null, bool readOnly = false );
    bool saveDocument( Document* document, const QString& filePath = QString::null, const QString& encoding = QString::null );
    bool closeDocument( Document* document );
    
protected:
    Ui_UIMain* ui;
    
    virtual void changeEvent( QEvent* event );
    QString currentWorkingDirectory() const;
    QString cleanFilePath( const QString& filePath ) const;
    QHash<QString, Document*> openedDocuments() const;
    Document* documentForFilePath( const QString& filePath, const QHash<QString, Document*>& openedDocuments = QHash<QString, Document*>() ) const;
    void showError( const QString& text, QObject* buddy = 0 );
    void showDocumentError( Document* document );
    pFileDialogResult requestUserOpenFileNames( const QString& text, const QString& path, const QString& filters, const QString& encoding = QString::null );
    pFileDialogResult requestUserSaveFileName( const QString& text, const QString& path, const QString& filters, const QString& encoding = QString::null );
    bool requestUserConfirmation( const QString& message );

protected slots:
    bool actionNewPlainTextTriggered();
    bool actionOpenTriggered();
    bool actionOpenPlainTextTriggered();
    bool actionReloadTriggered();
    bool actionSaveTriggered();
    bool actionSaveAsTriggered();
    bool actionSaveAllTriggered();
    bool actionCloseTriggered();
    bool actionCloseAllTriggered();
    bool actionQuitTriggered();
};

#endif // UIMAIN_H
