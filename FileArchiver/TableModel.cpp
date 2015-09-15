#include <qt4/Qt/qimage.h>
#include <iostream>
#include "TableModel.h"
#include <QTableWidgetItem>
#include <QDateTime>
TableModel::TableModel(QObject *parent)
:QAbstractTableModel(parent){
    correctIcon = convertImage("./images/saved.jpg");
    wrongIcon = convertImage("./images/unsaved.jpg");
}

void TableModel::addTheData(std::vector<versionRec> *data)
{
    this->recordsCollection = data;
}

int TableModel::rowCount(const QModelIndex& /*parent*/)const
{
    return recordsCollection->size();
}

int TableModel::columnCount(const QModelIndex& /*parent*/)const
{
    return 4;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole){
        if(orientation == Qt::Horizontal){
            switch(section){
                case 0: return QString("Saved");
                case 1: return QString("Version#");
                case 2: return QString("Data");
                case 3: return QString("Size");
            }
        }
    }
    return QVariant();
}

QVariant TableModel::data(const QModelIndex &index, int role)const{
    if(role==Qt::DisplayRole){
        if(index.column()==1)
        {
            std::string temp;
            std::ostringstream converter;
            converter<<recordsCollection->at(index.row()).getVersionNumber();
            temp = converter.str();
            return QString(temp.c_str());
        }
        if(index.column()==2)
        {
            qint64 modifyTime = recordsCollection->at(index.row()).getModifyTime();
            QDateTime qModifyTime = QDateTime::fromMSecsSinceEpoch(modifyTime * 1000);
            return qModifyTime.toString("yyyy-MM-dd hh:mm:ss");
        }
        if(index.column()==3)
        {
            
            std::string temp;
            std::ostringstream converter;
            converter<<recordsCollection->at(index.row()).getLength();
            temp = converter.str();
            return QString(temp.c_str());
        }
        if(index.column()==0)
            return QVariant();
    }
    else
    {
        if((role==Qt::DecorationRole)&&(index.column()==0)){
            QByteArray imageBytes;
            if(recordsCollection->at(index.row()).getSymbol()==0)
                imageBytes= QByteArray::fromBase64(wrongIcon.c_str());
            else
                
                imageBytes= QByteArray::fromBase64(correctIcon.c_str());
            QImage img = QImage::fromData(imageBytes);
            return img;
        }
    }
    return QVariant();
}
    

Qt::ItemFlags TableModel::flags(const QModelIndex& index) const{
    if(!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractTableModel::flags(index)|Qt::ItemIsSelectable;
}

std::string TableModel::convertImage(std::string imgFile)
{
    QString qtImg(imgFile.c_str());
    QImage imgLoad;
    bool success = imgLoad.load(qtImg);
    if(!success){
        std::cout<<"Image failed to load for "<<imgFile<<std::endl;
        exit(1);
    }
        
    QByteArray qba;
    QBuffer qb(&qba);
    imgLoad.save(&qb,"JPG");
    
    QByteArray coded = qba.toBase64();
    std::string finalResult(coded);
    return finalResult;
}

void TableModel::resetData(std::vector<versionRec>*newRecords)
{
    beginResetModel();
    recordsCollection = newRecords;
    endResetModel();
    emit(dataChanged(QModelIndex(),QModelIndex()));
}
