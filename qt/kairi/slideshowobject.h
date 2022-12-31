#ifndef SLIDESHOWOBJECT_H
#define SLIDESHOWOBJECT_H

#include <QObject>
#include <QtWidgets>

class SlideshowObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QRect geometry READ geometry WRITE setGeometry)

public:
    explicit SlideshowObject(QObject *parent = nullptr);

    ~SlideshowObject();

    void setMotion(int pixmapWidth, int pixmapHeight, int spacer);

    void setParentWidget(QWidget* parentWidget);

    void setLabelList(QList<QLabel*> labelList);

    const QRect& geometry();

    void setGeometry(const QRect& rect);

private:
    int m_pixmapWidth;

    int m_pixmapHeight;

    int m_spacer;

    QWidget* m_parentWidget;

    QList<QLabel*> m_labelList;

    QRect m_rect;
};

inline void SlideshowObject::setMotion(int pixmapWidth, int pixmapHeight, int spacer)
{
    m_pixmapWidth = pixmapWidth;
    m_pixmapHeight = pixmapHeight;
    m_spacer = spacer;
}

inline void SlideshowObject::setParentWidget(QWidget *parentWidget)
{
    m_parentWidget = parentWidget;
}

inline void SlideshowObject::setLabelList(QList<QLabel*> labelList)
{
    m_labelList = labelList;
}

inline const QRect &SlideshowObject::geometry()
{
    return m_rect;
}

#endif // SLIDESHOWOBJECT_H
