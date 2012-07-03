#include "StackedDocumentToolBar.h"
#include "StackedDocument.h"
#include "CodeEditorAbstractor.h"
#include "DocumentEol.h"
#include "DocumentIndentation.h"
#include "DocumentPosition.h"

#include <FreshGui/pQueuedMessageWidget>

#include <QEvent>
#include <QStringListModel>
#include <QComboBox>
#include <QLayout>
#include <QApplication>
#include <QDebug>

// simple model to override Qt::ForegroundRole badly overwrited by some style menu bar css.

class DocumentToolBarModel : public QStringListModel {
public:
    DocumentToolBarModel( QObject* parent = 0 )
        : QStringListModel( parent ) {}
    
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const {
        switch ( role ) {
            // fix stupid styles that force css styles in menubar without doing proper matching
            case Qt::ForegroundRole:
                return QApplication::style()->standardPalette().brush( QPalette::Text );
            default:
                return QStringListModel::data( index, role );
        }
    }
};

// Languages model

class DocumentLanguageModel : public DocumentToolBarModel {
public:
    DocumentLanguageModel( QObject* parent = 0 )
        : DocumentToolBarModel( parent ) {}
    
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const {
        if ( index.isValid() ) {
            switch ( role ) {
                case Qt::DecorationRole: {
                    const QString language = DocumentToolBarModel::data( index, Qt::DisplayRole ).toString();
                    return Abstractors::codeEditor()->mimeTypeDB().iconForLanguage( language );
                }
            }
        }
        
        return DocumentToolBarModel::data( index, role );
    }
};

// Styles model

class DocumentStyleModel : public DocumentToolBarModel {
public:
    DocumentStyleModel( QObject* parent = 0 )
        : DocumentToolBarModel( parent ) {}
};

// StackedDocumentToolBar

