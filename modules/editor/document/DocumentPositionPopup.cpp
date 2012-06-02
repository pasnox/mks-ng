#include "DocumentPositionPopup.h"
#include "ui_DocumentPositionPopup.h"

#include <QEvent>

DocumentPositionPopup::DocumentPositionPopup( QWidget* parent )
    : QFrame( parent, Qt::Popup ),
    ui( new Ui_DocumentPositionPopup )
{
    ui->setupUi( this );
    
#if defined( HAS_QT_5 )
    connect( ui->sLine, &QSlider::valueChanged, this, &DocumentPositionPopup::lineChanged );
#else
    connect( ui->sLine, SIGNAL( valueChanged( int ) ), this, SIGNAL( lineChanged( int ) ) );
#endif
}

DocumentPositionPopup::~DocumentPositionPopup()
{
    delete ui;
}

void DocumentPositionPopup::setLine( int line, int lineCount )
{
    ui->sLine->setRange( 1, lineCount );
    ui->sbLine->setRange( 1, lineCount );
    ui->sLine->setValue( line +1 );
}

void DocumentPositionPopup::retranslateUi()
{
    // do your custom retranslate here
}

void DocumentPositionPopup::changeEvent( QEvent* event )
{
    QFrame::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}
