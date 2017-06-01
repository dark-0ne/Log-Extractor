#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_isCurrentlyExtracting = false;
    m_extractor = new Extractor();
    m_extractor->setIsCurrentlyExtracting(&m_isCurrentlyExtracting);

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
    if(m_isCurrentlyExtracting)
    {
        QMessageBox msgBox(this);
        msgBox.setText("Currently Extracting!");

        msgBox.exec();
        return;
    }
    else{
        m_extractor->setCurrent_player(ui->playerComboBox->currentIndex());
        m_extractor->setExtract_ball_pos(ui->ballPosCheckBox->isChecked());
        m_extractor->setProgressBar(ui->progressBar);
        m_extractor->setInputString(ui->inputText->text());
        m_extractor->setOutputString(ui->outputText->text());

//        connect(m_extractor,&Extractor::finished,m_extractor,&QObject::deleteLater);

        m_extractor->start();
    }
}

void MainWindow::extractFinished()
{
    m_isCurrentlyExtracting = false;
}
