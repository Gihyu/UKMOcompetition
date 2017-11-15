#pragma once
#include"Util.h"
#include"Block.h"
#include"OperBlock.h"
class City
{
private:
	int _id;
	static int _count;

	int _no;

	Block* _block;

	vector<OperBlock*> _soln;

public:
	City(int no,Block* block);

	int getNo() { return _no; }
	Block* getBlock() { return _block; }

	void setSoln(vector<OperBlock*> soln) { _soln = soln; }
	vector<OperBlock*> getSoln() { return _soln; }

	void print();
};

