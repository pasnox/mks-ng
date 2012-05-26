#ifndef RADIOGROUP_H
#define RADIOGROUP_H

#include <QWidget>
#include <QBoxLayout>

class QButtonGroup;
class QAbstractButton;

class RadioGroup : public QWidget
{
    Q_OBJECT

public:
    RadioGroup( QBoxLayout::Direction direction, QWidget* parent = 0 );
    
    void setCheckedId( int id );
    int checkedId() const;

    void addButton( QAbstractButton* button, int id );
    void removeButton( QAbstractButton* button );
    
protected:
    QBoxLayout* mLayout;
    QButtonGroup* mGroup;

signals:
    void buttonClicked( QAbstractButton* button );
    void buttonIdClicked( int id );
    void buttonPressed( QAbstractButton* button );
    void buttonIdPressed( int id );
    void buttonReleased( QAbstractButton* button );
    void buttonIdReleased( int id );
};

#endif // RADIOGROUP_H
