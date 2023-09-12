#ifndef LINECOUNTER_H
#define LINECOUNTER_H

#include <QWidget>
#include "cqtextedit.h"

class LineCounter : public QWidget
{
    Q_OBJECT

public:
    LineCounter(CQTextEdit * textusEditor);
    QSize sizeHint() const override;

signals:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    CQTextEdit * textusEditor;
};

#endif // LINECOUNTER_H
