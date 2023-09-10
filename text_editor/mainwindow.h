#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cqtextedit.h"

#include <QMainWindow>
#include <QString>
#include <map>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    CQTextEdit * textContainer;
    QString filePath;
    QString fileName;
    QLabel * labelCursor;
    std::map<int, QString> initialContents;

    void readFile();
    QString pathToNameFile();


private:
    Ui::MainWindow *ui;

private slots:
    void selectFile();
    void closeTab(int id);
    void cursorChanged();
    void tabChanged(int id);
    void hasBeenEdited(bool b);
};
#endif // MAINWINDOW_H
