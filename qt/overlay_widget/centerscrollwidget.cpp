#include "centerscrollwidget.h"
#include "ui_centerscrollwidget.h"
#include <QtWidgets>

CenterScrollWidget::CenterScrollWidget(QWidget* maskLayer, QWidget* maskWidget, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CenterScrollWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    connect(ui->closeButton, &QPushButton::clicked, [=](){
//        maskWidget->setGraphicsEffect(nullptr);
//        maskLayer->close();

        emit closeWidgetSign();
    });
}

CenterScrollWidget::~CenterScrollWidget()
{
    delete ui;
}
