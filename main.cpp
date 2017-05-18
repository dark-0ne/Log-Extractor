#include <QCoreApplication>
#include <iostream>
#include <string>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>


using namespace std;
int main(int argc, char *argv[])
{
    pair <double , double > ball;
    vector < pair <double , double> > left;
    vector < pair <double , double> > right;
    QFile file("/home/dark0ne/Documents/2DRCSS/Log-Extractor/input.txt");
    if(!file.open(QFile::ReadOnly |
                  QFile::Text))
    {
        qDebug() << " Could not open the file for reading";
        return 0;
    }
    QTextStream in(&file);
    QString myText = in.readAll();

    for(int f=1;f<311;f++){
        string show1 = "show ";
        string show2 = "show ";
        QString str1;
        QString str2;
        str1.setNum(f);
        str2.setNum(f+1);
        show1.append(str1.toStdString());
        show2.append(str2.toStdString());


        int index_num1 = myText.indexOf(QString::fromStdString(show1) , 0);
        int index_num2 = myText.indexOf(QString::fromStdString(show2) , 0);

        QString current_cycle_log = myText.mid(index_num1 , index_num2 - index_num1);

        int index_l[11];
        int index_r[11];

        int index_ball_data = current_cycle_log.indexOf("b");

        for(int i=0;i<11;i++){
            QString l(QString::fromStdString("l "));
            l.append(QString::number(i+1));
            index_l[i] = current_cycle_log.indexOf(l);
        }

        for(int i=0;i<11;i++){
            QString r(QString::fromStdString("r "));
            r.append(QString::number(i+1));
            index_r[i] = current_cycle_log.indexOf(r);
        }

        bool isSecondSpace = false;
        bool record = false;
        QString ball_x;
        QString ball_y;
        int i ;
        for(i= index_ball_data ; isSecondSpace == false ; i++){

            if(current_cycle_log[i] == QChar(' ')){
                if(record){
                    isSecondSpace = true;
                    continue;
                }
                record = true;
                continue;
            }


            if(record){
                ball_x.append(current_cycle_log.at(i));
            }


        }

        for(;current_cycle_log.at(i) != space;i++){
            ball_y.append(current_cycle_log.at(i));
        }

        ball.first = ball_x.toDouble();
        ball.second = ball_y.toDouble();

        QString l_x;
        QString l_y;
        for(int j=0;j<11;j++){
        int num_spaces_to_skip = 4;
        for(i = index_l[j];num_spaces_to_skip >0 ;i++)
            if(current_cycle_log.at(i).isSpace())
                num_spaces_to_skip--;
        index_l[j] = i;
        for(i;not current_cycle_log.at(i).isSpace() ; i++)
        {
            l_x.append(current_cycle_log.at(i));
        }

        for(++i;not current_cycle_log.at(i).isSpace() ; i++)
        {
            l_y.append(current_cycle_log.at(i));
        }


        pair <double , double> temp;

        temp.first = l_x.toDouble();
        temp.second = l_y.toDouble();
        left.push_back(temp);
        l_x.clear();
        l_y.clear();

        }

        QString r_x;
        QString r_y;

        for(int j=0;j<11;j++){
        int num_spaces_to_skip = 4;
        for(i = index_r[j];num_spaces_to_skip >0 ;i++)
            if(current_cycle_log.at(i).isSpace())
                num_spaces_to_skip--;
        index_r[j] = i;
        for(i;not current_cycle_log.at(i).isSpace() ; i++)
        {
            r_x.append(current_cycle_log.at(i));
        }

        for(++i;not current_cycle_log.at(i).isSpace() ; i++)
        {
            r_y.append(current_cycle_log.at(i));
        }


        pair <double , double> temp;

        temp.first = r_x.toDouble();
        temp.second = r_y.toDouble();
        right.push_back(temp);
        r_x.clear();
        r_y.clear();

        }

        cout << "Cycle " << f << ": " << endl;

        cout << "Ball: " << ball.first << " - " << ball.second << endl;
        for(int l=0;l < left.size();l++){
            cout << "Left " << l << ": " << left.at(l).first<< " - "<< left.at(l).second << endl;
        }
        for(int r=0;r < right.size();r++){
            cout << "Right " << r << ": " << right.at(r).first << " - "<< right.at(r).second << endl;
        }

        //TODO: Write in output
        left.clear();
        right.clear();

    }



    file.close();

}
