#ifndef ABSTRACTORCOMBOBOX_H
#define ABSTRACTORCOMBOBOX_H

#include <QComboBox>

#include "Abstractors.h"

class AbstractorComboBox : public QComboBox
{
    Q_OBJECT
    
public:
    AbstractorComboBox( Abstractors::Type type, QWidget* parent = 0 );
    virtual ~AbstractorComboBox();
};

#endif // ABSTRACTORCOMBOBOX_H
