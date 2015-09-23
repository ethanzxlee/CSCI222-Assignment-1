/* 
 * File:   GetCommentForm.h
 * Author: parallels
 *
 * Created on September 23, 2015, 8:08 PM
 */

#ifndef _GETCOMMENTFORM_H
#define	_GETCOMMENTFORM_H

#include "ui_GetCommentForm.h"

class GetCommentForm : public QDialog {
    Q_OBJECT
public:
    GetCommentForm();
    virtual ~GetCommentForm();
    QString getComment();
    bool getOk();
signals:
    void accepted();
    void rejected();
public slots:
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
    Ui::GetCommentForm widget;
    QString comment;
    bool ok;
};

#endif	/* _GETCOMMENTFORM_H */
