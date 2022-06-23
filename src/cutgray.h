#ifndef CUTGRAY_H
#define CUTGRAY_H

#include <QDialog>

namespace Ui {
class CutGray;
}

class CutGray : public QDialog
{
    Q_OBJECT

public:
    explicit CutGray(QWidget *parent = nullptr);
    ~CutGray();
public slots:

    void recieve_coord_handle(int);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::CutGray *ui;
    int mode;

signals:
    void send_cutgray(int, int, int, int, int);
};

#endif // CUTGRAY_H
