#ifndef SETTINGSNODEDIALOGBUILDER_H
#define SETTINGSNODEDIALOGBUILDER_H

#include <QDialog>

class SettingsNode;
class SettingsNodeDialogBuilderDelegate;

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
    virtual bool boldGroupTitle() const;
    
    bool build( const SettingsNode& node );

private:
    class SettingsNodeDialogBuilderPrivate* d;
};

#endif // SETTINGSNODEDIALOGBUILDER_H
