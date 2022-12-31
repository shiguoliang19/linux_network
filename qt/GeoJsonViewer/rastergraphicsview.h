#ifndef RASTERGRAPHICSVIEW_H
#define RASTERGRAPHICSVIEW_H

#include "graphicsview.h"
#include "commonfunctions.h"

class RasterGraphicsView : public GraphicsView
{
    Q_OBJECT

public:
    RasterGraphicsView(QWidget *parent = nullptr);
    ~RasterGraphicsView();

    void setGeometryType(const QString& type);

private:
    void finishGeometryEdit();

signals:
    void finishEdit(Geometry* geometry);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QGraphicsPathItem* item_;
    Geometry geometry_;
};

#endif // RASTERGRAPHICSVIEW_H
