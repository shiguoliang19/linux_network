#include "slideshowdelegate.h"

SlideshowDelegate::SlideshowDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void SlideshowDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
    const QAbstractItemModel *model = index.model();

    QPalette::ColorGroup cg = (option.state & QStyle::State_Enabled) ?
        (option.state & QStyle::State_Active) ?
                    QPalette::Normal :
                    QPalette::Inactive :
                    QPalette::Disabled;

    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, QColor("#00BCD4"));

    int num = index.row() + 1;
    QModelIndex titleIndex =  index.sibling(index.row(), 0);
    QString title = model->data(titleIndex).toString();

    QModelIndex imageCountIndex = index.sibling(index.row(), 1);
    int imageCount = model->data(imageCountIndex).toInt();
    QModelIndex saveDateIndex = index.sibling(index.row(), 5);
    QString saveDate = model->data(saveDateIndex).toString();
    QString subTitle = QString("%1P 更新于%2").arg(imageCount).arg(saveDate);

    /* draw text */
    painter->save();

    if (option.state & QStyle::State_Selected)
        painter->setPen(QPen(Qt::white));

    painter->setFont(QFont("PingFang SC", 32));
    QRect numRect(option.rect.x(), option.rect.y(), option.rect.height(), option.rect.height());
    painter->drawText(numRect, Qt::AlignCenter, QString::number(num));

    QString text;
    if (option.state & QStyle::State_Selected)
        text = QString("<p style=\"font-size:16pt; font-weight:600; color: #ffffff;\">%1</p> <p style=\"font-size:12pt; color:#efefef;\">%2</p>").arg(title).arg(subTitle);
    else
        text = QString("<p style=\"font-size:16pt; color: #000000;\">%1</p> <p style=\"font-size:12pt; color:#1f1f1f;\">%2</p>").arg(title).arg(subTitle);

    QRect textRect(option.rect.x() + option.rect.height(), option.rect.y(),
                  option.rect.width() - option.rect.height(), option.rect.height());

    painter->translate(textRect.topLeft());
    QTextDocument doc;
    doc.setHtml(text);
    QAbstractTextDocumentLayout::PaintContext paintContext;
    doc.documentLayout()->draw(painter, paintContext);

    painter->restore();

    /* draw bottom line */
    painter->save();
    painter->setPen(Qt::gray);
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
    painter->restore();
}

QSize SlideshowDelegate::sizeHint(const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    return QSize(60, 60);
}
