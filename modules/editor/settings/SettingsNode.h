#ifndef SETTINGSNODE_H
#define SETTINGSNODE_H

#include <QExplicitlySharedDataPointer>
#include <QVariant>
#include <QIcon>
#include <QStringList>

class QSettings;

class SettingsNode
{
public:
    enum TypeFlag {
        PageFlag = 0x1,
        GroupFlag = 0x2,
        ValueFlag = 0x4,
        VerticalFlag = 0x8,
        HorizontalFlag = 0x10
    };
    
    enum Type {
        Invalid = -1,
        Root = 0,
        VerticalPage = VerticalFlag | PageFlag,
        HorizontalPage = HorizontalFlag | PageFlag,
        VerticalGroup = VerticalFlag | GroupFlag,
        HorizontalGroup = HorizontalFlag | GroupFlag,
        VerticalValue = VerticalFlag | ValueFlag,
        HorizontalValue = HorizontalFlag | ValueFlag,
        VerticalGroupValue = VerticalFlag | GroupFlag | ValueFlag,
        HorizontalGroupValue = HorizontalFlag | GroupFlag | ValueFlag
    };
    
    typedef QList<SettingsNode> List;
    
    SettingsNode();
    SettingsNode( int type, int guiType = -1 );
    SettingsNode( int type, const QString& label );
    SettingsNode( const SettingsNode& other );
    virtual ~SettingsNode();
    
    bool operator==( const SettingsNode& other ) const;
    bool operator!=( const SettingsNode& other ) const;
    operator QVariant() const;
    SettingsNode& operator=( const SettingsNode& other );
    
    bool isNull() const;
    
    SettingsNode parent() const;
    void setParent( const SettingsNode& parent );
    
    int type() const;
    void setType( int type );
    
    QString label() const;
    void setLabel( const QString& label );
    
    QString help() const;
    void setHelp( const QString& help );
    
    QString name() const;
    void setName( const QString& name );
    
    QVariant value() const;
    void setValue( const QVariant& value );
    
    QVariant guiData() const;
    void setGuiData( const QVariant& guiData );
    
    int guiType() const;
    void setGuiType( int guiType );
    
    QIcon icon() const;
    void setIcon( const QIcon& icon );
    
    SettingsNode::List children() const;
    void setChildren( const SettingsNode::List& children );
    
    void addChild( const SettingsNode& child );
    void removeChild( const SettingsNode& child );
    
    void save( QSettings* settings );
    void load( QSettings* settings );
    
    // helpers
    void setTypes( int type, int guiType = -1 );
    void setContent( const QString& name, const QVariant& value );
    void setGuiContent( const QString& label, const QIcon& icon );
    
    SettingsNode addChild( int type, int guiType = -1 );
    
    SettingsNode addVPage( const QString& label, const QIcon& icon );
    SettingsNode addHPage( const QString& label, const QIcon& icon );
    
    SettingsNode addVGroup( const QString& label, const QString& name );
    SettingsNode addHGroup( const QString& label, const QString& name );
    
    SettingsNode addVValue( const QString& name, const QVariant& value, int guiType );
    SettingsNode addHValue( const QString& name, const QVariant& value, int guiType );

private:
    QExplicitlySharedDataPointer<class SettingsNodePrivate> d;
};

uint qHash( const SettingsNode& node );

#define Q_DECLARE_QVARIANT_OPERATOR \
    operator QVariant() const { return QVariant::fromValue( *this ); }

#define Q_DECLARE_QVARIANT_CLASS( CLASS_NAME, PARENT_CLASS_NAME ) \
    class CLASS_NAME : public PARENT_CLASS_NAME {   \
    public: Q_DECLARE_QVARIANT_OPERATOR \
        CLASS_NAME( const PARENT_CLASS_NAME& param = PARENT_CLASS_NAME() ) : PARENT_CLASS_NAME( param ) {} \
    }; Q_DECLARE_METATYPE( CLASS_NAME )

typedef QPair<int, int> PairIntPrivate;
typedef QPair<double, double> PairDoublePrivate;

Q_DECLARE_QVARIANT_CLASS( PairInt, PairIntPrivate )
Q_DECLARE_QVARIANT_CLASS( PairDouble, PairDoublePrivate )

#endif // SETTINGSNODE_H
