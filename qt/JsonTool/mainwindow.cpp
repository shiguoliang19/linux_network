#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore>
#include <QtNetwork>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    const QMetaObject &mo = QCryptographicHash::staticMetaObject;
    QMetaEnum meta_enum = mo.enumerator(mo.indexOfEnumerator("Algorithm"));
    for(int i = 0; i < meta_enum.keyCount(); ++i)
    {
        QString text = meta_enum.key(i);
        ui->algorithm_combo_box->addItem(text);
    }
    ui->algorithm_combo_box->setCurrentText("Md5");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_source_text_edit_textChanged()
{    
    QString source_json = ui->source_text_edit->toPlainText();

    QJsonParseError json_error;
    QJsonDocument json_document = QJsonDocument::fromJson(source_json.toUtf8(), &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        QString indented_json = json_document.toJson(QJsonDocument::Indented);
        ui->target_text_edit->setText(indented_json);

        //QString compact_json = json_document.toJson(QJsonDocument::Compact);
        //ui->target_text_edit->setText(indented_json + "\n" + compact_json);
    }
    else
    {
        ui->target_text_edit->setText(json_error.errorString());
    }
}

void MainWindow::on_data_text_edit_textChanged()
{
    QString data = ui->data_text_edit->toPlainText();
    if(data.isEmpty())
    {
        ui->result_text_edit->clear();
        return;
    }

    QCryptographicHash::Algorithm method = (QCryptographicHash::Algorithm)ui->algorithm_combo_box->currentIndex();
    QByteArray hash = QCryptographicHash::hash(data.toUtf8(), method);
    QString result = hash.toHex();
    ui->result_text_edit->setText(result);
}

void MainWindow::on_algorithm_combo_box_currentIndexChanged(int index)
{
    on_data_text_edit_textChanged();
}

void MainWindow::on_test_button_clicked()
{
    QString domain = ui->domain_text_edit->toPlainText();
    QHostInfo host_info = QHostInfo::fromName(domain);

    for(QHostAddress& host_address : host_info.addresses())
    {
        ui->ip_text_edit->setPlainText(host_address.toString());
    }
}

