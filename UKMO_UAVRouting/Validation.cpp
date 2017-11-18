#include "Validation.h"

Validation::Validation(vector<OperBlock*> soln):_soln(soln)
{
}

Validation::Validation(vector<City*> cities):_cityList(cities)
{
}

double Validation::validate()
{
	double obj = 0.0;
	cout <<endl<< ">>> validation " << endl;
	for (int i = 1; i < _cityList.size(); ++i)
	{
		_cityList[i]->print();
		obj += validate(_cityList[i]->getSoln());
	}
	return obj;
}

double Validation::validate(vector<OperBlock*> soln)
{
	double obj = 0.0;
	bool crashFlag = false;
	sort(soln.begin(), soln.end(), OperBlock::cmpBySolnTime);

	int curTime = _beginTime;//9*60 mins
	int step = 2;//2mins
	Block* last = soln.front()->getBlock();
	for (auto&tmp : soln)
	{
		int tmpT = tmp->getSolnTime();
		while (curTime < tmpT)
		{
			if (last->getMeasureWind(curTime / 60) > _wind)
			{
				crashFlag = true;
				cout << "\tcrash:";
				printInfo(last, curTime);
			}
			curTime += step;
		}
		last = tmp->getBlock();
		if (last->getMeasureWind(curTime / 60) > _wind)
		{
			crashFlag = true;
			cout << "\tcrash:";
			printInfo(last, curTime);
		}
		curTime += step;
	}


	obj = soln.back()->getSolnTime()-_beginTime;
	if (crashFlag)
	{
		obj = _maxCost;
	}
	cout << "\tobj:" << obj << endl;
	return obj;
}

void Validation::printInfo(Block * block, int min)
{
	block->print();
	cout << "\t" << Util::getTimeStr(Util::getTime(min)) << "\tforecast:" << block->getWind(min/60) << "\tmeasure:" << block->getMeasureWind(min/60);
	cout << endl;
}
