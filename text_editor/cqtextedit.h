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

    QString initialContent;
    int cursorColumn;
    int cursorRow;
    bool textChanged;

    //void lineCountPaintEvent(QPaintEvent *event);
    //int lineCounterWidth();

    //il faut faire un mytextedit dans lequel tu ajoutes l'envoi du signal et un connect avec un slot
    // qui vérifie chaque fois que text esdit est changé si ce'est le mme
    // et après avoir collé le mytextedit tu poses un connect
    //avec un slot charger de mettre une étoile ou de ne pas la mettre

signals:
    void textHasChanged(bool);

protected:
    //void resizeEvent(QResizeEvent *event) override;

private:
    //QWidget *lineCounter;
    //QLabel *cursorLabel;

public slots:


private slots:
    void hasBeenEdited();
    void cursorChanged();
    //void updateLineCounterWidth(int newLineCount);
    //void updateLineCounter(const QRect &rect, int y);
};



#endif // CQTEXTEDIT_H
