#include "mainwindow.h"
#include <QApplication>
#include "extractor.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        return a.exec();
    } else {
        std::cout << "Starting extraction!" << std::endl;
        Extractor *m_extractor = new Extractor();

        bool player_to_extract[22];
        for (int count; count < 22; count++) {
            player_to_extract[count] = true;
        }
        QString input_path(argv[1]);
        QString output_path(argv[2]);

        m_extractor->setExtr_ball(true);
        m_extractor->set_extract_players(player_to_extract);
        m_extractor->setInputString(input_path);
        m_extractor->setOutputString(output_path);
        m_extractor->setExtract_pos(true);
        m_extractor->setExtract_vel(true);
        m_extractor->setExtract_stamina(true);
        m_extractor->setExtract_angles(true);


        m_extractor->run();

        std::cout << "Finished extraction!" << std::endl;
    }

}
