#include "invert.h"
#include "ui_invert.h"

Invert::Invert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Invert)
{
    ui->setupUi(this);
    ui->label_5->hide();
    ui->rad->hide();
}

Invert::~Invert()
{
    delete ui;
}

void Invert::on_radioButton_2_toggled(bool checked)
{
    if(checked){
        ui->label_5->hide();
        ui->label_5->setEnabled(false);
        ui->rad->hide();
        ui->rad->setEnabled(false);
        ui->label_2->show();
        ui->label_2->setEnabled(true);
        ui->label_4->show();
        ui->label_4->setEnabled(true);
        ui->x2->show();
        ui->x2->setEnabled(true);
        ui->y2->show();
        ui->y2->setEnabled(true);
    }
}

void Invert::on_radioButton_toggled(bool checked)
{
    if(checked){
        ui->label_5->show();
        ui->label_5->setEnabled(true);
        ui->rad->show();
        ui->rad->setEnabled(true);
        ui->label_2->hide();
        ui->label_2->setEnabled(false);
        ui->label_4->hide();
        ui->label_4->setEnabled(false);
        ui->x2->hide();
        ui->x2->setEnabled(false);
        ui->y2->hide();
        ui->y2->setEnabled(false);
    }
}

void Invert::on_pushButton_2_clicked()
{
    this->close();
}

void Invert::on_pushButton_clicked()
{
    int x1 = ui->x1->toPlainText().toInt(),
        y1 = ui->y1->toPlainText().toInt();
    if(ui->radioButton->isChecked()){
        if(x1 <= 0 || y1 <= 0){
            QMessageBox::information(this, "Сообщение", "Некоторые поля не заполнены или заполнены неверно!\n\nПодсказка: введите натуральные числа");
            return;
        }
        emit send_invert(x1, 0, y1, 0, ui->rad->toPlainText().toFloat(), 1);
    }
    else{
        int x2 = ui->x2->toPlainText().toInt(),
            y2 = ui->y2->toPlainText().toInt();
        if(x1 <= 0 || x2 <= 0 || y1 <= 0 || y2 <= 0){
            QMessageBox::information(this, "Сообщение", "Некоторые поля не заполнены или заполнены неверно!\n\nПодсказка: введите натуральные числа");
            return;
        }
        else if(std::abs(x2 - x1) != std::abs(y2 - y1)){
            QMessageBox::information(this, "Сообщение", "Выделенная область не является квадратом!");
            return;
        }
        emit send_invert(x1, x2, y1, y2, 0.0, 0);
    }
    this->close();
}
