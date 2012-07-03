#include "AbstractorComboBox.h"

AbstractorComboBox::AbstractorComboBox( Abstractors::Type type, QWidget* parent )
    : QComboBox( parent )
{
    const QStringList abstractors = Abstractors::entries( type );
    
    foreach ( const QString& abstractor, abstractors ) {
        addItem( abstractor, abstractor );
    }
}

AbstractorComboBox::~AbstractorComboBox()
{
}
