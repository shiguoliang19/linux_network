#ifndef PARSEGENJSON_H
#define PARSEGENJSON_H

#include <QtCore>
#include <QtGui>

#include "commondefines.h"
#include "commonfunctions.h"

class ParseGenJson : public QObject
{
    Q_OBJECT

public:
    explicit ParseGenJson(QObject *parent = nullptr);
    ~ParseGenJson();

    //加载JSON格式文件
    const GenJson& loadGenJsonFile(const QString& name);

private:
    //解析GenJSON
    void parseGenJson(const QJsonObject& json_object);
    //解析Feature
    void parseFeature(const QJsonObject& json_object);
    //解析Properties
    void parseProperties(const QJsonObject &json_object,Properties& properties);
    //解析Geometry
    void parseGeometry(const QJsonObject& json_object,Geometry& geometry);

private:
    GenJson gen_json_;
};

#endif // PARSEGENJSON_H
