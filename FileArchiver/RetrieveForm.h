/* 
 * File:   RetrieveForm.h
 * Author: parallels
 *
 * Created on September 2, 2015, 9:08 PM
 */

#ifndef _RETRIEVEFORM_H
#define	_RETRIEVEFORM_H

#include "ui_RetrieveForm.h"
#include <iostream>
#include "qfiledialog.h"
#include "qmessagebox.h"
#include <QInputDialog>
#include <QIcon>

class RetrieveForm : public QDialog {
    Q_OBJECT
public:
    RetrieveForm();
    virtual ~RetrieveForm();
    QString getFilename();
    QString getDirectoryPath();
signals:
    void accepted();
    void rejected();
public slots:
    /**
     * Function similar to selectFile in MainWindow class
     * but it is used for selection of directory wanted
     */
    void selectionDir(); 
    
    /**
     * Function for button "Ok"
     * save the information filled in by user
     */
    void accept();
    
    /**
     * Function for button "Cancel"
     * The information filled will be deleted
     */
    void reject();
private:
    QString fileName;
    QString directoryPath;
    Ui::RetrieveForm widget;
};

#endif	/* _RETRIEVEFORM_H */
