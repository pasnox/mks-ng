#include "SettingsNode.h"

#include <QSharedData>
#include <QSettings>
#include <QDebug>

uint qHash( const SettingsNode& node )
{
    return qHash( node.name() );
}

// SettingsNodePrivate

class SettingsNodePrivate : public QSharedData
{
public:
    SettingsNodePrivate()
        : QSharedData(),
            type( -1 ), name(  ),
            label(  ), help(  ),
            value(  ), guiType(  ),
            icon(  ), parent(  ),
            guiData(  ), children(  )
    {
    }
    
    SettingsNodePrivate( int _type, int _guiType )
        : QSharedData(),
            type( _type ), name(  ),
            label(  ), help(  ),
            value(  ), guiType( _guiType ),
            icon(  ), parent(  ),
            guiData(  ), children(  )
    {
    }
    
    SettingsNodePrivate( const SettingsNodePrivate& other )
        : QSharedData( other ),
            type( other.type ), name( other.name ),
            label( other.label ), help( other.help ),
            value( other.value ), guiType( other.guiType ),
            icon( other.icon ), parent( other.parent ),
            guiData( other.guiData ), children( other.children )
    {
    }
    
    QString settingsKey() const {
        QStringList parts;
        
        if ( name.isEmpty() ) {
            return QString::null;
        }
        else {
            parts.prepend( name );
        }
        
        SettingsNode node = parent;
        QString part;
        
        while ( !node.isNull() ) {
            part = node.name();
            
            if ( !part.isEmpty() ) {
                parts.prepend( part );
            }
            
            node = node.parent();
        }
        
        // general keyword as begin of path is considered internal to qt, rename it.
        if ( !parts.isEmpty() && parts.first().compare( "general", Qt::CaseInsensitive ) == 0 ) {
            parts.first() = "sn_general";
        }
        
        return parts.join( "/" );
    }
    
    int type;
    QString name;
    QString label;
    QString help;
    QVariant value;
    int guiType;
    QIcon icon;
    SettingsNode parent;
    QVariant guiData;
    SettingsNode::List children;
};

// SettingsNode

SettingsNode::SettingsNode()
    : d( 0 )
{
}

SettingsNode::SettingsNode( int type, int guiType )
    : d( new SettingsNodePrivate( type, guiType ) )
{
}

SettingsNode::SettingsNode( int type, const QString& label )
    : d( new SettingsNodePrivate( type, -1 ) )
{
    setLabel( label );
}

SettingsNode::SettingsNode( const SettingsNode& other )
    : d( other.d )
{
}

SettingsNode::~SettingsNode()
{
}

bool SettingsNode::operator==( const SettingsNode& other ) const
{
    return d == other.d;
}

bool SettingsNode::operator!=( const SettingsNode& other ) const
{
    return !operator==( other );
}

SettingsNode& SettingsNode::operator=( const SettingsNode& other )
{
    if ( operator==( other ) ) {
        return *this;
    }
    
    d = other.d;
    return *this;
}

bool SettingsNode::isNull() const
{
    return !d;
}

SettingsNode SettingsNode::parent() const
{
    return d ? d->parent : SettingsNode();
}

void SettingsNode::setParent( const SettingsNode& parent )
{
    if ( d ) {
        d->parent = parent;
    }
}

int SettingsNode::type() const
{
    return d ? d->type : SettingsNode::Invalid;
}

void SettingsNode::setType( int type )
{
    if ( d ) {
        d->type = type;
    }
}

QString SettingsNode::label() const
{
    return d ? d->label : QString::null;
}

void SettingsNode::setLabel( const QString& label )
{
    if ( d ) {
        d->label = label;
    }
}

QString SettingsNode::help() const
{
    return d ? d->help : QString::null;
}

void SettingsNode::setHelp( const QString& help )
{
    if ( d ) {
        d->help = help;
    }
}

QString SettingsNode::name() const
{
    return d ? d->name : QString::null;
}

void SettingsNode::setName( const QString& name )
{
    if ( d ) {
        d->name = name;
    }
}

QVariant SettingsNode::value() const
{
    return d ? d->value : QVariant();
}

void SettingsNode::setValue( const QVariant& value )
{
    if ( d ) {
        d->value = value;
    }
}

QVariant SettingsNode::guiData() const
{
    return d ? d->guiData : QVariant();
}

