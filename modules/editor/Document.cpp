#include "Document.h"

#include <QEvent>

Document::Document( QWidget* parent )
    : QPlainTextEdit( parent )
{
}

Document::~Document()
{
}

void Document::retranslateUi()
{
    // do your custom retranslate here
}

void Document::changeEvent( QEvent* event )
{
    QWidget::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}
