#include "Document.h"
#include "Abstractors.h"
#include "CodeEditorAbstractor.h"

#include <QEvent>
#include <QMetaObject>
#include <QTextCodec>
#include <QVariant>
#include <QFile>
#include <QIcon>
#include <QDir>
#include <QDateTime>
#include <QMimeDatabase>
#include <QDebug>

int Document::mDocumentCount = 0;

Document::Document( const CodeEditorAbstractor* codeEditorAbstractor, QWidget* parent )
    : BaseWidget( parent ),
        mCodeEditorAbstractor( codeEditorAbstractor )
{
#if USE_MDI_AREA == 1
    setWindowFlags( Qt::FramelessWindowHint );
#else
#endif
}

Document::~Document()
{
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
                return iconForState( state );
            }
            
            const QString filePath = this->property( Document::FilePath ).toString();
            
            if ( !filePath.isEmpty() ) {
                return iconForFileName( filePath );
            }
            
            const QString language = this->property( Document::Language ).toString();
            
            if ( !language.isEmpty() ) {
                return iconForLanguage( language );
            }
            
            const QString text = this->property( Document::Text ).toString();
            
            if ( !text.isEmpty() ) {
                return iconForContent( filePath );
            }
            
            const bool newFile = this->property( Document::NewFile ).toBool();
            
            return QIcon::fromTheme( newFile ? "document-new" : "text-x-generic" );
        }
        case Document::Title: {
            const QString fileName = QFileInfo( this->property( Document::FilePath ).toString() ).fileName();
            return QString( "%1[*]" ).arg( fileName );
        }
        case Document::Eol:
            return Document::Unix;
        case Document::Indent:
            return Document::Spaces;
        case Document::TabWidth:
            return 4;
        case Document::IndentWidth:
            return 4;
        case Document::Ruler:
            return Document::NoRuler;
        case Document::TextEncoding:
            return "UTF-8";
        case Document::LineWrap:
            return Document::NoWrap;
        case Document::LineNumberMargin:
            return true;
        case Document::FoldMargin:
            return true;
        case Document::SymbolMargin:
            return true;
        case Document::ChangeMargin:
            return true;
        default:
            return QVariant();
    }
}

void Document::setProperty( int property, const QVariant& value )
{
    if ( property != Document::LastError ) {
        setProperty( Document::LastError, QVariant() );
    }
    
    if ( property == Document::FilePath ) {
        setProperty( Document::Title, QVariant() );
        
        if ( this->property( Document::Language ).isNull() && !value.toString().isEmpty() ) {
            emit propertyChanged( Document::Language );
        }
    }
    
    Q_UNUSED( value );
}

void Document::updateSharedProperties()
{
    const bool locked = blockSignals( true );
    setProperty( Document::Eol, Document::property( Document::Eol ) );
    setProperty( Document::Indent, Document::property( Document::Indent ) );
    setProperty( Document::TabWidth, Document::property( Document::TabWidth ) );
    setProperty( Document::IndentWidth, Document::property( Document::IndentWidth ) );
    setProperty( Document::Ruler, Document::property( Document::Ruler ) );
    setProperty( Document::LineWrap, Document::property( Document::LineWrap ) );
    //setProperty( Document::TextEncoding, Document::property( Document::TextEncoding ) );
    setProperty( Document::LineNumberMargin, Document::property( Document::LineNumberMargin ) );
    setProperty( Document::FoldMargin, Document::property( Document::FoldMargin ) );
    setProperty( Document::SymbolMargin, Document::property( Document::SymbolMargin ) );
    setProperty( Document::ChangeMargin, Document::property( Document::ChangeMargin ) );
    blockSignals( locked );
}

bool Document::open( const QString& filePath, const QString& encoding )
{
    if ( filePath.isEmpty() ) {
        setProperty( Document::LastError, tr( "No given file path to open the document." ) );
        return false;
    }
    
    QFile file( filePath );
    
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        setProperty( Document::LastError, tr( "Can't open file '%1' (%2)" ).arg( filePath ).arg( file.errorString() ) );
        return false;
    }
    
    QTextCodec* codec = textCodec( encoding );
    
    if ( mCodeEditorAbstractor ) {
        setProperty( Document::Language, mCodeEditorAbstractor->languageForFileName( filePath ) );
    }
    
    setProperty( Document::NewFile, false );
    setProperty( Document::FilePath, QDir::cleanPath( filePath ) );
    setProperty( Document::TextEncoding, codec->name() );
    setProperty( Document::InitialText, codec->toUnicode( file.readAll() ) );
    setProperty( Document::State, Document::Unmodified );
    
    return true;
}

