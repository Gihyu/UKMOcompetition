#pragma once
#include"ACO.h"

class Ant
{
	
private:
	Block * _sourceBlock;

	int _targetX;
	int _targetY;

	vector<OperBlock *> _path;

	OperBlock * _presentOperBlock;

	int routeGrade;

	bool stopCondition;

public:
	Ant();
	~Ant();

	void search();
	void ChooseNextBlcok();	

	
	void setPresentOperBlock(Block * presentLeg);
	

	vector<OperBlock *> getPath(){return _path;}

	void setBest() { routeGrade = 2; }
	void setGood() { routeGrade = 1; }
	void setBad() { routeGrade = 0; }
	int getRouteGrade() { return routeGrade; }

	bool is_in_path(Block * wantoGo);
	
	bool is_a_valid_choice(Block * wantoGo);

};