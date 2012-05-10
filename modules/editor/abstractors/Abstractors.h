#ifndef ABSTRACTORS_H
#define ABSTRACTORS_H

#include <QObject>

#include "BaseAbstractor.h"

class QMetaObject;
class QString;
class QStringList;
class QMimeDatabase;

namespace Abstractors
{
    enum Type {
        CodeEditor = 0
    };
    
    void add( Abstractors::Type type, const QMetaObject* meta );
    void remove( Abstractors::Type type, const QMetaObject* meta );
    QStringList entries( Abstractors::Type type );
    void setCurrent( Abstractors::Type type, const QString& entry );
    QString current( Abstractors::Type type );
    BaseAbstractor* create( Abstractors::Type type, QObject* parent = 0 );
    template <class T> T castedCreate( Abstractors::Type type, QObject* parent = 0 );
    
    const QMimeDatabase& mimeDatabase();
};

template <class T> T Abstractors::castedCreate( Abstractors::Type type, QObject* parent )
{
    BaseAbstractor* object = Abstractors::create( type, parent );
    
    if ( !object ) {
        return 0;
    }
    
    T casted = qobject_cast<T>( object );
    
    if ( !casted ) {
        object->deleteLater();
    }
    
    return casted;
}

#endif // ABSTRACTORS_H
