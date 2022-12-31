#include "rastergraphicsview.h"

RasterGraphicsView::RasterGraphicsView(QWidget *parent)
    : GraphicsView(parent),
      item_(nullptr)
{

}

RasterGraphicsView::~RasterGraphicsView()
{

}

void RasterGraphicsView::setGeometryType(const QString& type)
{
    if(geometry_.type.isEmpty())
    {
        item_=new QGraphicsPathItem();
        scene()->addItem(item_);
    }
    else
    {
        item_->setPath(QPainterPath());
        geometry_.clear();
    }

    geometry_.type=type;
}

void RasterGraphicsView::finishGeometryEdit()
{
    emit finishEdit(&geometry_);
    geometry_.clear();
}

void RasterGraphicsView::wheelEvent(QWheelEvent *event)
{
    const QPointF scene = mapToScene(event->pos());
    qreal factor = std::pow(1.0015, event->angleDelta().y());
    scale(factor, factor);
    const QPointF mouse = mapFromScene(scene);
    const QPointF move = mouse - event->pos();
    horizontalScrollBar()->setValue(move.x() + horizontalScrollBar()->value());
    verticalScrollBar()->setValue(move.y() + verticalScrollBar()->value());
}

void RasterGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QPoint mouse_pos=event->pos();
    QPointF scene_pos=mapToScene(mouse_pos);

    if(geometry_.type=="Point")
    {
        Point& point=geometry_.coordinates.point;
        point=scene_pos;
        item_->setPath(geometry_.getPath());
        finishGeometryEdit();
    }
    else if(geometry_.type=="LineString")
    {
        LineString& line_string=geometry_.coordinates.line_string;
        if(line_string.isEmpty())
        {
            line_string.append(scene_pos);
            item_->setPath(geometry_.getPath());
        }
        else
        {
            const QPointF last_pos=line_string.constLast();
            const QPoint last_mouse_pos=mapFromScene(last_pos);
            const QPoint move=last_mouse_pos-mouse_pos;
            if(move.manhattanLength()>10)
            {
                line_string.append(scene_pos);
                item_->setPath(geometry_.getPath());
            }
            else
            {
                finishGeometryEdit();
            }
        }
    }
    else if(geometry_.type=="Polygon")
    {
        Polygon& polygon=geometry_.coordinates.polygon;
        if(polygon.isEmpty())
        {
            polygon.append(scene_pos);
            item_->setPath(geometry_.getPath());
        }
        else
        {
            const QPointF& first_pos=polygon.constFirst();
            const QPoint first_mouse_pos=mapFromScene(first_pos);
            const QPoint move=first_mouse_pos-mouse_pos;
            if(move.manhattanLength()>10)
            {
                polygon.append(scene_pos);
                item_->setPath(geometry_.getPath());
            }
            else
            {
                polygon.append(first_pos);
                item_->setPath(geometry_.getPath());
                finishGeometryEdit();
            }
        }
    }

    return GraphicsView::mousePressEvent(event);
}
