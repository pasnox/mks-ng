#include "SettingsNodeDialogBuilder.h"
#include "ui_SettingsNodeDialogBuilder.h"
#include "SettingsNodeDialogBuilderDelegate.h"
#include "SettingsNode.h"

#include <QToolTip>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPushButton>
#include <QGroupBox>
#include <QDebug>

// SettingsNodeDialogBuilderPrivate

class SettingsNodeDialogBuilderPrivate : public QObject
{
    Q_OBJECT

public:
    SettingsNodeDialogBuilderPrivate( SettingsNodeDialogBuilder* builder );
    ~SettingsNodeDialogBuilderPrivate();
    
    bool build( const SettingsNode& node );
    
    void addToParentSettingsNode( QWidget* widget, const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate );
    QWidget* createSettingsNodeEditor( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate );
    QBoxLayout* createSettingsNodeLayout( QWidget* widget, const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate );
    QWidget* createSettingsNodePage( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate );
    QWidget* createSettingsNodeGroup( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate );
    QWidget* createSettingsNodeValue( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate );
    QWidget* createSettingsNodeGroupValue( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate );
    bool recurseBuild( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate );

public slots:
    void twPages_itemSelectionChanged();
    void dbbButtons_clicked( QAbstractButton* button );

public:
    SettingsNodeDialogBuilder* mBuilder;
    Ui_SettingsNodeDialogBuilder* ui;
    SettingsNode mRootNode;
    SettingsNodeDialogBuilderDelegate* mDelegate;
    QHash<SettingsNode, QTreeWidgetItem*> mItems;
    QHash<SettingsNode, QWidget*> mEditors;
};

SettingsNodeDialogBuilderPrivate::SettingsNodeDialogBuilderPrivate( SettingsNodeDialogBuilder* builder )
    :
        mBuilder( builder ),
        ui( new Ui_SettingsNodeDialogBuilder ),
        mDelegate( 0 )
{
    ui->setupUi( builder );
    
    connect( ui->twPages, &QTreeWidget::itemSelectionChanged, this, &SettingsNodeDialogBuilderPrivate::twPages_itemSelectionChanged );
    connect( ui->dbbButtons, &QDialogButtonBox::clicked, this, &SettingsNodeDialogBuilderPrivate::dbbButtons_clicked );
}

SettingsNodeDialogBuilderPrivate::~SettingsNodeDialogBuilderPrivate()
{
    delete ui;
}

bool SettingsNodeDialogBuilderPrivate::build( const SettingsNode& node )
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

void SettingsNodeDialogBuilderPrivate::addToParentSettingsNode( QWidget* widget, const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate )
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

QWidget* SettingsNodeDialogBuilderPrivate::createSettingsNodeEditor( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate )
{
    Q_ASSERT( delegate );
    
    if ( node.isNull() ) {
        return 0;
    }
    
    QWidget* editor = delegate->createEditor( node, mBuilder );
    
    if ( editor ) {
        mEditors[ node ] = editor;
        delegate->setEditorData( editor, node );
    }
    
    return editor;
}

QBoxLayout* SettingsNodeDialogBuilderPrivate::createSettingsNodeLayout( QWidget* widget, const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate )
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
        layout->setSpacing( mBuilder->spacing() );
    }
    else  {
        layout->setMargin( mBuilder->margin() );
        layout->setSpacing( mBuilder->spacing() );
    }
    
    return layout;
}

QWidget* SettingsNodeDialogBuilderPrivate::createSettingsNodePage( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate )
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
    
    QWidget* page = new QWidget( mBuilder );
    page->setToolTip( node.help() );
    createSettingsNodeLayout( page, node, delegate );
    addToParentSettingsNode( page, node, delegate );
    
    mItems[ node ] = item;
    mEditors[ node ] = page;
    
    return page;
}

