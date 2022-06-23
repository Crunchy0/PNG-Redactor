#include "seg.h"
#include "ui_seg.h"
#include <QMessageBox>

Seg::Seg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Seg)
{
    ui->setupUi(this);
    color = 0;
}

Seg::~Seg()
{
    delete ui;
}

void Seg::recieve_ct(int ct){
    color_type = ct;
}

void Seg::on_pushButton_clicked()
{
    QColor col = QColorDialog::getColor(Qt::white, this, "Выбор цвета");

    color = col.red() + 256*col.green() + 256*256*col.blue();
    if(!color_type){
        color = std::rintf((float)(color/3));
    }
    QString s = "QLabel {background-color: #";
    if(col.red() < 16){
        s.append("0");
    }
    s.append(QString::number(col.red(), 16));

    if(col.green() < 16){
        s.append("0");
    }
    s.append(QString::number(col.green(), 16));
    if(col.blue() < 16){
        s.append("0");
    }
    s.append(QString::number(col.blue(), 16));
    s.append(";}");
    ui->label_7->setStyleSheet(s);
}

void Seg::on_pushButton_2_clicked()
{
    int th = ui->thickness->text().toInt();
    int x = (ui->x2->toPlainText().toInt() - ui->x1->toPlainText().toInt());
    int y = (ui->y2->toPlainText().toInt() - ui->y1->toPlainText().toInt());
    int x1, x2, y1, y2;
    x1 = ui->x1->toPlainText().toInt();
    x2 = ui->x2->toPlainText().toInt();
    y1 = ui->y1->toPlainText().toInt();
    y2 = ui->y2->toPlainText().toInt();
    if(x1 <= 0 || x2 <= 0 || y1 <= 0 || y2 <= 0 || th <= 0){
        QMessageBox::information(this, "Сообщение", "Некоторые поля не заполнены или заполнены неверно!\n\nПодсказка: введите натуральные числа");
        return;
    }
    else if((th*th) > (x*x + y*y)){
        QMessageBox::information(this, "Сообщение", "Указанная толщина больше, чем длина!\n\nУстановлено маскимальное допустимое значение");
        while((th*th) > (x*x + y*y)){
            th--;
        }
        ui->thickness->setText(QString::number(th));
        return;
    }
    emit send_seg(x1, x2, y1, y2, th, color);
    this->close();
}

void Seg::on_pushButton_3_clicked()
{
    this->close();
}
