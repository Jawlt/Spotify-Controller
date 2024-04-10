/**
 * @author Jwalant Patel, Ross Cameron, Lance Cheong Youne, Ojas Singh Hunjan, Matthew Morelli
 * @date 2024-03-31
 * @brief This class calls the mainwindow.cpp and runs the QApplication
*/
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