void SettingsNode::setGuiData( const QVariant& guiData )
{
    if ( d ) {
        d->guiData = guiData;
    }
}

int SettingsNode::guiType() const
{
    return d ? d->guiType : -1;
}

void SettingsNode::setGuiType( int guiType )
{
    if ( d ) {
        d->guiType = guiType;
    }
}

QIcon SettingsNode::icon() const
{
    return d ? d->icon : QIcon();
}

void SettingsNode::setIcon( const QIcon& icon )
{
    if ( d ) {
        d->icon = icon;
    }
}

SettingsNode::List SettingsNode::children() const
{
    return d ? d->children : SettingsNode::List();
}

void SettingsNode::setChildren( const SettingsNode::List& children )
{
    if ( d ) {
        d->children = children;
    }
}

void SettingsNode::addChild( const SettingsNode& child )
{
    if ( d && !child.isNull() ) {
        d->children << child;
        child.d->parent = *this;
    }
}

void SettingsNode::removeChild( const SettingsNode& child )
{
    if ( d && !child.isNull() ) {
        d->children.removeOne( child );
        child.d->parent = SettingsNode();
    }
}

void SettingsNode::save( QSettings* settings )
{
    Q_ASSERT( settings );
    Q_ASSERT( d );
    
    if ( d->type & SettingsNode::ValueFlag && !d->name.isEmpty() ) {
        settings->setValue( d->settingsKey(), d->value );
    }
    
    for ( int i = 0; i < d->children.count(); i++ ) {
        SettingsNode& child = d->children[ i ];
        child.save( settings );
    }
}

void SettingsNode::load( QSettings* settings )
{
    Q_ASSERT( settings );
    Q_ASSERT( d );
    
    if ( d->type & SettingsNode::ValueFlag && !d->name.isEmpty() ) {
        d->value = settings->value( d->settingsKey(), d->value );
    }
    
    for ( int i = 0; i < d->children.count(); i++ ) {
        SettingsNode& child = d->children[ i ];
        child.load( settings );
    }
}

// Helpers

void SettingsNode::setTypes( int type, int guiType )
{
    setType( type );
    setGuiType( guiType );
}

void SettingsNode::setContent( const QString& name, const QVariant& value )
{
    setName( name );
    setValue( value );
}

void SettingsNode::setGuiContent( const QString& label, const QIcon& icon )
{
    setLabel( label );
    setIcon( icon );
}

SettingsNode SettingsNode::addChild( int type, int guiType )
{
    if ( d ) {
        SettingsNode node( type, guiType );
        addChild( node );
        return node;
    }
    
    return SettingsNode();
}

SettingsNode SettingsNode::addVPage( const QString& label, const QIcon& icon )
{
    if ( d ) {
        SettingsNode node( SettingsNode::VerticalPage );
        node.setGuiContent( label, icon );
        addChild( node );
        return node;
    }
    
    return SettingsNode();
}

SettingsNode SettingsNode::addHPage( const QString& label, const QIcon& icon )
{
    if ( d ) {
        SettingsNode node( SettingsNode::HorizontalPage );
        node.setGuiContent( label, icon );
        addChild( node );
        return node;
    }
    
    return SettingsNode();
}

SettingsNode SettingsNode::addVGroup( const QString& label, const QString& name )
{
    if ( d ) {
        SettingsNode node( SettingsNode::VerticalGroup, -1 );
        node.setName( name );
        node.setLabel( label );
        addChild( node );
        return node;
    }
    
    return SettingsNode();
}

SettingsNode SettingsNode::addHGroup( const QString& label, const QString& name )
{
    if ( d ) {
        SettingsNode node( SettingsNode::HorizontalGroup, -1 );
        node.setName( name );
        node.setLabel( label );
        addChild( node );
        return node;
    }
    
    return SettingsNode();
}

SettingsNode SettingsNode::addVValue( const QString& name, const QVariant& value, int guiType )
{
    if ( d ) {
        SettingsNode node( SettingsNode::VerticalValue, guiType );
        node.setContent( name, value );
        addChild( node );
        return node;
    }
    
    return SettingsNode();
}

SettingsNode SettingsNode::addHValue( const QString& name, const QVariant& value, int guiType )
{
    if ( d ) {
        SettingsNode node( SettingsNode::HorizontalValue, guiType );
        node.setContent( name, value );
        addChild( node );
        return node;
    }
    
    return SettingsNode();
}
