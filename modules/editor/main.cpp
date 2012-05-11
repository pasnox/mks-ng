#include <QApplication>

#include "UIMain.h"

#include "Abstractors.h"
#include "SourceHighlightQtCodeEditor.h"

int main( int argc, char** argv )
{
    QApplication app( argc, argv );
    
    Abstractors::add( Abstractors::CodeEditor, &SourceHighlightQtCodeEditor::staticMetaObject );
    
    UIMain w;
    w.showMaximized();
    
    return app.exec();
}