bool Document::save( const QString& filePath, const QString& encoding )
{
    QString fn = filePath.isEmpty() ? property( Document::FilePath ).toString() : filePath;
    
    if ( fn.isEmpty() ) {
        setProperty( Document::LastError, tr( "No given file path to save the document." ) );
        return false;
    }
    
    const QString backupFn = QString( "%1.%2" ).arg( fn ).arg( QDateTime::currentDateTime().toString( "" ) );
    
    if ( QFile::exists( fn ) ) {
        if ( !QFile::rename( fn, backupFn ) ) {
            setProperty( Document::LastError, tr( "Can't rename '%1' for doing atomic save." ).arg( filePath ) );
            return false;
        }
    }
    
    QFile file( fn );
    
    if ( !file.open( QIODevice::WriteOnly ) ) {
        setProperty( Document::LastError, tr( "Can't save file '%1' (%2)" ).arg( filePath ).arg( file.errorString() ) );
        return false;
    }
    
    if ( !file.resize( 0 ) ) {
        setProperty( Document::LastError, tr( "Can't truncate '%1' for doing atomic save." ).arg( filePath ) );
        return false;
    }
    
    const Document::EolHint eol = Document::EolHint( property( Document::Eol ).toInt() );
    QTextCodec* codec = textCodec( encoding );
    QString text = property( Document::Text ).toString();
    
    // fix possible bad eol when pasting
    text.replace( "\r\n", "\n" );
    text.replace( "\n\r", "\n" );
    text.replace( "\r", "\n" );
    
    // use correct eol
    switch ( eol ) {
        case Document::Unix:
            // nothing to do
            break;
        case Document::Windows:
            text.replace( "\n", "\r\n" );
            break;
        case Document::MacOS:
            text.replace( "\n", "\r" );
            break;
    }
    
    if ( file.write( codec->fromUnicode( text ) ) == -1 ) {
        setProperty( Document::LastError, file.errorString() );
        return false;
    }
    
    setProperty( Document::NewFile, false );
    setProperty( Document::FilePath, QDir::cleanPath( fn ) );
    setProperty( Document::TextEncoding, codec->name() );
    setProperty( Document::State, Document::Unmodified );
    
    if ( QFile::exists( backupFn ) ) {
        if ( !QFile::remove( backupFn ) ) {
            setProperty( Document::LastError, tr( "Can't remove '%1' backup for finishing atomic save." ).arg( backupFn ) );
        }
    }
    
    return true;
}

void Document::close()
{
    clearProperties();
    initialize();
}

void Document::initialize()
{
    updateSharedProperties();
    
    setProperty( Document::NewFile, true );
    setProperty( Document::Title, tr( "New file #%1" ).arg( ++Document::mDocumentCount ) );
    setProperty( Document::State, Document::Unmodified );
}

QTextCodec* Document::textCodec( const QString& encoding ) const
{
    QString name = encoding.isEmpty() ? property( Document::TextEncoding ).toString() : encoding;
    
    if ( name.isEmpty() ) {
        name = Document::property( Document::TextEncoding ).toString();
    }
    
    return QTextCodec::codecForName( name.toLocal8Bit() );
}

QIcon Document::iconForState( Document::StateHints state ) const
{
    return mCodeEditorAbstractor ? mCodeEditorAbstractor->iconForState( state ) : QIcon();
}

QIcon Document::iconForFileName( const QString& fileName ) const
{
    return mCodeEditorAbstractor ? mCodeEditorAbstractor->iconForFileName( fileName ) : QIcon();
}

QIcon Document::iconForLanguage( const QString& language ) const
{
    return mCodeEditorAbstractor ? mCodeEditorAbstractor->iconForLanguage( language ) : QIcon();
}

QIcon Document::iconForContent( const QString& content ) const
{
    return mCodeEditorAbstractor ? mCodeEditorAbstractor->iconForContent( content ) : QIcon();
}
