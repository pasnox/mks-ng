#ifndef DOCUMENTINDENTATION_H
#define DOCUMENTINDENTATION_H

#include <QToolButton>

#include "Document.h"

class QEvent;

class DocumentIndentation : public QToolButton
{
    Q_OBJECT
    
public:
    DocumentIndentation( QWidget* parent = 0 );
    virtual ~DocumentIndentation();
    
    void setMode( Document::IndentHint mode );
    Document::IndentHint mode() const;
    
    void setIndentWidth( int width );
    int indentWidth() const;
    
    void setTabWidth( int width );
    int tabWidth() const;

public slots:
    virtual void retranslateUi();
    void showPopup();
    
protected:
    Document::IndentHint mMode;
    int mIndentWidth;
    int mTabWidth;
    
    virtual void changeEvent( QEvent* event );
    void propertiesChanged();

protected slots:
    void popup_modeChanged( int mode );
    void popup_indentWidthChanged( int width );
    void popup_tabWidthChanged( int width );

signals:
    void modeChanged( Document::IndentHint mode );
    void indentWidthChanged( int width );
    void tabWidthChanged( int width );
    void convertionRequested( const DocumentPropertiesDiscover::GuessedProperties& properties, bool askUserconfirmation );
};

#endif // DOCUMENTINDENTATION_H
