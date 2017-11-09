#pragma once
#include"Util.h"
class City
{
private:
	int _id;

	int _no;

	//coordinate
	int _x;
	int _y;

public:
	City(int id,int no,int x,int y);

	void print();
};

