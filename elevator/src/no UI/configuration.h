#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include<vector>

/** Elevator */
vector<vector<int>> serveList;
vector<int> e1Levels = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20 };
vector<int> e2Levels = { 1,2,3,4,5,6,7,8,9,10 };
vector<int> e3Levels = { 0,1,2,3,4,5,6,7,8,9,10 };
vector<int> e4Levels = { 0,1,11,12,13,14,15,16,17,18,19,20 };

int numLimit[4] = { 10,10,20,20 };
int weightLimit[4] = { 800,800,1600,2000 };
int initTime[4] = {10, 4, 5, 7};
int waitingTime = 1;
int movingTime = 0.1;

/** Passenger */
vector<int> ID  = {  1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20 };
vector<int> Wit = { 150,90,100,88,68,79,120,110,90,80,200,100,50,60,60,55,74,80,75,65 };
vector<int> Des = { -1,20,15, 8,12,11,10, 6, 9,10,20,15, 6, 7, 8, 9,17, 2,-1,19 };
vector<int> Cur = { 11, 0, 6,12,12,10,11,15,-1,20,-1, 1, 1,17,19,-1,17, 9,10,10 };
#endif /* CONFIGURATION_H */