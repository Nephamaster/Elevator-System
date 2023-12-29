#include "Elevator.cpp"
#include "Passenger.cpp"
#include "Controller.cpp"

using namespace std;

/** 第三阶段程序
 * -[改进的调度算法算法]
 *  实现了 扫描算法 ScanRunner 和 折返扫描算法 LookRunner
 * -[模式选择]
 *  为用户提供了三种算法模式的选择界面
 * -[测试用例]
 *  编写了 20 位乘客的测试用例，涵盖多种请求类型
 * -[性能测试]
 *  1.统计三种算法下控制系统的电梯选择方案
 *  2.每部电梯服务每位乘客所需时间以及总时间
 *  3.所用乘客从请求发出到到达目的地所花费的总时间
*/

int main()
{
    int mode;
    cout << "Please Enter the Elevator Runner:" << endl;
    cout << "BUS: 0" << endl << "SCAN: 1" << endl << "LOOK: 2" <<endl;
    cin >> mode;

    Controller *controller = new Controller(mode);
    cout << "System Init Completed." << endl;
    
    int psgNum = ID.size();
    cout << "The Number of the Passengers is: " << psgNum << endl;
    for(int i = 0; i < ID.size(); ++i)
    {
        Passenger psg = Passenger(ID[i], Wit[i], Des[i], Cur[i]);
        psg.setReqTime();
        controller->reqRecv(psg);
    }

    cout << endl << "-----------------------------------------------------------------" << endl << endl;
    
    while(TRUE)
    {
        Sleep(12000);
        int sumNum = 0;
        for(int id=0; id<4; ++id)
            sumNum += controller->elev[id]->getServedNum();
        if(sumNum == psgNum)
        {
            controller->performTest();
            break;
        }
        cout << "The Number of Served Passengers is: " << sumNum << endl;
    }
    return 0;
}