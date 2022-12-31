#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H

#include <QtWidgets>

/*
 * geometry type:
 * 1. Point                 QPointF
 * 2. MultiPoint            QVector<QPointF>
 * 3. LineString            QLineF
 * 4. MultiLineString       QVector<QLineF>
 * 5. Polygon               QPolygonF
 * 6. MultiPolygon          QVector<QPolygonF>
 * 7. GeometryCollection
*/

typedef QPointF                 Point;
typedef QVector<Point>          MultiPoint;
typedef QVector<QPointF>        LineString;
typedef QVector<LineString>     MultiLineString;
typedef QPolygonF               Polygon;
typedef QVector<Polygon>        MultiPolygon;

struct Geometry
{
public:
    QPainterPath getPath() const;

    QPointF getCenter() const;

    void translate(qreal dx,qreal dy);

    void scale(qreal sx,qreal sy);

    void setTransform(const QTransform& transform);

    void clear();

public:
    static void setPointTransform(Point& point,const QTransform& transform);
    static void setMultiPointTransform(MultiPoint& multi_point,const QTransform& transform);
    static void setLineStringTransform(LineString& line_string,const QTransform& transform);
    static void setMultiLineStringTransform(MultiLineString& multi_line_string,const QTransform& transform);
    static void setPolygonTransform(Polygon& polygon,const QTransform& transform);
    static void setMultiPolygonTransform(MultiPolygon& multi_polygon,const QTransform& transform);

    static QPainterPath getPointPath(const Point& point);
    static QPainterPath getMultiPointPath(const MultiPoint& multi_point);
    static QPainterPath getLineStringPath(const LineString& line_string);
    static QPainterPath getMultiLineStringPath(const MultiLineString& multi_line_string);
    static QPainterPath getPolygonPath(const Polygon& polygon);
    static QPainterPath getMultiPolygonPath(const MultiPolygon& multi_polygon);

public:
    QString type;
    struct Coordinates
    {
        Point point;
        MultiPoint multi_point;
        LineString line_string;
        MultiLineString multi_line_string;
        Polygon polygon;
        MultiPolygon multi_polygon;
    }coordinates;
};
Q_DECLARE_METATYPE(Geometry*)

#endif // COMMONFUNCTIONS_H
