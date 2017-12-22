#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <QString>

#include <vector>
#include "extractor.h"

using namespace std;

class handler : public QObject {
    Q_OBJECT
  public:
    handler();

    void set_extract_players(bool extr[22]);

    void setExtr_ball(bool value);

    void setExtract_pos(bool value);

    void setExtract_vel(bool value);

    void setExtract_stamina(bool value);

    void setExtract_angles(bool value);

    void setInputString(const QString &value);

    void setOutputString(const QString &value);

    void start();

  public slots:
    void finished_one_cycle();

  signals:
    void signal_progress_bar(int value);

  private:
    QString inputString;        //Path to input file
    QString outputString;       //Path to output file

    int last_cycle;             //Last cycle in log
    int cycles_done;

    bool player_to_extract[22]; //Which players to extract
    bool extr_ball;             //Extract ball or not
    bool extract_pos;           //Extract position
    bool extract_vel;           //Extract velocity
    bool extract_stamina;       //Extract player stamina
    bool extract_angles;

    vector<vector<LogOutStructure> > outputs;

    void write_to_file();
    void findLastCycle();

};

#endif // HANDLER_H
