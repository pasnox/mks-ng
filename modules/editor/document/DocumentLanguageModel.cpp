#include "DocumentLanguageModel.h"
#include "CodeEditorAbstractor.h"

#include <QApplication>

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
    Q_ASSERT( mAbstractor );
    
    if ( index.isValid() ) {
        switch ( role ) {
            case Qt::DecorationRole: {
                const QString language = QStringListModel::data( index, Qt::DisplayRole ).toString();
                return mAbstractor->mimeTypeDB().iconForLanguage( language );
            }
            
            // fix stupid styles that force css styles in menubar without doing proper matching
            case Qt::ForegroundRole:
                return QApplication::style()->standardPalette().brush( QPalette::Text );
        }
    }
    
    return QStringListModel::data( index, role );
}
