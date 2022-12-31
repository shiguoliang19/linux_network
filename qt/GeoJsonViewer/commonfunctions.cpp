#include "commonfunctions.h"

QPainterPath Geometry::getPath() const
{
    QPainterPath path;
    if(type=="Point")
        path=getPointPath(coordinates.point);
    else if(type=="MultiPoint")
        path=getMultiPointPath(coordinates.multi_point);
    else if(type=="LineString")
        path=getLineStringPath(coordinates.line_string);
    else if(type=="MultiLineString")
        path=getMultiLineStringPath(coordinates.multi_line_string);
    else if(type=="Polygon")
        path=getPolygonPath(coordinates.polygon);
    else if(type=="MultiPolygon")
        path=getMultiPolygonPath(coordinates.multi_polygon);
    return path;
}

QPointF Geometry::getCenter() const
{
    QPainterPath path=getPath();
    QPointF center=path.boundingRect().center();
    return center;
}

void Geometry::translate(qreal dx,qreal dy)
{
    setTransform(QTransform::fromTranslate(dx,dy));
}

void Geometry::scale(qreal sx,qreal sy)
{
    setTransform(QTransform::fromScale(sx,sy));
}

void Geometry::clear()
{
    type.clear();
    coordinates.point=QPointF();
    coordinates.multi_point.clear();
    coordinates.line_string.clear();
    coordinates.multi_line_string.clear();
    coordinates.polygon.clear();
    coordinates.multi_polygon.clear();
}

void Geometry::setTransform(const QTransform& transform)
{
    if(type=="Point")
        setPointTransform(coordinates.point,transform);
    else if(type=="MultiPoint")
        setMultiPointTransform(coordinates.multi_point,transform);
    else if(type=="LineString")
        setLineStringTransform(coordinates.line_string,transform);
    else if(type=="MultiLineString")
        setMultiLineStringTransform(coordinates.multi_line_string,transform);
    else if(type=="Polygon")
        setPolygonTransform(coordinates.polygon,transform);
    else if(type=="MultiPolygon")
        setMultiPolygonTransform(coordinates.multi_polygon,transform);
}

void Geometry::setPointTransform(Point& point,const QTransform& transform)
{
    point=transform.map(point);
}
void Geometry::setMultiPointTransform(MultiPoint& multi_point,const QTransform& transform)
{
    for(int i=0;i<multi_point.size();++i)
        setPointTransform(multi_point[i],transform);
}
void Geometry::setLineStringTransform(LineString& line_string,const QTransform& transform)
{
    for(int i=0;i<line_string.size();++i)
        line_string[i]=transform.map(line_string[i]);
}
void Geometry::setMultiLineStringTransform(MultiLineString& multi_line_string,const QTransform& transform)
{
    for(int i=0;i<multi_line_string.size();++i)
        setLineStringTransform(multi_line_string[i],transform);
}
void Geometry::setPolygonTransform(Polygon& polygon,const QTransform& transform)
{
    polygon=transform.map(polygon);
}
void Geometry::setMultiPolygonTransform(MultiPolygon& multi_polygon,const QTransform& transform)
{
    for(int i=0;i<multi_polygon.size();++i)
        multi_polygon[i]=transform.map(multi_polygon[i]);
}

QPainterPath Geometry::getPointPath(const Point& point)
{
    QPainterPath path;
    path.addEllipse(point,2,2);
    path.addEllipse(point,5,5);
    return path;
}

QPainterPath Geometry::getMultiPointPath(const MultiPoint& multi_point)
{
    QPainterPath path;
    for(int i=0;i<multi_point.size();++i)
    {
        const Point& point=multi_point.at(i);
        path.addPath(getPointPath(point));
    }
    return path;
}

QPainterPath Geometry::getLineStringPath(const LineString& line_string)
{
    QPainterPath path;
    for(int i=0;i<line_string.size();++i)
    {
        if(i==0)
            path.moveTo(line_string.at(i));
        else
            path.lineTo(line_string.at(i));
    }
    return path;
}

QPainterPath Geometry::getMultiLineStringPath(const MultiLineString& multi_line_string)
{
    QPainterPath path;
    for(int i=0;i<multi_line_string.size();++i)
    {
        const LineString& line_string=multi_line_string.at(i);
        path.addPath(getLineStringPath(line_string));
    }
    return path;
}

QPainterPath Geometry::getPolygonPath(const Polygon& polygon)
{
    QPainterPath path;
    for(int i=0;i<polygon.size();++i)
    {
        if(i==0)
            path.moveTo(polygon.at(i));
        else
            path.lineTo(polygon.at(i));
    }
    return path;
}

QPainterPath Geometry::getMultiPolygonPath(const MultiPolygon& multi_polygon)
{
    QPainterPath path;
    for(int i=0;i<multi_polygon.size();++i)
    {
        const Polygon& polygon=multi_polygon.at(i);
        path.addPath(getPolygonPath(polygon));
    }
    return path;
}
