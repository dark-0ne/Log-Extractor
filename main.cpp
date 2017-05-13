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
    vector <pair <double , double > > ball;
    vector < pair <double , double> > left;
    vector < pair <double , double> > right;
    QChar space(' ');
    string s;
    s.
    QFile file("/home/arminsadreddin/Documents/posLogExtractor/input.txt");
    if(!file.open(QFile::ReadOnly |
                  QFile::Text))
    {
        qDebug() << " Could not open the file for reading";
        return 0;
    }
    QTextStream in(&file);
    QString myText = in.readAll();


    for(int f=0;f<311;f++){
        string show1 = "show ";
        string show2 = "show ";
        QString str1;
        QString str2;
        str1.setNum(f);
        str2.setNum(f+1);
        show1.append(str1.toStdString());
        show2.append(str2.toStdString());


        int index_num1 = myText.indexOf(QString::fromStdString(show1));
        int index_num2 = myText.indexOf(QString::fromStdString(show2));

        QString current_cycle_log = myText.mid(index_num1 , index_num2 - index_num1);


        int index_l[12];
        int index_r[12];


        int index_ball_data = current_cycle_log.indexOf("((b)");

        for(int i=1;i<12;i++){
            QString l(QString::fromStdString("((l "));
            l.append(i);
            l.append(")");
            index_l[i] = current_cycle_log.indexOf(l);
        }

        for(int i=1;i<12;i++){
            QString r(QString::fromStdString("((r "));
            r.append(i);
            r.append(")");
            index_r[i] = current_cycle_log.indexOf(r);
        }

        bool isSecondSpace = false;
        bool record = false;
        QString ball_x;
        QString ball_y;
        int i ;
        for(i= index_ball_data ; isSecondSpace == false ; i++){

            if(current_cycle_log.at(i) == space){
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


        cout << f << " : " << ball_x.toStdString() << " - "<<ball_y.toStdString() << "\n";



    }





    //    int j = 13;
    //    stringstream convert; // stringstream used for the conversion
    //    convert << j;
    //    show = show.append(convert.str());

    //    cout << i ;
    //    //qDebug() << myText;

    //    file.close();

}
