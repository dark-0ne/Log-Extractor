#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <QString>

#include <vector>

using namespace std;

class Extractor
{
public:
    Extractor(QString in, QString out);
    bool execute();

private:
    QString m_inputString;
    QString m_outputString;

    vector<pair <double,double> > m_ball_vec;
    vector<vector <pair <double,double> > > m_left_pos_vec;
    vector<vector <pair <double,double> > > m_right_pos_vec;

    bool extract_ball_pos(QString input);
    bool extract_left_pos(QString input);
    bool extract_right_pos(QString input);

    bool write_to_file();

    int skip_characters(QString input, int current_index, int num_skip_chars);
};

#endif // EXTRACTOR_H
