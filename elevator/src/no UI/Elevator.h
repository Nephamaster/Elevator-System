#pragma once
#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <ctime>
#include <windows.h>
#include "Passenger.h"

using namespace std;

/** 电梯类 
 * 属性：
 * 包括电梯编号、人数重量限制、当前人数和重量、当前所在楼层、运动方向
 * 以及乘客进出时长、移动一层时长和服务楼层列表
 * 方法：
 * 1.predTime：预估到达目标楼层时长，供系统调度使用
 * 2.findRequest：获取乘客请求
 * 3.move：楼层间移动
 * 4.waiting：等待乘客进出
*/
class Elevator
{
private:
    friend class Passenger;

    int ID;         //电梯编号
    int numLim;     //乘客人数限制
    int weightLim;  //乘客重量限制
    int curLevel;   //当前所在楼层号
    int curCount;   //当前乘坐人数
    int curWeight;  //当前承载重量
    int servedNum;  //已服务人数

    int moveDirect; //运动方向 - 0静止，1向上，-1向下
    int waitTime;   //乘客进出时长
    int moveTime;   //移动一层时长

    bool serveLevels[21]; //服务楼层 true，不服务楼层 false

    vector<int> aboveSeq;   //上方请求队列(相对当前位置)
    vector<int> belowSeq;   //下方请求队列(相对当前位置)

public:
    vector<Passenger> inEle[21];    //inEle[x] 代表从 x 楼进电梯的乘客
    vector<Passenger> outEle[21];   //outEle[x] 代表从 x 楼出电梯的乘客
    vector<Passenger> servedPsg;    //已服务乘客

    Elevator(int id, int nl, int wl, int cl, int cc, int cw, int mov, int wt, int mt);
    ~Elevator();

    bool AbSeqEmp(){return aboveSeq.empty();}
    bool BeSeqEmp(){return belowSeq.empty();}

    void setcurCount(int count){ this->curCount = count; }
    void setcurWeight(int weight){ this->curWeight = weight; }
    void setcurLevel(int level){ this->curLevel = level; }
    void setmoveDirect(int direct){ this->moveDirect = direct; }
    
    void setAboveSeq(const vector<int>seq){ aboveSeq = seq; }
    void setBelowSeq(const vector<int>seq){ belowSeq = seq; }
    void setServeLevel(vector<int> num);

    int getID(){ return ID; }
    int getcurCount(){ return curCount; }
    int getcurweight(){ return curWeight; }
    int getcurLevel(){ return curLevel; }
    int getmoveDirect(){ return moveDirect; }
    int getServedNum(){ return servedNum; }
    int getAbHead();
    int getBeHead();
    
    int predTime(int deslevel);
    void findRequest(int deslevel);
    void move(int level);
    void waiting();
    
    void showInfo();
};

#endif /* ELEVATOR_H */