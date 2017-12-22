#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QListWidgetItem>
#include <vector>

#include <QDialog>

namespace Ui {
class ConfigWindow;
}

class ConfigWindow : public QDialog {
    Q_OBJECT

  public:
    explicit ConfigWindow(QWidget *parent = 0);
    ~ConfigWindow();

  signals:
    void sendConfig(bool pos,bool vel,bool sta, bool ang ,bool ball,std::vector<bool> players);

  private slots:
    void on_okButton_clicked();

    void on_canselButton_clicked();

    void on_toggleLeftButton_clicked();

    void on_toggleRightButton_clicked();

  private:
    Ui::ConfigWindow *ui;
    QListWidgetItem *pos_check_box;
    QListWidgetItem *vel_check_box;
    QListWidgetItem *stamina_check_box;
    QListWidgetItem *angle_check_box;
    QListWidgetItem *ball_check_box;
    std::vector<QListWidgetItem *> player_check_boxes;
};

#endif // CONFIGWINDOW_H
