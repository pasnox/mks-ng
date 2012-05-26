#include "SettingsNodeDialogBuilderDelegate.h"
#include "SettingsNode.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QDebug>

#include <FreshGui/pColorButton>
#include <FreshGui/pFontButton>

bool SettingsNodeDialogBuilderDelegate::editorNeedLabel( const SettingsNode& node ) const
{
    switch ( node.guiType() ) {
        case QVariant::Bool:
            return node.type() & SettingsNode::GroupFlag ? true : false;
        default:
            return true;
    }
}

QWidget* SettingsNodeDialogBuilderDelegate::createEditor( const SettingsNode& node, QWidget* parent ) const
{
    switch ( node.guiType() ) {
        case QVariant::Color:
            return new pColorButton( parent );
        case QVariant::Bool:
            if ( node.type() & SettingsNode::GroupFlag ) {
                return new QGroupBox( parent );
            }
            else {
                return new QCheckBox( parent );
            }
        case QVariant::Font:
            return new pFontButton( parent );
        case QVariant::Int:
            return new QSpinBox( parent );
        case QVariant::Double:
            return new QDoubleSpinBox( parent );
        case QVariant::String:
            if ( node.guiData().toStringList().isEmpty() ) {
                return new QLineEdit( parent );
            }
            else {
                return new QComboBox( parent );
            }
    }
    
    qWarning() << Q_FUNC_INFO << "Unhandled type" << node.name() << node.value() << node.guiType() << QVariant::typeToName( QVariant::Type( node.guiType() ) );
    
    return 0;
}

void SettingsNodeDialogBuilderDelegate::setEditorData( QWidget* editor, const SettingsNode& node ) const
{
    const QVariant value = node.value();
    editor->setToolTip( node.help() );
    
    switch ( node.guiType() ) {
        case QVariant::Color: {
            pColorButton* button = qobject_cast<pColorButton*>( editor );
            Q_ASSERT( button );
            button->setAlphaEnabled( node.guiData().toBool() );
            button->setColor( value.value<QColor>() );
            break;
        }
        case QVariant::Bool: {
            if ( node.type() & SettingsNode::GroupFlag ) {
                QGroupBox* gb = qobject_cast<QGroupBox*>( editor );
                Q_ASSERT( gb );
                gb->setTitle( node.label() );
                gb->setCheckable( true );
                gb->setChecked( value.toBool() );
            }
            else {
                QCheckBox* cb = qobject_cast<QCheckBox*>( editor );
                Q_ASSERT( cb );
                cb->setText( node.label() );
                cb->setChecked( value.toBool() );
            }
            break;
        }
        case QVariant::Font: {
            pFontButton* button = qobject_cast<pFontButton*>( editor );
            Q_ASSERT( button );
            button->setFont( value.value<QFont>() );
            break;
        }
        case QVariant::Int: {
            QSpinBox* sb = qobject_cast<QSpinBox*>( editor );
            const PairInt pair = node.guiData().value<PairInt>();
            Q_ASSERT( sb );
            sb->setRange( pair.first, pair.second );
            sb->setValue( value.toInt() );
            break;
        }
        case QVariant::Double: {
            QDoubleSpinBox* sb = qobject_cast<QDoubleSpinBox*>( editor );
            const PairDouble pair = node.guiData().value<PairDouble>();
            Q_ASSERT( sb );
            sb->setRange( pair.first, pair.second );
            sb->setValue( value.toInt() );
            break;
        }
        case QVariant::String: {
            const QStringList list = node.guiData().toStringList();
            if ( list.isEmpty() ) {
                QLineEdit* le = qobject_cast<QLineEdit*>( editor );
                Q_ASSERT( le );
                le->setText( value.toString() );
            }
            else {
                QComboBox* cb = qobject_cast<QComboBox*>( editor );
                Q_ASSERT( cb );
                cb->addItems( list );
                cb->setCurrentIndex( cb->findText( value.toString() ) );
            }
            break;
        }
        default:
            qWarning() << Q_FUNC_INFO << "Unhandled type" << node.name() << node.value() << node.guiType() << QVariant::typeToName( QVariant::Type( node.guiType() ) );
            break;
    }
}

void SettingsNodeDialogBuilderDelegate::setNodeData( QWidget* editor, SettingsNode& node ) const
{
    switch ( node.guiType() ) {
        case QVariant::Color: {
            pColorButton* button = qobject_cast<pColorButton*>( editor );
            Q_ASSERT( button );
            node.setValue( button->color() );
            break;
        }
        case QVariant::Bool: {
            if ( node.type() & SettingsNode::GroupFlag ) {
                QGroupBox* gb = qobject_cast<QGroupBox*>( editor );
                Q_ASSERT( gb );
                node.setValue( gb->isChecked() );
            }
            else {
                QCheckBox* cb = qobject_cast<QCheckBox*>( editor );
                Q_ASSERT( cb );
                node.setValue( cb->isChecked() );
            }
            break;
        }
        case QVariant::Font: {
            pFontButton* button = qobject_cast<pFontButton*>( editor );
            Q_ASSERT( button );
            node.setValue( button->font() );
            break;
        }
        case QVariant::Int: {
            QSpinBox* sb = qobject_cast<QSpinBox*>( editor );
            Q_ASSERT( sb );
            node.setValue( sb->value() );
            break;
        }
        case QVariant::Double: {
            QDoubleSpinBox* sb = qobject_cast<QDoubleSpinBox*>( editor );
            Q_ASSERT( sb );
            node.setValue( sb->value() );
            break;
        }
        case QVariant::String: {
            if ( node.guiData().toStringList().isEmpty() ) {
                QLineEdit* le = qobject_cast<QLineEdit*>( editor );
                Q_ASSERT( le );
                node.setValue( le->text() );
            }
            else {
                QComboBox* cb = qobject_cast<QComboBox*>( editor );
                Q_ASSERT( cb );
                node.setValue( cb->itemText( cb->currentIndex() ) );
            }
            break;
        }
        default:
            qWarning() << Q_FUNC_INFO << "Unhandled type" << node.name() << node.value() << node.guiType() << QVariant::typeToName( QVariant::Type( node.guiType() ) );
            break;
    }
}
