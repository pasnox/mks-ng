#ifndef APPLICATIONSETTINGSDELEGATE_H
#define APPLICATIONSETTINGSDELEGATE_H

#include "SettingsNodeDialogBuilderDelegate.h"

#include <QVariant>

class ApplicationSettingsDelegate : public SettingsNodeDialogBuilderDelegate
{
public:
    enum ExtendedVariantType {
        Eol = QVariant::UserType,
        Indent,
        Ruler,
        Wrap
    };
    
    virtual bool editorNeedLabel( const SettingsNode& node ) const;
    virtual QWidget* createEditor( const SettingsNode& node, QWidget* parent ) const;
    virtual void setEditorData( QWidget* editor, const SettingsNode& node ) const;
    virtual void setNodeData( QWidget* editor, SettingsNode& node ) const;
};

#endif // APPLICATIONSETTINGSDELEGATE_H
