#include "SourceHighlightQtDocument.h"

#include <QEvent>
#include <QBoxLayout>
#include <QPlainTextEdit>
#include <QIcon>
#include <QDebug>

SourceHighlightQtDocument::SourceHighlightQtDocument( QWidget* parent )
    : Document( parent ), mEditor( new QPlainTextEdit( this ) )
{
    Document::initialize();
    
    QBoxLayout* bLayout = new QBoxLayout( QBoxLayout::LeftToRight );
    bLayout->setMargin( 0 );
    bLayout->setSpacing( 9 );
    bLayout->addWidget( mEditor );
    
#if USE_MDI_AREA == 1
    QWidget* widget = new QWidget( this );
    widget->setLayout( bLayout );
    setWidget( widget );
#else
    setLayout( bLayout );
#endif

    connect( mEditor, &QPlainTextEdit::blockCountChanged, this, &SourceHighlightQtDocument::editor_blockCountChanged );
    connect( mEditor, &QPlainTextEdit::copyAvailable, this, &SourceHighlightQtDocument::editor_copyAvailable );
    connect( mEditor, &QPlainTextEdit::cursorPositionChanged, this, &SourceHighlightQtDocument::editor_cursorPositionChanged );
    connect( mEditor, &QPlainTextEdit::modificationChanged, this, &SourceHighlightQtDocument::editor_modificationChanged );
    connect( mEditor, &QPlainTextEdit::redoAvailable, this, &SourceHighlightQtDocument::editor_redoAvailable );
    connect( mEditor, &QPlainTextEdit::selectionChanged, this, &SourceHighlightQtDocument::editor_selectionChanged );
    connect( mEditor, &QPlainTextEdit::textChanged, this, &SourceHighlightQtDocument::editor_textChanged );
    connect( mEditor, &QPlainTextEdit::undoAvailable, this, &SourceHighlightQtDocument::editor_undoAvailable );
}

SourceHighlightQtDocument::~SourceHighlightQtDocument()
{
}

QVariant SourceHighlightQtDocument::property( int property ) const
{
    const QTextCursor cursor = mEditor->textCursor();
    const QTextDocument* document = mEditor->document();
    
    switch ( property ) {
        case Document::CopyAvailable:
            return !cursor.selectedText().isEmpty();
        case Document::CutAvailable:
            return !cursor.selectedText().isEmpty();
        case Document::PasteAvailable:
            return mEditor->canPaste();
        case Document::UndoAvailable:
            return document->availableUndoSteps() > 0;
        case Document::RedoAvailable:
            return document->availableRedoSteps() > 0;
        case Document::CursorPosition:
            return cursor.isNull() ? QPoint() : QPoint( cursor.positionInBlock(), cursor.blockNumber() );
        case Document::SelectionStart:
            return cursor.selectionStart();
        case Document::SelectionEnd:
            return cursor.selectionEnd();
        case Document::SelectionLength:
            return cursor.selectionEnd() -cursor.selectionStart();
        case Document::SelectedText:
            return cursor.selectedText();
        case Document::Text:
            return mEditor->toPlainText();
        case Document::ReadOnly:
            return mEditor->isReadOnly();
        case Document::State: {
            Document::StateHints state = Document::Unmodified;
            
            if ( document->isModified() ) {
                state |= Document::Modified;
            }
            
            return int( state );
        }
        case Document::SupportedLexers:
        case Document::SupportedThemes:
        
        case Document::Decoration:
        case Document::Title:
        case Document::FilePath:
        case Document::Eol:
        case Document::Indent:
        case Document::TabWidth:
        case Document::IndentWidth:
        case Document::Ruler:
        case Document::Lexer:
        case Document::LexerTheme:
        case Document::NewFile:
        case Document::LastError:
        case Document::TextEncoding:
        case Document::InitialText:
        default: {
            return mProperties.value( property, Document::property( property ) );
        }
    }
}

