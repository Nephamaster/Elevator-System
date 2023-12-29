#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H

#include <QMainWindow>
#include "Controller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow1; }
QT_END_NAMESPACE

class MainWindow1 : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow1(QWidget *parent = nullptr);
    ~MainWindow1();

private:
    QTime time;
    Ui::MainWindow *ui;
    Controller *Elesys;
    QTextBrowser* qtext[4];
    QLabel* pfloor[4][21];
    void setpointer();

private slots:
    void timerUpDate();
    void on_pushButton_clicked();
    void update_floor(int, int, int);
    void update_t1();
    void update_t2();
    void update_t3();
    void slot_run(int);
    void slot_static(int);
    void slot_down(int);
    void slot_up(int);
    void slot_wait(int, int);
    void slot_out(int, int);
    void slot_complete(int, int);
    void slot_in(int, int);
    void slot_move(int, int, int);

};
#endif /* MAINWINDOW_H */