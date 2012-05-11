#ifndef DOCUMENTPOSITION_H
#define DOCUMENTPOSITION_H

#include <QToolButton>

class QEvent;

class DocumentPosition : public QToolButton
{
    Q_OBJECT
    
public:
    DocumentPosition( QWidget* parent = 0 );
    virtual ~DocumentPosition();

public slots:
    virtual void retranslateUi();
    
protected:
    virtual void changeEvent( QEvent* event );
};

#endif // DOCUMENTPOSITION_H
