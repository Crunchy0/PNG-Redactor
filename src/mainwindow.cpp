#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"
#include "instr.h"
#include "arlabel.h"


#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include <cstring>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    temp = "temp.png";
    root = "/home";
    filter = "*.png";
    image = new PngClass;
    ui->label->setStyleSheet("QLabel { background-color : #ffffff; color : #2f002f; }");
}

MainWindow::~MainWindow()
{
    delete image;
    delete ui;
}

void MainWindow::on_open_triggered()
{

    QPixmap pix;
    char filename[200];
    strcpy(filename, qPrintable(QFileDialog::getOpenFileName(this, "Открыть файл", root, filter)));
    if(pix.load(filename)){
         image->read(filename);
         if(image->is_valid()){
             QString imgsize;
             imgsize.append("Размер изображения : ");
             imgsize.append(QString::number(image->get_width()));
             imgsize.append(" X ");
             imgsize.append(QString::number(image->get_height()));
             imgsize.append(" пикселей");
             ui->label_2->setText(imgsize);
             ui->label->setNewpixmap(pix);
             ui->save->setEnabled(true);
             ui->cut->setEnabled(true);
             ui->seg->setEnabled(true);
             ui->gray->setEnabled(true);
             ui->invert->setEnabled(true);             
         }
    }

}

void MainWindow::on_save_triggered()
{
    char filename[200];
    strcpy(filename, qPrintable(QFileDialog::getSaveFileName(this, "Сохранить файл", root, filter)));
    if(image->write(filename)){
        QMessageBox::information(this, "Сообщение", "Изображение успешно сохранено");
        ui->save->setEnabled(false);
        return;
    }
    else{
        QMessageBox::information(this, "Сообщение", "Не удалось сохранить изображение");
    }
}

void MainWindow::on_about_triggered()
{
    About *a = new About;
        a->setModal(a);
        a->show();
}


void MainWindow::recieve_seg(int x1, int x2, int y1, int y2, int thickness, int color)
{
    if(x1 > image->get_width() ||
       x2 > image->get_width() ||
       y1 > image->get_height() ||
       y2 > image->get_height()){
        QMessageBox::information(this, "Сообщение", "Координаты концов отрезка выходят за границы изображения!");
        return;
    }
    image->otrezok(x1 - 1, x2, y1 - 1, y2, thickness, color);
    if(image->write(temp)){
        image->free_self();
        QPixmap pix;
        if(pix.load(temp)){
            image->read(temp);
            if(image->is_valid()){
                QString imgsize;
                imgsize.append("Размер изображения : ");
                imgsize.append(QString::number(image->get_width()));
                imgsize.append(" X ");
                imgsize.append(QString::number(image->get_height()));
                imgsize.append(" пикселей");
                ui->label_2->setText(imgsize);
                ui->label->setNewpixmap(pix);
            }
        }
    }
}

void MainWindow::recieve_cutgray(int x1, int x2, int y1, int y2, int mode)
{
    if(x1 > image->get_width() ||
       x2 > image->get_width() ||
       y1 > image->get_height() ||
       y2 > image->get_height()){
        QMessageBox::information(this, "Ошибка", "Указанная область выходит за границы изображения!");
        return;
    }
    if(!mode){
        image->cut(y1 - 1, y2 - 1, x1 - 1, x2 - 1);
    }
    else{
        image->gray(y1 - 1, y2 - 1, x1 - 1, x2 - 1);
    }
    if(image->write(temp)){
        image->free_self();
        QPixmap pix;
        if(pix.load(temp)){
            image->read(temp);
            if(image->is_valid()){
                QString imgsize;
                imgsize.append("Размер изображения : ");
                imgsize.append(QString::number(image->get_width()));
                imgsize.append(" X ");
                imgsize.append(QString::number(image->get_height()));
                imgsize.append(" пикселей");
                ui->label_2->setText(imgsize);
                ui->label->setNewpixmap(pix);
            }
        }
    }
}

void MainWindow::recieve_invert(int x1, int x2, int y1, int y2, float r, int mode)
{
    if(x1 > image->get_width() ||
       x2 > image->get_width() ||
       y1 > image->get_height() ||
       y2 > image->get_height()){
        if(mode){
            QMessageBox::information(this, "Сообщение", "Координаты центра выходят за границы изображения!");
        }
        else{
            QMessageBox::information(this, "Сообщение", "Координаты углов описанного квадрата выходят за границы изображения!");
        }
        return;
    }
    if(mode){
        image->invert(y1 - 1, x1 - 1, r);
    }
    else{
        image->invert(y1 - 1, y2 - 1, x1 - 1, x2 - 1);
    }
    if(image->write(temp)){
        image->free_self();
        QPixmap pix;
        if(pix.load(temp)){
            image->read(temp);
            if(image->is_valid()){
                QString imgsize;
                imgsize.append("Размер изображения : ");
                imgsize.append(QString::number(image->get_width()));
                imgsize.append(" X ");
                imgsize.append(QString::number(image->get_height()));
                imgsize.append(" пикселей");
                ui->label_2->setText(imgsize);
                ui->label->setNewpixmap(pix);
            }
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event){
    remove(temp);
    image->free_self();
    event->accept();
}

void MainWindow::on_cut_triggered()
{
    cg = new CutGray;
    connect(cg, SIGNAL(send_cutgray(int, int, int, int, int)), this, SLOT(recieve_cutgray(int, int, int, int, int)));
    cg->recieve_coord_handle(0);
    cg->setModal(cg);
    cg->show();
}

void MainWindow::on_invert_triggered()
{
    inv = new Invert;
    connect(inv, SIGNAL(send_invert(int, int, int, int, float, int)), this, SLOT(recieve_invert(int, int, int, int, float, int)));
    inv->setModal(inv);
    inv->show();
}

void MainWindow::on_gray_triggered()
{
    cg = new CutGray;
    connect(cg, SIGNAL(send_cutgray(int, int, int, int, int)), this, SLOT(recieve_cutgray(int, int, int, int, int)));
    cg->recieve_coord_handle(1);
    cg->setModal(cg);
    cg->show();
}

void MainWindow::on_seg_triggered()
{
    sg = new Seg;
    connect(sg, SIGNAL(send_seg(int, int, int, int, int, int)), this, SLOT(recieve_seg(int, int, int, int, int, int)));
    sg->recieve_ct(image->get_color_type());
    sg->setModal(sg);
    sg->show();
}


void MainWindow::on_instr_triggered()
{
    instr *ins = new instr;
    ins->setModal(ins);
    ins->show();
}

void MainWindow::on_pushButton_clicked()
{
    this->close();

}