QWidget* SettingsNodeDialogBuilderPrivate::createSettingsNodeGroup( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate )
{
    if ( node.isNull() ) {
        return 0;
    }
    
    QWidget* widget = 0;
    
    if ( node.label().isEmpty() ) {
        QFrame* frame = new QFrame( mBuilder );
        widget = frame;
    }
    else {
        QGroupBox* group = new QGroupBox( mBuilder );
        group->setTitle( node.label() );
        widget = group;
    }
    
    createSettingsNodeLayout( widget, node, delegate );
    addToParentSettingsNode( widget, node, delegate );
    
    mEditors[ node ] = widget;
    
    return widget;
}

QWidget* SettingsNodeDialogBuilderPrivate::createSettingsNodeValue( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate )
{
    if ( node.isNull() ) {
        return 0;
    }
    
    QWidget* editor = createSettingsNodeEditor( node, delegate );
    
    if ( !editor ) {
        return false;
    }
    
    if ( delegate->editorNeedLabel( node ) ) {
        QLabel* label = new QLabel( mBuilder );
        label->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred );
        label->setText( node.label() );
        label->setToolTip( node.help() );
        
        QWidget* widget = new QWidget( mBuilder );
        QBoxLayout* layout = createSettingsNodeLayout( widget, node, delegate );
        layout->setMargin( 0 );
        layout->addWidget( label );
        layout->addWidget( editor );
        
        editor = widget;
    }
    
    addToParentSettingsNode( editor, node, delegate );
    
    return editor;
}

QWidget* SettingsNodeDialogBuilderPrivate::createSettingsNodeGroupValue( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate )
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

bool SettingsNodeDialogBuilderPrivate::recurseBuild( const SettingsNode& node, SettingsNodeDialogBuilderDelegate* delegate )
{
    switch ( node.type() ) {
        case SettingsNode::Root:
            mBuilder->setWindowIcon( node.icon() );
            mBuilder->setWindowTitle( node.label() );
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

void SettingsNodeDialogBuilderPrivate::twPages_itemSelectionChanged()
{
    const QTreeWidgetItem* item = ui->twPages->selectedItems().value( 0 );
    ui->lTitle->setText( item ? item->text( 0 ) : QString::null );
    ui->lTitleIcon->setPixmap( item ? item->icon( 0 ).pixmap( QSize( ui->twPages->iconSize() ) ) : QPixmap() );
    ui->swPages->setCurrentIndex( item->type() );
    
    const bool enabled = ui->swPages->currentIndex() != -1 && !ui->swPages->currentWidget()->toolTip().isEmpty();
    ui->dbbButtons->button( QDialogButtonBox::Help )->setEnabled( enabled );
}

void SettingsNodeDialogBuilderPrivate::dbbButtons_clicked( QAbstractButton* button )
{
    const QDialogButtonBox::StandardButton type = ui->dbbButtons->standardButton( button );
    
    switch ( type ) {
        case QDialogButtonBox::Help: {
            QWidget* page = ui->swPages->currentWidget();
            QToolTip::showText( mBuilder->geometry().topLeft(), page->toolTip(), page );
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
            dbbButtons_clicked( ui->dbbButtons->button( QDialogButtonBox::Apply ) );
            mBuilder->accept();
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
            mBuilder->reject();
            break;
        default:
            Q_ASSERT( 0 );
            break;
    }
}


// SettingsNodeDialogBuilder

SettingsNodeDialogBuilder::SettingsNodeDialogBuilder( QWidget* parent )
    : QDialog( parent ),
        d( new SettingsNodeDialogBuilderPrivate( this ) )
{
    /*layout()->setMargin( margin() );
    layout()->setSpacing( spacing() );*/
}

SettingsNodeDialogBuilder::~SettingsNodeDialogBuilder()
{
    delete d;
}

SettingsNode SettingsNodeDialogBuilder::rootNode() const
{
    return d->mRootNode;
}

SettingsNodeDialogBuilderDelegate* SettingsNodeDialogBuilder::delegate() const
{
    return d->mDelegate;
}

void SettingsNodeDialogBuilder::setDelegate( SettingsNodeDialogBuilderDelegate* delegate )
{
    d->mDelegate = delegate;
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
    return d->build( node );
}

#include "SettingsNodeDialogBuilder.moc"
