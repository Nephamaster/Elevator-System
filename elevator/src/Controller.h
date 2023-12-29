#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <thread>
#include "Elevator.h"
#include "Passenger.h"
using namespace std;

/** 系统控制类
 * 属性：
 * 驱动程序模式、四部电梯
 * 方法：
 * 1.STFdispatch：最短时间优先调度算法
 * 2.reqRecv：以最短时间优先算法为乘客选取最省时的电梯
 * 3.performTest: 调度算法性能测试
*/
class Controller: public QObject
{
    Q_OBJECT

private:
    bool reqLevel[21];  // reqLevel[x]: True有请求，False无请求
    int servedNum;      //已服务乘客总数
    int mode;           //程序驱动模式

public:
    Elevator* elev[4];

    Controller(int mod);
    ~Controller();

    int getServedNum(){ return servedNum; }
    int STFdispatch(int curLevel);
    void performTest();
    void reqRecv(Passenger psg);

private slots:
    void servedNumAdd(){ servedNum ++; }
};

#endif /* CONTROLLER_H */