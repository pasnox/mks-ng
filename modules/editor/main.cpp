#include <QApplication>

#include <FreshCore/pSettings>

#include "main.h"
#include "UIMain.h"

#include "Abstractors.h"
#include "SourceHighlightQtCodeEditor.h"
#include "QodeEditCodeEditor.h"
#include "ApplicationSettings.h"

int main( int argc, char** argv )
{
    // create application
    QApplication app( argc, argv );
    QApplication::setApplicationName( APPLICATION_NAME );
    QApplication::setApplicationVersion( APPLICATION_VERSION );
    QApplication::setOrganizationDomain( APPLICATION_DOMAIN );
    QApplication::setOrganizationName( APPLICATION_ORGANIZATION );
    QApplication::setWindowIcon( QIcon( ":/icons/mks-ng.png" ) );
    
    // init settings properties
    pSettings::setDefaultProperties( pSettings::Properties( APPLICATION_NAME, APPLICATION_VERSION, pSettings::Auto ) );
    
    // enable abstractors
    Abstractors::add( Abstractors::CodeEditor, &SourceHighlightQtCodeEditor::staticMetaObject );
    Abstractors::add( Abstractors::CodeEditor, &QodeEditCodeEditor::staticMetaObject );
    //Abstractors::add( Abstractors::CodeEditor, &QScintillaCodeEditor::staticMetaObject );
    
    // load settings
    pSettings settings;
    ApplicationSettings& appSettings = Abstractors::applicationSettings();
    appSettings.load( &settings );
    
    // init abstractors
    Abstractors::setCurrent( Abstractors::CodeEditor, appSettings.abstractors.codeEditor.value().toString() );
    
    // create main window
    UIMain w;
    w.setWindowIcon( w.currentWindowIcon() );
    w.setWindowTitle( w.currentWindowTitle() );
    w.show();
    
    w.openPlainText( ( QStringList()
        << "/home/pasnox/Temporaire/indent_finder-1.4/test_files/mixed4/arabic.c"
        << "/home/pasnox/Temporaire/Notepad" 
    ) );
    
    // execute application
    const int result = app.exec();
    // save settings
    appSettings.save( &settings );
    return result;
}
