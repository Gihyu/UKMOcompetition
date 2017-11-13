#pragma once
#include"Util.h"
#include"Block.h"
class City
{
private:
	int _id;
	static int _count;

	int _no;

	Block* _block;

public:
	City(int no,Block* block);

	Block* getBlock() { return _block; }

	void print();
};

