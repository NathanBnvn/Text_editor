#ifndef CQTEXTEDIT_H
#define CQTEXTEDIT_H

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
    int id;
    int cursor_column;
    int cursor_row;

    //il faut faire un mytextedit dans lequel tu ajoutes l'envoi du signal et un connect avec un slot
    // qui vérifie chaque fois que text esdit est changé si ce'est le mme
    // et après avoir collé le mytextedit tu poses un connect
    //avec un slot charger de mettre une étoile ou de ne pas la mettre


signals:
    bool textHasChanged(bool);

private:

public slots:
    void hasBeenEdited();
    void cursorChanged();

private slots:
};



#endif // CQTEXTEDIT_H
