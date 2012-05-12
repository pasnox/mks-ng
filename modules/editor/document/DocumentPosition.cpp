#include "DocumentPosition.h"
#include "DocumentPositionPopup.h"

#include <QEvent>
#include <QApplication>
#include <QDesktopWidget>

DocumentPosition::DocumentPosition( QWidget* parent )
    : QToolButton( parent )
{
    setToolTip( tr( "Cursor position. Click to change." ) );
    setMaximumHeight( 21 );
    
    connect( this, &QToolButton::clicked, this, &DocumentPosition::showPopup );
}

DocumentPosition::~DocumentPosition()
{
}

void DocumentPosition::setPosition( const QPoint& pos, int lineCount )
{
    mPos = pos;
    mLineCount = lineCount;
    propertiesChanged();
}

QPoint DocumentPosition::position() const
{
    return mPos;
}

void DocumentPosition::retranslateUi()
{
    // do your custom retranslate here
}

void DocumentPosition::showPopup()
{
    DocumentPositionPopup* popup = new DocumentPositionPopup( this );
    QRect desktopRect = QApplication::desktop()->availableGeometry( this );
    
    QRect r( mapToGlobal( rect().bottomLeft() ) + QPoint( 0, 5 ), popup->size() );
    
    if ( r.right() > desktopRect.right() ) {
        r.moveRight( desktopRect.right() );
    }
    
    if ( r.top() < desktopRect.top() ) {
        r.moveTop( desktopRect.top() );
    }
    
    popup->setAttribute( Qt::WA_DeleteOnClose );
    popup->setLine( position().y(), mLineCount );
    popup->move( r.topLeft() );
    popup->show();
    
    connect( popup, &DocumentPositionPopup::lineChanged, this, &DocumentPosition::popup_lineChanged );
}

void DocumentPosition::changeEvent( QEvent* event )
{
    QToolButton::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}

void DocumentPosition::propertiesChanged()
{
    const QString fakeText = tr( "Line: %1 Column: %2" ).arg( 65536 ).arg( 32768 );
    setMinimumWidth( QFontMetrics( font() ).width( fakeText ) +( 9 *2 ) );
    
    setText( tr( "Line: %1 Column: %2" ).arg( mPos.y() +1 ).arg( mPos.x() +1 ) );
}

void DocumentPosition::popup_lineChanged( int line )
{
    setPosition( QPoint( mPos.x(), line -1 ), mLineCount );
    emit positionChanged( position() );
    
}
