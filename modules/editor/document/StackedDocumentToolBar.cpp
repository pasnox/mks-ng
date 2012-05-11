#include "StackedDocumentToolBar.h"
#include "StackedDocument.h"
#include "CodeEditorAbstractor.h"
#include "DocumentEol.h"
#include "DocumentIndentation.h"

#include <QEvent>
#include <QComboBox>
#include <QDebug>

StackedDocumentToolBar::StackedDocumentToolBar( QWidget* parent )
    : QToolBar( parent ),
        mStacker( 0 ),
        cbLanguages( 0 ),
        cbStyles( 0 ),
        deMode( 0 ),
        diMode( 0 )
{
}

StackedDocumentToolBar::~StackedDocumentToolBar()
{
}

void StackedDocumentToolBar::setStackedDocument( StackedDocument* stacker )
{
    if ( mStacker ) {
        clear();
        cbLanguages = 0;
        cbStyles = 0;
        deMode = 0;
    }
    
    mStacker = stacker;
    
    if ( mStacker ) {
        CodeEditorAbstractor* cea = mStacker->codeEditorAbstractor();
        QWidget* spacer = new QWidget( this );
        cbLanguages = new QComboBox( this );
        cbStyles = new QComboBox( this );
        deMode = new DocumentEol( this );
        diMode = new DocumentIndentation( this );
        
        spacer->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
        cbLanguages->addItems( cea->supportedLanguages() );
        cbStyles->addItems( cea->supportedStyles() );
        
        addWidget( spacer );
        addWidget( cbLanguages );
        addWidget( cbStyles );
        addWidget( deMode );
        addWidget( diMode );
        
        // connections
        typedef void (QComboBox::*QComboBoxActivatedQString)(const QString&);
        QComboBoxActivatedQString ptr = &QComboBox::activated;
        
        connect( mStacker, &StackedDocument::currentDocumentChanged, this, &StackedDocumentToolBar::stacker_currentDocumentChanged );
        connect( cbLanguages, ptr, this, &StackedDocumentToolBar::cbLanguages_activated );
        connect( cbStyles, ptr, this, &StackedDocumentToolBar::cbStyles_activated );
        connect( deMode, &DocumentEol::modeChanged, this, &StackedDocumentToolBar::deMode_modeChanged );
        connect( diMode, &DocumentIndentation::modeChanged, this, &StackedDocumentToolBar::diMode_modeChanged );
        connect( diMode, &DocumentIndentation::indentWidthChanged, this, &StackedDocumentToolBar::diMode_indentWidthChanged );
        connect( diMode, &DocumentIndentation::tabWidthChanged, this, &StackedDocumentToolBar::diMode_tabWidthChanged );
    }
    
    stacker_currentDocumentChanged( mStacker ? mStacker->currentDocument() : 0 );
}

void StackedDocumentToolBar::retranslateUi()
{
    // do your custom retranslate here
}

void StackedDocumentToolBar::changeEvent( QEvent* event )
{
    QToolBar::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}

void StackedDocumentToolBar::stacker_currentDocumentChanged( Document* document )
{
    if ( mStacker && document ) {
        const QString language = document->property( Document::Language ).toString();
        const QString style = document->property( Document::Style ).toString();
        
        cbLanguages->setCurrentIndex( cbLanguages->findText( language ) );
        cbStyles->setCurrentIndex( cbStyles->findText( style ) );
        deMode->setMode( Document::EolHint( document->property( Document::Eol ).toInt() ) );
        diMode->setMode( Document::IndentHint( document->property( Document::Indent ).toInt() ) );
        diMode->setIndentWidth( document->property( Document::IndentWidth ).toInt() );
        diMode->setTabWidth( document->property( Document::TabWidth ).toInt() );
    }
    else {
        cbLanguages->setCurrentIndex( -1 );
        cbStyles->setCurrentIndex( -1 );
    }
    
    setEnabled( mStacker && mStacker->count() > 0 );
}

void StackedDocumentToolBar::cbLanguages_activated( const QString& language )
{
    Q_ASSERT( mStacker );
    Document* document = mStacker->currentDocument();
    
    if ( document ) {
        document->setProperty( Document::Language, language );
    }
}

void StackedDocumentToolBar::cbStyles_activated( const QString& style )
{
    Q_ASSERT( mStacker );
    Document* document = mStacker->currentDocument();
    
    if ( document ) {
        document->setProperty( Document::Style, style );
    }
}

void StackedDocumentToolBar::deMode_modeChanged( Document::EolHint mode )
{
    Q_ASSERT( mStacker );
    Document* document = mStacker->currentDocument();
    
    if ( document ) {
        document->setProperty( Document::Eol, mode );
    }
}

void StackedDocumentToolBar::diMode_modeChanged( Document::IndentHint mode )
{
    Q_ASSERT( mStacker );
    Document* document = mStacker->currentDocument();
    
    if ( document ) {
        document->setProperty( Document::Indent, mode );
    }
}

void StackedDocumentToolBar::diMode_indentWidthChanged( int width )
{
    Q_ASSERT( mStacker );
    Document* document = mStacker->currentDocument();
    
    if ( document ) {
        document->setProperty( Document::IndentWidth, width );
    }
}

void StackedDocumentToolBar::diMode_tabWidthChanged( int width )
{
    Q_ASSERT( mStacker );
    Document* document = mStacker->currentDocument();
    
    if ( document ) {
        document->setProperty( Document::TabWidth, width );
    }
}
