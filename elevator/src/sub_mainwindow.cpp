#include "sub_mainwindow.h"
#include "ui_sub_mainwindow.h"
#include "mainwindow.h"
#include "QtCore"
#include "QLineEdit"
#include "QLabel"
#include "QString"

sub_MainWindow::sub_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::sub_MainWindow)
{
    int mode = 2;//设置算法模式(0-Bus,1-scan,2-look)

    MainWindow *w = new MainWindow(this);
    w->show();
    ui->setupUi(this);
    this->setFixedSize(2100,1500);
    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(22,218,208, 100), stop:1 rgba(248,248,255, 210));");

    ui->label_systime->setStyleSheet("background-color:rgb(248,248,255);");
    ui->label_7->setStyleSheet("background-color:rgb(248,248,255);border:2px solid rgb(248,248,255);border-radius:10px;");
    ui->label_8->setStyleSheet("background-color:rgb(248,248,255);border:2px solid rgb(248,248,255);border-radius:10px;");
    ui->label_9->setStyleSheet("background-color:rgb(248,248,255);border:2px solid rgb(248,248,255);border-radius:10px;");
    ui->label_10->setStyleSheet("background-color:rgb(248,248,255);border:2px solid rgb(248,248,255);border-radius:10px;");

    ui->textBrowser_0->setStyleSheet("background-color:rgb(248,248,255);border:2px solid rgb(0,0,0);border-radius:10px;");
    ui->textBrowser_1->setStyleSheet("background-color:rgb(248,248,255);border:2px solid rgb(0,0,0);border-radius:10px;");
    ui->textBrowser_2->setStyleSheet("background-color:rgb(248,248,255);border:2px solid rgb(0,0,0);border-radius:10px;");
    ui->textBrowser_3->setStyleSheet("background-color:rgb(248,248,255);border:2px solid rgb(0,0,0);border-radius:10px;");
    ui->textEdit->setStyleSheet("background-color:rgb(248,248,255);color:rgb(65,105,225);border:2px solid rgb(0,0,0);border-radius:10px;");

    ui->widget_3->setStyleSheet("background-color:rgb(248,248,255);border:2px solid rgb(0,0,0);border-radius:10px;");
    ui->widget_2->setStyleSheet("background-color:rgb(248,248,255);border:2px solid rgb(0,0,0);border-radius:10px;");
    ui->widget_4->setStyleSheet("background-color:rgb(248,248,255);border:2px solid rgb(0,0,0);border-radius:10px;");
    ui->widget->setStyleSheet("background-color:rgb(248,248,255);border:2px solid rgb(0,0,0);border-radius:10px;");
    ui->widget_9->setStyleSheet("background-color:rgb(248,248,255);border:2px solid rgb(0,0,0);border-radius:10px;");

    void nowtime();
    time.start();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpDate()));
    connect(timer, SIGNAL(timeout()), this, SLOT(update_t1()));
    connect(timer, SIGNAL(timeout()), this, SLOT(update_t2()));
    connect(timer, SIGNAL(timeout()), this, SLOT(update_t3()));
    timer->start(1000/333);

    Elesys = new Controller(mode);
    string str1 = "结对项目-电梯调度子系统(0-Bus,1-scan,2-look):";
    string str = str1 + to_string(mode) ;
    this->setWindowTitle(str.c_str());
    setpointer();

    Elevator* elev0 = Elesys->elev[0];
    Elevator* elev1 = Elesys->elev[1];
    Elevator* elev2 = Elesys->elev[2];
    Elevator* elev3 = Elesys->elev[3];

    connect(elev0, SIGNAL(signal_arrive_floor(int, int, int)), this, SLOT(update_floor(int, int, int)));
    connect(elev1, SIGNAL(signal_arrive_floor(int, int, int)), this, SLOT(update_floor(int, int, int)));
    connect(elev2, SIGNAL(signal_arrive_floor(int, int, int)), this, SLOT(update_floor(int, int, int)));
    connect(elev3, SIGNAL(signal_arrive_floor(int, int, int)), this, SLOT(update_floor(int, int, int)));

    connect(elev0, SIGNAL(signal_run(int)), this, SLOT(slot_run(int)));
    connect(elev1, SIGNAL(signal_run(int)), this, SLOT(slot_run(int)));
    connect(elev2, SIGNAL(signal_run(int)), this, SLOT(slot_run(int)));
    connect(elev3, SIGNAL(signal_run(int)), this, SLOT(slot_run(int)));

    connect(elev0, SIGNAL(signal_static(int)), this, SLOT(slot_static(int)));
    connect(elev1, SIGNAL(signal_static(int)), this, SLOT(slot_static(int)));
    connect(elev2, SIGNAL(signal_static(int)), this, SLOT(slot_static(int)));
    connect(elev3, SIGNAL(signal_static(int)), this, SLOT(slot_static(int)));

    connect(elev0, SIGNAL(signal_down(int)), this, SLOT(slot_down(int)));
    connect(elev1, SIGNAL(signal_down(int)), this, SLOT(slot_down(int)));
    connect(elev2, SIGNAL(signal_down(int)), this, SLOT(slot_down(int)));
    connect(elev3, SIGNAL(signal_down(int)), this, SLOT(slot_down(int)));

    connect(elev0, SIGNAL(signal_up(int)), this, SLOT(slot_up(int)));
    connect(elev1, SIGNAL(signal_up(int)), this, SLOT(slot_up(int)));
    connect(elev2, SIGNAL(signal_up(int)), this, SLOT(slot_up(int)));
    connect(elev3, SIGNAL(signal_up(int)), this, SLOT(slot_up(int)));

    connect(elev0, SIGNAL(signal_wait(int, int)), this, SLOT(slot_wait(int, int)));
    connect(elev0, SIGNAL(signal_wait(int, int)), this, SLOT(slot_wait(int, int)));
    connect(elev0, SIGNAL(signal_wait(int, int)), this, SLOT(slot_wait(int, int)));
    connect(elev0, SIGNAL(signal_wait(int, int)), this, SLOT(slot_wait(int, int)));

    connect(elev0, SIGNAL(signal_out(int, int)), this, SLOT(slot_out(int, int)));
    connect(elev1, SIGNAL(signal_out(int, int)), this, SLOT(slot_out(int, int)));
    connect(elev2, SIGNAL(signal_out(int, int)), this, SLOT(slot_out(int, int)));
    connect(elev3, SIGNAL(signal_out(int, int)), this, SLOT(slot_out(int, int)));

    connect(elev0, SIGNAL(signal_complete(int, int)), this, SLOT(slot_complete(int, int)));
    connect(elev1, SIGNAL(signal_complete(int, int)), this, SLOT(slot_complete(int, int)));
    connect(elev2, SIGNAL(signal_complete(int, int)), this, SLOT(slot_complete(int, int)));
    connect(elev3, SIGNAL(signal_complete(int, int)), this, SLOT(slot_complete(int, int)));

    connect(elev0, SIGNAL(signal_in(int, int)), this, SLOT(slot_in(int, int)));
    connect(elev1, SIGNAL(signal_in(int, int)), this, SLOT(slot_in(int, int)));
    connect(elev2, SIGNAL(signal_in(int, int)), this, SLOT(slot_in(int, int)));
    connect(elev3, SIGNAL(signal_in(int, int)), this, SLOT(slot_in(int, int)));

    connect(elev0, SIGNAL(signal_move(int, int, int)), this, SLOT(slot_move(int, int, int)));
    connect(elev1, SIGNAL(signal_move(int, int, int)), this, SLOT(slot_move(int, int, int)));
    connect(elev2, SIGNAL(signal_move(int, int, int)), this, SLOT(slot_move(int, int, int)));
    connect(elev3, SIGNAL(signal_move(int, int, int)), this, SLOT(slot_move(int, int, int)));

    connect(w, SIGNAL(info(int,int,int,int)), this, SLOT(get_info(int,int,int,int)));
}

