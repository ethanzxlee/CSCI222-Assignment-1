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
SOURCES += FileArchiver.cpp fileRec.cpp fileRecTestRun.cpp helperFuncs.cpp main.cpp
HEADERS += FileArchiver.h fileRec.h helperFuncs.h md5.h
FORMS +=
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
LIBS += -Wl,-rpath,/usr/lib /usr/lib/libmysqlcppconn.so -lboost_iostreams  
