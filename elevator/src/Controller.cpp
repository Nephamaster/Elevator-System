#include "Elevator.h"
#include "Controller.h"
#include "configuration.h"
#include <QtConcurrent>
#include <thread>
#include <QThread>
#include "QtCore"
#include <QDebug>

using namespace std;

Controller::Controller(int mod)
{
    servedNum = 0;
    mode = mod;
    serveList.push_back(e1Levels);
    serveList.push_back(e2Levels);
    serveList.push_back(e3Levels);
    serveList.push_back(e4Levels);

    for(int i = 0; i < 4; i++)
    {
        elev[i] = new Elevator(i, numLimit[i], weightLimit[i], 1, 0, 0, 0, waitingTime, movingTime, mod);
        elev[i]->setServeLevel(serveList[i]);
    }

    elev[0]->start();
    elev[1]->start();
    elev[2]->start();
    elev[3]->start();

    connect(elev[0], SIGNAL(signal_add_one_completed_passenger()), this, SLOT(servedNumAdd()));
    connect(elev[1], SIGNAL(signal_add_one_completed_passenger()), this, SLOT(servedNumAdd()));
    connect(elev[2], SIGNAL(signal_add_one_completed_passenger()), this, SLOT(servedNumAdd()));
    connect(elev[3], SIGNAL(signal_add_one_completed_passenger()), this, SLOT(servedNumAdd()));
}

Controller::~Controller(){}

/** 电梯选择算法 - 最短时间优先：
 * 调用到达发出请求的乘客所在楼层预估时间最短的电梯 */
int Controller::STFdispatch(int curLevel)
{
    vector<int> prior;
    for(int i = 0; i < 4; i++)
    {
        prior.push_back(elev[i]->predTime(curLevel));
        qDebug()<<elev[i]->predTime(curLevel);
    }
    int answerID = min_element(prior.begin(), prior.end()) - prior.begin();
    return answerID;
}

/** 乘客请求应答
 * 以最短时间优先算法为乘客选取最省时的电梯
*/
void Controller::reqRecv(Passenger psg)
{
    int curLevel = psg.getCur();
    int anwserId = STFdispatch(curLevel);
    qDebug()<<"anwserId:"<<anwserId;
    elev[anwserId]->findRequest(curLevel);
    elev[anwserId]->inEle[curLevel].push_back(psg);

}

/** 调度算法性能测试
 * 显示每部电梯服务乘客总数，计算每名乘客从发起请求到抵达目的楼层所用时间
*/
void Controller::performTest()
{
    cout << "< CONTROLLER > Runner: ";
    if(mode == 0) cout << "BUS" << endl;
    else if(mode == 1) cout << "SCAN" << endl;
    else cout << "LOOK" << endl;
    int totalTime = 0;
    for(int id=0; id<4; ++id)
    {
        cout << "               Elevator " << elev[id]->getID();
        cout << " Has Served " << elev[id]->getServedNum() << " Passengers." << endl;
        cout << "               Cost Time:" << endl;
        while(!elev[id]->servedPsg.empty())
        {
            Passenger psg = elev[id]->servedPsg.back();
            elev[id]->servedPsg.pop_back();
            cout << "               Passenger " << psg.getID() << ": " << psg.getCosTime() << endl;
            totalTime += psg.getCosTime();
        }
    }
    cout << "               Total Cost Time: " << totalTime << endl;
}