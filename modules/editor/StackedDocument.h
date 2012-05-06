#ifndef STACKEDDOCUMENT_H
#define STACKEDDOCUMENT_H

#include <QStackedWidget>

class QEvent;

class Document;

class StackedDocument : public QStackedWidget
{
    Q_OBJECT
    
public:
    StackedDocument( QWidget* parent = 0 );
    virtual ~StackedDocument();
    
    int	addDocument( Document* document );
    Document* currentDocument() const;
    int indexOf( Document* document ) const;
    int insertDocument( int index, Document* document );
    void removeDocument( Document* document );
    Document* document( int index ) const;

public slots:
    virtual void retranslateUi();
    void setCurrentDocument( Document* document );
    
protected:
    virtual void changeEvent( QEvent* event );

protected slots:
    void _q_documentInserted( int index, Document* document );
    void _q_currentChanged( int index );
    void _q_widgetRemoved( int index );

signals:
    void documentInserted( int index );
    void documentInserted( Document* document  );
    void currentDocumentChanged( int index );
    void currentDocumentChanged( Document* document );
    void documentAboutToBeRemoved( int index );
    void documentAboutToBeRemoved( Document* document );
    void documentRemoved( int index );
    void documentRemoved( Document* document );
};

#endif // STACKEDDOCUMENT_H
