#include "DocumentIndentation.h"
#include "DocumentIndentationPopup.h"

#include <QEvent>
#include <QApplication>
#include <QDesktopWidget>

DocumentIndentation::DocumentIndentation( QWidget* parent )
    : QToolButton( parent )
{
    setToolTip( tr( "Indentation properties. Click to change." ) );
    
    connect( this, &QToolButton::clicked, this, &DocumentIndentation::showPopup );
}

DocumentIndentation::~DocumentIndentation()
{
}

void DocumentIndentation::retranslateUi()
{
    // do your custom retranslate here
}

void DocumentIndentation::showPopup()
{
    DocumentIndentationPopup* popup = new DocumentIndentationPopup( this );
    QRect desktopRect = QApplication::desktop()->availableGeometry( this );
    
    QRect r( mapToGlobal( rect().bottomLeft() ) + QPoint( 0, 5 ), popup->size() );
    
    if ( r.right() > desktopRect.right() ) {
        r.moveRight( desktopRect.right() );
    }
    
    if ( r.top() < desktopRect.top() ) {
        r.moveTop( desktopRect.top() );
    }
    
    popup->setAttribute( Qt::WA_DeleteOnClose );
    popup->setMode( mode() );
    popup->setIndentWidth( indentWidth() );
    popup->setTabWidth( tabWidth() );
    popup->move( r.topLeft() );
    popup->show();
    
    connect( popup, &DocumentIndentationPopup::modeChanged, this, &DocumentIndentation::popup_modeChanged );
    connect( popup, &DocumentIndentationPopup::indentWidthChanged, this, &DocumentIndentation::popup_indentWidthChanged );
    connect( popup, &DocumentIndentationPopup::tabWidthChanged, this, &DocumentIndentation::popup_tabWidthChanged );
    connect( popup, &DocumentIndentationPopup::convertionRequested, this, &DocumentIndentation::popup_convertionRequested );
}

void DocumentIndentation::changeEvent( QEvent* event )
{
    QToolButton::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}

void DocumentIndentation::propertiesChanged()
{
    const QString fakeText = tr( "%1 Tabs (%2)" ).arg( 16 ).arg( 16 );
    setMinimumWidth( QFontMetrics( font() ).width( fakeText ) +( 9 *2 ) );
    
    switch ( mMode ) {
        case Document::Spaces:
            setText( tr( "%1 Spaces" ).arg( mIndentWidth ) );
            break;
        case Document::Tabs:
            setText( tr( "%1 Tabs (%2)" ).arg( mIndentWidth ).arg( mTabWidth ) );
            break;
    }
}

void DocumentIndentation::popup_modeChanged( int mode )
{
    const Document::IndentHint m = Document::IndentHint( mode );
    setMode( m );
    emit modeChanged( m );
}

void DocumentIndentation::popup_indentWidthChanged( int width )
{
    setIndentWidth( width );
    emit indentWidthChanged( width );
}

void DocumentIndentation::popup_tabWidthChanged( int width )
{
    setTabWidth( width );
    emit tabWidthChanged( width );
}

void DocumentIndentation::popup_convertionRequested()
{
}

void DocumentIndentation::setMode( Document::IndentHint mode )
{
    mMode = mode;
    propertiesChanged();
}

Document::IndentHint DocumentIndentation::mode() const
{
    return mMode;
}

void DocumentIndentation::setIndentWidth( int width )
{
    mIndentWidth = width;
    propertiesChanged();
}

int DocumentIndentation::indentWidth() const
{
    return mIndentWidth;
}

void DocumentIndentation::setTabWidth( int width )
{
    mTabWidth = width;
    propertiesChanged();
}

int DocumentIndentation::tabWidth() const
{
    return mTabWidth;
}