StackedDocumentToolBar::StackedDocumentToolBar( QWidget* parent )
    : QToolBar( parent ),
        mStacker( 0 ),
        dlmLanguages( new DocumentLanguageModel( this ) ),
        dsmStyles( new DocumentStyleModel( this ) ),
        cbLanguages( 0 ),
        cbStyles( 0 ),
        deMode( 0 ),
        diMode( 0 ),
        dpCursor( 0 )
{
    setMovable( false );
    setFixedHeight( 18 );
    layout()->setMargin( 0 );
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
        dpCursor = 0;
    }
    
    mStacker = stacker;
    
    if ( mStacker ) {
        CodeEditorAbstractor* cea = Abstractors::codeEditor();
        QWidget* spacer = new QWidget( this );
        cbLanguages = new QComboBox( this );
        cbStyles = new QComboBox( this );
        deMode = new DocumentEol( this );
        diMode = new DocumentIndentation( this );
        dpCursor = new DocumentPosition( this );
        
        dlmLanguages->setStringList( cea->supportedLanguages() );
        dsmStyles->setStringList( cea->supportedStyles() );
        
        spacer->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
        cbLanguages->setMaximumHeight( 21 );
        cbLanguages->setModel( dlmLanguages );
        cbStyles->setMaximumHeight( 21 );
        cbStyles->setModel( dsmStyles );
        deMode->setMaximumHeight( 21 );
        diMode->setMaximumHeight( 21 );
        dpCursor->setMaximumHeight( 21 );
        
        addWidget( spacer );
        addWidget( cbLanguages );
        addWidget( cbStyles );
        addWidget( deMode );
        addWidget( diMode );
        addWidget( dpCursor );
        
        // connections
#if defined( HAS_QT_5 )
        typedef void (QComboBox::*QComboBoxActivatedQString)(const QString&);
        QComboBoxActivatedQString ptr = &QComboBox::activated;
        
        connect( mStacker, &StackedDocument::currentDocumentChanged, this, &StackedDocumentToolBar::stacker_currentDocumentChanged );
        connect( mStacker, &StackedDocument::currentDocumentPropertyChanged, this, &StackedDocumentToolBar::stacker_currentDocumentPropertyChanged );
        connect( cbLanguages, ptr, this, &StackedDocumentToolBar::cbLanguages_activated );
        connect( cbStyles, ptr, this, &StackedDocumentToolBar::cbStyles_activated );
        connect( deMode, &DocumentEol::modeChanged, this, &StackedDocumentToolBar::deMode_modeChanged );
        connect( diMode, &DocumentIndentation::modeChanged, this, &StackedDocumentToolBar::diMode_modeChanged );
        connect( diMode, &DocumentIndentation::indentWidthChanged, this, &StackedDocumentToolBar::diMode_indentWidthChanged );
        connect( diMode, &DocumentIndentation::tabWidthChanged, this, &StackedDocumentToolBar::diMode_tabWidthChanged );
        connect( diMode, &DocumentIndentation::convertionRequested, this, &StackedDocumentToolBar::diMode_convertionRequested );
        connect( dpCursor, &DocumentPosition::positionChanged, this, &StackedDocumentToolBar::dpCursor_positionChanged );
#else
        connect( mStacker, SIGNAL( currentDocumentChanged( Document* ) ), this, SLOT( stacker_currentDocumentChanged( Document* ) ) );
        connect( mStacker, SIGNAL( currentDocumentPropertyChanged( Document*, int ) ), this, SLOT( stacker_currentDocumentPropertyChanged( Document*, int ) ) );
        connect( cbLanguages, SIGNAL( activated( const QString& ) ), this, SLOT( cbLanguages_activated( const QString& ) ) );
        connect( cbStyles, SIGNAL( activated( const QString& ) ), this, SLOT( cbStyles_activated( const QString& ) ) );
        connect( deMode, SIGNAL( modeChanged( Document::EolHint ) ), this, SLOT( deMode_modeChanged( Document::EolHint ) ) );
        connect( diMode, SIGNAL( modeChanged( Document::IndentHint ) ), this, SLOT( diMode_modeChanged( Document::IndentHint ) ) );
        connect( diMode, SIGNAL( indentWidthChanged( int ) ), this, SLOT( diMode_indentWidthChanged( int ) ) );
        connect( diMode, SIGNAL( tabWidthChanged( int ) ), this, SLOT( diMode_tabWidthChanged( int ) ) );
        connect( diMode, SIGNAL( convertionRequested( const DocumentPropertiesDiscover::GuessedProperties&, bool ) ), this, SLOT( diMode_convertionRequested( const DocumentPropertiesDiscover::GuessedProperties&, bool ) ) );
        connect( dpCursor, SIGNAL( positionChanged( const QPoint& ) ), this, SLOT( dpCursor_positionChanged( const QPoint& ) ) );
#endif
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

void StackedDocumentToolBar::codeEditorAbstractor_supportedLanguagesChanged( const QStringList& languages )
{
    const QString currentLanguage = cbLanguages->currentText();
    dlmLanguages->setStringList( languages );
    cbLanguages->setCurrentIndex( cbLanguages->findText( currentLanguage ) );
}

void StackedDocumentToolBar::codeEditorAbstractor_supportedStylesChanged( const QStringList& styles )
{
    const QString currentStyle = cbStyles->currentText();
    cbStyles->clear();
    cbStyles->addItems( styles );
    cbStyles->setCurrentIndex( cbStyles->findText( currentStyle ) );
}

void StackedDocumentToolBar::stacker_currentDocumentChanged( Document* document )
{
    if ( document ) {
        stacker_currentDocumentPropertyChanged( document, Document::Language );
        stacker_currentDocumentPropertyChanged( document, Document::Style );
        stacker_currentDocumentPropertyChanged( document, Document::Eol );
        stacker_currentDocumentPropertyChanged( document, Document::Indent );
        stacker_currentDocumentPropertyChanged( document, Document::IndentWidth );
        stacker_currentDocumentPropertyChanged( document, Document::TabWidth );
        stacker_currentDocumentPropertyChanged( document, Document::CursorPosition );
        stacker_currentDocumentPropertyChanged( document, Document::LineCount );
    }
    else {
        cbLanguages->setCurrentIndex( -1 );
        cbStyles->setCurrentIndex( -1 );
        dpCursor->setPosition( QPoint(), 1 );
    }
    
    cbLanguages->setEnabled( document );
    cbStyles->setEnabled( document );
    deMode->setEnabled( document );
    diMode->setEnabled( document );
    dpCursor->setEnabled( document );
}

void StackedDocumentToolBar::stacker_currentDocumentPropertyChanged( Document* document, int property )
{
    if ( document ) {
        switch ( property ) {
            case Document::Language: {
                const QString language = document->property( Document::Language ).toString();
                cbLanguages->setCurrentIndex( cbLanguages->findText( language ) );
                break;
            }
            case Document::Style: {
                const QString style = document->property( Document::Style ).toString();
                cbStyles->setCurrentIndex( cbStyles->findText( style ) );
                break;
            }
            case Document::Eol: {
                const Document::EolHint eol = Document::EolHint( document->property( Document::Eol ).toInt() );
                deMode->setMode( eol );
                break;
            }
            case Document::Indent: {
                const Document::IndentHint indent = Document::IndentHint( document->property( Document::Indent ).toInt() );
                diMode->setMode( indent );
                break;
            }
            case Document::IndentWidth: {
                const int indentWidth = document->property( Document::IndentWidth ).toInt();
                diMode->setIndentWidth( indentWidth );
                break;
            }
            case Document::TabWidth: {
                const int tabWidth = document->property( Document::TabWidth ).toInt();
                diMode->setTabWidth( tabWidth );
                break;
            }
            case Document::CursorPosition: {
                const QPoint cursorPosition = document->property( Document::CursorPosition ).toPoint();
                const int lineCount = document->property( Document::LineCount ).toInt();
                dpCursor->setPosition( cursorPosition, lineCount );
                break;
            }
        }
    }
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

void StackedDocumentToolBar::diMode_convertionRequested( const DocumentPropertiesDiscover::GuessedProperties& properties, bool askUserconfirmation )
{
    Q_ASSERT( mStacker );
    Document* document = mStacker->currentDocument();
    
    if ( document ) {
        if ( askUserconfirmation ) {
            pQueuedMessage message;
            message.message = tr( "The indentation properties have changed, apply them ?" );
            message.buttons[ QDialogButtonBox::Yes ] = QString::null;
            message.buttons[ QDialogButtonBox::No ] = QString::null;
            message.object = document;
            message.slot = "queuedMessageClicked";
            message.userData = QVariant::fromValue( properties );
            
            emit appendMessage( message );
        }
        else {
            document->convertTo( properties );
        }
    }
}

void StackedDocumentToolBar::dpCursor_positionChanged( const QPoint& pos )
{
    Q_ASSERT( mStacker );
    Document* document = mStacker->currentDocument();
    
    if ( document ) {
        document->setProperty( Document::CursorPosition, pos );
    }
}
