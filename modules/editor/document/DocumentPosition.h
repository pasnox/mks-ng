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
    
    void setPosition( const QPoint& pos, int lineCount );
    QPoint position() const;

public slots:
    virtual void retranslateUi();
    void showPopup();
    
protected:
    QPoint mPos;
    int mLineCount;
    
    virtual void changeEvent( QEvent* event );
    void propertiesChanged();

protected slots:
    void popup_lineChanged( int line );

signals:
    void positionChanged( const QPoint& pos );
};

#endif // DOCUMENTPOSITION_H
