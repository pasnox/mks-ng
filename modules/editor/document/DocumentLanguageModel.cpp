#include "DocumentLanguageModel.h"
#include "CodeEditorAbstractor.h"

DocumentLanguageModel::DocumentLanguageModel( QObject* parent )
    : QStringListModel( parent ), mAbstractor( 0 )
{
}

DocumentLanguageModel::~DocumentLanguageModel()
{
}

void DocumentLanguageModel::setCodeEditorAbstractor( CodeEditorAbstractor* cea )
{
    if ( cea == mAbstractor ) {
        return;
    }
    
    mAbstractor = cea;
    
    emit dataChanged( index( 0, 0 ), index( rowCount(), 0 ) );
}

CodeEditorAbstractor* DocumentLanguageModel::codeEditorAbstractor() const
{
    return mAbstractor;
}

QVariant DocumentLanguageModel::data( const QModelIndex& index, int role ) const
{
    if ( index.isValid() && role == Qt::DecorationRole ) {
        if ( mAbstractor ) {
            const QString language = QStringListModel::data( index, Qt::DisplayRole ).toString();
            return mAbstractor->iconForLanguage( language );
        }
    }
    
    return QStringListModel::data( index, role );
}
