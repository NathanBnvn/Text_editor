#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cqtextedit.h"

#include <QMainWindow>
#include <QString>
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

    void readFile();
    QString pathToNameFile();

private:
    Ui::MainWindow *ui;

private slots:
    void selectFile();
    void hasBeenEdited(bool edited);
    void closeTab(int id);
    void tabChanged(int id);
    void showSearch(bool show);
    void showSwap(bool show);
};
#endif // MAINWINDOW_H
