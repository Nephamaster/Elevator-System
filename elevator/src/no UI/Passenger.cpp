#include "Passenger.h"

Passenger::Passenger(int id, int wt, int des, int cur)
{
    ID = id;
    Weight = wt;

    if(des == -1)
        DesFloor = 0;     //地下一层作为0层处理
    else DesFloor = des;

    if(cur == -1)
        CurFloor = 0;
    else CurFloor = cur;

    elevID = 0;
    direct = des - cur;
    reqTime = 0;
    cosTime = 0;
}

Passenger::~Passenger(){}