/* 
 * File:   MainWindow.h
 * Author: parallels
 *
 * Created on August 31, 2015, 10:14 PM
 */

#ifndef _MAINWINDOW_H
#define	_MAINWINDOW_H

#include "ui_MainWindow.h"
#include "TableModel.h"
#include "FileArchiver.h"
#include <vector>
#include <iostream>
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(std::vector<versionRec>* Data);
    virtual ~MainWindow();
    
public slots:
    /**
     * Function for "Select file" button in GUI
     * allow users to choose the file for upload and save
     * It will display message either the file upload have
     * been saved or not.
     * It yes, it will display message and previous version(s) of file in table
     * If no, it will just display message to notice user this is first time
     */
    void selectFile();                          
    
    /**
     * Function for display table model in GUI
     * it will get the latest information from getVersionInfo(FileArchiver)
     * then display version(s) of file in GUI
     */
    void retrieveVersionDataForFile();          
    
    /**
     * Function for "Save Current" button
     * Allow user to save the different version of file
     * in condition the file wanted to save must not same 
     * with last previous file been saved to database
     */
    void saveCurrent();                        
    
    /**
     * Function to get know the position or number of version of file
     * that user select in table in GUI
     * 
     * @param index to get the row selected
     */
    void selectionVersionEntryTableDisplay(const QModelIndex& index); 
    
    /**
     * Get the comment of version of file selected by user
     */
    void showComment();                         
    
    /**
     * Function for "Retrieve Version" button
     * It will call "RetrieveForm" class to display a GUI interface
     * to get information of new Directory Path and new File Name
     * before pass to retrieveFile(FileArchiver)
     */
    void retrieveVersion();          
    
    /**
     * Function for "Set as reference" button
     * Allow user to select the version of the file be initial file
     * Then drop the file version before it
     * If file select is Version 0, then it will not work
     */
    void setAsReference();    
    
private:
    Ui::MainWindow widget;
    /**
     * Keep the data from getVersionInfo(FileArchiver)
     */
    std::vector<versionRec>* data;
    
    /**
     * Table Model for setting display in GUI
     */
    TableModel *tableModel;
    
    /**
     * Keep the path of file selected through "Select File" button
     */
    QString fileSelect;
    
    /**
     *  Instance of FileArchiver which allow MainWindow fuctions to communicate
     * and get info from it
     */
    FileArchiver file;
    
    /**
     * Keep the version of file selected by user through table in GUI
     */
    int fileVersionSelectedInTable;
    
    /**
     * Keep total number of file can be selected to set as reference
     */
    int totalEnableForSelection;
    
    /**
     * To detect the fileSelect has been saved or not for display purpose
     */
    bool saveFile;
    
    /**
     * To detect the status of program can carry out save process or not
     */
    bool enableSave;
    
    /**
     * One of the parameter need in QMessageBox::information, and so on
     */
    QWidget*parent;  
};

#endif	/* _MAINWINDOW_H */
