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

	static int flyTime;

	static int maxIterationACO;



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

