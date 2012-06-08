#ifndef PLAINTEXTEDITOR_H
#define PLAINTEXTEDITOR_H

#include <QPlainTextEdit>

class PlainTextEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    enum Ruler {
        NoRuler = 0x0,
        LineRuler = 0x1,
        BackgroundRuler = 0x2
    };
    
    PlainTextEditor( QWidget* parent = 0 );
    virtual ~PlainTextEditor();
    
    PlainTextEditor::Ruler rulerMode() const;
    int rulerWidth() const;

public slots:
    void setRulerMode( PlainTextEditor::Ruler mode );
    void setRulerWidth( int width );

protected:
    class Private;
    PlainTextEditor::Private* d;
    
    virtual bool event( QEvent* event );
    virtual void paintEvent( QPaintEvent* event );
};

#endif // PLAINTEXTEDITOR_H
