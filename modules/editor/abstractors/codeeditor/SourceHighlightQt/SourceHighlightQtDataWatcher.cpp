#include "SourceHighlightQtDataWatcher.h"

#include <srchilite/settings.h>

#include <QApplication>
#include <QDir>
#include <QDebug>

SourceHighlightQtDataWatcher::SourceHighlightQtDataWatcher( QObject* parent )
    : QFileSystemWatcher( parent )
{
    mSuffixes[ SourceHighlightQtDataWatcher::Languages ]
        << "*.lang"
    ;
    mSuffixes[ SourceHighlightQtDataWatcher::OutputLanguages ]
        << "*.outlang"
    ;
    mSuffixes[ SourceHighlightQtDataWatcher::Styles ]
        << "*.style"
        //<< "*.css"
    ;
    
    setFilePath( QString( "%1/../../data/source-highlight" ).arg( qApp->applicationDirPath() ) );
    
#if defined( HAS_QT_5 )
    connect( this, &QFileSystemWatcher::directoryChanged, this, &SourceHighlightQtDataWatcher::_q_directoryChanged );
#else
    connect( this, SIGNAL( directoryChanged( const QString& ) ), this, SLOT( _q_directoryChanged( const QString& ) ) );
#endif
}

SourceHighlightQtDataWatcher::~SourceHighlightQtDataWatcher()
{
}

QString SourceHighlightQtDataWatcher::filePath() const
{
    return mFilePath;
}

void SourceHighlightQtDataWatcher::setFilePath( const QString& filePath )
{
    if ( mFilePath == filePath ) {
        return;
    }
    
    if ( !mFilePath.isEmpty() ) {
        removePath( mFilePath );
    }
    
    mFilePath = filePath;
    srchilite::Settings::setGlobalDataDir( mFilePath.toStdString() );
    
    if ( !mFilePath.isEmpty() ) {
        addPath( mFilePath );
    }
    
    update();
}

QStringList SourceHighlightQtDataWatcher::files( SourceHighlightQtDataWatcher::Type type ) const
{
    return mFiles.value( type );
    
}

void SourceHighlightQtDataWatcher::update()
{
    srchilite::Settings::retrieveDataDir( true );
    
    const QDir::SortFlags sortFlags = QDir::Name | QDir::Type | QDir::IgnoreCase | QDir::LocaleAware;
    QDir dir( mFilePath );
    
    if ( !dir.exists() ) {
        return;
    }
    
    // get files
    QStringList lang = dir.entryList( mSuffixes[ SourceHighlightQtDataWatcher::Languages ], QDir::Files, sortFlags );
    QStringList outLang = dir.entryList( mSuffixes[ SourceHighlightQtDataWatcher::OutputLanguages ], QDir::Files, sortFlags );
    QStringList styles = dir.entryList( mSuffixes[ SourceHighlightQtDataWatcher::Styles ], QDir::Files, sortFlags );
    
    for ( int i = 0; i < lang.count(); i++ ) {
        QString& file = lang[ i ];
        file = QFileInfo( file ).baseName();
    }
    
    for ( int i = 0; i < outLang.count(); i++ ) {
        QString& file = outLang[ i ];
        file = QFileInfo( file ).baseName();
    }
    
    for ( int i = 0; i < styles.count(); i++ ) {
        QString& file = styles[ i ];
        file = QFileInfo( file ).baseName();
    }
    
    // tell about changes
    if ( lang != mFiles.value( SourceHighlightQtDataWatcher::Languages ) ) {
        mFiles[ SourceHighlightQtDataWatcher::Languages ] = lang;
        emit filesChanged( SourceHighlightQtDataWatcher::Languages );
    }
    
    if ( outLang != mFiles.value( SourceHighlightQtDataWatcher::OutputLanguages ) ) {
        mFiles[ SourceHighlightQtDataWatcher::OutputLanguages ] = outLang;
        emit filesChanged( SourceHighlightQtDataWatcher::OutputLanguages );
    }
    
    if ( styles != mFiles.value( SourceHighlightQtDataWatcher::Styles ) ) {
        mFiles[ SourceHighlightQtDataWatcher::Styles ] = styles;
        emit filesChanged( SourceHighlightQtDataWatcher::Styles );
    }
}

void SourceHighlightQtDataWatcher::_q_directoryChanged( const QString& path )
{
    if ( path == mFilePath ) {
        update();
    }
}
