/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTableView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *fileField;
    QPushButton *selectFileButton;
    QPushButton *SaveCurrentButton;
    QPushButton *RetrieveVersionButton;
    QPushButton *SetReferenceButton;
    QPushButton *ShowCommentButton;
    QTableView *fileView;
    QPlainTextEdit *warningFrame;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(591, 578);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 66, 17));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 70, 131, 17));
        QFont font;
        font.setPointSize(15);
        font.setBold(false);
        font.setWeight(50);
        label_2->setFont(font);
        fileField = new QLineEdit(centralwidget);
        fileField->setObjectName(QString::fromUtf8("fileField"));
        fileField->setEnabled(false);
        fileField->setGeometry(QRect(50, 20, 431, 27));
        selectFileButton = new QPushButton(centralwidget);
        selectFileButton->setObjectName(QString::fromUtf8("selectFileButton"));
        selectFileButton->setGeometry(QRect(490, 20, 98, 27));
        SaveCurrentButton = new QPushButton(centralwidget);
        SaveCurrentButton->setObjectName(QString::fromUtf8("SaveCurrentButton"));
        SaveCurrentButton->setGeometry(QRect(450, 90, 141, 27));
        RetrieveVersionButton = new QPushButton(centralwidget);
        RetrieveVersionButton->setObjectName(QString::fromUtf8("RetrieveVersionButton"));
        RetrieveVersionButton->setGeometry(QRect(450, 130, 141, 27));
        SetReferenceButton = new QPushButton(centralwidget);
        SetReferenceButton->setObjectName(QString::fromUtf8("SetReferenceButton"));
        SetReferenceButton->setGeometry(QRect(450, 170, 141, 27));
        ShowCommentButton = new QPushButton(centralwidget);
        ShowCommentButton->setObjectName(QString::fromUtf8("ShowCommentButton"));
        ShowCommentButton->setGeometry(QRect(450, 210, 141, 27));
        fileView = new QTableView(centralwidget);
        fileView->setObjectName(QString::fromUtf8("fileView"));
        fileView->setGeometry(QRect(20, 90, 421, 431));
        warningFrame = new QPlainTextEdit(centralwidget);
        warningFrame->setObjectName(QString::fromUtf8("warningFrame"));
        warningFrame->setEnabled(false);
        warningFrame->setGeometry(QRect(20, 120, 421, 401));
        warningFrame->setBackgroundVisible(true);
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Version Data", 0, QApplication::UnicodeUTF8));
        selectFileButton->setText(QApplication::translate("MainWindow", "Select File", 0, QApplication::UnicodeUTF8));
        SaveCurrentButton->setText(QApplication::translate("MainWindow", "Save Current", 0, QApplication::UnicodeUTF8));
        RetrieveVersionButton->setText(QApplication::translate("MainWindow", "Retrieve Version", 0, QApplication::UnicodeUTF8));
        SetReferenceButton->setText(QApplication::translate("MainWindow", "Set As Reference", 0, QApplication::UnicodeUTF8));
        ShowCommentButton->setText(QApplication::translate("MainWindow", "Show Comment", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
