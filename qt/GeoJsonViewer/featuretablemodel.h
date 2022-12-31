#ifndef FEATURETABLEMODEL_H
#define FEATURETABLEMODEL_H

#include <QAbstractTableModel>
#include "parsegenjson.h"

#include <QtWidgets>

struct FeatureInfo
{
    QString name;
    QPointF center;
    Geometry geometry;
};

class FeatureTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    FeatureTableModel(QObject *parent = nullptr);
    ~FeatureTableModel();

    void setModelData(const QVector<FeatureInfo*>& feature_info_vector);

    //Grpah Model
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //Edit Model
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    QVector<FeatureInfo*> feature_info_vector_;
};

#endif // FEATURETABLEMODEL_H
