#include "handler.h"
#include <QTextStream>
#include <QFile>
#include <QXmlStreamWriter>
#include <vector>
#include <QMessageBox>
#include "extractor.h"
#include <QFuture>
#include <qtconcurrentrun.h>

using namespace std;

handler::handler() {
    cycles_done = 0;

    for (int i=0; i<4; i++) {
        vector<LogOutStructure> tmp;
        outputs.push_back(tmp);
    }
}

void handler::set_extract_players(bool extract_pl[22]) {
    for (int count = 0; count < 22; count++) {
        this->player_to_extract[count] = extract_pl[count];
    }
}

void handler::setExtr_ball(bool value) {
    extr_ball = value;
}

void handler::setExtract_pos(bool value) {
    extract_pos = value;
}

void handler::setExtract_vel(bool value) {
    extract_vel = value;
}

void handler::setExtract_stamina(bool value) {
    extract_stamina = value;
}

void handler::setExtract_angles(bool value) {
    extract_angles = value;
}

void handler::setInputString(const QString &value) {
    inputString = value;
}

void handler::setOutputString(const QString &value) {
    outputString = value;
}

void handler::start() {
    QFile inputFile(inputString);

    if (!inputFile.open(QFile::ReadOnly |
                        QFile::Text)) {
        qDebug() << " Could not open the file for reading";
        return;
    }

    findLastCycle();
    QTextStream in(&inputFile);
    QString input = in.readAll();

    QString show_str = "show ";
    QString append_str;
    append_str.setNum(last_cycle/4);
    show_str.append(append_str);

    int index_quarter_end = input.indexOf(show_str, 0);

    QString first_quarter_log = input.mid(0,index_quarter_end);

    show_str = "show ";
    if (last_cycle/2 !=3000) {
        append_str.setNum(last_cycle/2);
    } else {
        append_str.setNum((last_cycle-1)/2);
    }
    show_str.append(append_str);

    int index_quarter_start = index_quarter_end;

    index_quarter_end = input.indexOf(show_str, index_quarter_start);

    QString second_quarter_log = input.mid(index_quarter_start,index_quarter_end-index_quarter_start);

    show_str = "show ";
    if (3*last_cycle/4 !=3000) {
        append_str.setNum(3*last_cycle/4);
    } else {
        append_str.setNum(3*(last_cycle-1)/4);
    }
    show_str.append(append_str);

    index_quarter_start = index_quarter_end;

    index_quarter_end = input.indexOf(show_str, index_quarter_start);

    QString third_quarter_log = input.mid(index_quarter_start,index_quarter_end-index_quarter_start);

    QString fourth_quarter_log = input.right(last_cycle-index_quarter_end);

    inputFile.close();

    Extractor extractor1;

    extractor1.setExtr_ball(extr_ball);
    extractor1.set_extract_players(player_to_extract);
    extractor1.setExtract_pos(extract_pos);
    extractor1.setExtract_vel(extract_vel);
    extractor1.setExtract_stamina(extract_stamina);
    extractor1.setExtract_angles(extract_angles);
    extractor1.setLog_out_structure(&outputs[0]);

    extractor1.setLog(first_quarter_log);
//    extractor[1].setLog(second_quarter_log);
//    extractor[2].setLog(third_quarter_log);
//    extractor[3].setLog(fourth_quarter_log);

    QFuture<void> t1 = QtConcurrent::run(extractor1,&Extractor::run);
//    QFuture<void> t2 = QtConcurrent::run(extractor[1],&Extractor::run);
//    QFuture<void> t3 = QtConcurrent::run(extractor[2],&Extractor::run);
//    QFuture<void> t4 = QtConcurrent::run(extractor[3],&Extractor::run);

    t1.waitForFinished();
//    t2.waitForFinished();
//    t3.waitForFinished();
//    t4.waitForFinished();

    write_to_file();
    emit signal_progress_bar(100);
}

void handler::finished_one_cycle() {

    emit signal_progress_bar(++cycles_done/last_cycle);
}

void handler::findLastCycle() {
    QFile inputFile(inputString);

    if (!inputFile.open(QFile::ReadOnly |
                        QFile::Text)) {
        qDebug() << " Could not open the file for reading!";
        return;
    }

    QTextStream in(&inputFile);
    QString input = in.readAll();
    int current_cycle = 3001;
    int counter = 6001;

    while (counter > 0) {
        QString show1("show ");
        QString show2("show ");

        show1.append(QString::number(current_cycle));
        show2.append(QString::number(current_cycle + 1));

        int index_start = input.indexOf(show1, 0);
        int index_next = input.indexOf(show2, 0);

        if (index_start != -1 && index_next == -1) {
            break;
        }

        if (index_start == -1) {
            current_cycle = current_cycle % 2 == 0 ? current_cycle / 2 : (current_cycle + 1) / 2;
        } else {
            int next = 6000 - current_cycle;
            current_cycle += next % 2 == 0 ? next / 2 : (next + 1) / 2;
        }

        counter--;
    }


    last_cycle = current_cycle;
}

