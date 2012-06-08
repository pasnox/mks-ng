#ifndef DOCUMENTINDENTATIONPOPUP_H
#define DOCUMENTINDENTATIONPOPUP_H

#include <QFrame>

#include "Document.h"

class Ui_DocumentIndentationPopup;
class QEvent;

class DocumentIndentationPopup : public QFrame
{
    Q_OBJECT

public:
    DocumentIndentationPopup( QWidget* parent = 0 );
    virtual ~DocumentIndentationPopup();
    
    void setMode( Document::IndentHint mode );
    void setIndentWidth( int width );
    void setTabWidth( int width );

public slots:
    virtual void retranslateUi();
    
protected:
    Ui_DocumentIndentationPopup* ui;
    
    virtual void changeEvent( QEvent* event );
    virtual void showEvent( QShowEvent* event );
    virtual void closeEvent( QCloseEvent* event );

protected slots:
    void propertiesChanged();
    void on_pbConvertDocument_clicked();

signals:
    void modeChanged( int mode );
    void indentWidthChanged( int width );
    void tabWidthChanged( int width );
    void convertionRequested( const DocumentPropertiesDiscover::GuessedProperties& properties, bool askUserconfirmation );
};

#endif // DOCUMENTINDENTATIONPOPUP_H
