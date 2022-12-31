#ifndef CENTERSCROLLWIDGET_H
#define CENTERSCROLLWIDGET_H

#include <QWidget>

namespace Ui {
class CenterScrollWidget;
}

class CenterScrollWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CenterScrollWidget(QWidget* widgetLayer, QWidget* maskWidget, QWidget *parent = nullptr);
    ~CenterScrollWidget();

signals:
    void closeWidgetSign();

private:
    Ui::CenterScrollWidget *ui;
};

#endif // CENTERSCROLLWIDGET_H
