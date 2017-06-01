#include "extractor.h"

#include <QFile>
#include <QDebug>

#include <iostream>
#include <vector>

using namespace std;

Extractor::Extractor(QString in, QString out,int current_player,bool extract_ball_pos)
{
    m_inputString = in;
    m_outputString = out;
    m_current_player = current_player;
    m_extract_ball_pos = extract_ball_pos;
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

    for(int current_cycle_count=1;current_cycle_count<5999;current_cycle_count++){
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
        {
            if(current_cycle_count ==2999)
            {
                current_cycle_count++;
                continue;
            }
            else { break; }
        }
        m_num_cycles = current_cycle_count;
        QString current_cycle_log = input.mid(index_start , index_end - index_start);

        if(m_extract_ball_pos)
        {
            extract_ball_pos(current_cycle_log);
        }
        if(m_current_player == 0){
            extract_left_pos(current_cycle_log);
            extract_right_pos(current_cycle_log);
        }
        else if(m_current_player < 12){
            // for left
            extract_left_pos(current_cycle_log,m_current_player);
        }
        else{
            //for right
            extract_right_pos(current_cycle_log,m_current_player);
        }


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

    LogOutStructure tmp_log;

    tmp_log.cycle = m_num_cycles;
    tmp_log.ball_vec = temp;

    m_log_out_structure.push_back(tmp_log);

    return true;
}


bool Extractor::extract_left_pos(QString input){
    QString l_x;
    QString l_y;

    LogOutStructure tmp_log;
    int index_l[11];

    for(int i=0;i<11;i++){
        QString l(QString::fromStdString("(l "));
        l.append(QString::number(i+1));
        index_l[i] = input.indexOf(l);
        l.append(")");
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
        tmp_log.left_pos_vec.push_back(temp);
        l_x.clear();
        l_y.clear();

    }

    tmp_log.cycle = m_num_cycles;

    m_log_out_structure.push_back(tmp_log);

}

bool Extractor::extract_right_pos(QString input){
    LogOutStructure tmp_log;
    QString r_x;
    QString r_y;

    int index_r[11];

    for(int i=0;i<11;i++){
        QString r(QString::fromStdString("(r "));
        r.append(QString::number(i+1));
        index_r[i] = input.indexOf(r);
        r.append(")");
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
        tmp_log.right_pos_vec.push_back(temp);
        r_x.clear();
        r_y.clear();

    }
    tmp_log.cycle = m_num_cycles;
    m_log_out_structure.push_back(tmp_log);
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
        for(int counter = 0;counter < m_log_out_structure.size();counter++){
            QTextStream stream(&outputFile);

            stream << "Cycle " << m_log_out_structure.at(counter).cycle <<endl;

            if(m_extract_ball_pos)
                stream << "ball_pos " << m_log_out_structure.at(counter).ball_vec.first << " " <<m_log_out_structure.at(counter).ball_vec.second << endl;

            if(m_current_player == 0)
            {
                for(int player_counter = 0; player_counter < m_log_out_structure.at(counter).left_pos_vec.size();player_counter++)
                {
                    stream << "left_pos_" << player_counter+1 << " " << m_log_out_structure.at(counter).left_pos_vec.at(player_counter).first
                           << " " <<m_log_out_structure.at(counter).left_pos_vec.at(player_counter).second << endl;
                }
                for(int player_counter = 0; player_counter < m_log_out_structure.at(counter).right_pos_vec.size();player_counter++)
                {
                    stream << "right_pos_" << player_counter+1 << " " << m_log_out_structure.at(counter).right_pos_vec.at(player_counter).first
                           << " " <<m_log_out_structure.at(counter).right_pos_vec.at(player_counter).second << endl;
                }

            }
            else if(m_current_player < 12)
            {
                stream << "left_pos_" << m_current_player << " " << m_log_out_structure.at(counter).left_pos_vec.at(0).first
                       << " " <<m_log_out_structure.at(counter).left_pos_vec.at(0).second << endl;
            }
            else
            {
                stream << "right_pos_" << m_current_player << " " << m_log_out_structure.at(counter).right_pos_vec.at(0).first
                       << " " <<m_log_out_structure.at(counter).right_pos_vec.at(0).second << endl;
            }

            stream.flush();
        }
    }
    outputFile.close();
}

bool Extractor::extract_left_pos(QString input,int player_unum){
    LogOutStructure tmp_log;
    QString l_x;
    QString l_y;

    int index_l;
    QString l(QString::fromStdString("(l "));
    l.append(QString::number(player_unum));
    l.append(")");
    index_l = input.indexOf(l);

    int i =skip_characters(input,index_l,4);
    for(i;not input.at(i).isSpace() ; i++)
    {
        l_x.append(input.at(i));
    }

    for(++i;not input.at(i).isSpace() ; i++)
    {
        l_y.append(input.at(i));
    }
    pair <double,double> tmp;

    tmp.first = l_x.toDouble();
    tmp.second = l_y.toDouble();
    tmp_log.left_pos_vec.push_back(tmp);
    l_x.clear();
    l_y.clear();

    tmp_log.cycle = m_num_cycles;

    m_log_out_structure.push_back(tmp_log);
}

bool Extractor::extract_right_pos(QString input,int player_unum){
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
