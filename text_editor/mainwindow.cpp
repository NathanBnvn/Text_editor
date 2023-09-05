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

namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Textus Editor");
    ui->tabWidget->removeTab(1);
    ui->tabWidget->setTabsClosable(true);

    connect (ui->actionOpenFile, SIGNAL(triggered(bool)), this, SLOT(selectFile()));
    connect (ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect (ui->textContainer->document(), SIGNAL(contentsChanged()), this, SLOT(hasBeenEdited()));
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

    MainWindow::fileName = MainWindow::pathToNameFile();

    if(ui->textContainer->document()->isEmpty())
    {
        MainWindow::textContainer = QObject::findChild<QTextEdit*>("textContainer");
        ui->tabWidget->setTabText(0, fileName);
    } else {
        MainWindow::textContainer = new QTextEdit();
        ui->tabWidget->addTab(textContainer, fileName);
    }

    readFile();
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
        qDebug() << choice;

        QByteArray content = QString(textContainer->toPlainText()).toUtf8();

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

// QFileDialog::history() Returns the browsing history of the filedialog as a list of paths.

bool MainWindow::hasBeenEdited()
{
    //ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), fileName +" *");
    return true;
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
}

QString MainWindow::pathToNameFile()
{
    if(filePath.isEmpty())
        return "Nouveau document";

    return QString::fromStdString(
        fs::path(filePath.toStdString()).filename().generic_string());
}


