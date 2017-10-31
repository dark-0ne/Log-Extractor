#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_extractor = new Extractor();
    
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
    fileName.append(".xml");
    ui->outputText->setText(fileName);
    
    
}

void MainWindow::on_outputButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
                                                    tr("XML Files(*.xml)"));
    ui->outputText->setText(fileName);
}

void MainWindow::on_extractButton_clicked()
{
    if (ui->inputText->text().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText("Please enter log path!");
        msgBox.exec();
        return;
    }
    
    else if (ui->progressBar->value() != 0 && ui->progressBar->value() != 100) {
        QMessageBox msgBox(this);
        msgBox.setText("Please wait we are currently extracting!");
        msgBox.exec();
        return;
    } else {
        m_extractor->setCurrent_player(ui->playerComboBox->currentIndex());
        m_extractor->setExtract_ball_pos(ui->ballPosCheckBox->isChecked());
        m_extractor->setProgressBar(ui->progressBar);
        m_extractor->setInputString(ui->inputText->text());
        m_extractor->setOutputString(ui->outputText->text());
        
        m_extractor->start();
    }
}
