#ifndef SUB_MAINWINDOW_H
#define SUB_MAINWINDOW_H

#include <QMainWindow>
#include "Controller.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class sub_MainWindow;
}
QT_END_NAMESPACE

class sub_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit sub_MainWindow(QWidget *parent = nullptr);
    ~sub_MainWindow();

private:
    QTime time;
    Controller *Elesys;
    QTextBrowser* qtext[4];
    QLabel* pfloor[4][21];
    void setpointer();
    Ui::sub_MainWindow *ui;

private slots:
    void timerUpDate();
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
    void get_info(int,int,int,int);
};

#endif /* SUB_MAINWINDOW_H */