#include <QApplication>

#include "UIMain.h"
#include "SourceHighlightQtDocument.h"

int main( int argc, char** argv )
{
    QApplication app( argc, argv );
    
    Document::registerDocumentAbstraction( &SourceHighlightQtDocument::staticMetaObject );
    
    UIMain w;
    w.show();
    
    return app.exec();
}
