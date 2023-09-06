#include "cqtextedit.h"
#include <QString>
#include <map>
#include <iostream>

using namespace std;



CQTextEdit::CQTextEdit(QWidget *parent): QTextEdit(parent)
{

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));
    connect(this, SIGNAL(textChanged()), this, SLOT(hasBeenEdited()));

}


void CQTextEdit::hasBeenEdited()
{
    if(initialContent == this->toPlainText())
        emit textHasChanged(false);
    else
        emit textHasChanged(true);
}

void CQTextEdit::cursorChanged()
{
    cursor_column = this->textCursor().blockNumber();
    cursor_row = this->textCursor().positionInBlock();
}

