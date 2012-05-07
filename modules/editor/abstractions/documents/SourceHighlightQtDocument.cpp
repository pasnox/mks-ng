#include "SourceHighlightQtDocument.h"

#include <QEvent>
#include <QBoxLayout>
#include <QPlainTextEdit>
#include <QIcon>

SourceHighlightQtDocument::SourceHighlightQtDocument( QWidget* parent )
    : Document( parent )
{
    QPlainTextEdit* pte = new QPlainTextEdit( this );
    QBoxLayout* bLayout = new QBoxLayout( QBoxLayout::LeftToRight );
    bLayout->setMargin( 0 );
    bLayout->setSpacing( 9 );
    bLayout->addWidget( pte );
    
    static int i = 0;
    pte->setPlainText( QString::number( ++i ) );
    setProperty( Document::Decoration, QIcon::fromTheme( "kate" ) );
    setProperty( Document::FileName, tr( "New file #%1" ).arg( i ) );
    
#if USE_MDI_AREA == 1
    QWidget* widget = new QWidget( this );
    widget->setLayout( bLayout );
    setWidget( widget );
#else
    setLayout( bLayout );
#endif
}

SourceHighlightQtDocument::~SourceHighlightQtDocument()
{
}

QVariant SourceHighlightQtDocument::property( Document::Property property ) const
{
    switch ( property ) {
        default: {
            return mProperties.value( property );
            break;
        }
    }
}

void SourceHighlightQtDocument::setProperty( Document::Property property, const QVariant& value )
{
    switch ( property ) {
        default: {
            if ( value.isNull() ) {
                mProperties.remove( property );
            }
            else {
                mProperties[ property ] = value;
            }
            
            break;
        }
    }
}
