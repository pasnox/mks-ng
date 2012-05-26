#include "EolRadioGroup.h"
#include "Document.h"

#include <QRadioButton>

EolRadioGroup::EolRadioGroup( QWidget* parent )
    : RadioGroup( QBoxLayout::LeftToRight, parent )
{
    QRadioButton* rbUnix = new QRadioButton( "Unix/Linux", this );
    QRadioButton* rbMac = new QRadioButton( "Mac OS (< OS X)", this );
    QRadioButton* rbDos = new QRadioButton( "DOS/Windows", this );
    
    addButton( rbUnix, Document::Unix );
    addButton( rbMac, Document::MacOS );
    addButton( rbDos, Document::DOS );
}
