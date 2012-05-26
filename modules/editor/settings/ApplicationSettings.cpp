#include "ApplicationSettings.h"
#include "ApplicationSettingsDelegate.h"
#include "Document.h"

#include <FreshCore/pCoreUtils>

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
    
    font = addHValue( "font", QFont(), QVariant::Font );
    font.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Font" ) );
    
    codec = addHValue( "codec", "UTF-8", QVariant::String );
    codec.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Text codec" ) );
    codec.setGuiData( pCoreUtils::textCodecs() );
    
    SettingsNode indentationGroup = addVGroup( QT_TRANSLATE_NOOP( "ApplicationSettings", "Indentation" ), "indentation" );
    SettingsNode eolGroup = addHGroup( QT_TRANSLATE_NOOP( "ApplicationSettings", "End of line" ), "eol" );
    
    indentType = indentationGroup.addHValue( "type", Document::Spaces, ApplicationSettingsDelegate::Indent );
    
    indentWidth = indentationGroup.addHValue( "width", 4, QVariant::Int );
    indentWidth.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Width" ) );
    indentWidth.setGuiData( PairInt( qMakePair( 0, 16 ) ) );
    
    tabWidth = indentationGroup.addHValue( "tabWidth", 4, QVariant::Int );
    tabWidth.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Tab width" ) );
    tabWidth.setGuiData( PairInt( qMakePair( 0, 16 ) ) );
    
    eol = eolGroup.addHValue( "type", Document::Unix, ApplicationSettingsDelegate::Eol );
}

EditorSettings::Colors::Colors()
    : SettingsNode( SettingsNode::VerticalPage )
{
    setName( "colors" );
    setGuiContent( QT_TRANSLATE_NOOP( "ApplicationSettings", "Colors" ), QIcon::fromTheme( "format-text-color" ) );
    setHelp( QT_TRANSLATE_NOOP( "ApplicationSettings", "Editor colors help" ) );
    
    SettingsNode editorGroup = addHGroup( QT_TRANSLATE_NOOP( "ApplicationSettings", "Editor" ), "editor" );
    SettingsNode selectionGroup = addHGroup( QT_TRANSLATE_NOOP( "ApplicationSettings", "Selection" ), "selection" );
    SettingsNode caretLineGroup = addHGroup( QT_TRANSLATE_NOOP( "ApplicationSettings", "Caret line" ), "caretLne" );
    
    paper = editorGroup.addHValue( "paper", QColor( 255, 255, 255, 255 ), QVariant::Color );
    paper.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Paper" ) );
    paper.setGuiData( true );
    
    pen = editorGroup.addHValue( "pen", QColor( 0, 0, 0, 255 ), QVariant::Color );
    pen.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Pen" ) );
    
    selectionBackground = selectionGroup.addHValue( "background", QColor( 189, 255, 155, 255 ), QVariant::Color );
    selectionBackground.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Background" ) );
    selectionBackground.setGuiData( true );
    
    selectionForeground = selectionGroup.addHValue( "foreground", QColor( 0, 0, 0, 255 ), QVariant::Color );
    selectionForeground.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Foreground" ) );
    selectionForeground.setGuiData( true );
    
    lineBackground = caretLineGroup.addHValue( "background", QColor( 170, 170, 255, 150 ), QVariant::Color );
    lineBackground.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Background" ) );
    lineBackground.setGuiData( true );
    
    lineForeground = caretLineGroup.addHValue( "foreground", QColor( 0, 0, 0, 255 ), QVariant::Color );
    lineForeground.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Foreground" ) );
    lineForeground.setGuiData( true );
}

EditorSettings::EditorSettings()
    : SettingsNode( SettingsNode::VerticalPage )
{
    setName( "editor" );
    setGuiContent( QT_TRANSLATE_NOOP( "ApplicationSettings", "Editor" ), QIcon::fromTheme( "story-editor" ) );
    setHelp( QT_TRANSLATE_NOOP( "ApplicationSettings", "Editor help" ) );
    
    SettingsNode uponOpenGroup = addVGroup( QT_TRANSLATE_NOOP( "ApplicationSettings", "Upon open" ), "uponOpen" );
    
    detectEol = uponOpenGroup.addHValue( "detectEol", true, QVariant::Bool );
    detectEol.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Try to detect end of line" ) );
    detectEol.setHelp( QT_TRANSLATE_NOOP( "ApplicationSettings", "Try to detect end of line of the document" ) );
    
    detectIndentation = uponOpenGroup.addHValue( "detectIndentation", true, QVariant::Bool );
    detectIndentation.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Try to detect indentation" ) );
    detectIndentation.setHelp( QT_TRANSLATE_NOOP( "ApplicationSettings", "Try to detect indentation of the document" ) );
    
    convertEol = uponOpenGroup.addHValue( "convertEol", false, QVariant::Bool );
    convertEol.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Force setted end of line" ) );
    convertEol.setHelp( QT_TRANSLATE_NOOP( "ApplicationSettings", "Convert the end of line of the document according to the setted end of line" ) );
    
    convertIndentation = uponOpenGroup.addHValue( "convertIndentation", false, QVariant::Bool );
    convertIndentation.setLabel( QT_TRANSLATE_NOOP( "ApplicationSettings", "Force setted indentation" ) );
    convertIndentation.setHelp( QT_TRANSLATE_NOOP( "ApplicationSettings", "Convert the indentation of the document according to the setted indentation" ) );
    
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

ApplicationSettings::ApplicationSettings( QObject* parent )
    : QObject( parent ), SettingsNode( SettingsNode::Root )
{
    setGuiContent( QT_TRANSLATE_NOOP( "ApplicationSettings", "Preferences" ), QIcon::fromTheme( "preferences-other" ) );
    
    addChild( general );
    addChild( editor );
}
