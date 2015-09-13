/*
 * File:   main.cpp
 * Author: parallels
 *
 * Created on August 31, 2015, 10:13 PM
 */

#include <QtGui/QApplication>
#include "MainWindow.h"
int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);
    // create and show your widgets here
    std::vector<versionRec> data;
    MainWindow win(&data);
    win.show();
    return app.exec();
}
