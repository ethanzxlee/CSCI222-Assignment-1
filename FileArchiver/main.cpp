/*
 * File:   main.cpp
 * Author: zhexian
 *
 * Created on 29 August 2015, 12:44 AM
 */

#include <QApplication>

#include "FileArchiver.h"

int main(int argc, char *argv[]) {
    FileArchiver fileArchiver;
    fileArchiver.insertNew("/home/zhexian/test.jpg", "a comment");
        //fileArchiver.differs("/home/zhexian/image.jpg");

    return 0;
//    QApplication app(argc, argv);
//
//    return app.exec();
//    return 0;
}
