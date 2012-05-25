#include "SettingsNodeDialogBuilder.h"
#include "ui_SettingsNodeDialogBuilder.h"
#include "SettingsNodeDialogBuilderDelegate.h"

#include <QToolTip>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QGroupBox>
#include <QPushButton>
#include <QDebug>

SettingsNodeDialogBuilder::SettingsNodeDialogBuilder( QWidget* parent )
    : QDialog( parent ),
        ui( new Ui_SettingsNodeDialogBuilder ),
        mDelegate( 0 )
{
    ui->setupUi( this );
    /*layout()->setMargin( margin() );
    layout()->setSpacing( spacing() );*/
}

SettingsNodeDialogBuilder::~SettingsNodeDialogBuilder()
{
    delete ui;
}

SettingsNode SettingsNodeDialogBuilder::rootNode() const
{
    return mRootNode;
}

SettingsNodeDialogBuilderDelegate* SettingsNodeDialogBuilder::delegate() const
{
    return mDelegate;
}

void SettingsNodeDialogBuilder::setDelegate( SettingsNodeDialogBuilderDelegate* delegate )
{
    mDelegate = delegate;
}

int SettingsNodeDialogBuilder::margin() const
{
    return 6;
}

int SettingsNodeDialogBuilder::spacing() const
{
    return 4;
}

bool SettingsNodeDialogBuilder::build( const SettingsNode& node )
{
    if ( !mRootNode.isNull() ) {
        return mRootNode == node;
    }
    
    if ( !node.type() == SettingsNode::Root ) {
        return false;
    }
    
    mRootNode = node;
    
    SettingsNodeDialogBuilderDelegate* delegate = mDelegate;
    
    if ( !delegate ) {
        delegate = new SettingsNodeDialogBuilderDelegate;
    }
    
    const bool result = recurseBuild( mRootNode, delegate );
    
    if ( !mDelegate ) {
        delete delegate;
    }
    
    if ( result ) {
        ui->twPages->setCurrentItem( ui->twPages->topLevelItem( 0 ) );
        ui->twPages->expandAll();
    }
    
    return result;
}

void SettingsNodeDialogBuilder::addToParentSettingsNode( QWidget* widget, const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate )
{
    Q_UNUSED( delegate );
    
    if ( node.isNull() || node.parent().isNull() ) {
        return;
    }
    
    if ( node.type() & SettingsNode::PageFlag ) {
        ui->swPages->addWidget( widget );
    }
    else {
        QWidget* parentWidget = mEditors.value( node.parent() );
        Q_ASSERT( parentWidget );
        parentWidget->layout()->addWidget( widget );
    }
}

QWidget* SettingsNodeDialogBuilder::createSettingsNodeEditor( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate )
{
    Q_ASSERT( delegate );
    
    if ( node.isNull() ) {
        return 0;
    }
    
    QWidget* editor = delegate->createEditor( node, this );
    
    if ( editor ) {
        mEditors[ node ] = editor;
        delegate->setEditorData( editor, node );
    }
    
    return editor;
}

QBoxLayout* SettingsNodeDialogBuilder::createSettingsNodeLayout( QWidget* widget, const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate )
{
    Q_ASSERT( delegate );
    
    if ( node.isNull() ) {
        return 0;
    }
    
    QBoxLayout* layout = 0;
    
    if ( node.type() & SettingsNode::VerticalFlag ) {
        layout = new QVBoxLayout( widget );
    }
    else if ( node.type() & SettingsNode::HorizontalFlag ) {
        layout = new QHBoxLayout( widget );
    }
    else {
        return 0;
    }
    
    if ( widget->inherits( "QFrame" ) ) {
        layout->setMargin( 0 );
        layout->setSpacing( spacing() );
    }
    else  {
        layout->setMargin( margin() );
        layout->setSpacing( spacing() );
    }
    
    return layout;
}

QWidget* SettingsNodeDialogBuilder::createSettingsNodePage( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate )
{
    if ( node.isNull() ) {
        return 0;
    }
    
    QTreeWidgetItem* parentItem = mItems.value( node.parent() );
    QTreeWidgetItem* item = new QTreeWidgetItem( parentItem, mItems.count() );
    
    item->setIcon( 0, node.icon() );
    item->setText( 0, node.label() );
    item->setToolTip( 0, node.help() );
    
    if ( !parentItem ) {
        ui->twPages->addTopLevelItem( item );
    }
    
    QWidget* page = new QWidget( this );
    page->setToolTip( node.help() );
    createSettingsNodeLayout( page, node, delegate );
    addToParentSettingsNode( page, node, delegate );
    
    mItems[ node ] = item;
    mEditors[ node ] = page;
    
    return page;
}

QWidget* SettingsNodeDialogBuilder::createSettingsNodeGroup( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate )
{
    if ( node.isNull() ) {
        return 0;
    }
    
    QWidget* widget = 0;
    
    if ( node.label().isEmpty() ) {
        QFrame* frame = new QFrame( this );
        widget = frame;
    }
    else {
        QGroupBox* group = new QGroupBox( this );
        group->setTitle( node.label() );
        widget = group;
    }
    
    createSettingsNodeLayout( widget, node, delegate );
    addToParentSettingsNode( widget, node, delegate );
    
    mEditors[ node ] = widget;
    
    return widget;
}

