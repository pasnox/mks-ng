#ifndef DOCUMENTEOL_H
#define DOCUMENTEOL_H

#include <QToolButton>

#include "Document.h"

class QEvent;
class QActionGroup;

class DocumentEol : public QToolButton
{
    Q_OBJECT
    
public:
    DocumentEol( QWidget* parent = 0 );
    virtual ~DocumentEol();
    
    void setMode( Document::EolHint mode );
    Document::EolHint mode() const;

public slots:
    virtual void retranslateUi();
    
protected:
    QActionGroup* mGroup;
    QHash<Document::EolHint, QAction*> mActions;
    
    virtual void changeEvent( QEvent* event );

protected slots:
    void group_triggered( QAction* action );

signals:
    void modeChanged( Document::EolHint mode );
};

#endif // DOCUMENTEOL_H
