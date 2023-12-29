#include "Controller.h"
#include "configuration.h"
#include <thread>

using namespace std;

/** 电梯驱动程序
 * 驱动某台电梯以下面三种算法之一运行
 * mode = 0: BUS
 * mode = 1: SCAN
 * mode = 2: LOOK
*/
void Runner(int mode, Elevator* elev);

/** 电梯调度算法 - 基准算法BUS
 * 从-1 层一直到最高层（20 层），每一层都停，并且开门，让乘客进出，然后关门，继续向上走。直到最高层，再向下。
*/
void BusRunner(Elevator* elev);

/** 电梯调度算法 - SCAN
 * 对 BUS 算法的改进，电梯在所有楼层之间来回扫描运行，但只停靠有请求的或乘客目标楼层。
*/
void ScanRunner(Elevator* elev);

/** 电梯调度算法 - LOOK
 * 改进的 C-SCAN 算法，电梯在楼层之间来回运行，但若当前运行方向无请求或乘客目标楼层则立即折返。
*/
void LookRunner(Elevator* elev);

Controller::Controller(int mod)
{
    mode = mod;
    serveList.push_back(e1Levels);
    serveList.push_back(e2Levels);
    serveList.push_back(e3Levels);
    serveList.push_back(e4Levels);

    for(int i = 0; i < 4; i++)
    {
        elev[i] = new Elevator(i, numLimit[i], weightLimit[i], 1, 0, 0, 0, waitingTime, movingTime);
        elev[i]->setServeLevel(serveList[i]);
    }
    
    thread elev0(Runner, mode, elev[0]);
    elev0.detach();
    Sleep(initTime[0] * 100);
    cout << "< CONTROLLER > Elevator 0 Init Completed !" << endl;

    thread elev1(Runner, mode, elev[1]);
    elev1.detach();
    Sleep(initTime[1] * 100);
    cout << "< CONTROLLER > Elevator 1 Init Completed !" << endl;

    thread elev2(Runner, mode, elev[2]);
    elev2.detach();
    Sleep(initTime[2] * 100);
    cout << "< CONTROLLER > Elevator 2 Init Completed !" << endl;

    thread elev3(Runner, mode, elev[3]);
    elev3.detach();
    Sleep(initTime[3] * 100);
    cout << "< CONTROLLER > Elevator 3 Init Completed !" << endl;
}

Controller::~Controller(){}

/** 电梯选择算法 - 最短时间优先：
 * 调用到达发出请求的乘客所在楼层预估时间最短的电梯 */
int Controller::STFdispatch(int curLevel)
{
    vector<int> prior;
    for(int i = 0; i < 4; i++)
        prior.push_back(elev[i]->predTime(curLevel));
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
    elev[anwserId]->findRequest(curLevel);
    elev[anwserId]->inEle[curLevel].push_back(psg);
    cout << "< CONTROLLER > Passenger " << psg.getID() << " Gets In Elevator " << anwserId;
    cout << " From Level " << psg.getCur() << " To Level " << psg.getDes() << endl;
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

void Runner(int mode, Elevator* elev)
{
    if(mode == 0) BusRunner(elev);
    else if(mode == 1) ScanRunner(elev);
    else LookRunner(elev);
}

void BusRunner(Elevator* elev)
{
    if(elev->getmoveDirect() == 0) elev->setmoveDirect(1);
    
    while(TRUE)
    {
        if(elev->getmoveDirect() == 1)
        {
            elev->waiting();
            elev->move(elev->getcurLevel() + 1);
            if(elev->getcurLevel() == 20) elev->setmoveDirect(-1);
        }
        else if(elev->getmoveDirect() == -1)
        {
            elev->waiting();
            elev->move(elev->getcurLevel() - 1);
            if(elev->getcurLevel() == 0) elev->setmoveDirect(1);
        }
    }
}

void ScanRunner(Elevator* elev)
{
    if(elev->getmoveDirect() == 0) elev->setmoveDirect(1);
    
    while(TRUE)
    {
        int curLevel = elev->getcurLevel();
        if(elev->getmoveDirect() == 1)
        {
            if(!elev->inEle[curLevel].empty() || !elev->outEle[curLevel].empty())
                elev->waiting();
            elev->move(curLevel + 1);
            if(elev->getcurLevel() == 20) elev->setmoveDirect(-1);
        }
        else if(elev->getmoveDirect() == -1)
        {
            if(!elev->inEle[curLevel].empty() || !elev->outEle[curLevel].empty())
                elev->waiting();
            elev->move(curLevel - 1);
            if(elev->getcurLevel() == 0) elev->setmoveDirect(1);
        }
    }
}

void LookRunner(Elevator* elev)
{
    while(TRUE)
    {
        //当前无请求则回 1th 层
        if(elev->getmoveDirect() != 0 && elev->AbSeqEmp() && elev->BeSeqEmp())
        {
            elev->move(1);
            elev->setmoveDirect(0); //恢复静止
        }
        //当前静止在1层
        if(elev->getmoveDirect() == 0)
        {
            if (!elev->BeSeqEmp()) elev->setmoveDirect(-1);       //开始向下
            else if(!elev->AbSeqEmp()) elev->setmoveDirect(1);   //开始向上
        }
        //当前向上
        if(elev->getmoveDirect() == 1)
        {
            while(!elev->AbSeqEmp())   //处理上层乘客请求
            {
                elev->move(elev->getAbHead());
                elev->waiting();
            }
            elev->setmoveDirect(-1);         //开始向下
        }
        //当前向下
        if(elev->getmoveDirect() == -1)
        {
            while(!elev->BeSeqEmp())  //处理下层乘客请求
            {
                elev->move(elev->getBeHead());
                elev->waiting();
            }
            elev->setmoveDirect(1);         //开始向上
        }
    }
}