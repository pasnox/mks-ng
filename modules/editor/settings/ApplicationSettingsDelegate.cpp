#include "ApplicationSettingsDelegate.h"
#include "SettingsNode.h"
#include "EolRadioGroup.h"
#include "IndentationRadioGroup.h"
#include "RulerRadioGroup.h"
#include "WrapRadioGroup.h"

#include <QDebug>

bool ApplicationSettingsDelegate::editorNeedLabel( const SettingsNode& node ) const
{
    if ( node.guiType() < int( QVariant::UserType ) ) {
        return SettingsNodeDialogBuilderDelegate::editorNeedLabel( node );
    }
    
    switch ( node.guiType() ) {
        case ApplicationSettingsDelegate::Eol:
        case ApplicationSettingsDelegate::Indent:
        case ApplicationSettingsDelegate::Ruler:
        case ApplicationSettingsDelegate::Wrap:
            return false;
        default:
            qWarning() << Q_FUNC_INFO << "Unhandled type" << node.name() << node.value() << node.guiType() << QVariant::typeToName( QVariant::Type( node.guiType() ) );
            return false;
    }
}

QWidget* ApplicationSettingsDelegate::createEditor( const SettingsNode& node, QWidget* parent ) const
{
    if ( node.guiType() < int( QVariant::UserType ) ) {
        return SettingsNodeDialogBuilderDelegate::createEditor( node, parent );
    }
    
    switch ( node.guiType() ) {
        case ApplicationSettingsDelegate::Eol:
            return new EolRadioGroup( parent );
        case ApplicationSettingsDelegate::Indent:
            return new IndentationRadioGroup( parent );
        case ApplicationSettingsDelegate::Ruler:
            return new RulerRadioGroup( parent );
        case ApplicationSettingsDelegate::Wrap:
            return new WrapRadioGroup( parent );
        default:
            qWarning() << Q_FUNC_INFO << "Unhandled type" << node.name() << node.value() << node.guiType() << QVariant::typeToName( QVariant::Type( node.guiType() ) );
            break;
    }
    
    return 0;
}

void ApplicationSettingsDelegate::setEditorData( QWidget* editor, const SettingsNode& node ) const
{
    if ( node.guiType() < int( QVariant::UserType ) ) {
        SettingsNodeDialogBuilderDelegate::setEditorData( editor, node );
        return;
    }
    
    const QVariant value = node.value();
    editor->setToolTip( node.help() );
    
    switch ( node.guiType() ) {
        case ApplicationSettingsDelegate::Eol:
        case ApplicationSettingsDelegate::Indent:
        case ApplicationSettingsDelegate::Ruler:
        case ApplicationSettingsDelegate::Wrap: {
            RadioGroup* rg = qobject_cast<RadioGroup*>( editor );
            Q_ASSERT( rg );
            rg->setCheckedId( value.toInt() );
            break;
        }
        default:
            qWarning() << Q_FUNC_INFO << "Unhandled type" << node.name() << node.value() << node.guiType() << QVariant::typeToName( QVariant::Type( node.guiType() ) );
            break;
    }
}

void ApplicationSettingsDelegate::setNodeData( QWidget* editor, SettingsNode& node ) const
{
    if ( node.guiType() < int( QVariant::UserType ) ) {
        SettingsNodeDialogBuilderDelegate::setNodeData( editor, node );
        return;
    }
    
    switch ( node.guiType() ) {
        case ApplicationSettingsDelegate::Eol:
        case ApplicationSettingsDelegate::Indent:
        case ApplicationSettingsDelegate::Ruler:
        case ApplicationSettingsDelegate::Wrap: {
            RadioGroup* rg = qobject_cast<RadioGroup*>( editor );
            Q_ASSERT( rg );
            node.setValue( rg->checkedId() );
            break;
        }
        default:
            qWarning() << Q_FUNC_INFO << "Unhandled type" << node.name() << node.value() << node.guiType() << QVariant::typeToName( QVariant::Type( node.guiType() ) );
            break;
    }
}
