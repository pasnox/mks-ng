#ifndef DOCUMENTLANGUAGEMODEL_H
#define DOCUMENTLANGUAGEMODEL_H

#include <QStringListModel>

class CodeEditorAbstractor;

class DocumentLanguageModel : public QStringListModel
{
    Q_OBJECT
    
public:
    DocumentLanguageModel( QObject* parent = 0 );
    virtual ~DocumentLanguageModel();
    
    void setCodeEditorAbstractor( CodeEditorAbstractor* cea );
    CodeEditorAbstractor* codeEditorAbstractor() const;
    
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;

protected:
    CodeEditorAbstractor* mAbstractor;
};

#endif // DOCUMENTLANGUAGEMODEL_H
