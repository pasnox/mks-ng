#include "QodeEditDocument.h"
#include "QodeEditDataWatcher.h"
#include "CodeEditorAbstractor.h"

#include <editor/CodeEditor.h>

#include <QEvent>
#include <QBoxLayout>
#include <QIcon>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QFileInfo>
#include <QDebug>

QodeEditDocument::QodeEditDocument( QWidget* parent )
    : Document( parent ),
        mEditor( new CodeEditor( this ) )/*,
        mHighlighter( 0 )*/
{
    mEditor->setMarginStacker( new MarginStacker );
    mEditor->marginStacker()->setVisible( MarginStacker::LineSpacing );
    
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
    connect( mEditor, &QPlainTextEdit::blockCountChanged, this, &QodeEditDocument::editor_blockCountChanged );
    connect( mEditor, &QPlainTextEdit::copyAvailable, this, &QodeEditDocument::editor_copyAvailable );
    connect( mEditor, &QPlainTextEdit::cursorPositionChanged, this, &QodeEditDocument::editor_cursorPositionChanged );
    connect( mEditor, &QPlainTextEdit::modificationChanged, this, &QodeEditDocument::editor_modificationChanged );
    connect( mEditor, &QPlainTextEdit::undoAvailable, this, &QodeEditDocument::editor_undoAvailable );
    connect( mEditor, &QPlainTextEdit::redoAvailable, this, &QodeEditDocument::editor_redoAvailable );
    connect( mEditor, &QPlainTextEdit::selectionChanged, this, &QodeEditDocument::editor_selectionChanged );
    connect( mEditor, &QPlainTextEdit::textChanged, this, &QodeEditDocument::editor_textChanged );
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

QodeEditDocument::~QodeEditDocument()
{
}

QVariant QodeEditDocument::property( int property ) const
{
    return const_cast<QodeEditDocument*>( this )->propertyHelper( property, 0 );
}

void QodeEditDocument::setProperty( int property, const QVariant& value )
{
    propertyHelper( property, &value );
}

void QodeEditDocument::triggerAction( int action )
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

void QodeEditDocument::clearProperties()
{
    mEditor->clear();
    mProperties.clear();
    emit propertiesChanged();
}

MarginStacker::Type QodeEditDocument::propertyMargin( const int& property ) const
{
    switch ( property ) {
        case Document::SymbolMargin:
            return MarginStacker::LineBookmark;
        case Document::LineNumberMargin:
            return MarginStacker::LineNumber;
        case Document::FoldMargin:
            return MarginStacker::LineFold;
        case Document::ChangeMargin:
            return MarginStacker::LineRevision;
    }
    
    return MarginStacker::Invalid;
}

// to avoid to forget some properties in both property() / setProperty() we use one member for both.
// This may change in the futur api stabilization
QVariant QodeEditDocument::propertyHelper( int property, const QVariant* value )
{
    const QTextCursor currentCursor = mEditor->textCursor();
    QTextCursor cursor = currentCursor;
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
                mEditor->setCursorPosition( value->toPoint() );
            }
            else {
                return mEditor->cursorPosition();
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
                mEditor->setText( value->toString() );
            }
            else {
                return mEditor->text();
            }
            
            break;
        }
        
        case Document::InitialText: {
            if ( value ) {
                mEditor->setInitialText( value->toString() );
            }
            
            break;
        }
        
        case Document::ReadOnly: {
            if ( value ) {
                mEditor->setReadOnly( value->toBool() );
                
                /*if ( mHighlighter ) {
                    mHighlighter->setReadOnly( mEditor->isReadOnly() );
                }*/
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
                const bool modified = ( state & Document::Modified ) != 0;
                
                if ( document->isModified() != modified ) {
                    document->setModified( modified );
                }
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
                /*const QString language = QString( "%1.lang" ).arg( value->toString() );
                
                if ( mHighlighter && mHighlighter->getLangFile() == language ) {
                    break;
                }
                
                if ( !language.isEmpty() && Abstractors::codeEditor()->supportedLanguages().contains( value->toString() ) ) {
                    const QString style = mHighlighter ? mHighlighter->getFormattingStyle() : "default.style";
                    
                    if ( mHighlighter ) {
                        mHighlighter->deleteLater();
                        mHighlighter = 0;
                    }
                    
                    mHighlighter = new srchiliteqt::Qt4SyntaxHighlighter( document );
                    mHighlighter->setReadOnly( mEditor->isReadOnly() );
                    mHighlighter->init( language, style );
                    
                    emit propertyChanged( Document::Style );
                }*/
            }
            else {
                //return mHighlighter ? QFileInfo( mHighlighter->getLangFile() ).baseName() : QString::null;
                return QString();
            }
            
            break;
        }
        
        case Document::Style: {
            if ( value ) {
                /*if ( mHighlighter ) {
                    mHighlighter->setFormattingStyle( QString( "%1.style" ).arg( value->toString() ) );
                }*/
            }
            else {
                //return mHighlighter ? QFileInfo( mHighlighter->getFormattingStyle() ).baseName() : QString::null;
                return QString();
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
        
        case Document::Paper: {
            if ( value ) {
                mEditor->setPaper( value->value<QColor>() );
            }
            else {
                return mEditor->paper().color();
            }
            
            break;
        }
        
        case Document::Pen: {
            if ( value ) {
                mEditor->setPen( value->value<QColor>() );
            }
            else {
                return mEditor->pen().color();
            }
            
            break;
        }
        
        case Document::SelectionBackground: {
            if ( value ) {
                mEditor->setSelectionBackground( value->value<QColor>() );
            }
            else {
                return mEditor->selectionBackground().color();
            }
            
            break;
        }
        
        case Document::SelectionForeground: {
            if ( value ) {
                mEditor->setSelectionForeground( value->value<QColor>() );
            }
            else {
                return mEditor->selectionForeground().color();
            }
            
            break;
        }
        
        case Document::CaretLineBackground: {
            if ( value ) {
                mEditor->setCaretLineBackground( value->value<QColor>() );
            }
            else {
                return mEditor->caretLineBackground().color();
            }
            
            break;
        }
        
        case Document::CaretLineForeground: {
            if ( value ) {
                mEditor->setCaretLineForeground( value->value<QColor>() );
            }
            else {
                return mEditor->caretLineForeground().color();
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
                        mEditor->setRulerMode( CodeEditor::NoRuler );
                        break;
                    case Document::Line:
                        mEditor->setRulerMode( CodeEditor::LineRuler );
                        break;
                    case Document::Background:
                        mEditor->setRulerMode( CodeEditor::BackgroundRuler );
                        break;
                }
            }
            else {
                switch ( mEditor->rulerMode() ) {
                    case CodeEditor::NoRuler:
                        return Document::NoRuler;
                    case CodeEditor::LineRuler:
                        return Document::Line;
                    case CodeEditor::BackgroundRuler:
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
                mEditor->marginStacker()->setVisible( propertyMargin( property ), value->toBool() );
            }
            else {
                return mEditor->marginStacker()->isVisible( propertyMargin( property ) );
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
    
    if ( cursor != currentCursor && property != Document::InitialText ) {
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

void QodeEditDocument::editor_blockCountChanged( int newBlockCount )
{
    Q_UNUSED( newBlockCount );
    emit propertyChanged( Document::LineCount );
    emit propertiesChanged();
    Document::setProperty( Document::LineCount, QVariant() );
}

void QodeEditDocument::editor_copyAvailable( bool yes )
{
    Q_UNUSED( yes );
    emit propertyChanged( Document::CopyAvailable );
    emit propertyChanged( Document::CutAvailable );
    emit propertiesChanged();
    Document::setProperty( Document::CopyAvailable, QVariant() );
    Document::setProperty( Document::CutAvailable, QVariant() );
}

void QodeEditDocument::editor_cursorPositionChanged()
{
    emit propertyChanged( Document::CursorPosition );
    emit propertiesChanged();
    Document::setProperty( Document::CursorPosition, QVariant() );
}

void QodeEditDocument::editor_modificationChanged( bool changed )
{
    Q_UNUSED( changed );
    emit propertyChanged( Document::State );
    emit propertiesChanged();
    Document::setProperty( Document::State, QVariant() );
}

void QodeEditDocument::editor_undoAvailable( bool available )
{
    Q_UNUSED( available );
    emit propertyChanged( Document::UndoAvailable );
    emit propertiesChanged();
    Document::setProperty( Document::UndoAvailable, QVariant() );
}

void QodeEditDocument::editor_redoAvailable( bool available )
{
    Q_UNUSED( available );
    emit propertyChanged( Document::RedoAvailable );
    emit propertiesChanged();
    Document::setProperty( Document::RedoAvailable, QVariant() );
}

void QodeEditDocument::editor_selectionChanged()
{
    emit propertyChanged( Document::SelectedText );
    emit propertiesChanged();
    Document::setProperty( Document::SelectedText, QVariant() );
}

void QodeEditDocument::editor_textChanged()
{
    emit propertyChanged( Document::Text );
    emit propertiesChanged();
    Document::setProperty( Document::Text, QVariant() );
}
