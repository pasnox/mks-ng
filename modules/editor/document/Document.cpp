#include "Document.h"

#include <QEvent>
#include <QMetaObject>
#include <QTextCodec>
#include <QVariant>
#include <QFile>
#include <QIcon>
#include <QDir>
#include <QDateTime>
#include <QDebug>

QHash<QString, const QMetaObject*> Document::mAbstractors;
QMimeDatabase Document::mMimeDb;
int Document::mDocumentCount = 0;

Document::Document( QWidget* parent )
    : BaseWidget( parent )
{
#if USE_MDI_AREA == 1
    setWindowFlags( Qt::FramelessWindowHint );
#else
#endif
}

Document::~Document()
{
}

void Document::registerDocumentAbstraction( const QMetaObject* meta )
{
    Q_ASSERT( meta );
    mAbstractors[ QString::fromLocal8Bit( meta->className() ) ] = meta;
}

void Document::unregisterDocumentAbstraction( const QMetaObject* meta )
{
    Q_ASSERT( meta );
    mAbstractors.remove( QString::fromLocal8Bit( meta->className() ) );
}

QStringList Document::documentAbstractionKeys()
{
    return mAbstractors.keys();
}

Document* Document::createDocument( const QString& key, QWidget* parent )
{
    const QMetaObject* meta = mAbstractors.value( key );
    Q_ASSERT( meta );
    return qobject_cast<Document*>( meta->newInstance( Q_ARG( QWidget*, parent ) ) );
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
            
            const QString lexer = this->property( Document::Lexer ).toString();
            
            if ( !lexer.isEmpty() ) {
                return iconForLexer( lexer );
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
        
        if ( this->property( Document::Lexer ).isNull() && !value.toString().isEmpty() ) {
            emit propertyChanged( Document::Lexer );
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
        setProperty( Document::LastError, file.errorString() );
        return false;
    }
    
    QTextCodec* codec = textCodec( encoding );
    
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
            setProperty( Document::LastError, tr( "Can't rename the current document for doing atomic save." ) );
            return false;
        }
    }
    
    QFile file( fn );
    
    if ( !file.open( QIODevice::WriteOnly ) ) {
        setProperty( Document::LastError, file.errorString() );
        return false;
    }
    
    if ( !file.resize( 0 ) ) {
        setProperty( Document::LastError, tr( "Can't truncate the current document for doing atomic save." ) );
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
        case Document::Mac:
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
            setProperty( Document::LastError, tr( "Can't remove the current document backup for finishing atomic save." ) );
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
    if ( state & Document::ExternallyDeleted ) {
        return QIcon::fromTheme( "archive-remove" );
    }
    else if ( state & Document::ExternallyModified ) {
        return QIcon::fromTheme( "document-properties" );
    }
    else if ( state & Document::Modified ) {
        return QIcon::fromTheme( "document-save" );
    }
    
    return QIcon();
}

QIcon Document::iconForFileName( const QString& fileName ) const
{
    const QList<QMimeType> types = Document::mMimeDb.mimeTypesForFileName( fileName );
    
    foreach ( const QMimeType& type, types ) {
        const QIcon icon = QIcon::fromTheme( type.iconName() );
        
        if ( !icon.isNull() ) {
            return icon;
        }
    }
    
    return QIcon();
}

QIcon Document::iconForLexer( const QString& lexer ) const
{
    const QMimeType type = mimeTypeForLexer( lexer );
    return QIcon::fromTheme( type.iconName() );
}

QIcon Document::iconForContent( const QString& content ) const
{
    const QMimeType type = Document::mMimeDb.mimeTypeForData( content.toUtf8() );
    return QIcon::fromTheme( type.iconName() );
}
