#include "ApplicationSettings.h"
#include "Document.h"

// General Settings

GeneralSettings::GeneralSettings()
    : SettingsNode( SettingsNode::VerticalPage )
{
    setName( "general" );
    setGuiContent( QT_TRANSLATE_NOOP( "ApplicationSettings", "General" ), QIcon::fromTheme( "applications-other" ) );
    setHelp( QT_TRANSLATE_NOOP( "ApplicationSettings", "General help" ) );
    
    SettingsNode sessionGroup = addHGroup( QT_TRANSLATE_NOOP( "ApplicationSettings", "Session" ), "sessions" );
    SettingsNode documentsGroup = addHGroup( QT_TRANSLATE_NOOP( "ApplicationSettings", "Documents" ), "documents" );
    
    saveSession = sessionGroup.addHValue( "saveSession", true, QVariant::Bool );
    saveSession.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Save on close" ) );
    
    restoreSession = sessionGroup.addHValue( "restoreSession", true, QVariant::Bool );
    restoreSession.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Restore upon startup" ) );
    
    saveDocumentsOnActionTriggered = documentsGroup.addHValue( "saveDocumentsOnActionTriggered", true, QVariant::Bool );
    saveDocumentsOnActionTriggered.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Save on custom menu action triggered" ) );
}

// EditorSettings

EditorSettings::Margins::Margins()
    : SettingsNode( SettingsNode::VerticalPage )
{
    setName( "margins" );
    setGuiContent( QT_TRANSLATE_NOOP( "ApplicationSettings", "Margins" ), QIcon::fromTheme( "distribute-vertical-margin" ) );
    setHelp( QT_TRANSLATE_NOOP( "ApplicationSettings", "Editor margins help" ) );
    
    lineNumber = addHValue( "lineNumber", true, QVariant::Bool );
    lineNumber.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Line number" ) );
    
    fold = addHValue( "fold", true, QVariant::Bool );
    fold.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Fold" ) );
    
    symbol = addHValue( "symbol", true, QVariant::Bool );
    symbol.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Symbol" ) );
    
    lineState = addHValue( "lineState", true, QVariant::Bool );
    lineState.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Line state" ) );
}

EditorSettings::Documents::Documents()
    : SettingsNode( SettingsNode::VerticalPage )
{
    setName( "documents" );
    setGuiContent( QT_TRANSLATE_NOOP( "ApplicationSettings", "Documents" ), QIcon::fromTheme( "document-properties" ) );
    setHelp( QT_TRANSLATE_NOOP( "ApplicationSettings", "Editor documents help" ) );
    
    paper = addHValue( "paper", QColor( 255, 255, 255, 255 ), QVariant::Color );
    paper.setGuiData( true );
    paper.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Paper" ) );
    
    pen = addHValue( "pen", QColor( 0, 0, 0, 255 ), QVariant::Color );
    pen.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Pen" ) );
    
    font = addHValue( "font", QFont(), QVariant::Font );
    font.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Font" ) );
    
    indentWidth = addHValue( "indentWidth", 4, QVariant::Int );
    indentWidth.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Indent width" ) );
    indentWidth.setGuiData( PairInt( qMakePair( 0, 16 ) ) );
    
    tabWidth = addHValue( "tabWidth", 4, QVariant::Int );
    tabWidth.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Tab width" ) );
    tabWidth.setGuiData( PairInt( qMakePair( 0, 16 ) ) );
}

EditorSettings::Colors::Colors()
    : SettingsNode( SettingsNode::VerticalPage )
{
    setName( "colors" );
    setGuiContent( QT_TRANSLATE_NOOP( "ApplicationSettings", "Colors" ), QIcon::fromTheme( "format-text-color" ) );
    setHelp( QT_TRANSLATE_NOOP( "ApplicationSettings", "Editor colors help" ) );
    
    selectionBackground = addHValue( "selectionBackground", QColor( 189, 255, 155, 255 ), QVariant::Color );
    selectionBackground.setGuiData( true );
    selectionBackground.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Selection background" ) );
    
    selectionForeground = addHValue( "selectionForeground", QColor( 0, 0, 0, 255 ), QVariant::Color );
    selectionForeground.setGuiData( true );
    selectionForeground.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Selection foreground" ) );
    
    lineBackground = addHValue( "lineBackground", QColor( 170, 170, 255, 150 ), QVariant::Color );
    lineBackground.setGuiData( true );
    lineBackground.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Line background" ) );
    
    lineForeground = addHValue( "lineForeground", QColor( 0, 0, 0, 255 ), QVariant::Color );
    lineForeground.setGuiData( true );
    lineForeground.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Line foreground" ) );
}

EditorSettings::EditorSettings()
    : SettingsNode( SettingsNode::VerticalPage )
{
    setName( "editor" );
    setGuiContent( QT_TRANSLATE_NOOP( "ApplicationSettings", "Editor" ), QIcon::fromTheme( "story-editor" ) );
    setHelp( QT_TRANSLATE_NOOP( "ApplicationSettings", "Editor help" ) );
    
    addChild( margins );
    addChild( documents );
    addChild( colors );
    
    /*addHChildValue( ApplicationSettings::Eol, QT_TRANSLATE_NOOP( "ApplicationSettings", "End of line mode" ), Document::Unix );
    addHChildValue( ApplicationSettings::Indent, QT_TRANSLATE_NOOP( "ApplicationSettings", "Indent mode" ), Document::Spaces );*/
    
    /*addHChildValue( ApplicationSettings::Ruler, QT_TRANSLATE_NOOP( "ApplicationSettings", "End of line style" ), Document::NoRuler );
    addHChildValue( ApplicationSettings::TextEncoding, QT_TRANSLATE_NOOP( "ApplicationSettings", "Text codec" ), "UTF-8" );
    addHChildValue( ApplicationSettings::LineWrap, QT_TRANSLATE_NOOP( "ApplicationSettings", "Line wrap mode" ), Document::NoWrap );*/
    
    
}

// ApplicationSettings

ApplicationSettings::ApplicationSettings()
    : SettingsNode( SettingsNode::Root )
{
    setGuiContent( QT_TRANSLATE_NOOP( "ApplicationSettings", "Preferences" ), QIcon::fromTheme( "preferences-other" ) );
    
    addChild( general );
    addChild( editor );
}
