#include "Elevator.h"

Elevator::Elevator(int id, int nl, int wl, int cl, int cc, int cw, int mov, int wt, int mt)
{
    ID = id;
    numLim = nl;
    weightLim = wl;
    curCount = cc;
    curWeight = cw;
    curLevel = cl;
    moveDirect = mov;
    waitTime = wt;
    moveTime = mt;
    servedNum = 0;

    for(int i = 0; i< 21; i++)
        serveLevels[i] = false;
}

Elevator::~Elevator()
{
    aboveSeq.~vector();
    belowSeq.~vector();
}

/** 预估到达目标楼层时长，供系统调度使用 */
int Elevator::predTime(int deslevel)
{
    int preTime = 0;     //预估运行总时长
    int tmp = curLevel;
    
    if(serveLevels[deslevel] == false) return 100000;

    if(moveDirect == 0)  //电梯只在1楼静止
    {
        preTime = abs(deslevel - 1) * moveTime;
        return preTime;
    }

    if(curLevel > deslevel)  //电梯当前楼层高于目标楼层 
    {
        if(moveDirect == 1)
        {
            for(int i = 0; i < aboveSeq.size(); i++)
            {
                preTime += (aboveSeq[i] - tmp) * moveTime;
                tmp = aboveSeq[i];
                preTime += waitTime;
            }
            for(int i = 0; i < belowSeq.size(); i++)
            {
                if(belowSeq[i] >= deslevel)
                {
                    preTime += (tmp - belowSeq[i]) * moveTime;
                    tmp = belowSeq[i];
                    preTime += waitTime;
                }
                else break;
            }
            preTime += (tmp - deslevel) * moveTime;
        }

        else if(moveDirect == -1)
        {
            for(int i = 0; i < belowSeq.size(); i++)
            {
                if(belowSeq[i] >= deslevel)
                {
                    preTime += (tmp - belowSeq[i]) * moveTime;
                    tmp = belowSeq[i];
                    preTime += waitTime;
                }
                else break;
            }
            preTime += (tmp - deslevel) * moveTime;
        }
    }
    else if(curLevel == deslevel) preTime = 0;
    else
    {
        if(moveDirect == 1)
        {
            for(int i = 0; i < aboveSeq.size(); i++)
            {
                if(aboveSeq[i] <= deslevel)
                {
                    preTime += (aboveSeq[i] - tmp) * moveTime;
                    tmp = aboveSeq[i];
                    preTime += waitTime;
                }
                else break;
            }
            preTime += (deslevel - tmp) * moveTime;
        }
        else if(moveDirect == -1)
        {
            for(int i = 0; i < belowSeq.size(); i++)
            {
                preTime += (tmp - belowSeq[i]) * moveTime;
                tmp = belowSeq[i];
                preTime += waitTime;
            }
            for(int i = 0; i < aboveSeq.size(); i++)
            {
                if(aboveSeq[i] <= deslevel)
                {
                    preTime += (aboveSeq[i] - tmp) * moveTime;
                    tmp = aboveSeq[i];
                    preTime += waitTime;
                }
                else break;
            }
            preTime += (deslevel - tmp) * moveTime;
        }
    }
    return preTime;
}

/** 获取乘客请求 */
void Elevator::findRequest(int deslevel)
{
    if(deslevel >= curLevel)
    {
        aboveSeq.push_back(deslevel);
        sort(aboveSeq.begin(), aboveSeq.end());
    }
    else
    {
        belowSeq.push_back(deslevel);
        sort(belowSeq.begin(), belowSeq.end(), greater<int>());
    }
}

/** 等待乘客进出 */
void Elevator::waiting()
{
    while(!outEle[curLevel].empty())   //乘客出
    {
        Passenger psg = outEle[curLevel].back();
        outEle[curLevel].pop_back();
        psg.setCosTime();
        servedPsg.push_back(psg);
        cout << endl << "[ ELEVATOR " << ID << " ]" << " Passenger " << psg.getID() << " Has Reached." << endl << endl;
        curCount--;
        curWeight -= psg.getWeight();
        servedNum++;
    }

    while(!inEle[curLevel].empty())    //乘客进
    {
        Passenger psg = inEle[curLevel].back();
        //未超载或人数未满则可以进
        if(numLim >= curCount + 1 && weightLim - curWeight >= psg.getWeight())
        {
            curCount++;
            curWeight = curWeight + psg.getWeight();
            inEle[curLevel].pop_back();
            int Des = psg.getDes();
            outEle[Des].push_back(psg);
            findRequest(Des);
        }
        else break;
    }

    Sleep(waitTime * 1000); //乘客进出共需waitTime秒
}

/** 两个楼层间移动 */
void Elevator::move(int level)
{
    bool movDir = false; //默认向下
    if(curLevel <= level) movDir = true; //向上

    while(curLevel != level)
    {
        if(movDir)
        {
            curLevel++;
            Sleep(moveTime * 1000); //电梯移动需moveTime秒
        }
        else
        {
            curLevel--;
            Sleep(moveTime * 1000);
        }
    }
}

/** 设置该电梯服务楼层 */
void Elevator::setServeLevel(vector<int> num)
{
    for(int i = 0; i < num.size(); i++)
        serveLevels[num[i]] = true;
}

/** 展示电梯当前状态信息 */
void Elevator::showInfo()
{
    cout << "Elevator " << ID << ":" << endl;
    cout << "Passengers Number: " << curCount << "/" << numLim << ", Total Weight:" << curWeight << "KG/" << weightLim << "KG;" << endl;
    cout << "Current Level: " << curLevel << endl;
    cout << "Reachable Levels: ";
    for(int i = 0; i < 21; i++)
        if(serveLevels[i])
        {
            if(i != 0) cout << i <<" ";
            else cout << "-1 ";
        }
    if(moveDirect == 0) cout << endl << "At Rest." << endl;
    else if(moveDirect == 1) cout << endl << "Upward now." << endl;
    else cout << endl << "Downward now." << endl;
}

/** 获取上方最近请求楼层号 */
int Elevator::getAbHead()
{
    int tmp = aboveSeq[0];
    aboveSeq.erase(aboveSeq.begin());
    return tmp;
}

/** 获取下方最近请求楼层号 */
int Elevator::getBeHead()
{
    int tmp = belowSeq[0];
    belowSeq.erase(belowSeq.begin());
    return tmp;
}