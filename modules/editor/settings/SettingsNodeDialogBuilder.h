#ifndef SETTINGSNODEDIALOGBUILDER_H
#define SETTINGSNODEDIALOGBUILDER_H

#include <QDialog>

#include "SettingsNode.h"

class Ui_SettingsNodeDialogBuilder;
class SettingsNodeDialogBuilderDelegate;

class QBoxLayout;
class QTreeWidgetItem;
class QAbstractButton;

class SettingsNodeDialogBuilder : public QDialog
{
    Q_OBJECT
    
public:
    SettingsNodeDialogBuilder( QWidget* parent = 0 );
    virtual ~SettingsNodeDialogBuilder();
    
    SettingsNode rootNode() const;
    
    SettingsNodeDialogBuilderDelegate* delegate() const;
    void setDelegate( SettingsNodeDialogBuilderDelegate* delegate );
    
    virtual int margin() const;
    virtual int spacing() const;
    
    bool build( const SettingsNode& node );

protected:
    Ui_SettingsNodeDialogBuilder* ui;
    SettingsNode mRootNode;
    SettingsNodeDialogBuilderDelegate* mDelegate;
    QHash<SettingsNode, QTreeWidgetItem*> mItems;
    QHash<SettingsNode, QWidget*> mEditors;
    
    void addToParentSettingsNode( QWidget* widget, const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate );
    QWidget* createSettingsNodeEditor( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate );
    QBoxLayout* createSettingsNodeLayout( QWidget* widget, const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate );
    QWidget* createSettingsNodePage( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate );
    QWidget* createSettingsNodeGroup( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate );
    QWidget* createSettingsNodeValue( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate );
    QWidget* createSettingsNodeGroupValue( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate );
    bool recurseBuild( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate );

protected slots:
    void on_twPages_itemSelectionChanged();
    void on_dbbButtons_clicked( QAbstractButton* button );
};

#endif // SETTINGSNODEDIALOGBUILDER_H
