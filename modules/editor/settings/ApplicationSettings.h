#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include "SettingsNode.h"

#include <QObject>

class GeneralSettings : public SettingsNode
{
public:
    GeneralSettings();

    SettingsNode saveSession;
    SettingsNode restoreSession;
    SettingsNode saveDocumentsOnActionTriggered;
};

class EditorSettings : public SettingsNode
{
public:
    EditorSettings();
    
    class Margins : public SettingsNode {
    public:
        Margins();
        
        SettingsNode lineNumber;
        SettingsNode fold;
        SettingsNode symbol;
        SettingsNode lineState;
    };
    
    class Documents : public SettingsNode {
    public:
        Documents();
        
        SettingsNode codec;
        SettingsNode font;
        SettingsNode indentType;
        SettingsNode indentWidth;
        SettingsNode tabWidth;
        SettingsNode eol;
    };
    
    class Colors : public SettingsNode {
    public:
        Colors();
        
        SettingsNode paper;
        SettingsNode pen;
        SettingsNode selectionBackground;
        SettingsNode selectionForeground;
        SettingsNode lineBackground;
        SettingsNode lineForeground;
    };
    
    SettingsNode detectEol;
    SettingsNode convertEol;
    SettingsNode detectIndentation;
    SettingsNode convertIndentation;
    
    Margins margins;
    Documents documents;
    Colors colors;
};

class ApplicationSettings : public QObject, public SettingsNode
{
public:
    ApplicationSettings( QObject* parent = 0 );

    GeneralSettings general;
    EditorSettings editor;
};

#endif // APPLICATIONSETTINGS_H
