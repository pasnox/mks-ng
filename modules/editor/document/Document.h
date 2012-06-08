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
#include <QDialogButtonBox>

#include <DocumentPropertiesDiscover.h>

class pQueuedMessage;

class QEvent;
class QMetaObject;
class QTextCodec;

class CodeEditorAbstractor;
class CodeEditorMimeType;

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
        Unix = DocumentPropertiesDiscover::UnixEol,
        DOS = DocumentPropertiesDiscover::DOSEol,
        MacOS = DocumentPropertiesDiscover::MacOSEol
    };
    
    enum IndentHint {
        Spaces = DocumentPropertiesDiscover::SpacesIndent,
        Tabs = DocumentPropertiesDiscover::TabsIndent,
        TabsAndSpaces = DocumentPropertiesDiscover::MixedIndent
    };
    
    enum RulerHint {
        NoRuler = 0,
        Line = 1,
        Background = 2
    };
    
    enum WrapHint {
        NoWrap = 0,
        WidthWrap = 1
    };
    
    enum Property {
        Decoration = 0, // QIcon
        Title = 1, // QString
        FilePath = 2, // QString
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
        Ruler = 16, // RulerHint
        Language = 17, // QString
        Style = 18, // QString
        LineCount = 19, // int
        NewFile = 21, // bool
        LastError = 22, // QString
        TextEncoding = 23, // QString
        CopyAvailable = 24, // bool
        CutAvailable = 25, // bool
        PasteAvailable = 26, // bool
        ReadOnly = 27, // bool
        InitialText = 28, // QString
        LineWrap = 29, // WrapHint
        LineNumberMargin = 30, // bool
        FoldMargin = 31, // bool
        SymbolMargin = 32, // bool
        ChangeMargin = 33, // bool
        Font = 34, // QFont
        Paper = 35, // QColor
        Pen = 36, // QColor
        SelectionBackground = 37, // QColor
        SelectionForeground = 38, // QColor
        CaretLineBackground = 39, // QColor
        CaretLineForeground = 40, // QColor
        MimeType = 41, // QString
        //
        UserProperty = 1000 // first user usable extension
    };
    
    enum Action {
        Undo = 0,
        Redo = 1,
        Copy = 2,
        Cut = 3,
        Paste = 4
    };
    
    Document( const CodeEditorAbstractor* codeEditorAbstractor, QWidget* parent = 0 );
    virtual ~Document();
    
    const CodeEditorMimeType& mimeTypeDB() const;
    QString mimeType() const;
    
    virtual QVariant property( int property ) const;
    virtual void applyApplicationSettings( const DocumentPropertiesDiscover::GuessedProperties& properties = DocumentPropertiesDiscover::GuessedProperties::null );
    
    // the api don't care about unsaved content or already opened content
    // it's the caller responsability to check that before
    bool open( const QString& filePath, const QString& encoding = QString::null, bool readOnly = false );
    bool save( const QString& filePath = QString::null, const QString& encoding = QString::null );
    bool reload();
    void close();
    void convertTo( const DocumentPropertiesDiscover::GuessedProperties& properties );

public slots:
    virtual void retranslateUi();
    virtual void setProperty( int property, const QVariant& value );
    virtual void triggerAction( int action ) = 0;
    virtual void clearProperties() = 0;
    virtual void queuedMessageClicked( QDialogButtonBox::StandardButton button, const pQueuedMessage& message );
    
protected:
    const CodeEditorAbstractor* mCodeEditorAbstractor;
    static int mDocumentCount;
    
    virtual void changeEvent( QEvent* event );
    
    virtual void initialize();
    
    QTextCodec* textCodec( const QString& encoding = QString::null ) const;
    bool fileContent( QString& content, const QString& filePath, const QString& encoding = QString::null );
    
signals:
    void propertyChanged( int property );
    void propertiesChanged();
};

typedef QList<Document*> DocumentList;

#endif // DOCUMENT_H
