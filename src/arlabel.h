#ifndef ARLABEL_H
#define ARLABEL_H

#include <QWidget>
#include <QLabel>
#include <QPainter>

class ArLabel : public QLabel
{
public:
    ArLabel(QWidget *parent);
    void setNewpixmap(QPixmap &npixmap);
private:
    QPixmap pixmap;

protected:
    void paintEvent(QPaintEvent*) override;
};

#endif // ARLABEL_H
