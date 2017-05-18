#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "extractor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_inputButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
                                                    tr("Game Files (*.rcg)"));

    ui->inputText->setText(fileName);
    int index = fileName.indexOf(".rcg");
    fileName = fileName.left(index);
    fileName.append(".txt");
    ui->outputText->setText(fileName);


}

void MainWindow::on_outputButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
                                                    tr("Text Files (*.txt)"));
    ui->outputText->setText(fileName);
}

void MainWindow::on_extractButton_clicked()
{
    Extractor extractor(ui->inputText->text(),ui->outputText->text());
    extractor.execute();
}
