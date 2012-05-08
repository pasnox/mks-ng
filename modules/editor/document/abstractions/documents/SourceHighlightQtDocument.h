#ifndef SOURCEHIGHLIGHTQTDOCUMENT_H
#define SOURCEHIGHLIGHTQTDOCUMENT_H

#include "Document.h"

class QEvent;
class QPlainTextEdit;

class SourceHighlightQtDocument : public Document
{
    Q_OBJECT
    
public:
    Q_INVOKABLE SourceHighlightQtDocument( QWidget* parent = 0 );
    virtual ~SourceHighlightQtDocument();
    
    virtual QVariant property( int property ) const;

public slots:
    virtual void setProperty( int property, const QVariant& value );
    virtual void clearProperties();
    
protected:
    QPlainTextEdit* mEditor;
    QHash<int, QVariant> mProperties;
    //static QHash<QString>
    
    virtual QMimeType mimeTypeForLexer( const QString& lexer ) const;
    virtual QString lexerForMimeType( const QMimeType& type ) const;

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
