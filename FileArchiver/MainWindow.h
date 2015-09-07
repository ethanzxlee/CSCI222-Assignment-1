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
    MainWindow(std::vector<versionInfo>* Data);
    virtual ~MainWindow();
    
public slots:
    void selectFile();                          // For Select File Button
    void retrieveVersionDataForFile();          // For retrieve the version of file
    void saveCurrent();                         // For Save Current Button
    
    // For selection in table display
    void selectionVersionEntryTableDisplay(const QModelIndex& index); 
    void showComment();                         // For Show Comment button
    void retrieveVersion();                     // For Retrieve Version Button
    void setAsReference();                      // For Save As Reference Button
private:
    Ui::MainWindow widget;
    std::vector<versionInfo>* data;
    TableModel *tableModel;
    QString fileSelect;
    FileArchiver file;
    int fileVersionSelectedInTable;
    bool saveFile;
    QWidget*parent;                             // for QMessageBox::information
};

#endif	/* _MAINWINDOW_H */
