#include "Document.h"
#include "Abstractors.h"
#include "CodeEditorAbstractor.h"
#include "ApplicationSettings.h"

#include <QEvent>
#include <QMetaObject>
#include <QTextCodec>
#include <QVariant>
#include <QFile>
#include <QFileInfo>
#include <QIcon>
#include <QDateTime>
#include <QDebug>

#include <FreshGui/pQueuedMessageWidget>

int Document::mDocumentCount = 0;

Document::Document( QWidget* parent )
    : QWidget( parent )
{
}

Document::~Document()
{
}

const CodeEditorMimeType& Document::mimeTypeDB() const
{
    return Abstractors::codeEditor()->mimeTypeDB();
}

QString Document::mimeType() const
{
    return property( Document::MimeType ).toString();
}

void Document::retranslateUi()
{
    // do your custom retranslate here
}

void Document::changeEvent( QEvent* event )
{
    QWidget::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}

QVariant Document::property( int property ) const
{
    // handle default properties
    switch ( property ) {
        case Document::Decoration: {
            const Document::StateHints state = Document::StateHints( this->property( Document::State ).toInt() );
            
            if ( state != Document::Unmodified ) {
                return mimeTypeDB().iconForState( state );
            }
            
            const QString filePath = this->property( Document::FilePath ).toString();
            
            if ( !filePath.isEmpty() ) {
                return mimeTypeDB().iconForFileName( filePath );
            }
            
            const QString language = this->property( Document::Language ).toString();
            
            if ( !language.isEmpty() ) {
                return mimeTypeDB().iconForLanguage( language );
            }
            
            const QString text = this->property( Document::Text ).toString();
            
            if ( !text.isEmpty() ) {
                return mimeTypeDB().iconForContent( textCodec()->fromUnicode( text ) );
            }
            
            const bool newFile = this->property( Document::NewFile ).toBool();
            const QString type = mimeType();
            
            return QIcon::fromTheme( newFile ? "document-new" : mimeTypeDB().defaultMimeTypeIconName( type ) );
        }
        case Document::Title: {
            const QString filePath = this->property( Document::FilePath ).toString();
            return filePath.isEmpty() ? QString::null : QFileInfo( filePath ).fileName();
        }
        default:
            qWarning( "%s: Unhandled property %i", Q_FUNC_INFO, property );
            return QVariant();
    }
}

void Document::setProperty( int property, const QVariant& value )
{
    Q_UNUSED( value );
    
    if ( property != Document::LastError ) {
        setProperty( Document::LastError, QVariant() );
    }
    
    // update document decoration if the state of the document change
    if ( property == Document::State ) {
        setProperty( Document::Decoration, Document::property( Document::Decoration ) );
    }
    
    // update document title / if the filepath of the document change
    if ( property == Document::FilePath ) {
        setProperty( Document::Title, Document::property( Document::Title ) );
        
        /*if ( this->property( Document::Language ).isNull() && !value.toString().isEmpty() ) {
            emit propertyChanged( Document::Language );
            emit propertiesChanged();
        }*/
    }
}

void Document::queuedMessageClicked( QDialogButtonBox::StandardButton button, const pQueuedMessage& message )
{
    if ( message.userData.canConvert<DocumentPropertiesDiscover::GuessedProperties>() ) {
        if ( button == QDialogButtonBox::Yes ) {
            const DocumentPropertiesDiscover::GuessedProperties properties = message.userData.value<DocumentPropertiesDiscover::GuessedProperties>();
            convertTo( properties );
        }
    }
}

void Document::applyApplicationSettings( const DocumentPropertiesDiscover::GuessedProperties& properties )
{
    const ApplicationSettings& settings = Abstractors::applicationSettings();
    
    if ( properties != DocumentPropertiesDiscover::GuessedProperties::null ) {
        setProperty( Document::Eol, properties.eol );
        setProperty( Document::Indent, properties.indent );
        setProperty( Document::IndentWidth, properties.indentWidth );
        setProperty( Document::TabWidth, properties.tabWidth );
        setProperty( Document::TextEncoding, settings.editor.documents.codec );
    }
    
    setProperty( Document::Ruler, settings.editor.documents.ruler );
    setProperty( Document::LineWrap, settings.editor.documents.wrap );
    setProperty( Document::LineNumberMargin, settings.editor.margins.lineNumber );
    setProperty( Document::FoldMargin, settings.editor.margins.fold );
    setProperty( Document::SymbolMargin, settings.editor.margins.symbol );
    setProperty( Document::ChangeMargin, settings.editor.margins.lineState );
    setProperty( Document::Font, settings.editor.documents.font );
    setProperty( Document::Paper, settings.editor.colors.paper );
    setProperty( Document::Pen, settings.editor.colors.pen );
    setProperty( Document::SelectionBackground, settings.editor.colors.selectionBackground );
    setProperty( Document::SelectionForeground, settings.editor.colors.selectionForeground );
    setProperty( Document::CaretLineBackground, settings.editor.colors.caretLineBackground );
    setProperty( Document::CaretLineForeground, settings.editor.colors.caretLineForeground );
}

