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
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(868, 568);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 30, 66, 17));
        QFont font;
        font.setPointSize(12);
        label->setFont(font);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 70, 131, 17));
        QFont font1;
        font1.setPointSize(15);
        font1.setBold(false);
        font1.setWeight(50);
        label_2->setFont(font1);
        fileField = new QLineEdit(centralwidget);
        fileField->setObjectName(QString::fromUtf8("fileField"));
        fileField->setEnabled(false);
        fileField->setGeometry(QRect(50, 20, 661, 27));
        selectFileButton = new QPushButton(centralwidget);
        selectFileButton->setObjectName(QString::fromUtf8("selectFileButton"));
        selectFileButton->setGeometry(QRect(717, 20, 141, 27));
        SaveCurrentButton = new QPushButton(centralwidget);
        SaveCurrentButton->setObjectName(QString::fromUtf8("SaveCurrentButton"));
        SaveCurrentButton->setGeometry(QRect(720, 90, 141, 27));
        RetrieveVersionButton = new QPushButton(centralwidget);
        RetrieveVersionButton->setObjectName(QString::fromUtf8("RetrieveVersionButton"));
        RetrieveVersionButton->setGeometry(QRect(720, 140, 141, 27));
        SetReferenceButton = new QPushButton(centralwidget);
        SetReferenceButton->setObjectName(QString::fromUtf8("SetReferenceButton"));
        SetReferenceButton->setGeometry(QRect(720, 190, 141, 27));
        ShowCommentButton = new QPushButton(centralwidget);
        ShowCommentButton->setObjectName(QString::fromUtf8("ShowCommentButton"));
        ShowCommentButton->setGeometry(QRect(720, 240, 141, 27));
        fileView = new QTableView(centralwidget);
        fileView->setObjectName(QString::fromUtf8("fileView"));
        fileView->setEnabled(true);
        fileView->setGeometry(QRect(20, 90, 691, 431));
        fileView->setLayoutDirection(Qt::LeftToRight);
        fileView->setLineWidth(1);
        fileView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        fileView->setTextElideMode(Qt::ElideRight);
        fileView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        fileView->horizontalHeader()->setCascadingSectionResizes(true);
        MainWindow->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "File Archiver V1.3", 0, QApplication::UnicodeUTF8));
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
