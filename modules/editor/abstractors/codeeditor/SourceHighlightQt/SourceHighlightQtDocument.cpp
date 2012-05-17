#include "SourceHighlightQtDocument.h"
#include "SourceHighlightQtDataWatcher.h"

#include <srchiliteqt/Qt4SyntaxHighlighter.h>

#include <QEvent>
#include <QBoxLayout>
#include <QPlainTextEdit>
#include <QIcon>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QDebug>

SourceHighlightQtDocument::SourceHighlightQtDocument( const CodeEditorAbstractor* codeEditorAbstractor, QWidget* parent )
    : Document( codeEditorAbstractor, parent ),
        mEditor( new QPlainTextEdit( this ) ),
        mHighlighter( new srchiliteqt::Qt4SyntaxHighlighter( mEditor->document() ) )
{
    Document::initialize();
    
    mHighlighter->init( "default.lang", "default.style" );
    
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
            return cursor.hasSelection();
        case Document::CutAvailable:
            return cursor.hasSelection();
        case Document::PasteAvailable:
            //return mEditor->canPaste(); // should use this one but it's so slow call !!!!
            return QApplication::clipboard()->mimeData()->hasText();
        case Document::UndoAvailable:
            return document->isUndoAvailable();
        case Document::RedoAvailable:
            return document->isRedoAvailable();
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
        case Document::LineCount:
            return mEditor->blockCount();
        case Document::State: {
            Document::StateHints state = Document::Unmodified;
            if ( document->isModified() ) {
                state |= Document::Modified;
            }
            return int( state );
        }
        case Document::Language:
            return mHighlighter->getLangFile();
        case Document::Style:
            return mHighlighter->getFormattingStyle();
        
        case Document::Decoration:
        case Document::Title:
        case Document::FilePath:
        case Document::Eol:
        case Document::Indent:
        case Document::TabWidth:
        case Document::IndentWidth:
        case Document::Ruler:
        case Document::NewFile:
        case Document::LastError:
        case Document::TextEncoding:
        case Document::InitialText:
        case Document::LineNumberMargin:
        case Document::FoldMargin:
        case Document::SymbolMargin:
        case Document::ChangeMargin:
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
        case Document::LineCount:
        case Document::CopyAvailable:
        case Document::CutAvailable:
        case Document::PasteAvailable:
        case Document::UndoAvailable:
        case Document::RedoAvailable:
            return;
        case Document::CursorPosition: {
            const QPoint pos = value.toPoint();
            QTextBlock block = document->findBlockByLineNumber( pos.y() );
            int position = block.position();
            
            if ( pos.x() < block.length() ) {
                position += pos.x();
            }
            
            cursor.setPosition( position, QTextCursor::MoveAnchor );
            break;
        }
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
            break;
        case Document::InitialText:
            mEditor->setPlainText( value.toString() );
            break;
        case Document::ReadOnly:
            mEditor->setReadOnly( value.toBool() );
            
            if ( mHighlighter ) {
                mHighlighter->setReadOnly( value.toBool() );
            }
            
            break;
        case Document::State: {
            const Document::StateHints state = Document::StateHints( value.toInt() );
            document->setModified( state & Document::Modified );
            break;
        }
        case Document::LineWrap:
            mEditor->setLineWrapMode( value.toInt() == Document::NoWrap ? QPlainTextEdit::NoWrap : QPlainTextEdit::WidgetWidth );
            break;
        case Document::LineNumberMargin:
            break;
        case Document::FoldMargin:
            break;
        case Document::SymbolMargin:
            break;
        case Document::ChangeMargin:
            break;
        case Document::Language: {
            if ( mHighlighter && mHighlighter->getLangFile() == value.toString() ) {
                return;
            }
            
            const QString style = this->property( Document::Style ).toString();
            mHighlighter->deleteLater();
            mHighlighter = new srchiliteqt::Qt4SyntaxHighlighter( mEditor->document() );
            mHighlighter->init( value.toString(), style );
            break;
        }
        case Document::Style:
            if ( mHighlighter ) {
                mHighlighter->setFormattingStyle( value.toString() );
            }
            
            break;
        
        case Document::TabWidth:
            mEditor->setTabStopWidth( value.toInt() *QFontMetrics( mEditor->font() ).averageCharWidth() );
        case Document::Eol:
            //document->setModified( true );
        case Document::Indent:
        case Document::IndentWidth:
        case Document::Ruler:
        
        case Document::Decoration:
        case Document::Title:
        case Document::FilePath:
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
    
    if ( cursor != mEditor->textCursor() && property != Document::InitialText ) {
        mEditor->setTextCursor( cursor );
    }
    
    Document::setProperty( property, value );
    
    emit propertyChanged( property );
    emit propertiesChanged();
}

void SourceHighlightQtDocument::triggerAction( int action )
{
    switch ( action ) {
        case Document::Undo:
            mEditor->undo();
            return;
        case Document::Redo:
            mEditor->redo();
            return;
        case Document::Copy:
            mEditor->copy();
            return;
        case Document::Cut:
            mEditor->cut();
            return;
        case Document::Paste:
            mEditor->paste();
            return;
    }
}

void SourceHighlightQtDocument::clearProperties()
{
    mEditor->clear();
    mProperties.clear();
    emit propertiesChanged();
}

void SourceHighlightQtDocument::editor_blockCountChanged( int newBlockCount )
{
    Q_UNUSED( newBlockCount );
    emit propertyChanged( Document::LineCount );
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
    emit propertyChanged( Document::SelectedText );
    emit propertiesChanged();
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
