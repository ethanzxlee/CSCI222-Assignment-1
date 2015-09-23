/*
 * File:   GetCommentForm.cpp
 * Author: parallels
 *
 * Created on September 23, 2015, 8:08 PM
 */

#include "GetCommentForm.h"
#include <iostream>
GetCommentForm::GetCommentForm() {
    ok = false;
    widget.setupUi(this);
    connect(widget.buttonBox,SIGNAL(accepted()),this, SLOT(accept()));
    connect(widget.buttonBox,SIGNAL(rejected()),this, SLOT(reject()));
}

GetCommentForm::~GetCommentForm() {
}

void GetCommentForm::reject()
{
    QDialog::reject();
}

void GetCommentForm::accept()
{
    comment = widget.commentInput->toPlainText();
    if(comment.isEmpty())
        comment="-";
    ok = true;
    QDialog::accept();
}

QString GetCommentForm::getComment()
{
    return comment;
}

bool GetCommentForm::getOk()
{
    return ok;
}