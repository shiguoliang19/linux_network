#include "vectorgraphicsview.h"
#include "featuretablemodel.h"

#include <cmath>
#include <QtWidgets>

VectorGraphicsView::VectorGraphicsView(QWidget *parent)
    : GraphicsView(parent),
      feature_table_model_(nullptr),
      mode_(NONE)
{

}

VectorGraphicsView::~VectorGraphicsView()
{

}

void VectorGraphicsView::setFeatureTableModel(FeatureTableModel *feature_table_model)
{
    feature_table_model_=feature_table_model;
}

void VectorGraphicsView::wheelEvent(QWheelEvent *event)
{
    if(feature_table_model_)
    {
        const QPointF scene = mapToScene(event->pos());
        qreal factor = std::pow(1.0015, event->angleDelta().y());
        //feature_table_model_->scaleFeatureInfo(factor);
        const QPointF mouse = mapFromScene(scene*factor);
        const QPointF move = mouse - event->pos();
        horizontalScrollBar()->setValue(move.x() + horizontalScrollBar()->value());
        verticalScrollBar()->setValue(move.y() + verticalScrollBar()->value());
    }
}

void VectorGraphicsView::mousePressEvent(QMouseEvent *event)
{
    switch(mode_)
    {
    case POINT:
    {
        QPoint mouse_pos=event->pos();
        QPointF scene_pos=mapToScene(mouse_pos);

        QRectF rect;
        rect.setSize(QSizeF(5.0,5.0));
        rect.moveCenter(scene_pos);
        scene()->addEllipse(rect);
    }break;
    case LINE_STRING:
    {
        static QVector<QPointF> line_string;
        static QGraphicsPathItem* item=nullptr;
        if(!item)
        {
            item=new QGraphicsPathItem();
            scene()->addItem(item);
        }

        QPoint mouse_pos=event->pos();
        QPointF scene_pos=mapToScene(mouse_pos);

        if(line_string.isEmpty())
            line_string.append(scene_pos);
        else
        {
            const QPointF last_pos=line_string.constLast();

            QPoint last_mouse_pos=mapFromScene(last_pos);
            QPoint move=last_mouse_pos-mouse_pos;
            if(move.manhattanLength()>10)
                line_string.append(scene_pos);
        }

        QPainterPath path;
        for(int i=0;i<line_string.size();++i)
        {
            if(i==0)
                path.moveTo(line_string.at(i));
            else
                path.lineTo(line_string.at(i));
        }
        item->setPath(path);
    }break;
    case POLYGON:
    {
        static QPolygonF polygon;
        static QGraphicsPathItem* item=nullptr;
        if(!item)
        {
            item=new QGraphicsPathItem();
            scene()->addItem(item);
        }

        QPoint mouse_pos=event->pos();
        QPointF scene_pos=mapToScene(mouse_pos);

        if(polygon.isEmpty())
            polygon.append(scene_pos);
        else
        {
            const QPointF& first_pos=polygon.constFirst();

            QPoint first_mouse_pos=mapFromScene(first_pos);
            QPoint move=first_mouse_pos-mouse_pos;
            if(move.manhattanLength()>10)
                polygon.append(scene_pos);
            else
                polygon.append(first_pos);
            qDebug()<<move;
        }

        QPainterPath path;
        for(int i=0;i<polygon.size();++i)
        {
            if(i==0)
                path.moveTo(polygon.at(i));
            else
                path.lineTo(polygon.at(i));
        }
        item->setPath(path);
    }break;
    default:
    {
        GraphicsView::mousePressEvent(event);
    }break;
    }
}

void VectorGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    return GraphicsView::mouseMoveEvent(event);
}

void VectorGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    return GraphicsView::mouseReleaseEvent(event);
}
