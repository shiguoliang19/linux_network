#include "featuretablemodel.h"

FeatureTableModel::FeatureTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

FeatureTableModel::~FeatureTableModel()
{

}

void FeatureTableModel::setModelData(const QVector<FeatureInfo*> &feature_info_vector)
{
    beginResetModel();
    feature_info_vector_=feature_info_vector;
    endResetModel();

    emit dataChanged(index(0,0),index(feature_info_vector.size()-1,2));
}

QVariant FeatureTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant header_data;
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            switch (section)
            {
                case 0:header_data="name";break;
                case 1:header_data="center";break;
                case 2:header_data="geometry";break;
            }
        }
        else if(orientation==Qt::Vertical)
        {
            header_data=QString::number(section);
        }
    }
    return header_data;
}

int FeatureTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return feature_info_vector_.size();
}

int FeatureTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 3;
}

QVariant FeatureTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row=index.row();
    int column=index.column();
    FeatureInfo* feature_info=feature_info_vector_.at(row);
    QVariant data;
    if(role==Qt::DisplayRole)
    {
        switch (column) {
            case 0:
            {
                data=feature_info->name;
                break;
            }
            case 1:
            {
                data=QString("%1,%2").arg(feature_info->center.x(),0,'f',4).arg(feature_info->center.y(),0,'f',4);
                break;
            };
            case 2:
            {
                data=QVariant::fromValue<Geometry*>(&feature_info->geometry);
                break;
            }
        }
    }
    else if(role==Qt::EditRole)
    {
        switch (column) {
            case 0:
            {
                data=feature_info->name;
                break;
            }
            case 1:
            {
                data=feature_info->center;
                break;
            };
            case 2:
            {
                data=QVariant::fromValue<Geometry*>(&feature_info->geometry);
                break;
            }
        }
    }

    return data;
}

bool FeatureTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent,row,row+count-1);
    for(int i=0;i<count;++i)
    {
        FeatureInfo* feature_info=new FeatureInfo();
        feature_info_vector_.insert(row,feature_info);
    }
    endInsertRows();
    return true;
}

bool FeatureTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent,row,row+count-1);
    feature_info_vector_.remove(row,count);
    endRemoveRows();
    return true;
}

bool FeatureTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    int row=index.row();
    int column=index.column();
    FeatureInfo* feature_info=feature_info_vector_.at(row);
    if(role==Qt::EditRole)
    {
        switch (column)
        {
            case 0:
            feature_info->name=value.toString();
            break;
            case 1:
            feature_info->center=value.toPointF();
            break;
            case 2:
            feature_info->geometry=*value.value<Geometry*>();
            break;
        }
    }
    return true;
}
