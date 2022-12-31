#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parsegenjson.h"
#include "graphicsview.h"
#include "featuretablemodel.h"
#include "featuredelegate.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow())
{
    ui->setupUi(this);

    QGraphicsScene* scene=new QGraphicsScene();
    scene_=scene;
    ui->graphics_view->setScene(scene);

    FeatureTableModel* model=new FeatureTableModel();
    model_=model;
    ui->table_view->setModel(model);

    FeatureDelegate* delegate=new FeatureDelegate();
    delegate_=delegate;
    ui->table_view->setItemDelegateForColumn(2,delegate);

    ui->table_view->horizontalHeader()->setDefaultSectionSize(150);
    ui->table_view->verticalHeader()->setDefaultSectionSize(50);
    ui->table_view->horizontalHeader()->setStretchLastSection(true);

    connect(ui->point_button,&QToolButton::clicked,[=](){
        ui->graphics_view->setGeometryType("Point");
    });
    connect(ui->line_string_button,&QToolButton::clicked,[=](){
        ui->graphics_view->setGeometryType("LineString");
    });
    connect(ui->polygon_button,&QToolButton::clicked,[=](){
        ui->graphics_view->setGeometryType("Polygon");
    });

    connect(ui->open_button,&QToolButton::clicked,this,&MainWindow::openGeoJsonFile);
    connect(ui->fit_in_view_button,&QToolButton::clicked,this,&MainWindow::fitInView);
    connect(model,&FeatureTableModel::dataChanged,this,&MainWindow::onModelDataChanged);
    connect(ui->graphics_view,&RasterGraphicsView::finishEdit,[=](Geometry* geometry){
        QModelIndex index=ui->table_view->currentIndex();

        int row=0;
        if(index.isValid())
            row=index.row()+1;
        else
            row=model_->rowCount();
        model_->insertRow(row);

        QPointF center=geometry->getCenter();
        geometry->translate(-center.x(),-center.y());

        QModelIndex name_index=model_->index(row,0);
        QModelIndex center_index=model_->index(row,1);
        QModelIndex geometry_index=model_->index(row,2);
        model_->setData(name_index,QString("Name"),Qt::EditRole);
        model_->setData(center_index, center,Qt::EditRole);
        model_->setData(geometry_index,QVariant::fromValue<Geometry*>(geometry),Qt::EditRole);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openGeoJsonFile()
{
    QString name=QFileDialog::getOpenFileName();
    if(!name.isEmpty())
    {
        ui->graphics_view->resetTransform();
        QVector<FeatureInfo*> feature_info_vector=setupModelData(name);
        model_->setModelData(feature_info_vector);
    }
}

void MainWindow::fitInView()
{
    QRectF rect= scene_->itemsBoundingRect();
    ui->graphics_view->centerOn(rect.center());
}

void MainWindow::onModelDataChanged(const QModelIndex &top_left, const QModelIndex &bottom_right, const QVector<int> &roles)
{
    for(int i=top_left.row();i<=bottom_right.row();++i)
    {
        QString name=model_->index(i,0).data(Qt::EditRole).toString();
        QPointF center=model_->index(i,1).data(Qt::EditRole).toPointF();
        Geometry* geometry=model_->index(i,2).data(Qt::EditRole).value<Geometry*>();

        QGraphicsPathItem* item=new QGraphicsPathItem();
        item->setPos(center);

        QPainterPath path=geometry->getPath();
        QFont font("Times",12);
        QFontMetricsF font_metrics(font);
        QRectF rect=font_metrics.boundingRect(path.boundingRect(),Qt::AlignCenter,name);
        path.addText(rect.bottomLeft(),font,name);
        item->setPath(path);

        scene_->addItem(item);
    }
}

QVector<FeatureInfo*> MainWindow::setupModelData(const QString &name)
{
    QVector<FeatureInfo*> feature_info_vector;

    ParseGenJson parse_gen_json;
    const GenJson& gen_json=parse_gen_json.loadGenJsonFile(name);
    for(int i=0;i<gen_json.features.size();++i)
    {
        const Feature& feature=gen_json.features.at(i);
        FeatureInfo* feature_info=new FeatureInfo();

        Geometry geometry=feature.geometry;
        geometry.scale(100.0,-100.0);
        QPointF center=geometry.getCenter();        
        geometry.translate(-center.x(),-center.y());

        feature_info->name=feature.properties.name;
        feature_info->center=center;
        feature_info->geometry=geometry;

        feature_info_vector.append(feature_info);
    }

    return feature_info_vector;
}
