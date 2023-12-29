#include "mainwindow.h"
#include "sub_mainwindow.h"
#include "Elevator.h"
#include "Controller.h"
#include "Passenger.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sub_MainWindow w1;
    w1.show();

    //Elevator_Test();
    return a.exec();
}
