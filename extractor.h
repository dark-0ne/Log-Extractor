#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <QString>
#include <QThread>
#include <QProgressBar>

#include <vector>
#include "movable.h"
#include "player.h"

using namespace std;

struct LogOutStructure {
    int cycle;
    movable ball;
    vector <player > left;
    vector <player> right;
};

class Extractor : public QThread {
    Q_OBJECT

    friend class MainWindow;

  public:
    explicit Extractor(QWidget *parent=0);
    virtual ~Extractor();
    void run() override;

    void set_extract_players(bool extr[22]);

    void setInputString(const QString &inputString);

    void setOutputString(const QString &outputString);

    void setExtract_pos(bool value);

    void setExtract_vel(bool value);

    void setExtract_stamina(bool value);

    void setExtract_angles(bool value);

    void setExtr_ball(bool value);

  signals:
    void signal_progress_bar(int value);

  private:
    QString inputString;        //Path to input file
    QString outputString;       //Path to output file

    vector <LogOutStructure> log_out_structure;     //Vector used to save extracted data

    int last_cycle;             //Last cycle in log

    bool player_to_extract[22]; //Which players to extract
    bool extr_ball;             //Extract ball or not
    bool extract_pos;           //Extract position
    bool extract_vel;           //Extract velocity
    bool extract_stamina;       //Extract player stamina
    bool extract_angles;        //Extract player body & head(relative to body) angles

    void write_to_file();
    int skip_characters(QString *input, int current_index, int num_skip_chars);
    void findLastCycle();

    void extract_ball(QString *input);
    void extract_left(QString *input);      //Extract all required left player
    void extract_right(QString *input);     //Extract all required right player

    void extract_left(QString *input,int player_unum);  //Extract specific left player
    void extract_right(QString *input,int player_unum); //Extract specific right player


};

#endif // EXTRACTOR_H
