#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QPlainTextEdit>

class QEvent;

class Document : public QPlainTextEdit
{
    Q_OBJECT
    
public:
    Document( QWidget* parent = 0 );
    virtual ~Document();

public slots:
    virtual void retranslateUi();
    
protected:
    virtual void changeEvent( QEvent* event );
};

#endif // DOCUMENT_H
