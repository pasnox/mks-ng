#include "DocumentPosition.h"

#include <QEvent>

DocumentPosition::DocumentPosition( QWidget* parent )
    : QToolButton( parent )
{
}

DocumentPosition::~DocumentPosition()
{
}

void DocumentPosition::retranslateUi()
{
    // do your custom retranslate here
}

void DocumentPosition::changeEvent( QEvent* event )
{
    QToolButton::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}
