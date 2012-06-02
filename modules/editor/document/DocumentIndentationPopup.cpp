#include "DocumentIndentationPopup.h"
#include "ui_DocumentIndentationPopup.h"

#include <QEvent>

DocumentIndentationPopup::DocumentIndentationPopup( QWidget* parent )
    : QFrame( parent, Qt::Popup ),
    ui( new Ui_DocumentIndentationPopup )
{
    ui->setupUi( this );
    
#if defined( HAS_QT_5 )
    connect( ui->irgType, &RadioGroup::buttonIdClicked, this, &DocumentIndentationPopup::modeChanged );
    connect( ui->sIndentWidth, &QSlider::valueChanged, this, &DocumentIndentationPopup::indentWidthChanged );
    connect( ui->sTabWidth, &QSlider::valueChanged, this, &DocumentIndentationPopup::tabWidthChanged );
    connect( ui->pbConvertDocument, &QPushButton::clicked, this, &DocumentIndentationPopup::convertionRequested );
#else
    connect( ui->irgType, SIGNAL( buttonIdClicked( int ) ), this, SIGNAL( modeChanged( int ) ) );
    connect( ui->sIndentWidth, SIGNAL( valueChanged( int ) ), this, SIGNAL( indentWidthChanged( int ) ) );
    connect( ui->sTabWidth, SIGNAL( valueChanged( int ) ), this, SIGNAL( tabWidthChanged( int ) ) );
    connect( ui->pbConvertDocument, SIGNAL( clicked() ), this, SIGNAL( convertionRequested() ) );
#endif
}

DocumentIndentationPopup::~DocumentIndentationPopup()
{
    delete ui;
}

void DocumentIndentationPopup::retranslateUi()
{
    ui->retranslateUi( this );
    // do your custom retranslate here
}

void DocumentIndentationPopup::changeEvent( QEvent* event )
{
    QWidget::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}

void DocumentIndentationPopup::setMode( Document::IndentHint mode )
{
    ui->irgType->setCheckedId( mode );
}

void DocumentIndentationPopup::setIndentWidth( int width )
{
    ui->sIndentWidth->setValue( width );
}

void DocumentIndentationPopup::setTabWidth( int width )
{
    ui->sTabWidth->setValue( width );
}
