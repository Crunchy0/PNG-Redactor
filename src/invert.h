#ifndef INVERT_H
#define INVERT_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class Invert;
}

class Invert : public QDialog
{
    Q_OBJECT

public:
    explicit Invert(QWidget *parent = nullptr);
    ~Invert();

private slots:
    void on_radioButton_2_toggled(bool checked);

    void on_radioButton_toggled(bool checked);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::Invert *ui;

signals:
    void send_invert(int, int, int, int, float, int);
};

#endif // INVERT_H
