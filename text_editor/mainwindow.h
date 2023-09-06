#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTextEdit>
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

    QTextEdit * textContainer;
    QString filePath;
    QString fileName;
    QString initialContent;
    std::map<int, QString> initialContents;

    void readFile();
    QString pathToNameFile();

private:
    Ui::MainWindow *ui;

private slots:
    void selectFile();
    void closeTab(int id);
    bool hasBeenEdited();
    void cursorChanged();
};
#endif // MAINWINDOW_H
