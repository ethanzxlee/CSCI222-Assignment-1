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
class TableModel:public QAbstractTableModel
{
public:
    TableModel(QObject *parent);
    
    /**
     * To add the data to recordsCollection(variable in this class) at intial
     * @param data in vector form of versionInfo
     */
    void addTheData(std::vector<versionInfo> *data);
    
    /**
     * Get the number of row(s) needed for table display in GUI
     * @param parent
     * @return number of row(s) for table display
     */
    int rowCount(const QModelIndex &parent = QModelIndex())const;
    
    /**
     * Get the number of columns needed for table display in GUI
     * It has been fixed
     * @param parent
     * @return 4 as the result of 4 columns needed in GUI table
     */
    int columnCount(const QModelIndex &parent = QModelIndex())const;
    
    /**
     * Get the information needed for display in table
     * @param index
     * @param role
     * @return QVariant which maybe the QString of information or QImage
     */
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    
    /**
     * Get the title of header for table display
     * @param section
     * @param orientation
     * @param role
     * @return QString of header according to the 'role'
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    
    /**
     * Get the flag sign needed
     * @param index
     * @return the flags sign
     */
    Qt::ItemFlags flags(const QModelIndex &index)const;
    
    /**
     * Resize the icon for correct(saved) png & wrong(unsaved) png
     * @param imgFile
     * @return string of information of img
     */
    std::string resizeImage(std::string imgFile);
    
    /**
     * Reset the whole table for display in GUI
     * @param newRecords
     */
    void resetData(std::vector<versionInfo>*newRecords);
private:
    std::string correctIcon;
    std::string wrongIcon;
    std::vector<versionInfo>*recordsCollection;
    TableModel& operator=(const TableModel&);
    TableModel(const TableModel& orig);
    
};


#endif	/* TABLEMODEL_H */

