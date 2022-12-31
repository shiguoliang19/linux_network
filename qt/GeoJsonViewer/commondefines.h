#ifndef COMMONDEFINES_H
#define COMMONDEFINES_H

#include <QtWidgets>
#include "commonfunctions.h"

struct Properties
{
    Properties():childrenNum(0),parent(0),subFeatureIndex(0),acroutes(0){ }

    QString adcode;
    QString name;
    QPointF center;
    QPointF centroid;

    int childrenNum;
    QString level;
    int parent;
    int subFeatureIndex;
    int acroutes;
};

struct Feature
{
    QString type;
    Properties properties;
    Geometry geometry;
};

struct GenJson
{
    QString type;
    QString name;
    QVector<Feature> features;
};

#endif // COMMONDEFINES_H
