#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::playSlideshow(const QString& imageDir, int imageWidth, int imageHeight)
{
    QDir dir(imageDir);
    if(!dir.exists())
    {
        qDebug() << QString("dir not exists! image dir: %1").arg(imageDir);
        return;
    }


    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setSorting(QDir::Time);

    QList<QPixmap> pixmapList;
    for(const QFileInfo& fileInfo : dir.entryInfoList())
    {
        QString filePath = fileInfo.absoluteFilePath();
        QPixmap pixmap(filePath);
        pixmapList.push_back(pixmap);
    }
    ui->widget->setPosition(imageWidth, imageHeight, 20);
    ui->widget->setPixmapList(pixmapList);

    ui->widget->advance();
}

void MainWindow::on_open_button_clicked()
{
    QString imageDir = ui->image_dir_line_edit->text();
    if(!imageDir.isEmpty())
    {
        playSlideshow(imageDir, 500, 400);
    }
}
