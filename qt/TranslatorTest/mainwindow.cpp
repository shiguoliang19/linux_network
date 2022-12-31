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

void MainWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

QTranslator translator;

void MainWindow::on_pushButton_clicked()
{
//    qApp->removeTranslator(&translator);

    translator.load(":/res/en_tr.qm");
    qApp->installTranslator(&translator);
}

void MainWindow::on_pushButton_2_clicked()
{
//    qApp->removeTranslator(&translator);

    translator.load(":/res/zh_tr.qm");
    qApp->installTranslator(&translator);
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    qDebug() << index;
    if(index == 0)
    {
        translator.load(":/res/en_tr.qm");
        qApp->installTranslator(&translator);
    }
    else if(index == 1)
    {
        translator.load(":/res/zh_tr.qm");
        qApp->installTranslator(&translator);
    }
}