sub_MainWindow::~sub_MainWindow()
{
    delete ui;
}

void sub_MainWindow::update_floor(int id, int last_floor, int next_floor)
{
    if(next_floor == -1)next_floor = 0;
    if(last_floor == -1)last_floor = 0;
    pfloor[id][last_floor]->setStyleSheet("QLabel{background-color:rgb(166, 168, 164);border-color: rgb(36, 36, 36);}");
    pfloor[id][next_floor]->setStyleSheet("QLabel{background-color:rgb(255, 94, 41);border-color: rgb(36, 36, 36);}");
}

void sub_MainWindow::update_t1()
{
     ui->label_t1->setText(QString::number(Elesys->getServedNum()));
}

void sub_MainWindow::update_t2()
{
    ui->label_t2->setText(QString::number(time.elapsed()/1000));
}

void sub_MainWindow::update_t3()
{
    char strBuf[6];
    sprintf(strBuf,"%.1f",Elesys->getServedNum()*1.0/(time.elapsed()*1.0/1000)*60);
    ui->label_t3->setText(strBuf);
}

void sub_MainWindow::slot_run(int id)
{
    string str1 = "Elev";
    string str2 = " start.";
    string str = str1 + to_string(id) + str2;
    qtext[id]->append(str.c_str());
}

