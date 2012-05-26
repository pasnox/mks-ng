#ifndef SETTINGSNODEDIALOGBUILDERDELEGATE_H
#define SETTINGSNODEDIALOGBUILDERDELEGATE_H

class QWidget;

class SettingsNode;

class SettingsNodeDialogBuilderDelegate
{
public:
    virtual bool editorNeedLabel( const SettingsNode& node ) const;
    virtual QWidget* createEditor( const SettingsNode& node, QWidget* parent ) const;
    virtual void setEditorData( QWidget* editor, const SettingsNode& node ) const;
    virtual void setNodeData( QWidget* editor, SettingsNode& node ) const;
};

#endif // SETTINGSNODEDIALOGBUILDERDELEGATE_H