bool Document::open( const QString& filePath, const QString& encoding, bool readOnly )
{
    if ( filePath.isEmpty() ) {
        setProperty( Document::LastError, tr( "No given file path to open the document." ) );
        return false;
    }
    
    QString content;
    
    // get file content propertly decoded
    if ( !fileContent( content, filePath, encoding ) ) {
        return false;
    }
    
    const ApplicationSettings& settings = Abstractors::applicationSettings();
    const bool detectEol = settings.editor.detectEol.value().toBool();
    const bool detectIndent = settings.editor.detectIndentation.value().toBool();
    const bool convertEol = settings.editor.convertEol.value().toBool();
    const bool convertIndent = settings.editor.convertIndentation.value().toBool();
    
    DocumentPropertiesDiscover::GuessedProperties properties;
    QString convertedContent = content;
    
    // guess the content properties if needed
    if ( detectEol || detectIndent ) {
        properties = DocumentPropertiesDiscover::guessContentProperties( content, detectEol, detectIndent );
    }
    
    // convert the content if needed
    if ( convertEol || convertIndent ) {
        const DocumentPropertiesDiscover::GuessedProperties from = detectEol && detectIndent ? properties : DocumentPropertiesDiscover::guessContentProperties( content, true, true );
        const DocumentPropertiesDiscover::GuessedProperties to;
        
        DocumentPropertiesDiscover::convertContent( convertedContent, from, to, convertEol, convertIndent );
        
        if ( convertEol ) {
            properties.eol = to.eol;
        }
        
        if ( convertIndent ) {
            properties.indent = to.indent;
            properties.indentWidth = to.indentWidth;
            properties.tabWidth = to.tabWidth;
        }
    }
    
    // apply application properties to the document
    applyApplicationSettings( properties );
    
    // finalyze document
    setProperty( Document::NewFile, false );
    setProperty( Document::FilePath, filePath );
    setProperty( Document::TextEncoding, textCodec( encoding )->name() );
    setProperty( Document::MimeType, mimeTypeDB().mimeTypeForFileName( filePath ) );
    setProperty( Document::ReadOnly, readOnly );
    setProperty( Document::InitialText, content );
    setProperty( Document::State, Document::Unmodified );
    
    // set transformed content
    if ( convertedContent != content ) {
        setProperty( Document::Text, convertedContent );
    }
    
    // set content language
    const QString language = mimeTypeDB().languageForFileName( filePath );
    
    if ( property( Document::Language ).toString() != language ) {
        setProperty( Document::Language, language );
    }
    
    return true;
}

bool Document::save( const QString& filePath, const QString& encoding )
{
    QString fn = filePath.isEmpty() ? property( Document::FilePath ).toString() : filePath;
    
    if ( fn.isEmpty() ) {
        setProperty( Document::LastError, tr( "No given file path to save the document." ) );
        return false;
    }
    
    const QString backupFn = QString( "%1.%2" ).arg( fn ).arg( QDateTime::currentDateTime().toString( "yyyy-MM-dd-hh-mm-ss" ) );
    
    if ( QFile::exists( fn ) ) {
        if ( !QFile::rename( fn, backupFn ) ) {
            setProperty( Document::LastError, tr( "Can't rename '%1' for doing atomic save." ).arg( filePath ) );
            return false;
        }
    }
    
    QFile file( fn );
    
    if ( !file.open( QIODevice::WriteOnly ) ) {
        // restore original content
        if ( QFile::exists( backupFn ) ) {
            if ( !QFile::rename( backupFn, fn ) ) {
                setProperty( Document::LastError, tr( "Can't rename '%1' for doing atomic save." ).arg( backupFn ) );
                return false;
            }
        }
        
        setProperty( Document::LastError, tr( "Can't save file '%1' (%2)" ).arg( filePath ).arg( file.errorString() ) );
        return false;
    }
    
    if ( !file.resize( 0 ) ) {
        // close / delete
        file.close();
        file.remove();
        
        // restore original content
        if ( QFile::exists( backupFn ) ) {
            if ( !QFile::rename( backupFn, fn ) ) {
                setProperty( Document::LastError, tr( "Can't rename '%1' for doing atomic save." ).arg( backupFn ) );
                return false;
            }
        }
        
        setProperty( Document::LastError, tr( "Can't truncate '%1' for doing atomic save." ).arg( filePath ) );
        return false;
    }
    
    const int eol = property( Document::Eol ).toInt();
    const int indent = property( Document::Indent ).toInt();
    const int indentWidth = property( Document::IndentWidth ).toInt();
    const int tabWidth = property( Document::TabWidth ).toInt();
    const DocumentPropertiesDiscover::GuessedProperties from( eol, indent, indentWidth, tabWidth );
    const DocumentPropertiesDiscover::GuessedProperties to( from );
    QTextCodec* codec = textCodec( encoding );
    QString content = property( Document::Text ).toString();
    
    // convert eol if needed, may be not necessary when implementing  auto convertion of pasted text
    DocumentPropertiesDiscover::convertContent( content, from, to, true, false );
    
    if ( file.write( codec->fromUnicode( content ) ) == -1 ) {
        // close / delete
        file.close();
        file.remove();
        
        // restore original content
        if ( QFile::exists( backupFn ) ) {
            if ( !QFile::rename( backupFn, fn ) ) {
                setProperty( Document::LastError, tr( "Can't rename '%1' for doing atomic save." ).arg( backupFn ) );
                return false;
            }
        }
        
        setProperty( Document::LastError, file.errorString() );
        return false;
    }
    
    setProperty( Document::NewFile, false );
    setProperty( Document::FilePath, fn );
    setProperty( Document::TextEncoding, codec->name() );
    setProperty( Document::MimeType, mimeTypeDB().mimeTypeForFileName( fn ) );
    setProperty( Document::State, Document::Unmodified );
    
    // set content language
    const QString language = mimeTypeDB().languageForFileName( fn );
    
    if ( property( Document::Language ).toString() != language ) {
        setProperty( Document::Language, language );
    }
    
    if ( QFile::exists( backupFn ) ) {
        if ( !QFile::remove( backupFn ) ) {
            setProperty( Document::LastError, tr( "Can't remove '%1' backup for finishing atomic save." ).arg( backupFn ) );
        }
    }
    
    return true;
}

