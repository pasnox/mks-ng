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
};

#endif // UIMAIN_H