void sub_MainWindow::slot_static(int id)
{
    string str1 = "Elev";
    string str2 = " stop.";
    string str = str1 + to_string(id) + str2;
    qtext[id]->append(str.c_str());
}

void sub_MainWindow::slot_down(int id)
{
    string str1 = "Elev";
    string str2 = " down.";
    string str = str1 + to_string(id) + str2;
    qtext[id]->append(str.c_str());
}

void sub_MainWindow::slot_up(int id)
{
    string str1 = "Elev";
    string str2 = " up.";
    string str = str1 + to_string(id) + str2;
    qtext[id]->append(str.c_str());
}

void sub_MainWindow::slot_wait(int id, int floor)
{
    string str1 = "At ";
    string str2 = "th floor waiting for passengers.";
    string str = str1 + to_string(floor) +str2;
    qtext[id]->append(str.c_str());
}

void sub_MainWindow::slot_out(int id ,int passenger_id)
{
    string str1 = "Passenger ";
    string str2 = " get out of the elevator.";
    string str = str1 + to_string(passenger_id) +str2 + to_string(id);
    qtext[id]->append(str.c_str());
}

void sub_MainWindow::slot_complete(int id, int passenger_id)
{
    string str1 = "Complete  passenger ";
    string str2 = "'s request." ;
    string str = str1 + to_string(passenger_id) +str2;
    qtext[id]->append(str.c_str());
}

void sub_MainWindow::slot_in(int id, int passenger_id)
{
    string str1 = "Passenger ";
    string str2 = " enter the elevator.";
    string str = str1 + to_string(passenger_id) + str2 + to_string(id);
    qtext[id]->append(str.c_str());
}

void sub_MainWindow::slot_move(int id, int last_floor, int next_floor)
{
    string str1 = "Move: ";
    string str2 = "th floor->";
    string str3 = "th floor.";
    string str = str1 + to_string(last_floor) + str2 + to_string(next_floor) + str3;
    qtext[id]->append(str.c_str());
}

