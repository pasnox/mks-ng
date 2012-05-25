#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include "SettingsNode.h"

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
        
        SettingsNode paper;
        SettingsNode pen;
        SettingsNode font;
        SettingsNode indentWidth;
        SettingsNode tabWidth;
    };
    
    class Colors : public SettingsNode {
    public:
        Colors();
        
        SettingsNode selectionBackground;
        SettingsNode selectionForeground;
        SettingsNode lineBackground;
        SettingsNode lineForeground;
    };
    
    Margins margins;
    Documents documents;
    Colors colors;
};

class ApplicationSettings : public SettingsNode
{
public:
    enum ExtendedVariantType {
        FirstExtendedType = QVariant::UserType,
        Eol,
        Indent,
        Ruler,
        TextEncoding,
        LineWrap
    };
    
    ApplicationSettings();

    GeneralSettings general;
    EditorSettings editor;
};

#endif // APPLICATIONSETTINGS_H
