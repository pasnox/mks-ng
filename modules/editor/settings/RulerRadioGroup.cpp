#include "RulerRadioGroup.h"
#include "Document.h"

#include <QRadioButton>

RulerRadioGroup::RulerRadioGroup( QWidget* parent )
    : RadioGroup( QBoxLayout::LeftToRight, parent )
{
    QRadioButton* rbNone = new QRadioButton( tr( "None" ), this );
    QRadioButton* rbLine = new QRadioButton( tr( "Line" ), this );
    QRadioButton* rbBackground = new QRadioButton( tr( "Background" ), this );
    
    addButton( rbNone, Document::NoRuler );
    addButton( rbLine, Document::Line );
    addButton( rbBackground, Document::Background );
}
