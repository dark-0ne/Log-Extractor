#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "handler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {

    friend class Extractor;
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

  public slots:
    void update_progress_bar(int value);
    void extract_finished();
    void update_config(bool pos,bool vel,bool sta, bool ang ,bool ball,std::vector<bool> players);

  private slots:
    void on_inputButton_clicked();

    void on_outputButton_clicked();

    void on_extractButton_clicked();

    void on_configButton_clicked();

  private:
    Ui::MainWindow *ui;

    handler *m_handler;

    bool to_extract[4];
    bool extract_ball;
    bool extract_player[22];

};

#endif // MAINWINDOW_H
