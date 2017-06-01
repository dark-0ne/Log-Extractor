#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "extractor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{

    friend class Extractor;
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_inputButton_clicked();

    void on_outputButton_clicked();

    void on_extractButton_clicked();

    void extractFinished();

private:
    Ui::MainWindow *ui;

    bool m_isCurrentlyExtracting;

    Extractor *m_extractor;

};

#endif // MAINWINDOW_H
