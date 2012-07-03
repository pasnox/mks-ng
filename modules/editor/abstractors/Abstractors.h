#ifndef ABSTRACTORS_H
#define ABSTRACTORS_H

class QObject;
class QMetaObject;
class QString;
class QStringList;

class BaseAbstractor;
class CodeEditorAbstractor;
class ApplicationSettings;

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
    
    BaseAbstractor* instance( Abstractors::Type type, QObject* parent = 0 );
    CodeEditorAbstractor* codeEditor();
    
    ApplicationSettings& applicationSettings();
};

#endif // ABSTRACTORS_H
