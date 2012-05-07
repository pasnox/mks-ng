#ifndef STACKEDDOCUMENT_H
#define STACKEDDOCUMENT_H

#include <QStackedWidget>
#include <QPointer>

class QEvent;

class StackedDocumentModel;
class Document;

class StackedDocument : public QStackedWidget
{
    Q_OBJECT
    
public:
    StackedDocument( QWidget* parent = 0 );
    virtual ~StackedDocument();
    
    StackedDocumentModel* model() const;
    
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
    QPointer<StackedDocumentModel> mModel;
    
    virtual void changeEvent( QEvent* event );

protected slots:
    void _q_documentInserted( int index, Document* document );
    void _q_currentChanged( int index );
    void _q_widgetRemoved( int index );

signals:
    void documentIndexInserted( int index );
    void documentInserted( Document* document  );
    void documentIndexChanged( int index );
    void documentChanged( Document* document );
    void currentDocumentIndexChanged( int index );
    void currentDocumentChanged( Document* document );
    void documentIndexAboutToBeRemoved( int index );
    void documentAboutToBeRemoved( Document* document );
    void documentIndexRemoved( int index );
    void documentRemoved( Document* document );
};

#endif // STACKEDDOCUMENT_H
