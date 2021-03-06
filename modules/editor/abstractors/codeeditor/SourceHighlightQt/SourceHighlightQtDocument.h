#ifndef SOURCEHIGHLIGHTQTDOCUMENT_H
#define SOURCEHIGHLIGHTQTDOCUMENT_H

#include "Document.h"

#include <QPlainTextEdit>

class QEvent;

class PlainTextEditor;

namespace srchiliteqt {
    class Qt4SyntaxHighlighter;
};

class SourceHighlightQtDocument : public Document
{
    Q_OBJECT
    
public:
    SourceHighlightQtDocument( QWidget* parent = 0 );
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
