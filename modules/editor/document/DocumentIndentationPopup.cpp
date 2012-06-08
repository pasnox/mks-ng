#include "DocumentIndentationPopup.h"
#include "ui_DocumentIndentationPopup.h"

#include <QEvent>
#include <QMessageBox>

DocumentIndentationPopup::DocumentIndentationPopup( QWidget* parent )
    : QFrame( parent, Qt::Popup ),
    ui( new Ui_DocumentIndentationPopup )
{
    ui->setupUi( this );
    
/*#if defined( HAS_QT_5 )
    connect( ui->irgType, &RadioGroup::buttonIdClicked, this, &DocumentIndentationPopup::modeChanged );
    connect( ui->sIndentWidth, &QSlider::valueChanged, this, &DocumentIndentationPopup::indentWidthChanged );
    connect( ui->sTabWidth, &QSlider::valueChanged, this, &DocumentIndentationPopup::tabWidthChanged );
    connect( ui->pbConvertDocument, &QPushButton::clicked, this, &DocumentIndentationPopup::convertionRequested );
#else
    connect( ui->irgType, SIGNAL( buttonIdClicked( int ) ), this, SIGNAL( modeChanged( int ) ) );
    connect( ui->sIndentWidth, SIGNAL( valueChanged( int ) ), this, SIGNAL( indentWidthChanged( int ) ) );
    connect( ui->sTabWidth, SIGNAL( valueChanged( int ) ), this, SIGNAL( tabWidthChanged( int ) ) );
    connect( ui->pbConvertDocument, SIGNAL( clicked() ), this, SIGNAL( convertionRequested() ) );
#endif*/

#if defined( HAS_QT_5 )
    connect( ui->irgType, &RadioGroup::buttonIdClicked, this, &DocumentIndentationPopup::propertiesChanged );
    connect( ui->sIndentWidth, &QSlider::valueChanged, this, &DocumentIndentationPopup::propertiesChanged );
    connect( ui->sTabWidth, &QSlider::valueChanged, this, &DocumentIndentationPopup::propertiesChanged );
#else
    connect( ui->irgType, SIGNAL( buttonIdClicked( int ) ), this, SLOT( propertiesChanged() ) );
    connect( ui->sIndentWidth, SIGNAL( valueChanged( int ) ), this, SLOT( propertiesChanged() ) );
    connect( ui->sTabWidth, SIGNAL( valueChanged( int ) ), this, SLOT( propertiesChanged() ) );
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
    QFrame::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}

void DocumentIndentationPopup::showEvent( QShowEvent* event )
{
    ui->pbConvertDocument->setEnabled( false );
    
    QFrame::showEvent( event );
}

void DocumentIndentationPopup::closeEvent( QCloseEvent* event )
{
    if ( ui->pbConvertDocument->isEnabled() ) {
        DocumentPropertiesDiscover::GuessedProperties properties;
        
        properties.eol = -1; // use document eol
        properties.indent = ui->irgType->checkedId();
        properties.indentWidth = ui->sIndentWidth->value();
        properties.tabWidth = ui->sTabWidth->value();
        
        emit convertionRequested( properties, true );
    }
    
    QFrame::closeEvent( event );
}

void DocumentIndentationPopup::propertiesChanged()
{
    ui->pbConvertDocument->setEnabled( true );
}

void DocumentIndentationPopup::on_pbConvertDocument_clicked()
{
    DocumentPropertiesDiscover::GuessedProperties properties;
    
    properties.eol = -1; // use document eol
    properties.indent = ui->irgType->checkedId();
    properties.indentWidth = ui->sIndentWidth->value();
    properties.tabWidth = ui->sTabWidth->value();
    
    emit convertionRequested( properties, false );
    
    ui->pbConvertDocument->setEnabled( false );
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
