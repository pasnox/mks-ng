#ifndef DOCUMENT_H
#define DOCUMENT_H

#define USE_MDI_AREA 1

#if USE_MDI_AREA == 1
#define BaseWidget QMdiSubWindow
#include <QMdiSubWindow>
#else
#define BaseWidget QWidget
#include <QWidget>
#endif

#include <QHash>
#include <QStringList>

class QEvent;
class QMetaObject;

class Document : public BaseWidget
{
    Q_OBJECT
    Q_FLAGS( StateHint StateHints )
    
public:
    enum StateHint {
        Unmodified = 0x0,
        Modified = 0x1,
        ExternallyModified = 0x2,
        ExternallyDeleted = 0x4
    };
    
    Q_DECLARE_FLAGS( StateHints, StateHint )
    
    enum EolHint {
        Unix = 0,
        Windows = 1,
        Mac = 2
    };
    
    enum IndentHint {
        Spaces = 0,
        Tabs = 1
    };
    
    enum RulerHint {
        Line = 0,
        Background = 1
    };
    
    enum Property {
        Decoration = 0, // QIcon
        Title = 1, // QString
        FileName = 2, // QString
        State = 3, // StateHints
        UndoAvailable = 4, // bool
        RedoAvailable = 5, // bool
        CursorPosition = 6, // QPoint
        Eol = 7, // EolHint
        Indent = 8, // IndentHint
        TabWidth = 9, // int
        IndentWidth = 10, // int
        SelectionStart = 11, // int
        SelectionEnd = 12, // int
        SelectionLength = 13, // int
        SelectedText = 14, // QString
        Text = 15, // QString
        Ruler = 16 // RulerHint
    };
    
    Document( QWidget* parent = 0 );
    virtual ~Document();
    
    virtual QVariant property( Document::Property property ) const = 0;
    
    static void registerDocumentAbstraction( const QMetaObject* meta );
    static void unregisterDocumentAbstraction( const QMetaObject* meta );
    static QStringList documentAbstractionKeys();
    static Document* createDocument( const QString& key, QWidget* parent = 0 );

public slots:
    virtual void retranslateUi();
    virtual void setProperty( Document::Property property, const QVariant& value ) = 0;
    
public:
    static QHash<QString, const QMetaObject*> mAbstractors;
    
    virtual void changeEvent( QEvent* event );

signals:
    void propertyChanged( Document::Property property );
    void propertiesChanged();
};

#endif // DOCUMENT_H
