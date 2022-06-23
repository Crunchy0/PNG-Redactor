#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cutgray.h"
#include "invert.h"
#include "seg.h"
#include "pngclass.h"
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void recieve_seg(int, int, int, int, int, int);
    void recieve_cutgray(int, int, int, int, int);
    void recieve_invert(int, int, int, int, float, int);

private slots:
    void on_open_triggered();

    void on_save_triggered();

    void on_about_triggered();

    void closeEvent(QCloseEvent *event);


    void on_cut_triggered();

    void on_invert_triggered();

    void on_gray_triggered();

    void on_seg_triggered();


    void on_instr_triggered();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    CutGray *cg;
    Invert *inv;
    Seg *sg;
    PngClass *image;
    const char *temp, *root, *filter;
};
#endif // MAINWINDOW_H
