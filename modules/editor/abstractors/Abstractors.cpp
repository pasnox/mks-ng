#include "Abstractors.h"

#include <QHash>
#include <QStringList>
#include <QApplication>

namespace Abstractors {
    static QHash<Abstractors::Type, QHash<QString, const QMetaObject*> > mAbstractors;
    static QHash<Abstractors::Type, QString> mCurrentAbstractors;
    static ApplicationSettings* mApplicationSettings = 0;
}

void Abstractors::add( Abstractors::Type type, const QMetaObject* meta )
{
    Q_ASSERT( meta );
    Abstractors::mAbstractors[ type ][ QString::fromLocal8Bit( meta->className() ) ] = meta;
}

void Abstractors::remove( Abstractors::Type type, const QMetaObject* meta )
{
    Q_ASSERT( meta );
    Abstractors::mAbstractors[ type ].remove( QString::fromLocal8Bit( meta->className() ) );
}

QStringList Abstractors::entries( Abstractors::Type type )
{
    return Abstractors::mAbstractors[ type ].keys();
}

void Abstractors::setCurrent( Abstractors::Type type, const QString& entry )
{
    if ( Abstractors::mAbstractors[ type ].keys().contains( entry ) ) {
        Abstractors::mCurrentAbstractors[ type ] = entry;
    }
}

QString Abstractors::current( Abstractors::Type type )
{
    QString entry = Abstractors::mCurrentAbstractors[ type ];
    
    if ( entry.isEmpty() ) {
        entry = Abstractors::mAbstractors[ type ].keys().value( 0 );
    }
    
    return entry;
}

BaseAbstractor* Abstractors::create( Abstractors::Type type, QObject* parent )
{
    const QString entry = Abstractors::current( type );
    const QMetaObject* meta = Abstractors::mAbstractors[ type ][ entry ];
    Q_ASSERT( meta );
    return qobject_cast<BaseAbstractor*>( meta->newInstance( Q_ARG( QObject*, parent ) ) );
}

ApplicationSettings& Abstractors::applicationSettings()
{
    if ( !Abstractors::mApplicationSettings ) {
        Abstractors::mApplicationSettings = new ApplicationSettings( qApp );
    }
    
    return *Abstractors::mApplicationSettings;
}
