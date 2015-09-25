# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Release/GNU-Linux-x86
TARGET = FileArchiver
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
PKGCONFIG +=
QT = core gui widgets
SOURCES += FileArchiver.cpp GetCommentForm.cpp MainWindow.cpp RetrieveForm.cpp TableModel.cpp fileRec.cpp helperFuncs.cpp main.cpp versionRec.cpp
HEADERS += FileArchiver.h GetCommentForm.h MainWindow.h RetrieveForm.h TableModel.h fileRec.h helperFuncs.h ui_MainWindow.h ui_RetrieveForm.h versionRec.h
FORMS += GetCommentForm.ui MainWindow.ui RetrieveForm.ui
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Release/GNU-Linux-x86
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += . 
LIBS += 
