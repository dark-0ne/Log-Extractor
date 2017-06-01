#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <QString>

#include <vector>

using namespace std;

struct LogOutStructure{
    int cycle;
    pair <double,double> ball_vec;
    vector <pair <double,double> > left_pos_vec;
    vector <pair <double,double> > right_pos_vec;
};

class Extractor
{
public:
    Extractor(QString in, QString out,int current_player,bool extract_ball_pos);
    bool execute();

private:
    QString m_inputString;
    QString m_outputString;

    vector <LogOutStructure> m_log_out_structure;

    vector<pair <double,double> > m_ball_vec;
    vector<vector <pair <double,double> > > m_left_pos_vec;
    vector<vector <pair <double,double> > > m_right_pos_vec;

    bool extract_ball_pos(QString input);
    bool extract_left_pos(QString input);
    bool extract_right_pos(QString input);

    bool extract_left_pos(QString input,int player_unum);
    bool extract_right_pos(QString input,int player_unum);

    int m_current_player;
    int m_num_cycles;
    bool m_extract_ball_pos;

    bool write_to_file();

    int skip_characters(QString input, int current_index, int num_skip_chars);

};

#endif // EXTRACTOR_H
