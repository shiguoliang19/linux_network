#include "graphicsview.h"

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent),
      pan_(false)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSceneRect(INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX);
}

GraphicsView::~GraphicsView()
{

}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if(itemAt(event->pos())==nullptr)
        {
            pan_ = true;
            pan_start_=event->pos();
            setCursor(Qt::ClosedHandCursor);
        }
    }
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (pan_)
    {
        const QPointF move=pan_start_-event->pos();
        horizontalScrollBar()->setValue(move.x() + horizontalScrollBar()->value());
        verticalScrollBar()->setValue(move.y() + verticalScrollBar()->value());
        pan_start_=event->pos();
    }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        pan_ = false;
        setCursor(Qt::ArrowCursor);
    }
}
