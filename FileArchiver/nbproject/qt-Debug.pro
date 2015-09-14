# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Debug/GNU-Linux-x86
TARGET = FileArchiver
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += debug 
PKGCONFIG +=
QT = core gui widgets
SOURCES += FileArchiver.cpp MainWindow.cpp MurMurHash3.cpp RetrieveForm.cpp TableModel.cpp fileRec.cpp fileRecTestRun.cpp helperFuncs.cpp main.cpp versionRec.cpp
HEADERS += FileArchiver.h MainWindow.h MurMurHash3.h RetrieveForm.h TableModel.h fileRec.h helperFuncs.h ui_MainWindow.h ui_RetrieveForm.h versionRec.h
FORMS += MainWindow.ui RetrieveForm.ui
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/GNU-Linux-x86
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += . 
LIBS += -Wl,-rpath,/usr/lib /usr/lib/libmysqlcppconn.so -lboost_iostreams -lboost_filesystem -lboost_system  