void handler::write_to_file() {

    QFile outputFile(outputString);

    if (!outputFile.open(QIODevice::WriteOnly)) {
        QMessageBox msgBox;
        msgBox.setText("Can not write to file!");
        msgBox.exec();
        return;
    } else {

        QXmlStreamWriter xmlWriter(&outputFile);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();

        xmlWriter.writeStartElement("Data");
        for (int c=0; c<4; c++) {
            auto cycle_it = outputs[c].begin();
            for (; cycle_it != outputs[c].end(); cycle_it++) {

                xmlWriter.writeStartElement("Cycle");
                xmlWriter.writeAttribute("number", QString::number(cycle_it->cycle));

                //write ball pos
                if (extr_ball) {
                    xmlWriter.writeStartElement("Ball");

                    if (extract_pos) {
                        xmlWriter.writeAttribute("PosX", QString::number(cycle_it->ball.getPos().x, 'f', 4));
                        xmlWriter.writeAttribute("PosY", QString::number(cycle_it->ball.getPos().y, 'f', 4));
                    }

                    if (extract_vel) {
                        xmlWriter.writeAttribute("VelX", QString::number(cycle_it->ball.getVel().x, 'f', 4));
                        xmlWriter.writeAttribute("VelY", QString::number(cycle_it->ball.getVel().y, 'f', 4));
                    }

                    xmlWriter.writeEndElement();
                }
                //write left players
                auto player_it = cycle_it->left.begin();

                for (; player_it != cycle_it->left.end(); player_it++) {

                    xmlWriter.writeStartElement("Player");
                    xmlWriter.writeAttribute("Side", "Left");
                    xmlWriter.writeAttribute("Unum", QString::number(player_it->getUnum()));
                    if (extract_pos) {
                        xmlWriter.writeAttribute("PosX", QString::number(player_it->getPos().x, 'f', 4));
                        xmlWriter.writeAttribute("PosY", QString::number(player_it->getPos().y, 'f', 4));
                    }

                    if (extract_vel) {
                        xmlWriter.writeAttribute("VelX", QString::number(player_it->getVel().x, 'f', 4));
                        xmlWriter.writeAttribute("VelY", QString::number(player_it->getVel().y, 'f', 4));
                    }

                    if (extract_stamina) {
                        xmlWriter.writeAttribute("Stamina", QString::number(player_it->getStamina(),'f',4));
                    }

                    if (extract_angles) {
                        xmlWriter.writeAttribute("BodyAngle", QString::number(player_it->getBody_angle(), 'f', 4));
                        xmlWriter.writeAttribute("HeadAngle", QString::number(player_it->getHead_angle(), 'f', 4));
                    }
                    xmlWriter.writeEndElement();

                }

                player_it = cycle_it->right.begin();

                for (; player_it != cycle_it->right.end(); player_it++) {

                    xmlWriter.writeStartElement("Player");
                    xmlWriter.writeAttribute("Side", "Right");
                    xmlWriter.writeAttribute("Unum", QString::number(player_it->getUnum()));
                    if (extract_pos) {
                        xmlWriter.writeAttribute("PosX", QString::number(player_it->getPos().x, 'f', 4));
                        xmlWriter.writeAttribute("PosY", QString::number(player_it->getPos().y, 'f', 4));
                    }

                    if (extract_vel) {
                        xmlWriter.writeAttribute("VelX", QString::number(player_it->getVel().x, 'f', 4));
                        xmlWriter.writeAttribute("VelY", QString::number(player_it->getVel().y, 'f', 4));
                    }

                    if (extract_stamina) {
                        xmlWriter.writeAttribute("Stamina", QString::number(player_it->getStamina(),'f',4));
                    }

                    if (extract_angles) {
                        xmlWriter.writeAttribute("BodyAngle", QString::number(player_it->getBody_angle(), 'f', 4));
                        xmlWriter.writeAttribute("HeadAngle", QString::number(player_it->getHead_angle(), 'f', 4));
                    }
                    xmlWriter.writeEndElement();

                }
                xmlWriter.writeEndElement();
            }
            xmlWriter.writeEndElement();
            xmlWriter.writeEndDocument();

        }
    }

    outputFile.close();
}
