#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <QString>
#include <QThread>
#include <QProgressBar>

#include <vector>

using namespace std;

struct LogOutStructure{
    int cycle;
    pair <double,double> ball_vec;
    vector <pair <double,double> > left_pos_vec;
    vector <pair <double,double> > right_pos_vec;
};

class Extractor : public QThread
{
public:
    Extractor();
    void run() override;

    void setCurrent_player(int current_player);

    void setExtract_ball_pos(bool extract_ball_pos);

    void setProgressBar(QProgressBar *progressBar);

    void setIsCurrentlyExtracting(bool *isCurrentlyExtracting);

    void setInputString(const QString &inputString);

    void setOutputString(const QString &outputString);

signals:
    static void extracted();

private:
    QString m_inputString;
    QString m_outputString;
    QProgressBar *m_progressBar;

    vector <LogOutStructure> m_log_out_structure;

    int m_num_cycles;
    int m_last_cycle;

    int m_current_player;
    bool m_extract_ball_pos;

    bool *m_isCurrentlyExtracting;

    bool write_to_file();
    int skip_characters(QString input, int current_index, int num_skip_chars);
    void findLastCycle();

    bool extract_ball_pos(QString input);
    bool extract_left_pos(QString input);
    bool extract_right_pos(QString input);

    bool extract_left_pos(QString input,int player_unum);
    bool extract_right_pos(QString input,int player_unum);


};

#endif // EXTRACTOR_H