void SourceHighlightQtDocument::setProperty( int property, const QVariant& value )
{
    QTextCursor cursor = mEditor->textCursor();
    QTextDocument* document = mEditor->document();
    
    switch ( property ) {
        case Document::CopyAvailable:
        case Document::CutAvailable:
        case Document::PasteAvailable:
        case Document::UndoAvailable:
        case Document::RedoAvailable:
        case Document::SupportedLexers:
        case Document::SupportedThemes:
            return;
        case Document::CursorPosition:
            // todo
            break;
        case Document::SelectionStart:
            // todo
            break;
        case Document::SelectionEnd:
            // todo
            break;
        case Document::SelectionLength:
            // todo
            break;
        case Document::SelectedText:
            // todo
            break;
        case Document::Text:
            cursor.beginEditBlock();
            cursor.select( QTextCursor::Document );
            cursor.insertText( value.toString() );
            cursor.movePosition( QTextCursor::Start, QTextCursor::MoveAnchor );
            cursor.endEditBlock();
            mEditor->setTextCursor( cursor );
            break;
        case Document::InitialText:
            mEditor->setPlainText( value.toString() );
            break;
        case Document::ReadOnly:
            mEditor->setReadOnly( value.toBool() );
            break;
        case Document::State: {
            const Document::StateHints state = Document::StateHints( value.toInt() );
            document->setModified( state & Document::Modified );
            break;
        }
        
        case Document::Decoration:
        case Document::Title:
        case Document::FilePath:
        case Document::Eol:
        case Document::Indent:
        case Document::TabWidth:
        case Document::IndentWidth:
        case Document::Ruler:
        case Document::Lexer:
        case Document::LexerTheme:
        case Document::NewFile:
        case Document::LastError:
        case Document::TextEncoding:
        default: {
            if ( value.isNull() ) {
                mProperties.remove( property );
            }
            else {
                mProperties[ property ] = value;
            }
            
            break;
        }
    }
    
    Document::setProperty( property, value );
    
    emit propertyChanged( property );
    emit propertiesChanged();
}

void SourceHighlightQtDocument::clearProperties()
{
    mEditor->clear();
    mProperties.clear();
    emit propertiesChanged();
}

QMimeType SourceHighlightQtDocument::mimeTypeForLexer( const QString& lexer ) const
{
    return QMimeType();
}

QString SourceHighlightQtDocument::lexerForMimeType( const QMimeType& type ) const
{
    return QString::null;
}

void SourceHighlightQtDocument::editor_blockCountChanged( int newBlockCount )
{
    Q_UNUSED( newBlockCount );
    emit propertyChanged( Document::State );
    emit propertiesChanged();
}

void SourceHighlightQtDocument::editor_copyAvailable( bool yes )
{
    Q_UNUSED( yes );
    emit propertyChanged( Document::CopyAvailable );
    emit propertyChanged( Document::CutAvailable );
    emit propertiesChanged();
}

void SourceHighlightQtDocument::editor_cursorPositionChanged()
{
    emit propertyChanged( Document::CursorPosition );
    emit propertiesChanged();
}

void SourceHighlightQtDocument::editor_modificationChanged( bool changed )
{
    Q_UNUSED( changed );
    emit propertyChanged( Document::State );
    emit propertiesChanged();
}

void SourceHighlightQtDocument::editor_redoAvailable( bool available )
{
    Q_UNUSED( available );
    emit propertyChanged( Document::RedoAvailable );
    emit propertiesChanged();
}

void SourceHighlightQtDocument::editor_selectionChanged()
{
    //
}

void SourceHighlightQtDocument::editor_textChanged()
{
    emit propertyChanged( Document::State );
    emit propertiesChanged();
}

void SourceHighlightQtDocument::editor_undoAvailable( bool available )
{
    Q_UNUSED( available );
    emit propertyChanged( Document::UndoAvailable );
    emit propertiesChanged();
}
