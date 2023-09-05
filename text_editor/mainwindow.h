#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTextEdit>

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

    void readFile();
    QString pathToNameFile();

private:
    Ui::MainWindow *ui;

private slots:
    void selectFile();
    void closeTab(int id);
    bool hasBeenEdited();
};
#endif // MAINWINDOW_H
