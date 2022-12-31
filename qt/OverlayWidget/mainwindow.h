#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void horizontalScroll(QWidget* widget, QWidget* maskLayer, QWidget* maskWidget, const QSize& size, int spacer = 12);

    void verticalScroll(QWidget* widget, QWidget* maskLayer, QWidget* maskWidget, int height, int spacer);

    void rightScroll(QWidget* widget, QWidget* maskLayer, QWidget* maskWidget, int width);

    void messageScroll(QWidget* widget, QWidget* maskLayer, QWidget* maskWidget, const QSize& size, int topDistance, int rightDistance);

    void closeWidget(QPropertyAnimation* animation, QWidget* widgetLayer, QWidget* maskWidget);

private slots:

    void on_styleButton_clicked();

    void on_style2Button_clicked();

    void on_style3Button_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    int messageCount;
};
#endif // MAINWINDOW_H
