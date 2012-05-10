#ifndef SOURCEHIGHLIGHTQTDATAWATCHER_H
#define SOURCEHIGHLIGHTQTDATAWATCHER_H

#include <QFileSystemWatcher>
#include <QHash>
#include <QStringList>

class SourceHighlightQtDataWatcher : public QFileSystemWatcher
{
    Q_OBJECT
    
public:
    enum Type {
        Languages = 0,
        OutputLanguages = 1,
        Styles = 2
    };
    
    SourceHighlightQtDataWatcher( QObject* parent = 0 );
    virtual ~SourceHighlightQtDataWatcher();
    
    QString filePath() const;
    void setFilePath( const QString& filePath );
    
    QStringList files( SourceHighlightQtDataWatcher::Type type ) const;

protected:
    QString mFilePath;
    QHash<SourceHighlightQtDataWatcher::Type, QStringList> mFiles;
    QHash<SourceHighlightQtDataWatcher::Type, QStringList> mSuffixes;
    
    void update();

protected slots:
    void _q_directoryChanged( const QString& path );

signals:
    void filesChanged( SourceHighlightQtDataWatcher::Type type );
};

#endif // SOURCEHIGHLIGHTQTDATAWATCHER_H
