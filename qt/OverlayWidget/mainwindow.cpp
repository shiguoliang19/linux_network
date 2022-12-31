#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "centerscrollwidget.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    messageCount = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::horizontalScroll(QWidget *widget, QWidget *maskLayer, QWidget *maskWidget, const QSize &size, int spacer)
{
    // mask layer
    maskLayer->setAutoFillBackground(true);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(94, 94, 94, 180));
    maskLayer->setPalette(palette);

    maskLayer->setParent(maskWidget->parentWidget());
    maskLayer->setGeometry(maskWidget->geometry());
    maskLayer->show();

    // widget
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setOffset(0, 0);
    effect->setBlurRadius(20);
    effect->setColor(Qt::blue);
    widget->setGraphicsEffect(effect);
    widget->setParent(maskLayer);
    widget->show();

    // naimation
    QPoint offset;
    offset.rx() = (maskWidget->width() - size.width()) / 2;
    offset.ry() = spacer;
    QRect beginRect(offset.x(), offset.y() + -size.width(), size.width(), size.height());
    QRect endRect(offset.x(), offset.y(), size.width(), size.height());

    QPropertyAnimation* animation = new QPropertyAnimation(widget, "geometry");
    animation->setDuration(500);
    animation->setStartValue(beginRect);
    animation->setEndValue(endRect);
    animation->start();

    CenterScrollWidget* csw = dynamic_cast<CenterScrollWidget*>(widget);
    connect(csw, &CenterScrollWidget::closeWidgetSign, [=](){
        closeWidget(animation, maskLayer, maskWidget);
    });
}

void MainWindow::verticalScroll(QWidget *widget, QWidget *maskLayer, QWidget *maskWidget, int height, int spacer)
{
    // mask layer
    maskLayer->setAutoFillBackground(true);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(94, 94, 94, 180));
    maskLayer->setPalette(palette);

    maskLayer->setParent(maskWidget->parentWidget());
    maskLayer->setGeometry(maskWidget->geometry());
    maskLayer->show();

    // widget
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setOffset(0, 0);
    effect->setBlurRadius(20);
    effect->setColor(Qt::blue);
    widget->setGraphicsEffect(effect);
    widget->setParent(maskLayer);
    widget->show();

    QPoint offset;
    offset.rx() = spacer;
    offset.ry() = (maskWidget->height() - height) / 2;
    widget->setGeometry(offset.x(), offset.y(), maskLayer->width() - 2 * spacer, height);

    CenterScrollWidget* csw = dynamic_cast<CenterScrollWidget*>(widget);
    connect(csw, &CenterScrollWidget::closeWidgetSign, [=](){
        maskWidget->setGraphicsEffect(nullptr);
        maskLayer->close();
    });
}

void MainWindow::rightScroll(QWidget *widget, QWidget *maskLayer, QWidget *maskWidget, int width)
{
    // mask layer
    maskLayer->setAutoFillBackground(true);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(0, 0, 0, 120));
    maskLayer->setPalette(palette);

    maskLayer->setParent(maskWidget->parentWidget());
    maskLayer->setGeometry(maskWidget->geometry());
    maskLayer->show();

    // widget
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setOffset(0, 0);
    effect->setBlurRadius(20);
    effect->setColor(Qt::blue);
    widget->setGraphicsEffect(effect);
    widget->setParent(maskLayer);
    widget->show();

    // animation
    QPoint offset;
    offset.rx() = maskWidget->width();
    offset.ry() = 0;
    QRect beginRect(offset.x(), offset.y(), width, maskWidget->height());
    QRect endRect(offset.x() - width, offset.y(), width, maskWidget->height());

    QPropertyAnimation* animation = new QPropertyAnimation(widget, "geometry");
    animation->setDuration(500);
    animation->setStartValue(beginRect);
    animation->setEndValue(endRect);
    animation->start();

    CenterScrollWidget* csw = dynamic_cast<CenterScrollWidget*>(widget);
    connect(csw, &CenterScrollWidget::closeWidgetSign, [=](){
        closeWidget(animation, maskLayer, maskWidget);
    });
}

void MainWindow::messageScroll(QWidget *widget, QWidget *maskLayer, QWidget *maskWidget, const QSize& size, int topDistance, int rightDistance)
{
    // mask layer
    widget->resize(size);

    maskLayer->setParent(maskWidget->parentWidget());
    maskLayer->setGeometry(maskWidget->geometry());
    maskLayer->show();

    // widget
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setOffset(0, 0);
    effect->setBlurRadius(20);
    effect->setColor(Qt::blue);
    widget->setGraphicsEffect(effect);
    widget->setParent(maskLayer);
    widget->show();

    // naimation
    QPoint offset;
    offset.rx() = maskWidget->width() - widget->width() - rightDistance;
    offset.ry() = topDistance + messageCount * (size.height() + topDistance);
    QRect beginRect(offset.x() + size.width(), offset.y(), size.width(), size.height());
    QRect endRect(offset.x(), offset.y(), size.width(), size.height());

    QPropertyAnimation* animation = new QPropertyAnimation(widget, "geometry");
    animation->setDuration(500);
    animation->setStartValue(beginRect);
    animation->setEndValue(endRect);
    animation->start();

    ++messageCount;

    QTimer::singleShot(3000, [=](){
        closeWidget(animation, maskLayer, maskWidget);
        --messageCount;
    });
}

void MainWindow::closeWidget(QPropertyAnimation* animation, QWidget* maskLayer, QWidget* maskWidget)
{
    animation->stop();
    animation->setDirection(QPropertyAnimation::Backward);
    animation->start();

    connect(animation, &QPropertyAnimation::finished,[=](){
        animation->deleteLater();
        maskWidget->setGraphicsEffect(nullptr);
        maskLayer->close();
    });
}

void MainWindow::on_styleButton_clicked()
{
    QWidget* maskWidget = ui->frame;
    QWidget* maskLayer = new QWidget();
    CenterScrollWidget* widget = new CenterScrollWidget(maskLayer, maskWidget);
    horizontalScroll(widget, maskLayer, maskWidget, QSize(400, 300));
}

void MainWindow::on_style2Button_clicked()
{
    QWidget* maskWidget = ui->frame;
    QWidget* maskLayer = new QWidget();
    CenterScrollWidget* widget = new CenterScrollWidget(maskLayer, maskWidget);
    verticalScroll(widget, maskLayer, maskWidget, 100, 0);
}

void MainWindow::on_style3Button_clicked()
{
    QWidget* maskWidget = ui->frame;
    QWidget* maskLayer = new QWidget();
    CenterScrollWidget* widget = new CenterScrollWidget(maskLayer, maskWidget);
    rightScroll(widget, maskLayer, maskWidget, 300);
}

void MainWindow::on_pushButton_clicked()
{
    QWidget* maskWidget = ui->frame;
    QWidget* maskLayer = new QWidget();
    CenterScrollWidget* widget = new CenterScrollWidget(maskLayer, maskWidget);
    messageScroll(widget, maskLayer, maskWidget, QSize(300, 100), 20, 20);
}
