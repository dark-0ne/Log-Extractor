#include "extractor.h"

#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QXmlStreamWriter>

#include <iostream>
#include <vector>

using namespace std;

Extractor::Extractor(QWidget *parent) {
}

Extractor::~Extractor() {

}

void Extractor::run() {

    QFile inputFile(inputString);

    if (!inputFile.open(QFile::ReadOnly |
                        QFile::Text)) {
        qDebug() << " Could not open the file for reading";
        return;
    }

    findLastCycle();
    QTextStream in(&inputFile);
    QString input = in.readAll();

    for (int current_cycle_count = 1; current_cycle_count <= last_cycle; current_cycle_count++) {

        cout << "Starting cycle " << current_cycle_count << endl;

        QString show1("show ");
        QString show2("show ");
        QString str_start;
        QString str_end;
        str_start.setNum(current_cycle_count);
        str_end.setNum(current_cycle_count + 1);
        show1.append(str_start);
        show2.append(str_end);

        int index_start = input.indexOf(show1, 0);
        int index_end = input.indexOf(show2, 0);

        if (index_start == -1 && current_cycle_count == 3000) {
            continue;
        }

        if (index_start == -1 && index_end == -1) {
            break;
        }

        if (index_end == -1) {
            if (current_cycle_count == 2999) {
                show2 = "show 3001";
                index_end = input.indexOf(show2);
            } else if (current_cycle_count == 5999) {
                show2 = "msg 6000";
                index_end = input.indexOf(show2);
            }
        }

        QString current_cycle_log = input.mid(index_start, index_end - index_start);

        log_out_structure.push_back(LogOutStructure());

        log_out_structure.back().cycle = current_cycle_count;

        if (extr_ball) {
            extract_ball(&current_cycle_log);
        }

        extract_left(&current_cycle_log);
        extract_right(&current_cycle_log);

        input = input.mid(index_end);
        emit signal_progress_bar(100 * current_cycle_count / last_cycle);

    }

    inputFile.close();
    write_to_file();
    emit signal_progress_bar(100);

}


void Extractor::extract_ball(QString *input) {

    int index_ball_data = input->indexOf("b");

    QString pos_x;
    QString pos_y;

    QString vel_x;
    QString vel_y;

    int i = skip_characters(input, index_ball_data, 1);

    for (i ; not input->at(i).isSpace() ; i++) {
        pos_x.append(input->at(i));
    }

    for (++i; not input->at(i).isSpace(); i++) {
        pos_y.append(input->at(i));
    }


    for (++i; not input->at(i).isSpace(); i++) {
        vel_x.append(input->at(i));
    }

    for (++i; not input->at(i).isSpace(); i++) {
        vel_y.append(input->at(i));
    }

    LogOutStructure *tmp_log = &log_out_structure.back();

    tmp_log->ball.setPos(Vector2D(pos_x.toDouble(),pos_y.toDouble()));
    tmp_log->ball.setVel(Vector2D(vel_x.toDouble(),vel_y.toDouble()));
}


void Extractor::extract_left(QString *input) {

    for (int i = 1; i <= 11; i++) {
        if (player_to_extract[i]) {
            extract_left(input,i);
        }
    }
}

void Extractor::extract_right(QString *input) {
    for (int i = 1; i <= 11; i++) {
        if (player_to_extract[i+11]) {
            extract_right(input,i);
        }
    }
}

int Extractor::skip_characters(QString *input, int current_index, int num_skip_chars) {
    int i;

    for (i = current_index; num_skip_chars > 0 ; i++)
        if (input->at(i).isSpace()) {
            num_skip_chars--;
        }

    return i;
}

