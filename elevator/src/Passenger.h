#ifndef PASSENGER_H
#define PASSENGER_H

#include "QtCore"
#include<ctime>

/** 乘客类
 * 属性：
 * 体重、当前和目标楼层号、当前乘坐电梯号、请求移动方向、发起请求时长
 * 方法：
 * 获得各种信息
*/
class Passenger
{
private:
    int ID;           //个人ID
    int Weight;       //体重
    int DesFloor;     //本人的目标楼层号
    int CurFloor;     //本人当前所在楼层号
    int elevID;       //当前乘坐电梯号

    bool direct;      //乘客请求移动方向 - false下, true上
    time_t reqTime;   //乘客发起请求时长
    time_t cosTime;   //乘客花费时间

    int elevator_id;//乘坐的几号电梯

public:
    Passenger(int id, int wt, int des, int cur);
    ~Passenger();
    int getID(){ return ID; }
    int getWeight(){ return Weight; }
    int getDes(){ return DesFloor; }
    int getCur(){ return CurFloor; }
    time_t getReqtime(){ return reqTime; }
    int getCosTime(){ return cosTime; }
    void setReqTime(){ reqTime = time(NULL); }
    void setCosTime(){ cosTime = difftime(time(NULL), reqTime);}
};

#endif /* PASSENGER_H */