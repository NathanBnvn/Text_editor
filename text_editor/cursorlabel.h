#ifndef CURSORLABEL_H
#define CURSORLABEL_H

#include <QLabel>
#include "cqtextedit.h"

class CursorLabel : public QLabel
{

public:
    CursorLabel(CQTextEdit *editor);

private:
    //CQTextEdit * textusEditor;

};

#endif // CURSORLABEL_H
