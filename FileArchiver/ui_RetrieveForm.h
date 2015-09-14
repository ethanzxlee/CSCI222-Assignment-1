/********************************************************************************
** Form generated from reading UI file 'RetrieveForm.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RETRIEVEFORM_H
#define UI_RETRIEVEFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_RetrieveForm
{
public:
    QLabel *label;
    QLabel *label_2;
    QLineEdit *directoryField;
    QLineEdit *fileNameField;
    QPushButton *selectFileButton;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *RetrieveForm)
    {
        if (RetrieveForm->objectName().isEmpty())
            RetrieveForm->setObjectName(QString::fromUtf8("RetrieveForm"));
        RetrieveForm->resize(564, 284);
        label = new QLabel(RetrieveForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 30, 66, 17));
        label_2 = new QLabel(RetrieveForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 80, 66, 17));
        directoryField = new QLineEdit(RetrieveForm);
        directoryField->setObjectName(QString::fromUtf8("directoryField"));
        directoryField->setEnabled(false);
        directoryField->setGeometry(QRect(100, 30, 331, 27));
        fileNameField = new QLineEdit(RetrieveForm);
        fileNameField->setObjectName(QString::fromUtf8("fileNameField"));
        fileNameField->setGeometry(QRect(100, 80, 171, 27));
        selectFileButton = new QPushButton(RetrieveForm);
        selectFileButton->setObjectName(QString::fromUtf8("selectFileButton"));
        selectFileButton->setGeometry(QRect(440, 30, 111, 27));
        okButton = new QPushButton(RetrieveForm);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setGeometry(QRect(460, 230, 98, 27));
        cancelButton = new QPushButton(RetrieveForm);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(330, 230, 98, 27));

        retranslateUi(RetrieveForm);

        QMetaObject::connectSlotsByName(RetrieveForm);
    } // setupUi

    void retranslateUi(QDialog *RetrieveForm)
    {
        RetrieveForm->setWindowTitle(QApplication::translate("RetrieveForm", "RetrieveForm", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("RetrieveForm", "Directory", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("RetrieveForm", "Filename", 0, QApplication::UnicodeUTF8));
        selectFileButton->setText(QApplication::translate("RetrieveForm", "Select Directory", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("RetrieveForm", "OK", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("RetrieveForm", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RetrieveForm: public Ui_RetrieveForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RETRIEVEFORM_H
