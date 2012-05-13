#include <QApplication>

#include <FreshCore/pSettings>

#include "main.h"
#include "UIMain.h"

#include "Abstractors.h"
#include "SourceHighlightQtCodeEditor.h"

int main( int argc, char** argv )
{
    QApplication app( argc, argv );
    QApplication::setApplicationName( APPLICATION_NAME );
    QApplication::setApplicationVersion( APPLICATION_VERSION );
    QApplication::setOrganizationDomain( APPLICATION_DOMAIN );
    QApplication::setOrganizationName( APPLICATION_ORGANIZATION );
    QApplication::setWindowIcon( QIcon( ":/fresh/icons/info.png" ) );
    
    pSettings::setDefaultProperties( pSettings::Properties( APPLICATION_NAME, APPLICATION_VERSION, pSettings::Auto ) );
    
    Abstractors::add( Abstractors::CodeEditor, &SourceHighlightQtCodeEditor::staticMetaObject );
    //Abstractors::add( Abstractors::CodeEditor, &QScintillaCodeEditor::staticMetaObject );
    
    UIMain w;
    w.setWindowTitle( QString( "%1 v%2 (%3)[*]" ).arg( APPLICATION_NAME ).arg( APPLICATION_VERSION ).arg( APPLICATION_VERSION_LONG ) );
    w.setWindowIcon( QApplication::windowIcon() );
    w.show();
    
    return app.exec();
}
