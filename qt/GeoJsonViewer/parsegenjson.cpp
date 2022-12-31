#include "parsegenjson.h"

ParseGenJson::ParseGenJson(QObject *parent)
    : QObject(parent)
{

}

ParseGenJson::~ParseGenJson()
{

}

const GenJson& ParseGenJson::loadGenJsonFile(const QString& name)
{
    QByteArray data;
    QFile file(name);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        data=file.readAll();
        file.close();
    }

    const QJsonDocument json_document=QJsonDocument::fromJson(data);
    const QJsonObject json_object=json_document.object();
    parseGenJson(json_object);

    return gen_json_;
}

void ParseGenJson::parseGenJson(const QJsonObject& json_object)
{
    gen_json_.type=json_object.value("type").toString();
    gen_json_.name=json_object.value("name").toString();

    const QJsonArray json_array=json_object.value("features").toArray();
    for(int i=0;i<json_array.count();++i)
        parseFeature(json_array.at(i).toObject());
}

void ParseGenJson::parseFeature(const QJsonObject &json_object)
{
    Feature feature;
    feature.type=json_object.value("type").toString();
    parseProperties(json_object.value("properties").toObject(),feature.properties);
    parseGeometry(json_object.value("geometry").toObject(),feature.geometry);
    gen_json_.features.append(feature);
}

void ParseGenJson::parseProperties(const QJsonObject &json_object,Properties& properties)
{
    properties.adcode=json_object.value("adcode").toString();
    properties.name=json_object.value("name").toString();

    {
        QJsonArray json_array=json_object.value("center").toArray();
        if(json_array.size()==2)
        {
            properties.center.setX(json_array.at(0).toDouble());
            properties.center.setY(json_array.at(1).toDouble());
        }
    }

    {
        QJsonArray json_array=json_object.value("centroid").toArray();
        if(json_array.size()==2)
        {
            properties.centroid.setX(json_array.at(0).toDouble());
            properties.centroid.setY(json_array.at(1).toDouble());
        }
    }

    properties.childrenNum=json_object.value("childrenNum").toInt();
    properties.level=json_object.value("level").toString();

    {
        QJsonObject json_object2=json_object.value("parent").toObject();
        if(json_object2.size()==1)
        {
            properties.parent=json_object2.value("adcode").toInt();
        }
    }

    properties.subFeatureIndex=json_object.value("subFeatureIndex").toInt();

    {
        QJsonArray json_array=json_object.value("acroutes").toArray();
        if(json_array.size()==1)
        {
            properties.acroutes=json_array.at(0).toInt();
        }
    }
}

void ParseGenJson::parseGeometry(const QJsonObject &json_object,Geometry& geometry)
{    
    auto parsePoint=[](const QJsonArray& json_array,Geometry& geometry){
        Point& point=geometry.coordinates.point;
        if(json_array.size()==2)
        {
            qreal x=json_array[0].toDouble();
            qreal y=json_array[1].toDouble();
            point.setX(x);
            point.setY(y);
        }
    };

    auto parseMultiPoint=[](const QJsonArray& json_array,Geometry& geometry){
        MultiPoint& multi_point=geometry.coordinates.multi_point;
        for(int i=0;i<json_array.count();++i)
        {
            const QJsonArray& json_array2=json_array.at(i).toArray();
            if(json_array2.size()==2)
            {
                qreal x=json_array2.at(0).toDouble();
                qreal y=json_array2.at(1).toDouble();
                multi_point.append(QPointF(x,y));
            }
        }
    };

    auto parseLineString=[](const QJsonArray& json_array,Geometry& geometry){
        LineString& line_string=geometry.coordinates.line_string;
        for(int i=0;i<json_array.count();++i)
        {
            const QJsonArray& json_array2=json_array.at(i).toArray();
            if(json_array2.size()==2)
            {
                qreal x=json_array2.at(0).toDouble();
                qreal y=json_array2.at(1).toDouble();
                line_string.append(QPointF(x,y));
            }
        }
    };

    auto parseMultiLineString=[](const QJsonArray& json_array,Geometry& geometry){
        MultiLineString& multi_line_string=geometry.coordinates.multi_line_string;
        for(int i=0;i<json_array.count();++i)
        {
            const QJsonArray& json_array2=json_array.at(i).toArray();
            for(int j=0;j<json_array2.count();++j)
            {
                const QJsonArray& json_array3=json_array2.at(j).toArray();

                QPolygonF polygon;
                for(int k=0;k<json_array3.count();++k)
                {
                    QJsonArray json_array4=json_array3.at(k).toArray();
                    if(json_array4.size()==2)
                    {
                        qreal x=json_array4[0].toDouble();
                        qreal y=json_array4[1].toDouble();
                        polygon.append(QPointF(x,y));
                    }
                }
                multi_line_string.append(polygon);
            }
        }
    };

    auto parsePolygon=[](const QJsonArray& json_array,Geometry& geometry){
        Polygon& polygon=geometry.coordinates.polygon;
        for(int i=0;i<json_array.count();++i)
        {
            const QJsonArray& json_array2=json_array.at(i).toArray();
            for(int j=0;j<json_array2.count();++j)
            {
                const QJsonArray& json_array3=json_array2.at(j).toArray();
                if(json_array3.size()==2)
                {
                    qreal x=json_array3[0].toDouble();
                    qreal y=json_array3[1].toDouble();
                    polygon.append(QPointF(x,y));
                }
            }
        }
    };

    auto parseMultiPolygon=[](const QJsonArray& json_array,Geometry& geometry){
        MultiPolygon& multi_polygon=geometry.coordinates.multi_polygon;
        for(int i=0;i<json_array.count();++i)
        {
            const QJsonArray& json_array2=json_array.at(i).toArray();
            for(int j=0;j<json_array2.count();++j)
            {
                const QJsonArray& json_array3=json_array2.at(j).toArray();

                QPolygonF polygon;
                for(int k=0;k<json_array3.count();++k)
                {
                    QJsonArray json_array4=json_array3.at(k).toArray();
                    if(json_array4.size()==2)
                    {
                        qreal x=json_array4[0].toDouble();
                        qreal y=json_array4[1].toDouble();
                        polygon.append(QPointF(x,y));
                    }
                }
                multi_polygon.append(polygon);
            }
        }
    };

    geometry.type=json_object.value("type").toString();
    const QJsonArray& json_array=json_object.value("coordinates").toArray();
    if(geometry.type=="Point")
        parsePoint(json_array,geometry);
    else if(geometry.type=="MultiPoint")
        parseMultiPoint(json_array,geometry);
    else if(geometry.type=="LineString")
        parseLineString(json_array,geometry);
    else if(geometry.type=="MultiLineString")
        parseMultiLineString(json_array,geometry);
    else if(geometry.type=="Polygon")
        parsePolygon(json_array,geometry);
    else if(geometry.type=="MultiPolygon")
        parseMultiPolygon(json_array,geometry);
    else
        qDebug()<<"Unsupported format:"<<geometry.type;
}
