#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ScrollLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow(); 

private slots:
    void playSlideshow(const QString& imageDir, int imageWidth, int imageHeight);

    void on_open_button_clicked();

private:
    Ui::MainWindow *ui;

    ScrollLabel* m_label;
};
#endif // MAINWINDOW_H
