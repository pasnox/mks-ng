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
    
    virtual QVariant property( Document::Property property ) const;

public slots:
    virtual void setProperty( Document::Property property, const QVariant& value );
    
protected:
    QHash<Document::Property, QVariant> mProperties;
};

#endif // SOURCEHIGHLIGHTQTDOCUMENT_H
