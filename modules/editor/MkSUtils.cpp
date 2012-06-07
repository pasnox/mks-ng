#include "MkSUtils.h"

#include <QString>
#include <QRegExp>

QString MkSUtils::makeHTMLUrl( const QString& _url, bool disableUnderLine )
{
    QString url( _url );
    
    if ( !QRegExp( "\\w+://.*" ).exactMatch( url ) ) {
        url.prepend( "http://" );
    }
    
    return QString( "<a href=\"%1\"%2>%1</a>" )
        .arg( url )
        .arg( disableUnderLine ? " style=\"text-decoration: none;\"" : "" )
    ;
}
