#include "QodeEditDataWatcher.h"

#include <srchilite/settings.h>

#include <QApplication>
#include <QDir>
#include <QDebug>

QodeEditDataWatcher::QodeEditDataWatcher( QObject* parent )
    : QFileSystemWatcher( parent )
{
    mSuffixes[ QodeEditDataWatcher::Languages ]
        << "*.xml"
    ;
    mSuffixes[ QodeEditDataWatcher::Styles ]
        << "*.css"
    ;
    
    setFilePath( QString( "%1/../../data/katepart" ).arg( qApp->applicationDirPath() ) );
    
#if defined( HAS_QT_5 )
    connect( this, &QFileSystemWatcher::directoryChanged, this, &QodeEditDataWatcher::_q_directoryChanged );
#else
    connect( this, SIGNAL( directoryChanged( const QString& ) ), this, SLOT( _q_directoryChanged( const QString& ) ) );
#endif
}

QodeEditDataWatcher::~QodeEditDataWatcher()
{
}

QString QodeEditDataWatcher::filePath() const
{
    return mFilePath;
}

void QodeEditDataWatcher::setFilePath( const QString& filePath )
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

QStringList QodeEditDataWatcher::files( QodeEditDataWatcher::Type type ) const
{
    return mFiles.value( type );
    
}

void QodeEditDataWatcher::update()
{
    srchilite::Settings::retrieveDataDir( true );
    
    const QDir::SortFlags sortFlags = QDir::Name | QDir::Type | QDir::IgnoreCase | QDir::LocaleAware;
    QDir dir( mFilePath );
    
    if ( !dir.exists() ) {
        return;
    }
    
    // get files
    QStringList lang = dir.entryList( mSuffixes[ QodeEditDataWatcher::Languages ], QDir::Files, sortFlags );
    QStringList styles = dir.entryList( mSuffixes[ QodeEditDataWatcher::Styles ], QDir::Files, sortFlags );
    
    for ( int i = 0; i < lang.count(); i++ ) {
        QString& file = lang[ i ];
        file = QFileInfo( file ).baseName();
    }
    
    for ( int i = 0; i < styles.count(); i++ ) {
        QString& file = styles[ i ];
        file = QFileInfo( file ).baseName();
    }
    
    // tell about changes
    if ( lang != mFiles.value( QodeEditDataWatcher::Languages ) ) {
        mFiles[ QodeEditDataWatcher::Languages ] = lang;
        emit filesChanged( QodeEditDataWatcher::Languages );
    }
    
    if ( styles != mFiles.value( QodeEditDataWatcher::Styles ) ) {
        mFiles[ QodeEditDataWatcher::Styles ] = styles;
        emit filesChanged( QodeEditDataWatcher::Styles );
    }
}

void QodeEditDataWatcher::_q_directoryChanged( const QString& path )
{
    if ( path == mFilePath ) {
        update();
    }
}
