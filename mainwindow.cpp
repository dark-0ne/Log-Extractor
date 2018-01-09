#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QPixmap>
#include <QIcon>
#include <QCloseEvent>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QPixmap pixmap("/home/dark0ne/Documents/2DRCSS/Log-Extractor/settings.png");
    QIcon ButtonIcon(pixmap);
    ui->configButton->setIcon(ButtonIcon);
    ui->configButton->setIconSize(QSize(35,35));
    m_extractor = NULL;

    extract_ball = true;

    for (int i=0; i<3; i++) {
        to_extract[i]= true;
    }
    to_extract[3] = false;

    for (int i=0; i<22; i++) {
        extract_player[i]= true;
    }

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
    m_extractor = NULL;
}

void MainWindow::update_config(bool pos,bool vel,bool sta, bool ang ,bool ball,std::vector<bool> players) {

    to_extract[0]  = pos;
    to_extract[1]  = vel;
    to_extract[2]  = sta;
    to_extract[3]  = ang;

    for (int i=0; i<22; i++) {
        extract_player[i] = players.at(i);
    }

    extract_ball = ball;
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

    else if (m_extractor==NULL) {

        m_extractor = new Extractor(this);
        connect(m_extractor,SIGNAL(signal_progress_bar(int)),this,SLOT(update_progress_bar(int)));
        connect(m_extractor,SIGNAL(finished()),this,SLOT(extract_finished()));

        m_extractor->setExtr_ball(extract_ball);
        m_extractor->set_extract_players(extract_player);
        m_extractor->setInputString(ui->inputText->text());
        m_extractor->setOutputString(ui->outputText->text());
        m_extractor->setExtract_pos(to_extract[0]);
        m_extractor->setExtract_vel(to_extract[1]);
        m_extractor->setExtract_stamina(to_extract[2]);
        m_extractor->setExtract_angles(to_extract[3]);


        m_extractor->start();
    } else {
        QMessageBox msgBox(this);
        msgBox.setText("Please wait we are currently extracting!");
        msgBox.exec();
    }
}

void MainWindow::on_configButton_clicked() {
    ConfigWindow configWindow(this);

    connect(&configWindow,SIGNAL(sendConfig(bool,bool,bool,bool,bool,std::vector<bool>)),this,SLOT(update_config(bool,bool,
            bool,
            bool,bool,std::vector<bool>)));

    configWindow.exec();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Log Extractor",
                                         tr("Are you sure?\n"),
                                         QMessageBox::No | QMessageBox::Yes,
                                         QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}
