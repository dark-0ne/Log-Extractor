#include "extractor.h"

#include <QFile>
#include <QDebug>

#include <iostream>
#include <vector>

using namespace std;

Extractor::Extractor(QString in, QString out)
{
    m_inputString = in;
    m_outputString = out;
}

bool Extractor::execute(){

    QFile inputFile(m_inputString);

    if(!inputFile.open(QFile::ReadOnly |
                       QFile::Text))
    {
        qDebug() << " Could not open the file for reading";
        return false;
    }

    QTextStream in(&inputFile);
    QString input = in.readAll();

    for(int current_cycle_count=1;current_cycle_count<=6000;current_cycle_count++){
        QString show1("show ");
        QString show2 ("show ");
        QString str_start;
        QString str_end;
        str_start.setNum(current_cycle_count);
        str_end.setNum(current_cycle_count+1);
        show1.append(str_start);
        show2.append(str_end);


        int index_start = input.indexOf(show1 , 0);
        int index_end = input.indexOf(show2, 0);

        if(index_end == -1)
            break;
        QString current_cycle_log = input.mid(index_start , index_end - index_start);

        extract_ball_pos(current_cycle_log);
        extract_left_pos(current_cycle_log);
        extract_right_pos(current_cycle_log);

    }

    write_to_file();

    inputFile.close();
}


bool Extractor::extract_ball_pos(QString input){

    int index_ball_data = input.indexOf("b");

    QString ball_x;
    QString ball_y;

    int i = skip_characters(input,index_ball_data,1);

    for(i ;not input.at(i).isSpace() ; i++){
        ball_x.append(input.at(i));
    }

    i++;

    for(i;not input.at(i).isSpace();i++){
        ball_y.append(input.at(i));
    }

    pair<double,double> temp;

    temp.first = ball_x.toDouble();
    temp.second = ball_y.toDouble();

    m_ball_vec.push_back(temp);

    return true;
}


bool Extractor::extract_left_pos(QString input){
    vector <pair <double,double> > left;
    QString l_x;
    QString l_y;

    int index_l[11];

    for(int i=0;i<11;i++){
        QString l(QString::fromStdString("l "));
        l.append(QString::number(i+1));
        index_l[i] = input.indexOf(l);
    }

    for(int j=0;j<11;j++){
        int i =skip_characters(input,index_l[j],4);
        for(i;not input.at(i).isSpace() ; i++)
        {
            l_x.append(input.at(i));
        }

        for(++i;not input.at(i).isSpace() ; i++)
        {
            l_y.append(input.at(i));
        }

        pair <double , double> temp;

        temp.first = l_x.toDouble();
        temp.second = l_y.toDouble();
        left.push_back(temp);
        l_x.clear();
        l_y.clear();

    }

    m_left_pos_vec.push_back(left);
}

bool Extractor::extract_right_pos(QString input){
    vector <pair <double,double> > right;
    QString r_x;
    QString r_y;

    int index_r[11];

    for(int i=0;i<11;i++){
        QString r(QString::fromStdString("r "));
        r.append(QString::number(i+1));
        index_r[i] = input.indexOf(r);
    }

    for(int j=0;j<11;j++){
        int i =skip_characters(input,index_r[j],4);

        for(i;not input.at(i).isSpace() ; i++)
        {
            r_x.append(input.at(i));
        }

        for(++i;not input.at(i).isSpace() ; i++)
        {
            r_y.append(input.at(i));
        }


        pair <double , double> temp;

        temp.first = r_x.toDouble();
        temp.second = r_y.toDouble();
        right.push_back(temp);
        r_x.clear();
        r_y.clear();

    }

    m_right_pos_vec.push_back(right);
}

int Extractor::skip_characters(QString input, int current_index, int num_skip_chars){
    int i;
    for(i = current_index;num_skip_chars >0 ;i++)
        if(input.at(i).isSpace())
            num_skip_chars--;
    return i;
}

bool Extractor::write_to_file(){

    QFile outputFile(m_outputString);

    if (!outputFile.open(QIODevice::WriteOnly)) {
        //TODO: exception handling
        return false;
    } else {
        for(int counter = 0;counter < m_ball_vec.size();counter++){
            QTextStream stream(&outputFile);

            stream << "Cycle " << counter+1 <<endl;

            stream << "ball_pos " << m_ball_vec.at(counter).first << " " <<m_ball_vec.at(counter).second << endl;

            for(int player_counter = 0; player_counter < m_left_pos_vec.at(counter).size();player_counter++)
            {
                stream << "left_pos_" << player_counter+1 << " " << m_left_pos_vec.at(counter).at(player_counter).first << " " <<m_left_pos_vec.at(counter).at(player_counter).second << endl;
            }

            for(int player_counter = 0; player_counter < m_right_pos_vec.at(counter).size();player_counter++)
            {
                stream << "right_pos_" << player_counter+1 << " " << m_right_pos_vec.at(counter).at(player_counter).first << " " <<m_right_pos_vec.at(counter).at(player_counter).second << endl;
            }

            stream.flush();
        }
    }
    outputFile.close();
}
