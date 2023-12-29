#include "Elevator.h"

Ui::MainWindow *ui = nullptr;

void ui_init(Ui::MainWindow *ui_ptr)
{
    ui = ui_ptr;
}

Elevator::Elevator(int id, int nl, int wl, int cl, int cc, int cw, int mov, int wt, int mt,int mo)
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
    mode = mo;

    for(int i = 0; i< 21; i++)
        serveLevels[i] = false;

    qtext[0] = ui->textBrowser_0;
    qtext[1] = ui->textBrowser_1;
    qtext[2] = ui->textBrowser_2;
    qtext[3] = ui->textBrowser_3;
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
            int ab = aboveSeq.size();
            for(int i = 0; i < ab; i++)
            {
                preTime += (aboveSeq[i] - tmp) * moveTime;
                tmp = aboveSeq[i];
                preTime += waitTime;
            }
            int be = belowSeq.size();
            for(int i = 0; i < be; i++)
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
            int be = belowSeq.size();
            for(int i = 0; i < be; i++)
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
            int ab = aboveSeq.size();
            for(int i = 0; i < ab; i++)
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
            int be = belowSeq.size();
            for(int i = 0; i < be; i++)
            {
                preTime += (tmp - belowSeq[i]) * moveTime;
                tmp = belowSeq[i];
                preTime += waitTime;
            }
            int ab = aboveSeq.size();
            for(int i = 0; i < ab; i++)
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
    else if(deslevel < curLevel)
    {
        belowSeq.push_back(deslevel);
        sort(belowSeq.begin(), belowSeq.end(), greater<int>());
    }
}

/** 电梯驱动函数
 * 驱动某台电梯以下面三种算法之一运行
 * mode = 0: BUS
 * mode = 1: SCAN
 * mode = 2: LOOK
*/
void Elevator::run()
{
    if(mode==0)
        BusRunner();
    else if(mode==1)
        ScanRunner();
    else
        LookRunner();
}

/** 等待乘客进出 */
void Elevator::waiting()
{
    while (!outEle[curLevel].empty())  //乘客出
    {
        Passenger psg = outEle[curLevel].back();
        emit signal_out(ID, psg.getID());

        outEle[curLevel].pop_back();
        psg.setCosTime();
        servedPsg.push_back(psg);

        curCount--;
        curWeight -= psg.getWeight();
        servedNum++;

        emit signal_complete(ID, psg.getID());
        emit signal_add_one_completed_passenger();
    }

    while (!inEle[curLevel].empty())  //乘客进
    {
        Passenger psg = inEle[curLevel].back();
        //未超载或人数未满则可以进
        if(numLim >= curCount + 1 && weightLim - curWeight >= psg.getWeight())
        {
            emit signal_in(ID, psg.getID());

            curCount++;
            curWeight = curWeight + psg.getWeight();
            inEle[curLevel].pop_back();
            int Des = psg.getDes();
            outEle[Des].push_back(psg);
            findRequest(Des);

        }
        else break;

    }

    Sleep(waitTime * 1000);  //乘客进出共需waitTime秒
}

/** 两个楼层间移动 */
void Elevator::move(int level) {

    bool movDir = false; //默认向下
    if(curLevel <= level) movDir = true; //向上

    while(curLevel != level)
    {
        if(movDir)
        {
            int temp = curLevel;
            if(curLevel == 0)
                temp = -1;
            
            int uptemp = curLevel + 1;
            if(uptemp == 0)
                uptemp = -1;
           
            emit signal_move(ID, temp, uptemp);

            curLevel++;
            Sleep(moveTime * 1000); //电梯移动需moveTime秒
            emit signal_arrive_floor(ID, curLevel - 1, uptemp);
        }

        else
        {
            int temp = curLevel;
            if(curLevel == 0)
                temp = -1;

            int downtemp = curLevel - 1;
            if(downtemp == 0)
                downtemp = -1;
            
            emit signal_move(ID, temp, downtemp);

            curLevel--;
            Sleep(moveTime * 1000);
            emit signal_arrive_floor(ID, curLevel + 1, downtemp);
        }
    }



    return;
}

/** 设置该电梯服务楼层 */
void Elevator::setServeLevel(vector<int> num)
{
    int nu = num.size();
    for(int i = 0; i < nu; i++)
        serveLevels[num[i]] = true;
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

/** 电梯调度算法 - 基准算法 BUS
 * 从-1 层一直到最高层（20 层），每一层都停，并且开门，让乘客进出，然后关门，继续向上走。直到最高层，再向下。
*/
void Elevator:: BusRunner()
{
    emit signal_run(ID);
    if(getmoveDirect() == 0)
    {
        setmoveDirect(1);
        emit signal_up(ID);
    }

    while(TRUE)
    {
        if(getmoveDirect() == 1)
        {
            waiting();
            move(getcurLevel() + 1);
            if(getcurLevel() == 20)
            {
                setmoveDirect(-1);
                emit signal_down(ID);
            }
        }
        else if(getmoveDirect() == -1)
        {
            waiting();
            move(getcurLevel() - 1);
            if(getcurLevel() == 0)
            {
                setmoveDirect(1);
                emit signal_up(ID);
            }
        }
    }
}

/** 电梯调度算法 - SCAN
 * 对 BUS 算法的改进，电梯在所有楼层之间来回扫描运行，但只停靠有请求的或乘客目标楼层。
*/
void Elevator:: ScanRunner()
{
    emit signal_run(ID);
    if(getmoveDirect() == 0)
    {
        setmoveDirect(1);
        emit signal_up(ID);
    }

    while(TRUE)
    {
        int curLevel = getcurLevel();
        if(getmoveDirect() == 1)
        {
            if(!inEle[curLevel].empty() || !outEle[curLevel].empty())
                waiting();
            move(curLevel + 1);
            if(getcurLevel() == 20)
            {
                setmoveDirect(-1);
                emit signal_down(ID);
            }
        }
        else if(getmoveDirect() == -1)
        {
            if(!inEle[curLevel].empty() || !outEle[curLevel].empty())
                waiting();
            move(curLevel - 1);
            if(getcurLevel() == 0)
            {
                setmoveDirect(1);
                emit signal_up(ID);
            }
        }
    }
}

/** 电梯调度算法 - LOOK
 * 改进的 C-SCAN 算法，电梯在楼层之间来回运行，但若当前运行方向无请求或乘客目标楼层则立即折返。
*/
void Elevator:: LookRunner()
{
    emit signal_run(ID);
    while(TRUE)
    {
        //当前无请求则回 1th 层
        if(getmoveDirect() != 0 && AbSeqEmp() && BeSeqEmp())
        {
            move(1);
            setmoveDirect(0); //恢复静止
            emit signal_static(ID);
        }
        //当前静止在1层
        if(getmoveDirect() == 0)
        {
            if (!BeSeqEmp())
            {
                setmoveDirect(-1);
                emit signal_down(ID);
            }//开始向下
            else if(!AbSeqEmp())
            {
                setmoveDirect(1);
                emit signal_up(ID);
            }//开始向上

        }
        //当前向上
        if(getmoveDirect() == 1)
        {
            while(!AbSeqEmp())   //处理上层乘客请求
            {
                move(getAbHead());
                waiting();
            }
            setmoveDirect(-1);         //开始向下
            emit signal_down(ID);
        }
        //当前向下
        if(getmoveDirect() == -1)
        {
            while(!BeSeqEmp())  //处理下层乘客请求
            {
                move(getBeHead());
                waiting();
            }
            setmoveDirect(1);         //开始向上
            emit signal_up(ID);
        }
    }
}