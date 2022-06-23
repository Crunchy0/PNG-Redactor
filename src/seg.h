#ifndef SEG_H
#define SEG_H

#include <QColorDialog>
#include <QDialog>
#include <cmath>

namespace Ui {
class Seg;
}

class Seg : public QDialog
{
    Q_OBJECT

public:
    explicit Seg(QWidget *parent = nullptr);
    ~Seg();

public slots:
    void recieve_ct(int);

private:
    Ui::Seg *ui;
    int color;
    int color_type;
signals:
    void send_seg(int, int, int, int, int, int);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
};

#endif // SEG_H
