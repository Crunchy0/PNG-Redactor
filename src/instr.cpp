#include "instr.h"
#include "ui_instr.h"

instr::instr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::instr)
{
    ui->setupUi(this);
}

instr::~instr()
{
    delete ui;
}

void instr::on_pushButton_clicked()
{
    this->close();
}
