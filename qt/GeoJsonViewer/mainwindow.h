#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class FeatureInfo;
class QGraphicsScene;
class FeatureTableModel;
class FeatureDelegate;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //打开文件
    void openGeoJsonFile();

    //适应视图
    void fitInView();

    //模型数据变化
    void onModelDataChanged(const QModelIndex& top_left, const QModelIndex& bottom_right, const QVector<int>& roles);

    //设置模型
    QVector<FeatureInfo*> setupModelData(const QString& name);

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene_;
    FeatureTableModel* model_;
    FeatureDelegate* delegate_;
};
#endif // MAINWINDOW_H
