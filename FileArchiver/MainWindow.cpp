/*
 * File:   MainWindow.cpp
 * Author: parallels
 *
 * Created on August 31, 2015, 10:14 PM
 */

#include "MainWindow.h"
#include "RetrieveForm.h"
#include "helperFuncs.h"
#include <string>
MainWindow::MainWindow(std::vector<versionInfo>* Data) {
    data=Data;
    parent=0;
    widget.setupUi(this);
    
    tableModel = new TableModel(0);
    tableModel->addTheData(data);
    
    widget.fileView->setModel(tableModel);
    widget.fileView->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(widget.selectFileButton,SIGNAL(clicked()),this, SLOT(selectFile()));
    connect(widget.SaveCurrentButton,SIGNAL(clicked()),this, SLOT(saveCurrent()));
    connect(widget.SetReferenceButton,SIGNAL(clicked()),this, SLOT(setAsReference()));
    connect(widget.RetrieveVersionButton,SIGNAL(clicked()),this, SLOT(retrieveVersion()));
    connect(widget.ShowCommentButton,SIGNAL(clicked()),this, SLOT(showComment()));
}

MainWindow::~MainWindow() {
    
}
void MainWindow::selectFile()
{   
    QString fileSelection = QFileDialog::getOpenFileName(parent, "Open File",".",
            "Files (*.*)");
    if(!fileSelection.isEmpty()){
        fileSelect=fileSelection;
        widget.fileField->setText(fileSelection);
        saveFile = false;
    }

    if(file.exists(fileSelect.toStdString())){
        retrieveVersionDataForFile();
        widget.warningFrame->setPlainText("The older version(s) of file been stated as below");
    }
    else
        widget.warningFrame->setPlainText("The file does not been saved into the database yet");
    
}

void MainWindow::retrieveVersionDataForFile(){
    data->clear();
    std::vector<versionRec>temp = file.getVersionInfo(fileSelect.toStdString());
    unsigned int a;
    for(a=0;a<temp.size();a++)
    {
        versionInfo ver = &temp[a];
        //ver->symbolDecision = 1;
        data->push_back(ver);
    }
    // File have not been saved
    if(!saveFile)
    {
//        versionInfo ver;
//        ver->setVersionNumber(a);
//        ver->setModifyTime(getFileModifyTime(fileSelect.toStdString()));
//        ver->setLength(fileSize(fileSelect.toStdString()));
//        //ver->symbolDecision = 0;
        
//        data->push_back(ver);
    }
    tableModel->resetData(data);
    
    widget.fileView->setColumnWidth(0,50);
    widget.fileView->setColumnWidth(1,75);
    widget.fileView->setColumnWidth(2,250);
    widget.fileView->setColumnWidth(3,40);
}

void MainWindow::saveCurrent()
{
    bool ok;
    QString comment = QInputDialog::getText(this,"GetCommentForm","Comment",
                        QLineEdit::Normal,QDir::home().dirName(), &ok);
   
    if(ok)
    {    
        bool exist = file.exists(fileSelect.toStdString());
        if(exist)
        {
            bool different = file.differs(fileSelect.toStdString());
            if(different)
            {
                bool updateFile = file.update(fileSelect.toStdString(),comment.toStdString());
                if(!updateFile)
                {
                    std::string msg = "The file has NOT been save due to corruption";
                    QMessageBox::information(this,fileSelect,msg.c_str(),
                        QMessageBox::Ok,QMessageBox::Cancel);
                }
                else
                    saveFile = true;
            }
            else
            {
                std::string msg  ="The file has NOT been modified no save required";
                QMessageBox::information(this,fileSelect,msg.c_str(),
                        QMessageBox::Ok,QMessageBox::Cancel);
            }
        }
        else
        {
            file.insertNew(fileSelect.toStdString(), comment.toStdString());
            fileSelect.clear();
            widget.fileField->clear();
        }
        retrieveVersionDataForFile();
    }
    else
    {
        std::string msg = "The file has NOT no save due 'Cancel' button clicked";
        QMessageBox::information(parent,fileSelect,msg.c_str(),
                QMessageBox::Ok,QMessageBox::Cancel);
    }
}

void MainWindow::selectionVersionEntryTableDisplay(const QModelIndex& index)
{
    fileVersionSelectedInTable = index.row();
}

void MainWindow::showComment()
{
    string comment = data->at(fileVersionSelectedInTable)->getComment();
    QMessageBox::information(parent,"Comment for selected version", 
            comment.c_str(),QMessageBox::Ok,QMessageBox::Cancel);
}

void MainWindow::retrieveVersion()
{
    
    RetrieveForm *rf = new RetrieveForm;
    rf->exec();

    QString fileName = rf->getFilename();
    QString directoryPath = rf->getDirectoryPath();
    if(!fileName.isEmpty() & !directoryPath.isEmpty())
    {
        directoryPath.append('/');
        directoryPath.append(fileName);
        file.retrieveVersion(fileSelect.toStdString(), directoryPath.toStdString(),
                fileVersionSelectedInTable);
    }
    else
    {
        std::string msg = "There are incomplete information needed for 'Retrieve Version' button or Cancel button been clicked";
        QMessageBox::critical(this,"Retrieve Version",msg.c_str(),
            QMessageBox::Ok,QMessageBox::Cancel);
    }
        
}

void MainWindow::setAsReference()
{
    if(fileVersionSelectedInTable==0)
    {
        std::string msg = "The file elected is initial file. The function is aborted";
        QMessageBox::information(this,fileSelect,msg.c_str(),
            QMessageBox::Ok,QMessageBox::Cancel);
        return;
    }
    QMessageBox::StandardButton reply=QMessageBox::question(parent,
            "Set reference version","Set reference version",
            QMessageBox::Ok|QMessageBox::No);
    
    if(reply == QMessageBox::Ok)
    {
        bool ok;
        QString comment = QInputDialog::getText(this,"GetCommentForm","Comment",
                            QLineEdit::Normal,QDir::home().dirName(), &ok);

        if(ok)
        {
            bool success = file.setReference(fileSelect.toStdString(), 
                    fileVersionSelectedInTable,comment.toStdString());
            if(success)
            {
                char* msg;
                sprintf(msg,"The version(s) of file before No.%d drop successful",
                        fileVersionSelectedInTable);
                QMessageBox::information(this,fileSelect,msg,
                    QMessageBox::Ok,QMessageBox::Cancel);
            }
            else
            {
                
                std::string msg = "There are error(s) occur. The function is aborted";
                QMessageBox::critical(this,fileSelect,msg.c_str(),
                    QMessageBox::Ok,QMessageBox::Cancel);
            }
            retrieveVersionDataForFile();
        }
        else
        {
            QMessageBox msgBox;
            std::string errorMessage = "The user must click 'Ok' in GetCommentForm.";
            msgBox.setWindowTitle("Error!");
            msgBox.setText("Incomplete Progress");
            msgBox.setDetailedText(errorMessage.c_str());
            msgBox.exec();
        }
    }    
}
