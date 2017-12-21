#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::update_progress_bar(int value) {
    ui->progressBar->setValue(value);
}

void MainWindow::extract_finished() {
    m_extractor->quit();
    delete m_extractor;
}

void MainWindow::on_inputButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
                       tr("Game Files (*.rcg)"));

    ui->inputText->setText(fileName);
    int index = fileName.indexOf(".rcg");
    fileName = fileName.left(index);
    fileName.append(".xml");
    ui->outputText->setText(fileName);


}

void MainWindow::on_outputButton_clicked() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
                       tr("XML Files(*.xml)"));
    ui->outputText->setText(fileName);
}

void MainWindow::on_extractButton_clicked() {
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

        m_extractor = new Extractor(this);
        connect(m_extractor,SIGNAL(signal_progress_bar(int)),this,SLOT(update_progress_bar(int)));
        connect(m_extractor,SIGNAL(finished()),this,SLOT(extract_finished()));

        bool extract_players[22];
        for (int count = 0; count <22; count++) {
            extract_players[count] = true;
        }
        m_extractor->set_extract_players(extract_players);
        m_extractor->setExtr_ball(ui->ballPosCheckBox->isChecked());
        m_extractor->setInputString(ui->inputText->text());
        m_extractor->setOutputString(ui->outputText->text());
        m_extractor->setExtract_pos(true);
        m_extractor->setExtract_vel(true);
        m_extractor->setExtract_angles(true);
        m_extractor->setExtract_stamina(true);

        m_extractor->start();
    }
}
