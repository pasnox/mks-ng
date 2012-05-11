#include "DocumentIndentationPopup.h"
#include "ui_DocumentIndentationPopup.h"

#include <QEvent>
#include <QButtonGroup>

DocumentIndentationPopup::DocumentIndentationPopup( QWidget* parent )
    : QFrame( parent, Qt::Popup ),
    ui( new Ui_DocumentIndentationPopup ),
    mGroup( new QButtonGroup( this ) )
{
    ui->setupUi( this );
    mGroup->addButton( ui->rbSpaces, Document::Spaces );
    mGroup->addButton( ui->rbTabs, Document::Tabs );
    
    typedef void (QButtonGroup::*QButtonGroupButtonClickedInt)(int);
    QButtonGroupButtonClickedInt ptr = &QButtonGroup::buttonClicked;
    
    connect( mGroup, ptr, this, &DocumentIndentationPopup::modeChanged );
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
    mGroup->button( mode )->setChecked( true );
}

void DocumentIndentationPopup::setIndentWidth( int width )
{
    ui->sIndentWidth->setValue( width );
}

void DocumentIndentationPopup::setTabWidth( int width )
{
    ui->sTabWidth->setValue( width );
}
