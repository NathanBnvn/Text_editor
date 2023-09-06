#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cqtextedit.h"

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

    // QTextEdit* edit = qobject_cast<QTextEdit*>(tabWidget->widget(index));

    connect (ui->actionOpenFile, SIGNAL(triggered(bool)), this, SLOT(selectFile()));
    connect (ui->tabWidget->tabBar(), SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    connect (ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect (ui->tabWidget->currentWidget()->findChild<QTextEdit *>(), SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));
    //connect (textContainer, SIGNAL(textHasChanged(bool)), this, SLOT(test(bool)));
    //connect (textContainer, SIGNAL(cursorPositionChanged()), this, SLOT(test2()));

    ui->labelCursor->setText(QString(
        "Ligne: " + QString::number(textContainer->cursor_row) +
        ", Colonne: " + QString::number(textContainer->cursor_column)));
}

// -----------------------------------------------------------------------------------------

//non l'algorithme c'est : chaque fois que le QtextEdit est changé quant au contenu on doit vérifier
//si le contenu est égal à celui qui a été mis initialement
//s'il est égal on envoi un signal au tab pour lui dire c'est égal sinon un signal pour lui dire ce n'est pas égal
//comme tu feras des connect entre les textedit et les tab tu ne peux pas avoir de probleme
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
        MainWindow::textContainer = new CQTextEdit();
        ui->tabWidget->addTab(textContainer, fileName);
    }

    readFile();
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
    textContainer->initialContent = textContainer->toPlainText();
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

    if(hasBeenEdited(id)){
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


bool MainWindow::hasBeenEdited(int id)
{
    initialContents[id] = textContainer->initialContent;
    if(textContainer->initialContent.isEmpty())
        return false;

    if(textContainer->toPlainText().isEmpty())
        return false;

    qDebug() << initialContents[id] << "-" << textContainer->toPlainText();

    while(true)
    if(initialContents[id] == textContainer->toPlainText()){
        ui->tabWidget->setTabText(id, ui->tabWidget->tabText(id));
        return false;
    } else {
        if(!ui->tabWidget->tabText(id).contains(" *"))
        ui->tabWidget->setTabText(id,
                                  ui->tabWidget->tabText(id) +" *");

        return true;
    }
}


QString MainWindow::pathToNameFile()
{
    if(filePath.isEmpty())
        return "Nouveau document";

    return QString::fromStdString(
        fs::path(filePath.toStdString()).filename().generic_string());
}

void MainWindow::tabChanged(int id){
    hasBeenEdited(id);
}

void MainWindow::test(bool b){
    if(b){
        qDebug() << 200;
    }else{
        qDebug() << 2400;
    }
}

// Etape 3

void MainWindow::test2()
{
    ui->labelCursor->setText(QString(
        "Ligne: " + QString::number(textContainer->cursor_row) +
        ", Colonne: " + QString::number(textContainer->cursor_column)));
}

void MainWindow::cursorChanged()
{
    //int column = ui->tabWidget->currentWidget()->findChild<QTextEdit *>()->textCursor().blockNumber();
    //int row = ui->tabWidget->currentWidget()->findChild<QTextEdit *>()->textCursor().positionInBlock();
    //ui->labelCursor->setText(QString(
    //    "Ligne: " + QString::number(row) +
    //    ", Colonne: " + QString::number(column)));
}

//
// QFileDialog::history() Returns the browsing history of the filedialog as a list of paths.
