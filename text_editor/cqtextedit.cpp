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

using namespace std;



CQTextEdit::CQTextEdit(QWidget *parent): QTextEdit(parent)
{

   //lineCounter = new LineCounter(this);
   connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));
   connect(this, SIGNAL(textChanged()), this, SLOT(hasBeenEdited()));

   //connect(this->document(), SIGNAL(blockCountChanged(int)), this, SLOT(updateLineCounterWidth(int)));
   //connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateLineCounter()));
   //updateLineCounterWidth(0);

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
}


// Part 4

//int CQTextEdit::lineCounterWidth(){
//    int digits = 1;
//    int max = qMax(1, this->document()->blockCount());
//    while (max >= 10) {
//        max /= 10;
//        ++digits;
//    }

//    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
//    return space;
//}


//void CQTextEdit::updateLineCounterWidth(int /* newLineCount */)
//{
//    setViewportMargins(lineCounterWidth(), 0, 0, 0);
//}


//void CQTextEdit::updateLineCounter(const QRect &rect, int y)
//{
//    if (y)
//        lineCounter->scroll(0, y);
//    else
//        lineCounter->update(0, rect.y(), lineCounter->width(), rect.height());
//
//    if (rect.contains(viewport()->rect()))
//        updateLineCounterWidth(0);
//}

//void CQTextEdit::resizeEvent(QResizeEvent *e)
//{
//    CQTextEdit::resizeEvent(e);
//    QRect cr = contentsRect();
//    lineCounter->setGeometry(QRect(cr.left(), cr.top(), lineCounterWidth(), cr.height()));
//}


//void CQTextEdit::lineCountPaintEvent(QPaintEvent *event)
//{
    //QPainter painter(lineCounter);
    //painter.fillRect(event->rect(), Qt::black);

    //QTextBlock block = this->document()->firstBlock();
    //int blockNumber = block.blockNumber();
    //int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    //int bottom = top + qRound(blockBoundingRect(block).height());
    //while (block.isValid() && top <= event->rect().bottom()) {
    //    if (block.isVisible() && bottom >= event->rect().top()) {
    //        QString number = QString::number(blockNumber + 1);
    //        painter.setPen(Qt::white);
    //        painter.drawText(0, top, lineCounter->width(), fontMetrics().height(),
    //                         Qt::AlignRight, number);
    //    }

    //    block = block.next();
    //    top = bottom;
    //    bottom = top + qRound(blockBoundingRect(block).height());
    //    ++blockNumber;
    //}
//}
