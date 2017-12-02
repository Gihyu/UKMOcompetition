#pragma once

#include<iostream>
#include<fstream>
#include<stdio.h>

#include<string>
#include<ctime>

#include <algorithm>

#include<vector>
#include<stack>
#include<array>

#include<ctime>

#include<process.h>
#include<random>


using namespace std;

//ACO
#define ANTAMOUNT 100

#define MAXSTEP 360

class Util
{
public:
	static string InputPath;
	static string OutputPath;

	static double Epsilon;//0.0001
	const static int realizationCount = 10;//10 realization
	const static int hourCount = 18;//3:00-21:00
	static int realization;//TODO

	static int flyTime;
	static int maxTime;

	static int maxIterationACO;

	//BFS
	static double initRatio;

	static int maxX;
	static int maxY;
	static int maxIndex;

	//print
	static time_t startTime;//2017.11.13 00:00
	static string getTimeStr(time_t t);
	static time_t getTime(int hour, int min);
	static time_t getTime(int min);
	static void printCurTime();

};

