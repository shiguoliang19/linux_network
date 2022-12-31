#ifndef SLIDESHOWWIDGET_H
#define SLIDESHOWWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include "slideshowobject.h"

class SlideshowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SlideshowWidget(QWidget *parent = nullptr);

    ~SlideshowWidget();

    void setPosition(int pixmapWidth, int pixmapHeight, int spacer);

    void setPixmapList(const QList<QPixmap>& pixmapList);

    void animationFinished();

    void advance();

    QPropertyAnimation* getAnimation();

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    SlideshowObject* m_object;

    int m_pixmapWidth;

    int m_pixmapHeight;

    int m_spacer;

    QList<QLabel*> m_labelList;

    QList<QLabel*> m_playLabelList;

    QPropertyAnimation* m_animation;

};

#endif // SLIDESHOWWIDGET_H
