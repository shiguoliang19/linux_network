#ifndef SLIDESHOWDELEGATE_H
#define SLIDESHOWDELEGATE_H

#include <QObject>
#include <QtWidgets>

class SlideshowDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SlideshowDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

};

#endif // SLIDESHOWDELEGATE_H