bool Document::reload()
{
    const QString filePath = property( Document::FilePath ).toString();
    
    if ( filePath.isEmpty() ) {
        const QString title = property( Document::Title ).toString();
        setProperty( Document::LastError, tr( "Can't reload document '%1': No file path defined." ).arg( title ) );
        return false;
    }
    
    const QString encoding = property( Document::TextEncoding ).toString();
    const bool readOnly = property( Document::ReadOnly ).toBool();
    return open( filePath, encoding, readOnly );
}

void Document::close()
{
    clearProperties();
    initialize();
}

void Document::convertTo( const DocumentPropertiesDiscover::GuessedProperties& properties )
{
    const int eol = property( Document::Eol ).toInt();
    const int indent = property( Document::Indent ).toInt();
    const int indentWidth = property( Document::IndentWidth ).toInt();
    const int tabWidth = property( Document::TabWidth ).toInt();
    const DocumentPropertiesDiscover::GuessedProperties from( eol, indent, indentWidth, tabWidth );
    DocumentPropertiesDiscover::GuessedProperties to( properties );
    QString content = property( Document::Text ).toString();
    
    if ( to.eol == -1 ) {
        to.eol = eol;
    }
    
    if ( to.indent == -1 ) {
        to.indent = indent;
    }
    
    if ( to.indentWidth == -1 ) {
        to.indentWidth = indentWidth;
    }
    
    if ( to.tabWidth == -1 ) {
        to.tabWidth = tabWidth;
    }
    
    DocumentPropertiesDiscover::convertContent( content, from, to, true, true );
    
    setProperty( Document::Eol, to.eol );
    setProperty( Document::Indent, to.indent );
    setProperty( Document::IndentWidth, to.indentWidth );
    setProperty( Document::TabWidth, to.tabWidth );
    setProperty( Document::Text, content );
}

void Document::initialize()
{
    applyApplicationSettings( DocumentPropertiesDiscover::GuessedProperties() );
    
    setProperty( Document::NewFile, true );
    setProperty( Document::Title, tr( "New file #%1" ).arg( ++Document::mDocumentCount ) );
    setProperty( Document::State, Document::Unmodified );
}

QTextCodec* Document::textCodec( const QString& encoding ) const
{
    QString name = encoding.isEmpty() ? property( Document::TextEncoding ).toString() : encoding;
    
    if ( name.isEmpty() ) {
        name = Abstractors::applicationSettings().editor.documents.eol.value().toString();
    }
    
    return QTextCodec::codecForName( name.toLocal8Bit() );
}

bool Document::fileContent( QString& content, const QString& filePath, const QString& encoding )
{
    if ( filePath.isEmpty() ) {
        setProperty( Document::LastError, tr( "No given file path to get the document." ) );
        return false;
    }
    
    QFile file( filePath );
    
    if ( !file.open( QIODevice::ReadOnly ) ) {
        setProperty( Document::LastError, tr( "Can't open file '%1' (%2)" ).arg( filePath ).arg( file.errorString() ) );
        return false;
    }
    
    const QByteArray data = file.readAll();
    QTextCodec* codec = textCodec( encoding );
    content = codec->toUnicode( data );
    
    return true;
}
