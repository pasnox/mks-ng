#include "SourceHighlightQtDocument.h"
#include "SourceHighlightQtDataWatcher.h"
#include "CodeEditorAbstractor.h"

#include <srchiliteqt/Qt4SyntaxHighlighter.h>

#include <QEvent>
#include <QBoxLayout>
#include <QIcon>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QFileInfo>
#include <QDebug>

#include <QStyleOptionFrameV3>
#include <QPainter>

PlainTextEditor::PlainTextEditor( QWidget* parent )
    : QPlainTextEdit( parent ),
        mOriginalPalette( palette() ),
        mRulerMode( PlainTextEditor::NoRuler ),
        mRulerWidth( 80 )
{
    setAutoFillBackground( true );
    
#if defined( HAS_QT_5 )
    connect( this, &QPlainTextEdit::cursorPositionChanged, this, &PlainTextEditor::updateLine );
#else
    connect( this, SIGNAL( cursorPositionChanged() ), this, SLOT( updateLine() ) );
#endif
}

PlainTextEditor::Ruler PlainTextEditor::rulerMode() const
{
    return mRulerMode;
}

void PlainTextEditor::setRulerMode( PlainTextEditor::Ruler mode )
{
    mRulerMode = mode;
    viewport()->update();
}

int PlainTextEditor::rulerWidth() const
{
    return mRulerWidth;
}

void PlainTextEditor::setRulerWidth( int width )
{
    mRulerWidth = width;
    viewport()->update();
}

bool PlainTextEditor::event( QEvent* event )
{
    const QKeyEvent* ke = static_cast<QKeyEvent*>( event );
    
    switch ( event->type() ) {
        case QEvent::QEvent::Paint:
            paintFrame();
            return true;
        case QEvent::KeyPress: {
            if ( ke->key() == Qt::Key_Up || ke->key() == Qt::Key_Down ) {
                updateLine();
            }
            
            break;
        }
        default:
            break;
    }
    
    const bool result = QPlainTextEdit::event( event );
    
    switch ( event->type() ) {
        case QEvent::Resize:
            updateLine();
            break;
        case QEvent::KeyPress:
            if ( ke->key() == Qt::Key_Enter || ke->key() == Qt::Key_Return ) {
                updateLine();
            }
            
            break;
        default:
            break;
    }
    
    return result;
}

void PlainTextEditor::paintEvent( QPaintEvent* event )
{
    QPainter painter( viewport() );
    
    // draw caret line
    painter.fillRect( mLastCaretLineRect, palette().color( QPalette::Link ) );
    
    // draw ruler
    /*{
        const int rulerWidth = 80;
        int x = rulerWidth *painter.fontMetrics().averageCharWidth();
        painter.setPen( palette().color( QPalette::LinkVisited ) );
        painter.drawLine( x, 0, x, viewport()->height() );
    }*/
    
    painter.end();
    
    QPlainTextEdit::paintEvent( event );
}

void PlainTextEditor::scrollContentsBy( int dx, int dy )
{
    QPlainTextEdit::scrollContentsBy( dx, dy );
    updateLine();
}

void PlainTextEditor::paintFrame()
{
    QPainter painter( this );
    QStyleOptionFrameV3 option;
    
    option.initFrom( this );
    option.palette = mOriginalPalette;
    option.rect = frameRect();
    option.frameShape = frameShape();
    
    switch ( option.frameShape ) {
        case QFrame::Box:
        case QFrame::HLine:
        case QFrame::VLine:
        case QFrame::StyledPanel:
        case QFrame::Panel:
            option.lineWidth = lineWidth();
            option.midLineWidth = midLineWidth();
            break;
        default:
            // most frame styles do not handle customized line and midline widths
            // (see updateFrameWidth()).
            option.lineWidth = frameWidth();
            break;
    }

    if ( frameShadow() == QFrame::Sunken ) {
        option.state |= QStyle::State_Sunken;
    }
    else if ( frameShadow() == QFrame::Raised ) {
        option.state |= QStyle::State_Raised;
    }

    style()->drawControl( QStyle::CE_ShapedFrame, &option, &painter, this );
}

QRect PlainTextEditor::caretLineRect() const
{
    QRect rect = cursorRect().adjusted( 0, -1, 0, 2 );
    rect.setX( 0 );
    rect.setWidth( viewport()->width() );
    return rect;
}

void PlainTextEditor::updateLine()
{
    // update last pos to remove artifact
    if ( !mLastCaretLineRect.isNull() ) {
        viewport()->update( mLastCaretLineRect );
    }
    
    // update new cursor pos
    mLastCaretLineRect = caretLineRect();
    viewport()->update( mLastCaretLineRect );
}

