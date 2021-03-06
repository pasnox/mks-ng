#include "DocumentPosition.h"
#include "DocumentPositionPopup.h"

#include <QEvent>
#include <QApplication>
#include <QDesktopWidget>

DocumentPosition::DocumentPosition( QWidget* parent )
    : QToolButton( parent ),
        mPos( QPoint( -1, -1 ) ),
        mLineCount( -1 )
{
    setToolTip( tr( "Cursor position. Click to change." ) );
    setPosition( QPoint(), 1 );
    
#if defined( HAS_QT_5 )
    connect( this, &QToolButton::clicked, this, &DocumentPosition::showPopup );
#else
    connect( this, SIGNAL( clicked() ), this, SLOT( showPopup() ) );
#endif
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
    
#if defined( HAS_QT_5 )
    connect( popup, &DocumentPositionPopup::lineChanged, this, &DocumentPosition::popup_lineChanged );
#else
    connect( popup, SIGNAL( lineChanged( int ) ), this, SLOT( popup_lineChanged( int ) ) );
#endif
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
    const QString fakeText = tr( "Line: %1 Column: %2" ).arg( 65536 ).arg( 65536 /4 );
    setMinimumWidth( QFontMetrics( font() ).width( fakeText ) +( 9 *2 ) );
    
    setText( tr( "Line: %1 Column: %2" ).arg( mPos.y() +1 ).arg( mPos.x() +1 ) );
}

void DocumentPosition::popup_lineChanged( int line )
{
    setPosition( QPoint( mPos.x(), line -1 ), mLineCount );
    emit positionChanged( position() );
    
}
