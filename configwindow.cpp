#include <QListWidgetItem>
#include <vector>

#include "configwindow.h"
#include "ui_configwindow.h"

using namespace std;

ConfigWindow::ConfigWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigWindow) {
    ui->setupUi(this);

    //Add Extract Checkboxes
    pos_check_box = new QListWidgetItem("Position", ui->extractListWidget);
    pos_check_box->setFlags(pos_check_box->flags() | Qt::ItemIsUserCheckable);
    pos_check_box->setCheckState(Qt::Checked);

    vel_check_box = new QListWidgetItem("Velocity", ui->extractListWidget);
    vel_check_box->setFlags(vel_check_box->flags() | Qt::ItemIsUserCheckable);
    vel_check_box->setCheckState(Qt::Checked);

    stamina_check_box = new QListWidgetItem("Stamina", ui->extractListWidget);
    stamina_check_box->setFlags(stamina_check_box->flags() | Qt::ItemIsUserCheckable);
    stamina_check_box->setCheckState(Qt::Checked);

    angle_check_box = new QListWidgetItem("Body & Head Angle", ui->extractListWidget);
    angle_check_box->setFlags(angle_check_box->flags() | Qt::ItemIsUserCheckable);
    angle_check_box->setCheckState(Qt::Unchecked);


    //Add From Checkboxes
    ball_check_box = new QListWidgetItem("Ball", ui->fromListWidget);
    ball_check_box->setFlags(ball_check_box->flags() | Qt::ItemIsUserCheckable);
    ball_check_box->setCheckState(Qt::Checked);



    for (int count=0; count<11; count++) {
        QString str = "Left Player " + QString::number(count+1);
        QListWidgetItem *tmp = new QListWidgetItem(str, ui->fromListWidget);
        tmp->setFlags(tmp->flags() | Qt::ItemIsUserCheckable);
        tmp->setCheckState(Qt::Checked);

        player_check_boxes.push_back(tmp);

    }

    for (int count=0; count<11; count++) {
        QString str = "Right Player " + QString::number(count+1);
        QListWidgetItem *tmp = new QListWidgetItem(str, ui->fromListWidget);
        tmp->setFlags(tmp->flags() | Qt::ItemIsUserCheckable);
        tmp->setCheckState(Qt::Checked);

        player_check_boxes.push_back(tmp);

    }

}

ConfigWindow::~ConfigWindow() {
    delete ui;
    delete pos_check_box;
    delete vel_check_box;
    delete stamina_check_box;
    delete angle_check_box;

    player_check_boxes.clear();
}

void ConfigWindow::on_okButton_clicked() {

    vector<bool> players;
    bool ball = ball_check_box->checkState() == Qt::Checked ? true:false;

    bool pos = pos_check_box->checkState() == Qt::Checked ? true:false;
    bool vel = vel_check_box->checkState() == Qt::Checked ? true:false;
    bool sta = stamina_check_box->checkState() == Qt::Checked ? true:false;
    bool ang = angle_check_box->checkState() == Qt::Checked ? true:false;

    for (int i=0; i<22; i++) {
        player_check_boxes.at(i)->checkState() == Qt::Checked ? players.push_back(true):players.push_back(false);
    }

    emit sendConfig(pos,vel,sta,ang,ball,players);

    close();
}

void ConfigWindow::on_canselButton_clicked() {
    close();
}

void ConfigWindow::on_toggleLeftButton_clicked() {
    if (player_check_boxes.at(0)->checkState() == Qt::Checked) {
        for (int i=0; i<11; i++) {
            player_check_boxes.at(i)->setCheckState(Qt::Unchecked);
        }
    } else
        for (int i=0; i<11; i++) {
            player_check_boxes.at(i)->setCheckState(Qt::Checked);
        }
}

void ConfigWindow::on_toggleRightButton_clicked() {
    if (player_check_boxes.at(11)->checkState() == Qt::Checked) {
        for (int i=11; i<22; i++) {
            player_check_boxes.at(i)->setCheckState(Qt::Unchecked);
        }
    } else
        for (int i=11; i<22; i++) {
            player_check_boxes.at(i)->setCheckState(Qt::Checked);
        }
}
