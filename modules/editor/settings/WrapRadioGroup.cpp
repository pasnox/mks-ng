#include "WrapRadioGroup.h"
#include "Document.h"

#include <QRadioButton>

WrapRadioGroup::WrapRadioGroup( QWidget* parent )
    : RadioGroup( QBoxLayout::LeftToRight, parent )
{
    QRadioButton* rbNone = new QRadioButton( tr( "None" ), this );
    QRadioButton* rbWidth = new QRadioButton( tr( "Width" ), this );
    
    addButton( rbNone, Document::NoWrap );
    addButton( rbWidth, Document::WidthWrap );
}
