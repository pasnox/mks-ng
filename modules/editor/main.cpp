#include <QApplication>

#include "UIMain.h"

int main( int argc, char** argv )
{
    QApplication app( argc, argv );
    
    UIMain w;
    w.show();
    
    return app.exec();
}
