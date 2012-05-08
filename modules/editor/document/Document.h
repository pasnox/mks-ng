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
#include <QMimeDatabase>
#include <QStringList>

class QEvent;
class QMetaObject;
class QTextCodec;

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
        NoRuler = 0,
        Line = 1,
        Background = 2
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
        Lexer = 17, // QString
        LexerTheme = 18, // QString
        SupportedLexers = 19, // QStringList
        SupportedThemes = 20, // QStringList
        NewFile = 21, // bool
        LastError = 22, // QString
        TextEncoding = 23, // QString
        CopyAvailable = 24, // bool
        CutAvailable = 25, // bool
        PasteAvailable = 26, // bool
        ReadOnly = 27, // bool
        InitialText = 28, // QString
        //
        UserProperty = 1000 // first user usable extension
    };
    
    Document( QWidget* parent = 0 );
    virtual ~Document();
    
    virtual QVariant property( int property ) const;
    
    // the api don't care about unsaved content or already opened content
    // it's the caller responsability to check that before
    bool open( const QString& filePath, const QString& encoding = QString::null );
    bool save( const QString& filePath = QString::null, const QString& encoding = QString::null );
    void close();
    
    static void registerDocumentAbstraction( const QMetaObject* meta );
    static void unregisterDocumentAbstraction( const QMetaObject* meta );
    static QStringList documentAbstractionKeys();
    static Document* createDocument( const QString& key, QWidget* parent = 0 );

public slots:
    virtual void retranslateUi();
    virtual void setProperty( int property, const QVariant& value );
    virtual void clearProperties() = 0;
    
protected:
    static QHash<QString, const QMetaObject*> mAbstractors;
    static QMimeDatabase mMimeDb;
    static int mDocumentCount;
    
    virtual void changeEvent( QEvent* event );
    
    virtual void initialize();
    QTextCodec* textCodec( const QString& encoding ) const;
    QIcon iconForState( Document::StateHints state ) const;
    QIcon iconForFileName( const QString& fileName ) const;
    QIcon iconForLexer( const QString& lexer ) const;
    QIcon iconForContent( const QString& content ) const;
    virtual QMimeType mimeTypeForLexer( const QString& lexer ) const = 0;
    virtual QString lexerForMimeType( const QMimeType& type ) const = 0;

signals:
    void propertyChanged( int property );
    void propertiesChanged();
};

#endif // DOCUMENT_H
