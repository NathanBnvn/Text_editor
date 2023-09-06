#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QTabBar>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Textus Editor");
    ui->tabWidget->removeTab(1);
    ui->tabWidget->setTabsClosable(true);

    // QTextEdit* edit = qobject_cast<QTextEdit*>(tabWidget->widget(index))

    textContainer = ui->tabWidget->currentWidget()->findChild<QTextEdit *>();

    connect (ui->actionOpenFile, SIGNAL(triggered(bool)), this, SLOT(selectFile()));
    connect (ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect (textContainer->document(), SIGNAL(contentsChanged()), this, SLOT(hasBeenEdited()));
    connect (textContainer, SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::selectFile()
{
    MainWindow::filePath = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                "/home",
                                                tr("Document (*.txt *.doc *.rtf *odt)"));

    MainWindow::fileName = pathToNameFile();

    if(ui->textContainer->document()->isEmpty())
    {
        MainWindow::textContainer = ui->textContainer;
        ui->tabWidget->setTabText(0, fileName);
    } else {
        MainWindow::textContainer = new QTextEdit();
        ui->tabWidget->addTab(textContainer, fileName);
    }

    readFile();
    initialContents[ui->tabWidget->currentIndex()] = initialContent;
}


void MainWindow::closeTab(int id)
{
    if(ui->textContainer->document()->isEmpty())
        return;

    QMessageBox messageBox(this);
    messageBox.setWindowTitle(QString("Textus Editor - Save file"));
    messageBox.setText(QString("Document modifié"));
    messageBox.setInformativeText(QString("Souhaitez-vous sauvegarder les modifications ?"));
    messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Save);

    if(hasBeenEdited()){
        int choice = messageBox.exec();

        QByteArray content = textContainer->toPlainText().toUtf8();

        switch (choice) {
        case QMessageBox::Save:
            QFileDialog::saveFileContent(content,
                                         ui->tabWidget->tabText(
                                             ui->tabWidget->currentIndex()));
            ui->tabWidget->removeTab(id);
            break;
        case QMessageBox::Discard:
            ui->tabWidget->removeTab(id);
            break;
        case QMessageBox::Cancel:
            break;
        default:
            break;
        }

    }else {
        ui->tabWidget->removeTab(id);
    }
}


bool MainWindow::hasBeenEdited()
{
    if(initialContent.isEmpty())
        return false;

    if(textContainer->toPlainText().isEmpty())
        return false;

    qDebug() << initialContent;
    qDebug() << ui->tabWidget->currentIndex();

    if(initialContents[ui->tabWidget->currentIndex()] == textContainer->toPlainText()){
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),
                                  ui->tabWidget->tabText(ui->tabWidget->currentIndex()));
        qDebug() << ui->tabWidget->currentIndex();
        return false;
    } else {
        if(!ui->tabWidget->tabText(ui->tabWidget->currentIndex()).contains(" *"))
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),
                                  ui->tabWidget->tabText(ui->tabWidget->currentIndex()) +" *");
        return true;
    }
}


void MainWindow::readFile()
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;


    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        textContainer->append(line);
    }
    initialContent = textContainer->toPlainText();
}


QString MainWindow::pathToNameFile()
{
    if(filePath.isEmpty())
        return "Nouveau document";

    return QString::fromStdString(
        fs::path(filePath.toStdString()).filename().generic_string());
}


// Etape 3

void MainWindow::cursorChanged()
{
    int column = ui->tabWidget->currentWidget()->findChild<QTextEdit *>()->textCursor().blockNumber();
    int row = ui->tabWidget->currentWidget()->findChild<QTextEdit *>()->textCursor().positionInBlock();
    ui->labelCursor->setText(QString(
        "Ligne: " + QString::number(row) +
        ", Colonne: " + QString::number(column)));
}
