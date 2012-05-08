#ifndef SOURCEHIGHLIGHTQTDOCUMENT_H
#define SOURCEHIGHLIGHTQTDOCUMENT_H

#include "Document.h"

class QEvent;

class SourceHighlightQtDocument : public Document
{
    Q_OBJECT
    
public:
    Q_INVOKABLE SourceHighlightQtDocument( QWidget* parent = 0 );
    virtual ~SourceHighlightQtDocument();
    
    virtual QVariant property( int property ) const;

public slots:
    virtual void setProperty( int property, const QVariant& value );
    
protected:
    QHash<int, QVariant> mProperties;
};

#endif // SOURCEHIGHLIGHTQTDOCUMENT_H
