#include "ScrollLabel.h"

#include <QTimerEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>

#include <QDebug>

ScrollLabel::ScrollLabel(const QString &text, QWidget *parent)
    : QWidget(parent)
{
    m_direction = RightToLeft;

    setText(text);

    m_timerId = startTimer(20);
}

void ScrollLabel::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_timerId)
    {
        ++m_offset;

        if(m_offset > m_textWidth + m_labelWidth)
        {
            m_offset = 0;
        }

        update();
    }
    else
    {
        QWidget::timerEvent(event);
    }
}

void ScrollLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QFont font = painter.font();
    font.setPointSize(18);
    painter.setFont(font);

    QPen pen = painter.pen();
    pen.setColor(Qt::white);
    painter.setPen(pen);

    const int text_width = painter.fontMetrics().horizontalAdvance(text());
    if(text_width < width())
    {
        m_offset = width();
    }

    const int text_height = painter.fontMetrics().height();
    const int text_y = (height()+text_height) / 2-painter.fontMetrics().descent();

    if (m_textWidth != text_width && text_width > 0)
    {
        m_textWidth = text_width;
        m_offset = 0;
    }
    else
    {
        if(m_direction == RightToLeft)
            painter.drawText(m_labelWidth - m_offset, text_y, text());
        else
            painter.drawText(m_offset - m_textWidth, text_y, text());
    }
}

void ScrollLabel::resizeEvent(QResizeEvent *event)
{
    const int old_width = event->oldSize().width();
    const int new_width = event->size().width();

    if (new_width > 10)
    {
        m_labelWidth = new_width;

        if (new_width < old_width)
        {
            m_offset = 0;
        }
    }

    QWidget::resizeEvent(event);
}