SourceHighlightQtDocument::SourceHighlightQtDocument( const CodeEditorAbstractor* codeEditorAbstractor, QWidget* parent )
    : Document( codeEditorAbstractor, parent ),
        mEditor( new PlainTextEditor( this ) ),
        mHighlighter( 0 )
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

#if defined( HAS_QT_5 )
    connect( mEditor, &QPlainTextEdit::blockCountChanged, this, &SourceHighlightQtDocument::editor_blockCountChanged );
    connect( mEditor, &QPlainTextEdit::copyAvailable, this, &SourceHighlightQtDocument::editor_copyAvailable );
    connect( mEditor, &QPlainTextEdit::cursorPositionChanged, this, &SourceHighlightQtDocument::editor_cursorPositionChanged );
    connect( mEditor, &QPlainTextEdit::modificationChanged, this, &SourceHighlightQtDocument::editor_modificationChanged );
    connect( mEditor, &QPlainTextEdit::undoAvailable, this, &SourceHighlightQtDocument::editor_undoAvailable );
    connect( mEditor, &QPlainTextEdit::redoAvailable, this, &SourceHighlightQtDocument::editor_redoAvailable );
    connect( mEditor, &QPlainTextEdit::selectionChanged, this, &SourceHighlightQtDocument::editor_selectionChanged );
    connect( mEditor, &QPlainTextEdit::textChanged, this, &SourceHighlightQtDocument::editor_textChanged );
#else
    connect( mEditor, SIGNAL( blockCountChanged( int ) ), this, SLOT( editor_blockCountChanged( int ) ) );
    connect( mEditor, SIGNAL( copyAvailable( bool ) ), this, SLOT( editor_copyAvailable( bool ) ) );
    connect( mEditor, SIGNAL( cursorPositionChanged() ), this, SLOT( editor_cursorPositionChanged() ) );
    connect( mEditor, SIGNAL( modificationChanged( bool ) ), this, SLOT( editor_modificationChanged( bool ) ) );
    connect( mEditor, SIGNAL( undoAvailable( bool ) ), this, SLOT( editor_undoAvailable( bool ) ) );
    connect( mEditor, SIGNAL( redoAvailable( bool ) ), this, SLOT( editor_redoAvailable( bool ) ) );
    connect( mEditor, SIGNAL( selectionChanged() ), this, SLOT( editor_selectionChanged() ) );
    connect( mEditor, SIGNAL( textChanged() ), this, SLOT( editor_textChanged() ) );
#endif
}

SourceHighlightQtDocument::~SourceHighlightQtDocument()
{
}

QVariant SourceHighlightQtDocument::property( int property ) const
{
    return const_cast<SourceHighlightQtDocument*>( this )->propertyHelper( property, 0 );
}

