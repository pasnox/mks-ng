#include <QApplication>

#include <FreshCore/pSettings>

#include "main.h"
#include "UIMain.h"

#include "Abstractors.h"
#include "SourceHighlightQtCodeEditor.h"
#include "QodeEditCodeEditor.h"

int main( int argc, char** argv )
{
    QApplication app( argc, argv );
    QApplication::setApplicationName( APPLICATION_NAME );
    QApplication::setApplicationVersion( APPLICATION_VERSION );
    QApplication::setOrganizationDomain( APPLICATION_DOMAIN );
    QApplication::setOrganizationName( APPLICATION_ORGANIZATION );
    QApplication::setWindowIcon( QIcon( ":/icons/mks-ng.png" ) );
    
    pSettings::setDefaultProperties( pSettings::Properties( APPLICATION_NAME, APPLICATION_VERSION, pSettings::Auto ) );
    
    Abstractors::add( Abstractors::CodeEditor, &SourceHighlightQtCodeEditor::staticMetaObject );
    Abstractors::add( Abstractors::CodeEditor, &QodeEditCodeEditor::staticMetaObject );
    //Abstractors::add( Abstractors::CodeEditor, &QScintillaCodeEditor::staticMetaObject );
    
    UIMain w;
    w.setWindowIcon( w.currentWindowIcon() );
    w.setWindowTitle( w.currentWindowTitle() );
    w.show();
    
    //w.openPlainText( QStringList( "/home/pasnox/Temporaire/indent_finder-1.4/test_files/mixed4/arabic.c" ) );
    
    return app.exec();
}
