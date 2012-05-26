#include "IndentationRadioGroup.h"
#include "Document.h"

#include <QRadioButton>

IndentationRadioGroup::IndentationRadioGroup( QWidget* parent )
    : RadioGroup( QBoxLayout::LeftToRight, parent )
{
    QRadioButton* rbSpaces = new QRadioButton( tr( "Spaces" ), this );
    QRadioButton* rbTabs = new QRadioButton( tr( "Tabs" ), this );
    
    addButton( rbSpaces, Document::Spaces );
    addButton( rbTabs, Document::Tabs );
}
