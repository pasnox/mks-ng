#ifndef SOURCEHIGHLIGHTQTDOCUMENT_H
#define SOURCEHIGHLIGHTQTDOCUMENT_H

#include "Document.h"

#include <QPointer>

class QEvent;
class QPlainTextEdit;

namespace srchiliteqt {
    class Qt4SyntaxHighlighter;
}

class SourceHighlightQtDocument : public Document
{
    Q_OBJECT
    
public:
    SourceHighlightQtDocument( CodeEditorAbstractor* codeEditorAbstractor, QWidget* parent = 0 );
    virtual ~SourceHighlightQtDocument();
    
    virtual QVariant property( int property ) const;

public slots:
    virtual void setProperty( int property, const QVariant& value );
    virtual void clearProperties();
    
protected:
    QPlainTextEdit* mEditor;
    QPointer<srchiliteqt::Qt4SyntaxHighlighter> mHighlighter;
    QHash<int, QVariant> mProperties;

protected slots:
    void editor_blockCountChanged( int newBlockCount );
    void editor_copyAvailable( bool yes );
    void editor_cursorPositionChanged();
    void editor_modificationChanged( bool changed );
    void editor_redoAvailable( bool available );
    void editor_selectionChanged();
    void editor_textChanged();
    void editor_undoAvailable( bool available );
};

#endif // SOURCEHIGHLIGHTQTDOCUMENT_H
