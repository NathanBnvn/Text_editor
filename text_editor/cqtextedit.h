#ifndef CQTEXTEDIT_H
#define CQTEXTEDIT_H

#include <QLabel>
#include <QTextEdit>
#include <QString>
#include <iostream>

// Custom QTextEdit

class CQTextEdit : public QTextEdit
{

    Q_OBJECT
public:
    CQTextEdit(QWidget *parent = nullptr);

    bool textChanged;
    QString initialContent;

    int cursorRow;
    int cursorColumn;
    int cursorLabelHeight;

    int getFirstVisibleBlockId();
    int lineCounterWidth();
    void lineCountPaintEvent(QPaintEvent *event);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QWidget * lineCounter;
    QLabel * cursorLabel;

signals:
    void textHasChanged(bool);

public slots:



private slots:
    void hasBeenEdited();
    void cursorChanged();
    void updateLineCounter();
    void updateLineCounterWidth(int newLineCount);
};



#endif // CQTEXTEDIT_H
