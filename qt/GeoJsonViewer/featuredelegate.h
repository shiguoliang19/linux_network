#ifndef FEATUREDELEGATE_H
#define FEATUREDELEGATE_H

#include <QStyledItemDelegate>
#include "parsegenjson.h"

class FeatureDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit FeatureDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // FEATUREDELEGATE_H
