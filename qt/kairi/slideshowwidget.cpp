#include "slideshowwidget.h"

QT_BEGIN_NAMESPACE
extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

SlideshowWidget::SlideshowWidget(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);

    m_object = new SlideshowObject(this);

    m_animation = new QPropertyAnimation(m_object, "geometry");
    m_animation->setDuration(3000);
    connect(m_animation, &QPropertyAnimation::finished, this, &SlideshowWidget::animationFinished);
}

SlideshowWidget::~SlideshowWidget()
{

}

void SlideshowWidget::setPosition(int pixmapWidth, int pixmapHeight, int spacer)
{
    m_pixmapWidth = pixmapWidth;
    m_pixmapHeight = pixmapHeight;
    m_spacer = spacer;
    m_object->setMotion(pixmapWidth, pixmapHeight, spacer);
}

void SlideshowWidget::setPixmapList(const QList<QPixmap> &pixmapList)
{
    m_animation->stop();

    qDeleteAll(m_labelList);
    m_labelList.clear();

    for(int i = 0; i < pixmapList.size(); ++i)
    {        
        QPixmap pixmap(m_pixmapWidth, m_pixmapHeight);
        QPixmap backgroundPixmap = pixmapList.at(i).scaled(m_pixmapWidth, m_pixmapHeight, Qt::KeepAspectRatioByExpanding);
        QPixmap originPixmap = pixmapList.at(i).scaled(m_pixmapWidth, m_pixmapHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPainter painter(&pixmap);

        // background
        QPoint backgroundOffset;
        backgroundOffset.rx() = (pixmap.width() - backgroundPixmap.width()) / 2;
        backgroundOffset.ry() = (pixmap.height() - backgroundPixmap.height()) / 2;
        QImage image = backgroundPixmap.toImage();
        painter.save();
        painter.translate(backgroundOffset);
        qt_blurImage(&painter, image, 300, true, false);
        painter.restore();

        // origin
        QPoint originOffset;
        originOffset.rx() = (pixmap.width() - originPixmap.width()) / 2;
        originOffset.ry() = (pixmap.height() - originPixmap.height()) / 2;
        painter.drawPixmap(originOffset, originPixmap);

        // label
        painter.setPen("#DD657B");
        painter.setFont(QFont(".AppleSystemUIFont", 36));
        painter.drawText(QRect(0, 0, 100, 50), Qt::AlignCenter, QString("%1 P").arg(i + 1));

        QLabel* label = new QLabel(this);
        label->setPixmap(pixmap);
        label->show();

        m_labelList.push_back(label);
    }

    m_object->setParentWidget(this);
    m_object->setLabelList(m_labelList);

    m_playLabelList = m_labelList;
}

void SlideshowWidget::animationFinished()
{
    m_playLabelList.push_back(m_playLabelList.first());
    m_playLabelList.pop_front();
    m_object->setLabelList(m_playLabelList);
    m_animation->start();
}

void SlideshowWidget::advance()
{
    QPoint offset;
    offset.ry() = (height() - m_pixmapHeight) / 2.0;

    m_animation->setStartValue(QRect(offset.x(), offset.y(), m_pixmapWidth, m_pixmapHeight));
    m_animation->setEndValue(QRect(offset.x() - (m_pixmapWidth + m_spacer), offset.y(), m_pixmapWidth, m_pixmapHeight));
    m_animation->start();
}

QPropertyAnimation *SlideshowWidget::getAnimation()
{
    return m_animation;
}

void SlideshowWidget::mousePressEvent(QMouseEvent *event)
{
    QPropertyAnimation::State state = m_animation->state();
    if(state == QPropertyAnimation::Paused)
        m_animation->resume();
    else if(state == QPropertyAnimation::Running)
        m_animation->pause();

    return QWidget::mousePressEvent(event);
}