void SourceHighlightQtDocument::setProperty( int property, const QVariant& value )
{
    propertyHelper( property, &value );
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

QPalette::ColorRole SourceHighlightQtDocument::propertyColorRole( const int& property ) const
{
    switch ( property ) {
        case Document::Paper:
            return mEditor->viewport()->backgroundRole();
        case Document::Pen:
            return mEditor->viewport()->foregroundRole();
        case Document::SelectionBackground:
            return QPalette::Highlight;
        case Document::SelectionForeground:
            return QPalette::HighlightedText;
        case Document::CaretLineBackground:
            return QPalette::Link;
        case Document::CaretLineForeground:
            return QPalette::LinkVisited;
        default:
            return QPalette::NoRole;
    }
}

// to avoid to forget some properties in both property() / setProperty() we use one member for both.
// This may change in the futur api stabilization
QVariant SourceHighlightQtDocument::propertyHelper( int property, const QVariant* value )
{
    QTextCursor cursor = mEditor->textCursor();
    QTextDocument* document = mEditor->document();
    bool write = false;
    
    switch ( property ) {
        case Document::CopyAvailable:
        case Document::CutAvailable: {
            if ( !value ) {
                return cursor.hasSelection();
            }
            
            break;
        }
        
        case Document::PasteAvailable: {
            if ( !value ) {
                //return mEditor->canPaste(); // should use this one but it's so slow call !!!!
                return QApplication::clipboard()->mimeData()->hasText();
            }
            
            break;
        }
        
        case Document::UndoAvailable: {
            if ( !value ) {
                return document->isUndoAvailable();
            }
            
            break;
        }
        
        case Document::RedoAvailable: {
            if ( !value ) {
                return document->isRedoAvailable();
            }
            
            break;
        }
        
        case Document::CursorPosition: {
            if ( value ) {
                const QPoint pos = value->toPoint();
                const QTextBlock block = document->findBlockByLineNumber( pos.y() );
                const int position = block.position() +( pos.x() < block.length() ? pos.x() : 0 );
                cursor.setPosition( position, QTextCursor::MoveAnchor );
            }
            else {
                return cursor.isNull() ? QPoint() : QPoint( cursor.positionInBlock(), cursor.blockNumber() );
            }
            
            break;
        }
        
        case Document::SelectionStart: {
            if ( !value ) {
                return cursor.selectionStart();
            }
            
            break;
        }
        
        case Document::SelectionEnd: {
            if ( !value ) {
                return cursor.selectionEnd();
            }
            
            break;
        }
        
        case Document::SelectionLength: {
            if ( !value ) {
                return cursor.selectionEnd() -cursor.selectionStart();
            }
            
            break;
        }
        
        case Document::SelectedText: {
            if ( !value ) {
                return cursor.selectedText();
            }
            
            break;
        }
        
        case Document::Text: {
            if ( value ) {
                cursor.beginEditBlock();
                cursor.select( QTextCursor::Document );
                cursor.insertText( value->toString() );
                cursor.movePosition( QTextCursor::Start, QTextCursor::MoveAnchor );
                cursor.endEditBlock();
            }
            else {
                return mEditor->toPlainText();
            }
            
            break;
        }
        
        case Document::InitialText: {
            if ( value ) {
                mEditor->setPlainText( value->toString() );
            }
            
            break;
        }
        
        case Document::ReadOnly: {
            if ( value ) {
                mEditor->setReadOnly( value->toBool() );
                
                if ( mHighlighter ) {
                    mHighlighter->setReadOnly( mEditor->isReadOnly() );
                }
            }
            else {
                return mEditor->isReadOnly();
            }
            
            break;
        }
        
        case Document::LineCount: {
            if ( !value ) {
                return mEditor->blockCount();
            }
            
            break;
        }
        
        case Document::State: {
            if ( value ) {
                const Document::StateHints state = Document::StateHints( value->toInt() );
                document->setModified( state & Document::Modified );
            }
            else {
                Document::StateHints state = Document::Unmodified;
                
                if ( document->isModified() ) {
                    state |= Document::Modified;
                }
                
                return int( state );
            }
            
            break;
        }
        
        case Document::Language: {
            if ( value ) {
                const QString language = QString( "%1.lang" ).arg( value->toString() );
                
                if ( mHighlighter && mHighlighter->getLangFile() == language ) {
                    break;
                }
                
                if ( !language.isEmpty() && mCodeEditorAbstractor->supportedLanguages().contains( value->toString() ) ) {
                    const QString style = mHighlighter ? mHighlighter->getFormattingStyle() : "default.style";
                    
                    if ( mHighlighter ) {
                        mHighlighter->deleteLater();
                        mHighlighter = 0;
                    }
                    
                    mHighlighter = new srchiliteqt::Qt4SyntaxHighlighter( document );
                    mHighlighter->setReadOnly( mEditor->isReadOnly() );
                    mHighlighter->init( language, style );
                    
                    emit propertyChanged( Document::Style );
                }
            }
            else {
                return mHighlighter ? QFileInfo( mHighlighter->getLangFile() ).baseName() : QString::null;
            }
            
            break;
        }
        
        case Document::Style: {
            if ( value ) {
                if ( mHighlighter ) {
                    mHighlighter->setFormattingStyle( QString( "%1.style" ).arg( value->toString() ) );
                }
            }
            else {
                return mHighlighter ? QFileInfo( mHighlighter->getFormattingStyle() ).baseName() : QString::null;
            }
            
            break;
        }
        
        case Document::Font: {
            if ( value ) {
                document->setDefaultFont( value->value<QFont>() );
            }
            else {
                return document->defaultFont();
            }
            
            break;
        }
        
        case Document::Paper:
        case Document::Pen:
        case Document::SelectionBackground:
        case Document::SelectionForeground:
        case Document::CaretLineBackground:
        case Document::CaretLineForeground: {
            if ( value ) {
                QPalette pal = mEditor->palette();
                pal.setColor( propertyColorRole( property ), value->value<QColor>() );
                mEditor->setPalette( pal );
            }
            else {
                return mEditor->palette().color( propertyColorRole( property ) );
            }
            
            break;
        }
        
        case Document::LineWrap: {
            if ( value ) {
                mEditor->setLineWrapMode( value->toInt() == Document::NoWrap ? QPlainTextEdit::NoWrap : QPlainTextEdit::WidgetWidth );
            }
            else {
                return mEditor->lineWrapMode() == QPlainTextEdit::NoWrap ? Document::NoWrap : Document::WidthWrap;
            }
            
            break;
        }
        
        case Document::IndentWidth: {
            if ( value ) {
                document->setIndentWidth( value->toInt() *QFontMetrics( mEditor->font() ).averageCharWidth() );
            }
            else {
                return document->indentWidth() /QFontMetrics( mEditor->font() ).averageCharWidth();
            }
            
            break;
        }
        
        case Document::TabWidth: {
            if ( value ) {
                mEditor->setTabStopWidth( value->toInt() *QFontMetrics( mEditor->font() ).averageCharWidth() );
            }
            else {
                return mEditor->tabStopWidth() /QFontMetrics( mEditor->font() ).averageCharWidth();
            }
            
            break;
        }
        
        case Document::Ruler: {
            if ( value ) {
                switch ( value->toInt() ) {
                    case Document::NoRuler:
                        mEditor->setRulerMode( PlainTextEditor::NoRuler );
                        break;
                    case Document::Line:
                        mEditor->setRulerMode( PlainTextEditor::LineRuler );
                        break;
                    case Document::Background:
                        mEditor->setRulerMode( PlainTextEditor::BackgroundRuler );
                        break;
                }
            }
            else {
                switch ( mEditor->rulerMode() ) {
                    case PlainTextEditor::NoRuler:
                        return Document::NoRuler;
                    case PlainTextEditor::LineRuler:
                        return Document::Line;
                    case PlainTextEditor::BackgroundRuler:
                        return Document::Background;
                }
            }
            
            break;
        }
        
        case Document::LineNumberMargin:
        case Document::FoldMargin:
        case Document::SymbolMargin:
        case Document::ChangeMargin: {
            if ( value ) {
                //propertyMargin( property )->setVisible( value->toBool() );
            }
            else {
                //return propertyMargin( property )->isVisible();
                return false;
            }
            
            break;
        }
        
        case Document::Eol:
        case Document::Decoration:
        case Document::FilePath:
        case Document::Title:
        case Document::NewFile:
        case Document::Indent:
        case Document::TextEncoding:
        case Document::MimeType:
        case Document::LastError:
        default: {
            if ( value ) {
                write = true;
            }
            else {
                if ( mProperties.contains( property ) ) {
                    return mProperties.value( property );
                }
                
                return Document::property( property );
            }
            
            break;
        }
    }
    
    // mEditor->setCursorWidth ( int width )
    
    if ( !value ) {
        return QVariant();
    }
    
    if ( write ) {
        if ( value->isNull() ) {
            mProperties.remove( property );
        }
        else {
            mProperties[ property ] = *value;
        }
    }
    
    if ( cursor != mEditor->textCursor() && property != Document::InitialText ) {
        mEditor->setTextCursor( cursor );
    }
    
    switch ( property ) {
        // special case, they are emited by their internal signals
        case Document::LineCount:
        case Document::CopyAvailable:
        case Document::CutAvailable:
        case Document::CursorPosition:
        case Document::State:
        case Document::UndoAvailable:
        case Document::RedoAvailable:
        case Document::SelectedText:
        case Document::Text:
            break;
        // let inform about change
        default:
            emit propertyChanged( property );
            emit propertiesChanged();
            Document::setProperty( property, *value );
            break;
    }
    
    return QVariant();
}

void SourceHighlightQtDocument::editor_blockCountChanged( int newBlockCount )
{
    Q_UNUSED( newBlockCount );
    emit propertyChanged( Document::LineCount );
    emit propertiesChanged();
    Document::setProperty( Document::LineCount, QVariant() );
}

void SourceHighlightQtDocument::editor_copyAvailable( bool yes )
{
    Q_UNUSED( yes );
    emit propertyChanged( Document::CopyAvailable );
    emit propertyChanged( Document::CutAvailable );
    emit propertiesChanged();
    Document::setProperty( Document::CopyAvailable, QVariant() );
    Document::setProperty( Document::CutAvailable, QVariant() );
}

void SourceHighlightQtDocument::editor_cursorPositionChanged()
{
    emit propertyChanged( Document::CursorPosition );
    emit propertiesChanged();
    Document::setProperty( Document::CursorPosition, QVariant() );
}

void SourceHighlightQtDocument::editor_modificationChanged( bool changed )
{
    Q_UNUSED( changed );
    emit propertyChanged( Document::State );
    emit propertiesChanged();
    Document::setProperty( Document::State, QVariant() );
}

void SourceHighlightQtDocument::editor_undoAvailable( bool available )
{
    Q_UNUSED( available );
    emit propertyChanged( Document::UndoAvailable );
    emit propertiesChanged();
    Document::setProperty( Document::UndoAvailable, QVariant() );
}

void SourceHighlightQtDocument::editor_redoAvailable( bool available )
{
    Q_UNUSED( available );
    emit propertyChanged( Document::RedoAvailable );
    emit propertiesChanged();
    Document::setProperty( Document::RedoAvailable, QVariant() );
}

void SourceHighlightQtDocument::editor_selectionChanged()
{
    emit propertyChanged( Document::SelectedText );
    emit propertiesChanged();
    Document::setProperty( Document::SelectedText, QVariant() );
}

void SourceHighlightQtDocument::editor_textChanged()
{
    emit propertyChanged( Document::Text );
    emit propertiesChanged();
    Document::setProperty( Document::Text, QVariant() );
}