void Extractor::write_to_file() {

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

        auto cycle_it = log_out_structure.begin();

        xmlWriter.writeStartElement("Data");
        for (; cycle_it != log_out_structure.end(); cycle_it++) {

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

    outputFile.close();
}

void Extractor::extract_left(QString *input, int player_unum) {
    LogOutStructure *tmp_log = &log_out_structure.back();

    QString pos_x;
    QString pos_y;

    QString vel_x;
    QString vel_y;

    QString body_angle;
    QString head_angle;

    QString stamina;

    int index_l;
    QString l(QString::fromStdString("(l "));
    l.append(QString::number(player_unum));
    l.append(")");
    index_l = input->indexOf(l);

    int i = skip_characters(input, index_l, 4);

    for (i; not input->at(i).isSpace() ; i++) {
        pos_x.append(input->at(i));
    }

    for (++i; not input->at(i).isSpace() ; i++) {
        pos_y.append(input->at(i));
    }

    for (++i; not input->at(i).isSpace() ; i++) {
        vel_x.append(input->at(i));
    }

    for (++i; not input->at(i).isSpace() ; i++) {
        vel_y.append(input->at(i));
    }

    for (++i; not input->at(i).isSpace() ; i++) {
        body_angle.append(input->at(i));
    }

    for (++i; not input->at(i).isSpace() ; i++) {
        head_angle.append(input->at(i));
    }

    for (++i; input->at(i) != QChar('s')  ; i++) {}

    for (i +=2; not input->at(i).isSpace() ; i++) {
        stamina.append(input->at(i));
    }

    player plr_tmp;

    plr_tmp.setPos(Vector2D(pos_x.toDouble(),pos_y.toDouble()));
    plr_tmp.setVel(Vector2D(vel_x.toDouble(),vel_y.toDouble()));
    plr_tmp.setBody_angle(body_angle.toDouble());
    plr_tmp.setHead_angle(head_angle.toDouble());
    plr_tmp.setStamina(stamina.toFloat());
    plr_tmp.setUnum(player_unum);

    tmp_log->left.push_back(plr_tmp);
}

void Extractor::extract_right(QString *input, int player_unum) {
    LogOutStructure *tmp_log = &log_out_structure.back();

    QString pos_x;
    QString pos_y;

    QString vel_x;
    QString vel_y;

    QString body_angle;
    QString head_angle;

    QString stamina;

    int index_r;
    QString r(QString::fromStdString("(r "));
    r.append(QString::number(player_unum));
    r.append(")");
    index_r = input->indexOf(r);

    int i = skip_characters(input, index_r, 4);

    for (i; not input->at(i).isSpace() ; i++) {
        pos_x.append(input->at(i));
    }

    for (++i; not input->at(i).isSpace() ; i++) {
        pos_y.append(input->at(i));
    }

    for (++i; not input->at(i).isSpace() ; i++) {
        vel_x.append(input->at(i));
    }

    for (++i; not input->at(i).isSpace() ; i++) {
        vel_y.append(input->at(i));
    }

    for (++i; not input->at(i).isSpace() ; i++) {
        body_angle.append(input->at(i));
    }

    for (++i; not input->at(i).isSpace() ; i++) {
        head_angle.append(input->at(i));
    }

    for (++i; input->at(i) != QChar('s')  ; i++) {}

    for (i +=2; not input->at(i).isSpace() ; i++) {
        stamina.append(input->at(i));
    }

    player plr_tmp;

    plr_tmp.setPos(Vector2D(pos_x.toDouble(),pos_y.toDouble()));
    plr_tmp.setVel(Vector2D(vel_x.toDouble(),vel_y.toDouble()));
    plr_tmp.setBody_angle(body_angle.toDouble());
    plr_tmp.setHead_angle(head_angle.toDouble());
    plr_tmp.setStamina(stamina.toFloat());
    plr_tmp.setUnum(player_unum);

    tmp_log->right.push_back(plr_tmp);
}

void Extractor::setInputString(const QString &inputString) {
    this->inputString = inputString;
}

void Extractor::setOutputString(const QString &outputString) {
    this->outputString = outputString;
}

void Extractor::setExtract_pos(bool value) {
    extract_pos = value;
}

void Extractor::setExtract_vel(bool value) {
    extract_vel = value;
}

void Extractor::setExtract_stamina(bool value) {
    extract_stamina = value;
}

void Extractor::setExtract_angles(bool value) {
    extract_angles = value;
}

void Extractor::setExtr_ball(bool value) {
    extr_ball = value;
}

void Extractor::set_extract_players(bool extract_pl[22]) {
    for (int count = 0; count < 22; count++) {
        this->player_to_extract[count] = extract_pl[count];
    }
}

void Extractor::findLastCycle() {
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
