#ifndef SETTINGSNODEDIALOGBUILDER_H
#define SETTINGSNODEDIALOGBUILDER_H

#include <QDialog>

class SettingsNode;
class SettingsNodeDialogBuilderDelegate;

class SettingsNodeDialogBuilder : public QDialog
{
    Q_OBJECT
#if !defined( HAS_QT_5 )
    friend class SettingsNodeDialogBuilderPrivate;
#endif
    
public:
    SettingsNodeDialogBuilder( QWidget* parent = 0 );
    virtual ~SettingsNodeDialogBuilder();
    
    SettingsNode rootNode() const;
    
    SettingsNodeDialogBuilderDelegate* delegate() const;
    void setDelegate( SettingsNodeDialogBuilderDelegate* delegate );
    
    virtual int margin() const;
    virtual int spacing() const;
    virtual bool boldGroupTitle() const;
    
    bool build( const SettingsNode& node );

private:
    class SettingsNodeDialogBuilderPrivate* d;

signals:
    void applyClicked();
};

#endif // SETTINGSNODEDIALOGBUILDER_H
