#include "cutgray.h"
#include "ui_cutgray.h"
#include <QMessageBox>

CutGray::CutGray(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CutGray)
{
    ui->setupUi(this);
}

CutGray::~CutGray()
{
    delete ui;
}



void CutGray::recieve_coord_handle(int nmode)
{
    mode = nmode;
    if(mode){
        this->setWindowTitle("Обесцвечивание области");
    }
    else{
        this->setWindowTitle("Кадрирование");
    }
}



void CutGray::on_pushButton_clicked()
{
    int x1 = ui->x1->toPlainText().toInt(),
        x2 = ui->x2->toPlainText().toInt(),
        y1 = ui->y1->toPlainText().toInt(),
        y2 = ui->y2->toPlainText().toInt();
    if(x1 <= 0 || x2 <= 0 || y1 <= 0 || y2 <= 0){
        QMessageBox::information(this, "Сообщение", "Некоторые поля не заполнены или заполнены неверно!\n\nПодсказка: введите натуральные числа");
        return;
    }
    emit send_cutgray(x1, x2, y1, y2, mode);
    this->close();
}

void CutGray::on_pushButton_2_clicked()
{
    this->close();
}
