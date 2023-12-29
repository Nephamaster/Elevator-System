#ifndef CONTROLLER_H
#define CONTROLLER_H

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
class Controller
{
private:
    int mode;
public:
    Elevator* elev[4];
    Controller(int mod);
    ~Controller();

    int STFdispatch(int curLevel);
    void reqRecv(Passenger psg);
    void performTest();
};

#endif /* CONTROLLER_H */