void sub_MainWindow::setpointer()
{
    qtext[0] = ui->textBrowser_0;
    qtext[1] = ui->textBrowser_1;
    qtext[2] = ui->textBrowser_2;
    qtext[3] = ui->textBrowser_3;

    pfloor[0][0] = ui->label_0_0;
    pfloor[0][1] = ui->label_0_1;
    pfloor[0][2] = ui->label_0_2;
    pfloor[0][3] = ui->label_0_3;
    pfloor[0][4] = ui->label_0_4;
    pfloor[0][5] = ui->label_0_5;
    pfloor[0][6] = ui->label_0_6;
    pfloor[0][7] = ui->label_0_7;
    pfloor[0][8] = ui->label_0_8;
    pfloor[0][9] = ui->label_0_9;
    pfloor[0][10] = ui->label_0_10;
    pfloor[0][11] = ui->label_0_11;
    pfloor[0][12] = ui->label_0_12;
    pfloor[0][13] = ui->label_0_13;
    pfloor[0][14] = ui->label_0_14;
    pfloor[0][15] = ui->label_0_15;
    pfloor[0][16] = ui->label_0_16;
    pfloor[0][17] = ui->label_0_17;
    pfloor[0][18] = ui->label_0_18;
    pfloor[0][19] = ui->label_0_19;
    pfloor[0][20] = ui->label_0_20;
    pfloor[0][1]->setStyleSheet("QLabel{background-color:rgb(255,94,41);border-color: rgb(36, 36, 36);}");

    pfloor[1][0] = ui->label_1_0;
    pfloor[1][1] = ui->label_1_1;
    pfloor[1][2] = ui->label_1_2;
    pfloor[1][3] = ui->label_1_3;
    pfloor[1][4] = ui->label_1_4;
    pfloor[1][5] = ui->label_1_5;
    pfloor[1][6] = ui->label_1_6;
    pfloor[1][7] = ui->label_1_7;
    pfloor[1][8] = ui->label_1_8;
    pfloor[1][9] = ui->label_1_9;
    pfloor[1][10] = ui->label_1_10;
    pfloor[1][11] = ui->label_1_11;
    pfloor[1][12] = ui->label_1_12;
    pfloor[1][13] = ui->label_1_13;
    pfloor[1][14] = ui->label_1_14;
    pfloor[1][15] = ui->label_1_15;
    pfloor[1][16] = ui->label_1_16;
    pfloor[1][17] = ui->label_1_17;
    pfloor[1][18] = ui->label_1_18;
    pfloor[1][19] = ui->label_1_19;
    pfloor[1][20] = ui->label_1_20;
    pfloor[1][1]->setStyleSheet("QLabel{background-color:rgb(255,94,41);border-color: rgb(36, 36, 36);}");

    pfloor[2][0] = ui->label_2_0;
    pfloor[2][1] = ui->label_2_1;
    pfloor[2][2] = ui->label_2_2;
    pfloor[2][3] = ui->label_2_3;
    pfloor[2][4] = ui->label_2_4;
    pfloor[2][5] = ui->label_2_5;
    pfloor[2][6] = ui->label_2_6;
    pfloor[2][7] = ui->label_2_7;
    pfloor[2][8] = ui->label_2_8;
    pfloor[2][9] = ui->label_2_9;
    pfloor[2][10] = ui->label_2_10;
    pfloor[2][11] = ui->label_2_11;
    pfloor[2][12] = ui->label_2_12;
    pfloor[2][13] = ui->label_2_13;
    pfloor[2][14] = ui->label_2_14;
    pfloor[2][15] = ui->label_2_15;
    pfloor[2][16] = ui->label_2_16;
    pfloor[2][17] = ui->label_2_17;
    pfloor[2][18] = ui->label_2_18;
    pfloor[2][19] = ui->label_2_19;
    pfloor[2][20] = ui->label_2_20;
    pfloor[2][1]->setStyleSheet("QLabel{background-color:rgb(255,94,41);border-color: rgb(36, 36, 36);}");

    pfloor[3][0] = ui->label_3_0;
    pfloor[3][1] = ui->label_3_1;
    pfloor[3][2] = ui->label_3_2;
    pfloor[3][3] = ui->label_3_3;
    pfloor[3][4] = ui->label_3_4;
    pfloor[3][5] = ui->label_3_5;
    pfloor[3][6] = ui->label_3_6;
    pfloor[3][7] = ui->label_3_7;
    pfloor[3][8] = ui->label_3_8;
    pfloor[3][9] = ui->label_3_9;
    pfloor[3][10] = ui->label_3_10;
    pfloor[3][11] = ui->label_3_11;
    pfloor[3][12] = ui->label_3_12;
    pfloor[3][13] = ui->label_3_13;
    pfloor[3][14] = ui->label_3_14;
    pfloor[3][15] = ui->label_3_15;
    pfloor[3][16] = ui->label_3_16;
    pfloor[3][17] = ui->label_3_17;
    pfloor[3][18] = ui->label_3_18;
    pfloor[3][19] = ui->label_3_19;
    pfloor[3][20] = ui->label_3_20;
    pfloor[3][1]->setStyleSheet("QLabel{background-color:rgb(255,94,41);border-color: rgb(36, 36, 36);}");
    qDebug() << "setpointer successfully!";
}

void sub_MainWindow::timerUpDate()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    ui->label_systime->setText(str);
}

void sub_MainWindow::get_info(int id,int w,int d,int c)
{
    Passenger psg = Passenger(id, w, d, c);

    psg.setReqTime();
    Elesys->reqRecv(psg);
}