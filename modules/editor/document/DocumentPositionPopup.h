#ifndef DOCUMENTPOSITIONPOPUP_H
#define DOCUMENTPOSITIONPOPUP_H

#include <QFrame>

class Ui_DocumentPositionPopup;
class QEvent;

class DocumentPositionPopup : public QFrame
{
    Q_OBJECT
    
public:
    DocumentPositionPopup( QWidget* parent = 0 );
    virtual ~DocumentPositionPopup();
    
    void setLine( int line, int lineCount );

public slots:
    virtual void retranslateUi();
    
protected:
    Ui_DocumentPositionPopup* ui;
    
    virtual void changeEvent( QEvent* event );

signals:
    void lineChanged( int line );
};

#endif // DOCUMENTPOSITIONPOPUP_H
