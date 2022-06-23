#ifndef INSTR_H
#define INSTR_H

#include <QDialog>

namespace Ui {
class instr;
}

class instr : public QDialog
{
    Q_OBJECT

public:
    explicit instr(QWidget *parent = nullptr);
    ~instr();

private slots:
    void on_pushButton_clicked();

private:
    Ui::instr *ui;
};

#endif // INSTR_H
