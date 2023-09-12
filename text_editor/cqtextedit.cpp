#include "cqtextedit.h"
#include "linecounter.h"
#include <QString>
#include <iostream>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QTextBlock>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QRect>

using namespace std;


CQTextEdit::CQTextEdit(QWidget *parent): QTextEdit(parent)
{
   lineCounter = new LineCounter(this);
   cursorLabel = new QLabel(this);

   cursorLabelHeight = 20;
   updateLineCounterWidth(0);

   connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));
   connect(this, SIGNAL(textChanged()), this, SLOT(hasBeenEdited()));
   connect(this->document(), SIGNAL(blockCountChanged(int)), this, SLOT(updateLineCounterWidth(int)));
   connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateLineCounter()));
}


void CQTextEdit::hasBeenEdited()
{
   if(this->initialContent.isEmpty())
       return;

   if(this->toPlainText().isEmpty())
       return;

   if(this->initialContent == this->toPlainText()) {
       emit textHasChanged(false);
   } else {
       emit textHasChanged(true);
   }
}


void CQTextEdit::cursorChanged()
{
    cursorColumn = this->textCursor().blockNumber();
    cursorRow = this->textCursor().positionInBlock();
    cursorLabel->setText(QString("Ligne: " + QString::number(cursorRow) +
                                 ", Colonne: " + QString::number(cursorColumn)));
}


void CQTextEdit::resizeEvent(QResizeEvent *event)
{
    QTextEdit::resizeEvent(event);
    QRect cr = contentsRect();
    int margin = 2;
    lineCounter->setGeometry(QRect(cr.left(), cr.top(), lineCounterWidth(), cr.height()));
    cursorLabel->setGeometry(QRect(cr.left() + margin, (cr.bottom()-(cursorLabelHeight+margin)),
                                   cr.width(), cursorLabelHeight));
}

int CQTextEdit::lineCounterWidth(){
    int digits = 1;
    int max = qMax(1, this->document()->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 8 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}


void CQTextEdit::updateLineCounterWidth(int)
{
    setViewportMargins(lineCounterWidth(), 0, 0, 0);
}


void CQTextEdit::updateLineCounter()
{
    this->verticalScrollBar()->setSliderPosition(
        this->verticalScrollBar()->sliderPosition());

    QRect rect =  this->contentsRect();
    lineCounter->update(0, rect.y(), lineCounter->width(), rect.height());
    updateLineCounterWidth(0);

    int dy = this->verticalScrollBar()->sliderPosition();
    if (dy > -1) {
        lineCounter->scroll(0, dy);
    }

    int first_block_id = getFirstVisibleBlockId();
    if (first_block_id == 0 || this->textCursor().block().blockNumber() == first_block_id-1)
        this->verticalScrollBar()->setSliderPosition(dy-this->document()->documentMargin());
}



int CQTextEdit::getFirstVisibleBlockId()
{
    QTextCursor cursor = QTextCursor(this->document());
    cursor.movePosition(QTextCursor::Start);
    for(int i=0; i < this->document()->blockCount(); ++i)
    {
        QTextBlock block = cursor.block();

        QRect r1 = this->viewport()->geometry();
        QRect r2 = this->document()->documentLayout()
                       ->blockBoundingRect(block).translated(
                                                            this->viewport()->geometry().x(),
                                                            this->viewport()->geometry().y() - (
                                                                this->verticalScrollBar()->sliderPosition()
                                                                   ) ).toRect();

        if (r1.contains(r2, true)) {
            return i;
        }

        cursor.movePosition(QTextCursor::NextBlock);
    }
    return 0;
}

void CQTextEdit::lineCountPaintEvent(QPaintEvent *event)
{
    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    QPainter painter(lineCounter);
    painter.fillRect(event->rect(), Qt::white);
    int blockNumber = this->getFirstVisibleBlockId();

    QTextBlock block = this->document()->findBlockByNumber(blockNumber);
    QTextBlock prev_block = (blockNumber > 0) ? this->document()->findBlockByNumber(blockNumber-1) : block;
    int translate_y = (blockNumber > 0) ? -this->verticalScrollBar()->sliderPosition() : 0;

    int top = this->viewport()->geometry().top();

    int additional_margin;
    if (blockNumber == 0)
        additional_margin = (int) this->document()->documentMargin() -1 - this->verticalScrollBar()->sliderPosition();
    else
        additional_margin = (int) this->document()->documentLayout()->blockBoundingRect(prev_block)
                                .translated(0, translate_y).intersected(this->viewport()->geometry()).height();

    top += additional_margin;

    int bottom = top + (int) this->document()->documentLayout()->blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(-5, top,
                             lineCounter->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) this->document()->documentLayout()->blockBoundingRect(block).height();
        ++blockNumber;
    }
}
