#include "RadioGroup.h"
#include "Document.h"

#include <QButtonGroup>
#include <QAbstractButton>

RadioGroup::RadioGroup( QBoxLayout::Direction direction, QWidget* parent )
    : QWidget( parent ),
        mLayout( new QBoxLayout( direction, this ) ),
        mGroup( new QButtonGroup( this ) )
{
    mLayout->setMargin( 0 );
    mLayout->setSpacing( 9 );
    
    // overloaded signals are ugly to connect, let use old style connection
    connect( mGroup, SIGNAL( buttonClicked( QAbstractButton* ) ), this, SIGNAL( buttonClicked( QAbstractButton* ) ) );
    connect( mGroup, SIGNAL( buttonClicked( int ) ), this, SIGNAL( buttonIdClicked( int ) ) );
    connect( mGroup, SIGNAL( buttonPressed( QAbstractButton* ) ), this, SIGNAL( buttonPressed( QAbstractButton* ) ) );
    connect( mGroup, SIGNAL( buttonPressed( int ) ), this, SIGNAL( buttonIdPressed( int ) ) );
    connect( mGroup, SIGNAL( buttonReleased( QAbstractButton* ) ), this, SIGNAL( buttonReleased( QAbstractButton* ) ) );
    connect( mGroup, SIGNAL( buttonReleased( int ) ), this, SIGNAL( buttonIdReleased( int ) ) );
}

void RadioGroup::setCheckedId( int id )
{
    mGroup->button( id )->setChecked( true );
}

int RadioGroup::checkedId() const
{
    return mGroup->checkedId();
}

void RadioGroup::addButton( QAbstractButton* button, int id )
{
    button->setCheckable( true );
    mLayout->addWidget( button );
    mGroup->addButton( button, id );
}

void RadioGroup::removeButton( QAbstractButton* button )
{
    mLayout->removeWidget( button );
    mGroup->removeButton( button );
}
