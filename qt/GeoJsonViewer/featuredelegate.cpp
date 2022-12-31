#include "featuredelegate.h"
#include <QtWidgets>

FeatureDelegate::FeatureDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void FeatureDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    QColor color;
    if(option.state.testFlag(QStyle::State_Selected))
    {
        if(option.state.testFlag(QStyle::State_Active))
            color=option.palette.color(QPalette::HighlightedText);
        else
            color=option.palette.color(QPalette::WindowText);
    }
    painter->setPen(color);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->translate(option.rect.center());

    Geometry* geometry=index.data().value<Geometry*>();
    QPainterPath path=geometry->getPath();
    QRectF rect=path.boundingRect();

    QSizeF target_size=option.rect.size()-QSize(10,10);
    QSizeF current_size=rect.size();
    qreal width_factor=target_size.width()/current_size.width();
    qreal height_factor=target_size.height()/current_size.height();
    qreal factor=std::min(width_factor,height_factor);

    Geometry target_geometry=*geometry;
    target_geometry.scale(factor,factor);
    painter->drawPath(target_geometry.getPath());

    painter->restore();
}
