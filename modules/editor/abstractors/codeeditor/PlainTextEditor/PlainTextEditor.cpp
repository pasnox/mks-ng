#include "PlainTextEditor.h"

#include <QStyleOptionFrameV3>
#include <QPainter>

class PlainTextEditor::Private : QObject
{
    Q_OBJECT

public:
    Private( PlainTextEditor* _editor )
        : QObject( _editor ),
            editor( _editor ),
            originalPalette( _editor->palette() ),
            rulerMode( PlainTextEditor::NoRuler ),
            rulerWidth( 80 )
    {
        editor->setAutoFillBackground( true );
        
#if defined( HAS_QT_5 )
        connect( editor, &PlainTextEditor::cursorPositionChanged, editor->viewport(), &QWidget::update );
#else
        connect( editor, SIGNAL( cursorPositionChanged() ), editor->viewport(), SLOT( update() ) );
#endif
    }
    
    PlainTextEditor* editor;
    QPalette originalPalette;
    PlainTextEditor::Ruler rulerMode;
    int rulerWidth;

    QLine rulerLine() const
    {
        const QPoint offset = editor->contentOffset().toPoint();
        const int x = rulerWidth *QFontMetrics( editor->font() ).averageCharWidth();
        return QLine( QPoint( x, -4 ) +offset, QPoint( x, editor->viewport()->height() +4 ) +offset );
    }
    
    QRect rulerRect() const
    {
        const QPoint offset = editor->contentOffset().toPoint();
        const int x = rulerWidth *QFontMetrics( editor->font() ).averageCharWidth();
        QRect rect( QPoint( x +offset.x(), offset.y() -4 ), QSize( editor->viewport()->size() +QSize( 0, 4 ) ) );
        return rect;
    }
    
    QRect caretLineRect() const
    {
        QRect rect = editor->cursorRect().adjusted( 0, -1, 0, 2 );
        rect.setX( 0 );
        rect.setWidth( editor->viewport()->width() );
        return rect;
    }
    
    void paintFrame()
    {
        QPainter painter( editor );
        QStyleOptionFrameV3 option;
        
        option.initFrom( editor );
        option.palette = originalPalette;
        option.rect = editor->frameRect();
        option.frameShape = editor->frameShape();
        
        switch ( option.frameShape ) {
            case QFrame::Box:
            case QFrame::HLine:
            case QFrame::VLine:
            case QFrame::StyledPanel:
            case QFrame::Panel:
                option.lineWidth = editor->lineWidth();
                option.midLineWidth = editor->midLineWidth();
                break;
            default:
                // most frame styles do not handle customized line and midline widths
                // (see updateFrameWidth()).
                option.lineWidth = editor->frameWidth();
                break;
        }

        if ( editor->frameShadow() == QFrame::Sunken ) {
            option.state |= QStyle::State_Sunken;
        }
        else if ( editor->frameShadow() == QFrame::Raised ) {
            option.state |= QStyle::State_Raised;
        }

        editor->style()->drawControl( QStyle::CE_ShapedFrame, &option, &painter, editor );
    }
};

PlainTextEditor::PlainTextEditor( QWidget* parent )
    : QPlainTextEdit( parent ),
        d( new PlainTextEditor::Private( this ) )
{
}

PlainTextEditor::~PlainTextEditor()
{
}

PlainTextEditor::Ruler PlainTextEditor::rulerMode() const
{
    return d->rulerMode;
}

void PlainTextEditor::setRulerMode( PlainTextEditor::Ruler mode )
{
    d->rulerMode = mode;
    viewport()->update();
}

int PlainTextEditor::rulerWidth() const
{
    return d->rulerWidth;
}

void PlainTextEditor::setRulerWidth( int width )
{
    d->rulerWidth = width;
    viewport()->update();
}

bool PlainTextEditor::event( QEvent* event )
{
    switch ( event->type() ) {
        case QEvent::QEvent::Paint:
            d->paintFrame();
            return true;
        default:
            break;
    }
    
    return QPlainTextEdit::event( event );
}

void PlainTextEditor::paintEvent( QPaintEvent* event )
{
    QPainter painter( viewport() );
    
    // draw ruler
    switch ( d->rulerMode ) {
        case PlainTextEditor::NoRuler:
            break;
        case PlainTextEditor::LineRuler:
            painter.setPen( palette().color( QPalette::LinkVisited ) );
            painter.drawLine( d->rulerLine() );
            break;
        case PlainTextEditor::BackgroundRuler:
            painter.setPen( Qt::NoPen );
            painter.setBrush( palette().color( QPalette::LinkVisited ) );
            painter.drawRect( d->rulerRect() );
            break;
    }
    
    // draw caret line
    painter.fillRect( d->caretLineRect(), palette().color( QPalette::Link ) );
    
    // finished
    painter.end();
    
    // normal editor painting
    QPlainTextEdit::paintEvent( event );
}

#include "PlainTextEditor.moc"