QWidget* SettingsNodeDialogBuilder::createSettingsNodeValue( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate )
{
    if ( node.isNull() ) {
        return 0;
    }
    
    QWidget* editor = createSettingsNodeEditor( node, delegate );
    
    if ( !editor ) {
        return false;
    }
    
    if ( delegate->editorNeedLabel( node ) ) {
        QLabel* label = new QLabel( this );
        label->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred );
        label->setText( node.label() );
        label->setToolTip( node.help() );
        
        QWidget* widget = new QWidget( this );
        QBoxLayout* layout = createSettingsNodeLayout( widget, node, delegate );
        layout->setMargin( 0 );
        layout->addWidget( label );
        layout->addWidget( editor );
        
        editor = widget;
    }
    
    addToParentSettingsNode( editor, node, delegate );
    
    return editor;
}

QWidget* SettingsNodeDialogBuilder::createSettingsNodeGroupValue( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate )
{
    Q_ASSERT( delegate );
    
    if ( node.isNull() ) {
        return 0;
    }
    
    QWidget* editor = createSettingsNodeEditor( node, delegate );
    
    if ( !editor ) {
        return false;
    }
    
    createSettingsNodeLayout( editor, node, delegate );
    addToParentSettingsNode( editor, node, delegate );
    
    return editor;
}

bool SettingsNodeDialogBuilder::recurseBuild( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate )
{
    switch ( node.type() ) {
        case SettingsNode::Root:
            setWindowIcon( node.icon() );
            setWindowTitle( node.label() );
            break;
        case SettingsNode::VerticalPage:
        case SettingsNode::HorizontalPage:
            if ( !createSettingsNodePage( node, delegate ) ) {
                return false;
            }
            break;
        case SettingsNode::VerticalGroup:
        case SettingsNode::HorizontalGroup:
            if ( !createSettingsNodeGroup( node, delegate ) ) {
                return false;
            }
            break;
        case SettingsNode::VerticalValue:
        case SettingsNode::HorizontalValue:
            return createSettingsNodeValue( node, delegate );
        case SettingsNode::VerticalGroupValue:
        case SettingsNode::HorizontalGroupValue:
            if ( !createSettingsNodeGroupValue( node, delegate ) ) {
                return false;
            }
            break;
        case SettingsNode::Invalid:
        default:
            return false;
    }
    
    foreach ( const SettingsNode& child, node.children() ) {
        if ( !recurseBuild( child, delegate ) ) {
            return false;
        }
    }
    
    if ( node.type() & SettingsNode::PageFlag ) {
        QWidget* widget = mEditors[ node ];
        Q_ASSERT( widget );
        QBoxLayout* layout = qobject_cast<QBoxLayout*>( widget->layout() );
        layout->addStretch();
    }
    
    return true;
}

void SettingsNodeDialogBuilder::on_twPages_itemSelectionChanged()
{
    const QTreeWidgetItem* item = ui->twPages->selectedItems().value( 0 );
    ui->lTitle->setText( item ? item->text( 0 ) : QString::null );
    ui->lTitleIcon->setPixmap( item ? item->icon( 0 ).pixmap( QSize( ui->twPages->iconSize() ) ) : QPixmap() );
    ui->swPages->setCurrentIndex( item->type() );
    
    const bool enabled = ui->swPages->currentIndex() != -1 && !ui->swPages->currentWidget()->toolTip().isEmpty();
    ui->dbbButtons->button( QDialogButtonBox::Help )->setEnabled( enabled );
}

void SettingsNodeDialogBuilder::on_dbbButtons_clicked( QAbstractButton* button )
{
    const QDialogButtonBox::StandardButton type = ui->dbbButtons->standardButton( button );
    
    switch ( type ) {
        case QDialogButtonBox::Help: {
            QWidget* page = ui->swPages->currentWidget();
            QToolTip::showText( geometry().topLeft(), page->toolTip(), page );
            break;
        }
        case QDialogButtonBox::Reset: {
            SettingsNodeDialogBuilderDelegate* delegate = mDelegate;
            
            if ( !delegate ) {
                delegate = new SettingsNodeDialogBuilderDelegate;
            }
            
            foreach ( const SettingsNode& node, mEditors.keys() ) {
                if ( node.type() & SettingsNode::ValueFlag ) {
                    delegate->setEditorData( mEditors[ node ], node );
                }
            }
            
            if ( !mDelegate ) {
                delete delegate;
            }
            
            break;
        }
        case QDialogButtonBox::Ok:
            on_dbbButtons_clicked( ui->dbbButtons->button( QDialogButtonBox::Apply ) );
            accept();
            break;
        case QDialogButtonBox::Apply: {
            SettingsNodeDialogBuilderDelegate* delegate = mDelegate;
            
            if ( !delegate ) {
                delegate = new SettingsNodeDialogBuilderDelegate;
            }
            
            foreach ( SettingsNode node, mEditors.keys() ) {
                if ( node.type() & SettingsNode::ValueFlag ) {
                    delegate->setNodeData( mEditors[ node ], node );
                }
            }
            
            if ( !mDelegate ) {
                delete delegate;
            }
            
            break;
        }
        case QDialogButtonBox::Cancel:
            reject();
            break;
        default:
            Q_ASSERT( 0 );
            break;
    }
}
