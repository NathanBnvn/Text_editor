#include "linecounter.h"


LineCounter::LineCounter(CQTextEdit *editor) : QWidget(editor), textusEditor(editor){}

QSize LineCounter::sizeHint() const
{
    return QSize(textusEditor->lineCounterWidth(), 0);
}

void LineCounter::paintEvent(QPaintEvent *event)
{
    textusEditor->lineCountPaintEvent(event);
}
