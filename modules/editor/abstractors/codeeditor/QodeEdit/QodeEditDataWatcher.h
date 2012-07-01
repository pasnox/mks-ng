#ifndef QODEEDITDATAWATCHER_H
#define QODEEDITDATAWATCHER_H

#include <QFileSystemWatcher>
#include <QHash>
#include <QStringList>

class QodeEditDataWatcher : public QFileSystemWatcher
{
    Q_OBJECT
    
public:
    enum Type {
        Languages = 0,
        Styles = 1
    };
    
    QodeEditDataWatcher( QObject* parent = 0 );
    virtual ~QodeEditDataWatcher();
    
    QString filePath() const;
    void setFilePath( const QString& filePath );
    
    QStringList files( QodeEditDataWatcher::Type type ) const;

protected:
    QString mFilePath;
    QHash<QodeEditDataWatcher::Type, QStringList> mFiles;
    QHash<QodeEditDataWatcher::Type, QStringList> mSuffixes;
    
    void update();

protected slots:
    void _q_directoryChanged( const QString& path );

signals:
    void filesChanged( QodeEditDataWatcher::Type type );
};

#endif // QODEEDITDATAWATCHER_H
