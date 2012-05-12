#ifndef STACKEDDOCUMENTTOOLBAR_H
#define STACKEDDOCUMENTTOOLBAR_H

#include <QToolBar>

#include "Document.h"

class QEvent;
class QComboBox;

class StackedDocument;
class DocumentEol;
class DocumentIndentation;
class DocumentPosition;

class StackedDocumentToolBar : public QToolBar
{
    Q_OBJECT
    
public:
    StackedDocumentToolBar( QWidget* parent = 0 );
    virtual ~StackedDocumentToolBar();
    
    void setStackedDocument( StackedDocument* stacker );

public slots:
    virtual void retranslateUi();
    
protected:
    StackedDocument* mStacker;
    QComboBox* cbLanguages;
    QComboBox* cbStyles;
    DocumentEol* deMode;
    DocumentIndentation* diMode;
    DocumentPosition* dpCursor;
    
    virtual void changeEvent( QEvent* event );

protected slots:
    void stacker_currentDocumentChanged( Document* document );
    void stacker_currentDocumentPropertyChanged( Document* document, int property );
    void cbLanguages_activated( const QString& language );
    void cbStyles_activated( const QString& style );
    void deMode_modeChanged( Document::EolHint mode );
    void diMode_modeChanged( Document::IndentHint mode );
    void diMode_indentWidthChanged( int width );
    void diMode_tabWidthChanged( int width );
    void dpCursor_positionChanged( const QPoint& pos );
};

#endif // STACKEDDOCUMENTTOOLBAR_H
