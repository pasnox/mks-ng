#ifndef SOURCEHIGHLIGHTQTDOCUMENT_H
#define SOURCEHIGHLIGHTQTDOCUMENT_H

#include "Document.h"

#include <QPlainTextEdit>

class QEvent;

namespace srchiliteqt {
    class Qt4SyntaxHighlighter;
};

class PlainTextEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    enum Ruler {
        NoRuler = 0x0,
        LineRuler = 0x1,
        BackgroundRuler = 0x2
    };
    
    PlainTextEditor( QWidget* parent = 0 );
    
    PlainTextEditor::Ruler rulerMode() const;
    int rulerWidth() const;

public slots:
    void setRulerMode( PlainTextEditor::Ruler mode );
    void setRulerWidth( int width );

protected:
    QPalette mOriginalPalette;
    QRect mLastCaretLineRect;
    PlainTextEditor::Ruler mRulerMode;
    int mRulerWidth;
    
    virtual bool event( QEvent* event );
    virtual void paintEvent( QPaintEvent* event );
    virtual void scrollContentsBy( int dx, int dy );
    
    void paintFrame();
    QRect caretLineRect() const;

protected slots:
    void updateLine();
};

class SourceHighlightQtDocument : public Document
{
    Q_OBJECT
    
public:
    SourceHighlightQtDocument( const CodeEditorAbstractor* codeEditorAbstractor, QWidget* parent = 0 );
    virtual ~SourceHighlightQtDocument();
    
    virtual QVariant property( int property ) const;

public slots:
    virtual void setProperty( int property, const QVariant& value );
    virtual void triggerAction( int action );
    virtual void clearProperties();
    
protected:
    PlainTextEditor* mEditor;
    srchiliteqt::Qt4SyntaxHighlighter* mHighlighter;
    QHash<int, QVariant> mProperties;
    
    QPalette::ColorRole propertyColorRole( const int& property ) const;
    QVariant propertyHelper( int property, const QVariant* value );

protected slots:
    void editor_blockCountChanged( int newBlockCount );
    void editor_copyAvailable( bool yes );
    void editor_cursorPositionChanged();
    void editor_modificationChanged( bool changed );
    void editor_undoAvailable( bool available );
    void editor_redoAvailable( bool available );
    void editor_selectionChanged();
    void editor_textChanged();
};

#endif // SOURCEHIGHLIGHTQTDOCUMENT_H
