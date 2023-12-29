#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include<vector>

//Elevator
vector<vector<int>> serveList;
vector<int> e1Levels = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20 };
vector<int> e2Levels = { 1,2,3,4,5,6,7,8,9,10 };
vector<int> e3Levels = { 0,1,2,3,4,5,6,7,8,9,10 };
vector<int> e4Levels = { 0,1,11,12,13,14,15,16,17,18,19,20 };

int numLimit[4] = { 10,10,20,20 };
int weightLimit[4] = { 800,800,1600,2000 };
int initTime[4] = {10, 4, 5, 7};
int waitingTime = 5;
int movingTime = 1;
#endif /* CONFIGURATION_H */