#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "extractor.h"

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

  private slots:
    void on_inputButton_clicked();

    void on_outputButton_clicked();

    void on_extractButton_clicked();

  private:
    Ui::MainWindow *ui;

    Extractor *m_extractor;

};

#endif // MAINWINDOW_H
