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
    
    QString documentAbstractionKey() const;
    void setDocumentAbstractionKey( const QString& key );
    
    StackedDocumentModel* model() const;
    
    Document* createDocument() const;
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
    QString mDocumentAbstractionKey;
    QPointer<StackedDocumentModel> mModel;
    
    virtual void changeEvent( QEvent* event );
    void handleDocument( Document* document );
    void unhandleDocument( Document* document );

protected slots:
    void _q_documentInserted( int index, Document* document );
    void _q_documentPropertyChanged( int property );
    void _q_documentPropertiesChanged();
    void _q_currentChanged( int index );
    void _q_widgetRemoved( int index );

signals:
    void documentIndexInserted( int index );
    void documentInserted( Document* document  );
    void documentIndexPropertyChanged( int property, int index );
    void documentPropertyChanged( int property, Document* document );
    void documentIndexPropertiesChanged( int index );
    void documentPropertiesChanged( Document* document );
    void currentDocumentIndexChanged( int index );
    void currentDocumentChanged( Document* document );
    void documentIndexAboutToBeRemoved( int index );
    void documentAboutToBeRemoved( Document* document );
    void documentIndexRemoved( int index );
    void documentRemoved( Document* document );
};

#endif // STACKEDDOCUMENT_H
