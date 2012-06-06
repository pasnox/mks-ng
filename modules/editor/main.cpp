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
    QApplication::setWindowIcon( QIcon( ":/icons/mks-ng.png" ) );
    
    pSettings::setDefaultProperties( pSettings::Properties( APPLICATION_NAME, APPLICATION_VERSION, pSettings::Auto ) );
    
    Abstractors::add( Abstractors::CodeEditor, &SourceHighlightQtCodeEditor::staticMetaObject );
    //Abstractors::add( Abstractors::CodeEditor, &QScintillaCodeEditor::staticMetaObject );
    
    UIMain w;
    w.setWindowIcon( w.currentWindowIcon() );
    w.setWindowTitle( w.currentWindowTitle() );
    w.show();
    
    return app.exec();
}
