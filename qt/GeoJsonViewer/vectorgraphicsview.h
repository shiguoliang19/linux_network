#ifndef VECTORGRAPHICSVIEW_H
#define VECTORGRAPHICSVIEW_H

#include "graphicsview.h"

class FeatureTableModel;

class VectorGraphicsView : public GraphicsView
{
    Q_OBJECT

public:
    enum GenJsonInputMode{
        NONE=-1,
        POINT,
        LINE_STRING,
        POLYGON
    };

    VectorGraphicsView(QWidget *parent = nullptr);
    ~VectorGraphicsView();

    void setFeatureTableModel(FeatureTableModel* feature_table_model);

    void setGenJsonInputMode(GenJsonInputMode mode){ mode_=mode; }

protected:
    void wheelEvent(QWheelEvent* event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    FeatureTableModel* feature_table_model_;
    GenJsonInputMode mode_;
};

#endif // VECTORGRAPHICSVIEW_H
