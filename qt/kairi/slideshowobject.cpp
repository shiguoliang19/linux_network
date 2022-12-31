#include "slideshowobject.h"

SlideshowObject::SlideshowObject(QObject *parent)
    : QObject(parent),
      m_pixmapWidth(500),
      m_pixmapHeight(400),
      m_spacer(10)
{

}

SlideshowObject::~SlideshowObject()
{

}

void SlideshowObject::setGeometry(const QRect &rect)
{
    m_rect = rect;

    QPoint offset;
    offset.ry() = (m_parentWidget->height() - m_pixmapHeight) / 2.0;
    m_rect.setBottom(offset.y());

    for(int i = 0; i < m_labelList.size(); ++i)
    {
        QLabel* label = m_labelList.at(i);
        label->setGeometry(rect.x() + i * (m_pixmapWidth + m_spacer), offset.y(), m_pixmapWidth, m_pixmapHeight);
    }
}
