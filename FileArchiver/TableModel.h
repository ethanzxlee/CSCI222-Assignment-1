/* 
 * File:   TableModel.h
 *
 * Created on August 31, 2015, 10:43 PM
 * 
 * This is use to create class TableModel for List of version of file
 */

#ifndef TABLEMODEL_H
#define	TABLEMODEL_H
#include <iostream>
#include <QAbstractTableModel>
#include <QtGui>
#include <vector>
#include "versionRec.h"
typedef versionRec* versionInfo;
//typedef struct versioninfo
//{
//    int versionNumber;
//    std::string dataFile;
//    int sizeFile;
//    int symbolDecision;// 1 for correct icon, 0 for wrong icon
//}versionInfo;
class TableModel:public QAbstractTableModel
{
public:
    TableModel(QObject *parent);
    void addTheData(std::vector<versionInfo> *data);
    int rowCount(const QModelIndex &parent = QModelIndex())const;
    int columnCount(const QModelIndex &parent = QModelIndex())const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index)const;
    bool removeRows(int position, int rows, const QModelIndex& index);
    bool insertRows(int position, int rows, const QModelIndex &index);
    std::string resizeImage(std::string imgFile);
    void resetData(std::vector<versionInfo>*newRecords);
private:
    std::string correctIcon;
    std::string wrongIcon;
    std::vector<versionInfo>*recordsCollection;
    TableModel& operator=(const TableModel&);
    TableModel(const TableModel& orig);
    
};


#endif	/* TABLEMODEL_H */

