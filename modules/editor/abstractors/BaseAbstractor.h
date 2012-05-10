#ifndef BASEABSTRACTOR_H
#define BASEABSTRACTOR_H

#include <QObject>

class BaseAbstractor : public QObject
{
    Q_OBJECT

public:
    BaseAbstractor( QObject* parent = 0 )
        : QObject( parent ) {
    }
};

#endif // BASEABSTRACTOR_H
