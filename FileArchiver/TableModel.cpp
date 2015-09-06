#include <qt4/Qt/qimage.h>
#include "TableModel.h"

TableModel::TableModel(QObject *parent)
:QAbstractTableModel(parent){
    correctIcon = resizeImage("./images/correct.png");
    wrongIcon = resizeImage("./images/wrong.png");
}

void TableModel::addTheData(std::vector<versionInfo> *data)
{
    this->recordsCollection = data;
    std::vector<versionInfo>::const_iterator it = recordsCollection->begin();
    for(it;it!=recordsCollection->end();it++)
        std::cout<<"\ninfo in addTheData: "<<(*it)->dataFile<<endl;
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
                case 0: return QString("Version#");
                case 1: return QString("Data");
                case 2: return QString("Size");
                case 3: return QString("Saved");
            }
        }
    }
    return QVariant();
}

QVariant TableModel::data(const QModelIndex &index, int role)const{
    if(role==Qt::DisplayRole){
        
        if(index.column()==0)
        {
            char * temp;
            sprintf(temp,"%d",recordsCollection->at(index.row())->versionNumber);
            return QString(temp);
        }
        if(index.column()==1)
        {
            std::string temp = recordsCollection->at(index.row())->dataFile;
            return QString(temp.c_str());
        }
        if(index.column()==2)
        {
            char * temp;
            sprintf(temp,"%d",recordsCollection->at(index.row())->sizeFile);
            return QString(temp);
        }
        if(index.column()==3)
            return QVariant();
    }
    else
    {
        if((role==Qt::DecorationRole)&&(index.column()==3)){
            QByteArray imageBytes;
            if(recordsCollection->at(index.row())->symbolDecision==1)
                imageBytes= QByteArray::fromBase64(correctIcon.c_str());
            else
                
                imageBytes= QByteArray::fromBase64(wrongIcon.c_str());
            QImage img = QImage::fromData(imageBytes);
            return img;
        }
    }
    return QVariant();
}
    
bool TableModel::insertRows(int /*position*/, int /*rows*/, const QModelIndex &/*index*/){
    return true;
}

bool TableModel::removeRows(int /*position*/, int /*rows*/, const QModelIndex& /*index*/){
    return true;
}


Qt::ItemFlags TableModel::flags(const QModelIndex& index) const{
    if(!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractTableModel::flags(index)|Qt::ItemIsSelectable;
}

std::string TableModel::resizeImage(std::string imgFile)
{
    QString qtImg(imgFile.c_str());
    QImage imgLoad;
    bool success = imgLoad.load(qtImg);
    if(!success){
        std::cout<<"Image failed to load for "<<imgFile<<endl;
        exit(1);
    }
    
    QImage imgSmall = imgLoad.scaledToWidth(50, Qt::FastTransformation);
    
    QByteArray qba;
    QBuffer qb(&qba);
    imgSmall.save(&qb,"PNG");
    
    QByteArray coded = qba.toBase64();
    std::string finalResult(coded);
    return finalResult;
}