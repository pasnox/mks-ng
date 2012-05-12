#ifndef UIMAIN_H
#define UIMAIN_H

#include <FreshGui/pMainWindow>

class Ui_UIMain;
class QEvent;

class UIMain : public pMainWindow
{
    Q_OBJECT

public:
    UIMain( QWidget* parent = 0 );
    virtual ~UIMain();

public slots:
    virtual void retranslateUi();
    void open( const QStringList& filePaths, const QString& encoding = QString::null );
    void openPlainText( const QStringList& filePaths, const QString& encoding = QString::null );
    
protected:
    Ui_UIMain* ui;
    
    virtual void changeEvent( QEvent* event );

protected slots:
    void actionOpenTriggered();
    void actionOpenPlainTextTriggered();
    void actionReloadTriggered();
    void actionSaveTriggered();
    void actionSaveAllTriggered();
    void actionBackupTriggered();
    void actionCloseTriggered();
    void actionCloseAllTriggered();
    void actionQuitTriggered();
};

#endif // UIMAIN_H
