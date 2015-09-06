/*
 * File:   MainWindow.cpp
 * Author: parallels
 *
 * Created on August 31, 2015, 10:14 PM
 */

#include "MainWindow.h"
#include "RetrieveForm.h"
#include "helperFuncs.h"

MainWindow::MainWindow(std::vector<versionInfo>* Data) {
    data=Data;
    parent=0;
    widget.setupUi(this);
    
    tableModel = new TableModel(0);
    tableModel->addTheData(data);
    
    widget.fileView->setModel(tableModel);

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
    // if QFileDialog cant work, chg this -> parent
    QString fileSelection = QFileDialog::getOpenFileName(parent, "Open File",".",
            "Files (*.*)");
    if(!fileSelection.isEmpty()){
        fileSelect=fileSelection;
        widget.fileField->setText(fileSelection);
    }

    if(file.exists(fileSelect.toStdString())){
        retrieveVersionDataForFile();
        widget.warningFrame->clear();
    }
    else
        widget.warningFrame->setPlainText("The file does not been saved into the database yet");
    
}

void MainWindow::retrieveVersionDataForFile(){
    data->clear();
    std::vector<versionRec>temp = file.getVersionInfo(fileSelect.toStdString());
    
    for(unsigned int a=0;a<temp.size();a++)
    {
        //Assume getVersion is return version of file
        // getSize is return size of file
        // getData is return data of file
        versionInfo ver = &temp[a];
        ver->symbolDecision = 1;
        data->push_back(ver);
    }
    // File have not been saved
    if(!fileSelect.isEmpty())
    {
//        versionInfo ver;
//        ver->versionNumber= 12;
//        ver->setData("hello234");
//        ver->sizeFile = 12;
//        ver->symbolDecision = 0;
//        
//        tableModel->addRecord(ver);
    }
    tableModel->addTheData(data);
    widget.fileView->setModel(tableModel);
    widget.fileView->resizeRowsToContents();
    std::cout<<"\nok???1111\n";
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
                {
                    fileSelect.clear();
                    widget.fileField->clear();
                }
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
    string comment = file.getComment(fileSelect.toStdString(),fileVersionSelectedInTable);
    QMessageBox::information(parent,"Comment for selected version", 
            comment.c_str(),QMessageBox::Ok,QMessageBox::Cancel);
}

void MainWindow::retrieveVersion()
{
    
    RetrieveForm *rf = new RetrieveForm;
    rf->exec();

    QString fileName = rf->getFilename();
    QString directoryPath = rf->getDirectoryPath();

    file.retrieveVersion(fileVersionSelectedInTable, fileName.toStdString(),
            directoryPath.toStdString());
}

void MainWindow::setAsReference()
{
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
            int versionNumber;
            file.setReference(fileSelect.toStdString(), versionNumber,comment.toStdString());
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
