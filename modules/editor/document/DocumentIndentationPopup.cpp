#include "DocumentIndentationPopup.h"
#include "ui_DocumentIndentationPopup.h"

#include <QEvent>

DocumentIndentationPopup::DocumentIndentationPopup( QWidget* parent )
    : QFrame( parent, Qt::Popup ),
    ui( new Ui_DocumentIndentationPopup )
{
    ui->setupUi( this );
    
    connect( ui->irgType, &RadioGroup::buttonIdClicked, this, &DocumentIndentationPopup::modeChanged );
    connect( ui->sIndentWidth, &QSlider::valueChanged, this, &DocumentIndentationPopup::indentWidthChanged );
    connect( ui->sTabWidth, &QSlider::valueChanged, this, &DocumentIndentationPopup::tabWidthChanged );
    connect( ui->pbConvertDocument, &QPushButton::clicked, this, &DocumentIndentationPopup::convertionRequested );
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
