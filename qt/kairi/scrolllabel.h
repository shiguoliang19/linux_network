#ifndef SCROLLLABEL_H
#define SCROLLLABEL_H

#include <QWidget>

class ScrollLabel : public QWidget
{
    Q_OBJECT

public:
    enum Direction {
        NONE = 0,
        RightToLeft = 1,
        LeftToRight = 2
    };

public:

    ScrollLabel(const QString& text, QWidget *parent = nullptr);

    /* direction */
    Direction getDirection() const { return m_direction; }

    void setDirection(Direction direction) { m_direction = direction; }

    /* text */
    void setText(const QString& text) { m_text = text; }

    const QString& text() const { return m_text; }

protected:

    void timerEvent(QTimerEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    int m_timerId;

    QString m_text;

    Direction m_direction;

    int m_offset;

    int m_textWidth;

    int m_labelWidth;

};

#endif // SCROLLLABEL_H
