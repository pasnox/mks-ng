#ifndef UIMAIN_H
#define UIMAIN_H

#include <QMainWindow>

class Ui_UIMain;
class QEvent;

class UIMain : public QMainWindow
{
    Q_OBJECT

public:
    UIMain( QWidget* parent = 0 );
    virtual ~UIMain();

public slots:
    virtual void retranslateUi();
    
protected:
    Ui_UIMain* ui;
    
    virtual void changeEvent( QEvent* event );

protected slots:
    void on_cbLanguages_activated( const QString& language );
    void on_cbStyles_activated( const QString& style );
};

#endif // UIMAIN_H
