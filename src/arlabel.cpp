#include "arlabel.h"

ArLabel::ArLabel(QWidget *parent) : QLabel(parent)
{
}

void ArLabel::setNewpixmap(QPixmap &npixmap){
    pixmap = npixmap;
    update();
}

void ArLabel::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    if(!pixmap.isNull()){
        QPoint center(0, 0);
        QSize newsize = size();
        QPixmap newpixmap = pixmap.scaled(newsize, Qt::KeepAspectRatio);
        center.setX((newsize.width()-newpixmap.width())/2);
        center.setY((newsize.height()-newpixmap.height())/2);
        painter.drawPixmap(center, newpixmap);
    }
    QLabel::paintEvent(event);
}
