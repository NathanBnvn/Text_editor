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
    ui->searchFrame->hide();
    ui->swapFrame->hide();

    connect (ui->actionOpenFile, SIGNAL(triggered(bool)), this, SLOT(selectFile()));
    connect (ui->actionSearch, SIGNAL(triggered(bool)), this, SLOT(showSearch(bool)));
    connect (ui->actionSwap, SIGNAL(triggered(bool)), this, SLOT(showSwap(bool)));
    connect (ui->tabWidget->tabBar(), SIGNAL(tabBarClicked(int)), this, SLOT(tabChanged(int)));
    connect (ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    if(ui->tabWidget->currentIndex() == 0){
        connect (ui->textContainer, SIGNAL(textHasChanged(bool)), this, SLOT(hasBeenEdited(bool)));
    }

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

    fileName = pathToNameFile();

    if(ui->textContainer->document()->isEmpty())
    {
        textContainer = ui->textContainer;
        ui->tabWidget->setTabText(0, fileName);
    } else {
        textContainer = new CQTextEdit();
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
    QMessageBox messageBox(this);
    messageBox.setWindowTitle(QString("Textus Editor - Save file"));
    messageBox.setText(QString("Document modifié"));
    messageBox.setInformativeText(QString("Souhaitez-vous sauvegarder les modifications ?"));
    messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Save);

    if(textContainer->textChanged){
        int choice = messageBox.exec();

        QByteArray content = textContainer->toPlainText().toUtf8();

        switch (choice) {
        case QMessageBox::Save:
            QFileDialog::saveFileContent(content,
                                         ui->tabWidget->tabText(id));
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

    } else {
        ui->tabWidget->removeTab(id);
    }
}


void MainWindow::hasBeenEdited(bool edited)
{
    if(this->textContainer->initialContent.isEmpty())
        return;

    if(this->textContainer->toPlainText().isEmpty())
        return;

    if(edited){
        if(!ui->tabWidget->tabText(ui->tabWidget->currentIndex()).contains(" *"))
            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),
                                      ui->tabWidget->tabText(ui->tabWidget->currentIndex()) +" *");
        textContainer->textChanged = true;
    } else {
        QString tabName = ui->tabWidget->tabText(ui->tabWidget->currentIndex()).remove(" *");
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), tabName);
        textContainer->textChanged = false;
    }
}


QString MainWindow::pathToNameFile()
{
    if(filePath.isEmpty())
        return "Nouveau document";

    return QString::fromStdString(
        fs::path(filePath.toStdString()).filename().generic_string());
}

// optimisation posible
void MainWindow::tabChanged(int id){
    if(id > 0){
        connect (ui->tabWidget->widget(id), SIGNAL(textHasChanged(bool)), this, SLOT(hasBeenEdited(bool)));
    }

}

void MainWindow::showSearch(bool show){
    if(show){
        ui->searchFrame->show();
    }else{
        ui->searchFrame->hide();
    }
}

void MainWindow::showSwap(bool show){
    if(show){
        ui->searchFrame->show();
        ui->swapFrame->show();
    } else {
        ui->searchFrame->hide();
        ui->swapFrame->hide();
    }
}



//TP Editeur de texte

// **** Lecture de fichiers texte ****
// L’éditeur sera capable de lire plusieurs fichiers texte et de les afficher dans des onglets.
// Les onglets pourront être supprimés en appuyant sur une croix de fermeture
// Classses Qt utiles : QFile, QFileDialog, QTabBar, QTextDocument


// **** Écriture de fichiers texte ****
//  L’éditeur sera capable d’écrire sur disque les fichiers modifiés.
//  Classes Qt utiles : QFile, QFileDialog
//  l’édition d’un fichier modifié et non encore sauvegardé devra être signalé par
//  exemple par adjonction dans son nom d’un caractère *.
//  L’éditeur devra demander en cas de fermeture d’un fichier modifié si l’utilisateur
//  souhaite en faire sauvegarde ou abandonner les modifications


// **** Affichage de la ligne et de la colonne du curseur ****
// L’éditeur devra affichier la ligne et la colonne du curseur
// QTextCursor


// **** Recherche d’un texte dans un fichier ouvert ****
// L’éditeur devra être capable de permettre la recherche d’un texte dans un fichier
// avec éventuellement les options majuscules, mots seulement.


// **** Remplacement d’un texte dans un fichier ****
// L’éditeur devra être capable de permettre le remplacement d’un texte dans un fichier.


// **** Mémorisation des derniers fichiers ouverts ****
// L’éditeur devra être capable d’afficher les dix derniers fichiers ouverts.
//   Classes Qt utiles : QSettings

