#include "DocumentEol.h"

#include <QEvent>
#include <QActionGroup>
#include <QMenu>
#include <QDebug>

DocumentEol::DocumentEol( QWidget* parent )
    : QToolButton( parent ), mGroup( new QActionGroup( this ) )
{
    mActions[ Document::Unix ] = mGroup->addAction( QIcon( ":/icons/linux.png" ), "LF: Linux" );
    mActions[ Document::MacOS ] = mGroup->addAction( QIcon( ":/icons/macos.png" ), "CR: Mac OS (< OS X)" );
    mActions[ Document::Windows ] = mGroup->addAction( QIcon( ":/icons/windows.png" ), "CR+LF: Windows" );
    
    foreach ( QAction* action, mActions ) {
        action->setCheckable( true );
    }
    
    QMenu* menu = new QMenu( this );
    menu->addActions( mGroup->actions() );
    
    setPopupMode( QToolButton::InstantPopup );
    setIconSize( QSize( 16, 16 ) );
    setMaximumSize( QSize( 21, 21 ) );
    setToolTip( tr( "Line endings. Click to convert." ) );
    setMenu( menu );
    
    connect( mGroup, &QActionGroup::triggered, this, &DocumentEol::group_triggered );
}

DocumentEol::~DocumentEol()
{
}

void DocumentEol::setMode( Document::EolHint mode )
{
    mActions[ mode ]->trigger();
}

Document::EolHint DocumentEol::mode() const
{
    return mActions.key( mGroup->checkedAction() );
}

void DocumentEol::retranslateUi()
{
    // do your custom retranslate here
}

void DocumentEol::changeEvent( QEvent* event )
{
    QToolButton::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}

void DocumentEol::group_triggered( QAction* action )
{
    setIcon( action->icon() );
    emit modeChanged( mode() );
}
