#include "PathSolver.h"

PathSolver::PathSolver(Block * origin, Block* destination, vector<Block*> blocks):
	_origin(origin),_destination(destination),_blockList(blocks)
{
	cout << ">>> PathSolver create:" << endl;
	cout << "\tfrom:\t";
	_origin->print();
	cout << "\tto:\t";
	_destination->print();
	cout << endl;
}

PathSolver::PathSolver(Block * origin, vector<City*> desCities, vector<Block*> blocks):
	_origin(origin), _desCityList(desCities), _blockList(blocks)
{
	_destination = NULL;
	cout << ">>> PathSolver create:" << endl;
	cout << "\tfrom:\t";
	_origin->print();
	cout << "\tto:\t";
	for (auto&city:_desCityList)
	{
		city->print();
	}
	cout << endl;
}

void PathSolver::solve()
{	
	for (auto & block : _blockList)
	{
		block->setSituation(0);
		block->setViolations(0);
	}

	BFS* bfs = new BFS(_origin);//should be origin and destination!!
	//_soln = bfs->solve_by_anyCases_singleTarget(_destination);
	//sort(_soln.begin(), _soln.end(), OperBlock::cmpBySolnTime);

	_multiSoln = bfs->solve_by_anyCases_multiTarget();

	//之前的从下往上的约束搜索
	
	for (int i = 1; i < _desCityList.size(); ++i)
	{	
		if (_multiSoln[i - 1].empty())
		{	
			//double windratio = 14.5;
			double windratio = Util::initRatio;
			vector<OperBlock *> ratioSoln;
			while (ratioSoln.empty())
			{	
				windratio += 0.5;
				cout << "!!!!!!!!!Let's start to allow " << windratio << endl;
				cout << "!!!!!!!!!Let's start to allow "<<windratio<<" for city" << i << "(" << _desCityList[i]->getBlock()->getX() << "," << _desCityList[i]->getBlock()->getY() << ")!!!!!!!!!" << endl;
				cout << "!!!!!!!!!Let's start to allow " << windratio << endl;
				OperBlock * nullOper = NULL;
				for (auto & block : _blockList)
				{						
					block->setSituation(0);
					block->setViolations(0);
					block->setMyOperBlock(nullOper);
				}
				ratioSoln=bfs->solve_allow_windRatio_singleTarget(_desCityList[i]->getBlock(), windratio);
			}
			_desCityList[i]->setSoln(ratioSoln);
		}
		else
		{
			_desCityList[i]->setSoln(_multiSoln[i - 1]);//#city = 11; #soln = 10
		}
		
	}


	//现在的从15.0两分的形式搜索
/*	for (int i = 1; i < _desCityList.size(); ++i)
	{
		if (_multiSoln[i - 1].empty())
		{
			double windratio = 15.2;
			vector<OperBlock *> ratioSoln;
			int solnNum;
			bool findSoln = false;
			while (!findSoln)
			{
				windratio -=0.2;
				cout << "!!!!!!!!! " << windratio << endl;
				cout << "!!!!!!!!!Let's start to allow " << windratio << " for city" << i << "(" << _desCityList[i]->getBlock()->getX() << "," << _desCityList[i]->getBlock()->getY() << ")!!!!!!!!!" << endl;
				cout << "!!!!!!!!! " << windratio << endl;
				OperBlock * nullOper = NULL;
				for (auto & block : _blockList)
				{
					block->setSituation(0);
					block->setMyOperBlock(nullOper);
				}
				if (ratioSoln.empty())
				{
					ratioSoln = bfs->solve_allow_windRatio_singleTarget(_desCityList[i]->getBlock(), windratio);
					if (ratioSoln.empty())
					{
						break;
					}
					else
					{
						solnNum = ratioSoln[ratioSoln.size() - 1]->getSolnTime();
						cout << "!!!!!!!!!find a route with time " << solnNum << endl;
					}				
				}
				else
				{
					vector<OperBlock *> cmpSoln;
					cmpSoln = bfs->solve_allow_windRatio_singleTarget(_desCityList[i]->getBlock(), windratio);
					if (cmpSoln.empty())
					{							
						cout << "now stop the search for city" << i << endl;
						cout << endl;
						cout << endl;
						findSoln = true;
						break;
					}
					else
					{
						int cmpNum = cmpSoln[cmpSoln.size() - 1]->getSolnTime();
						//太大了就没必要换路径了
						if (windratio > 14.5 && cmpNum - solnNum > 480)
						{
							cout << "!!!!!!!!!that's too much drop with " << cmpNum - solnNum << endl;
							cout << "now stop the search for city" << i << endl;
							cout << endl;
							cout << endl;
							findSoln = true;
							break;
						}
						else if (windratio > 14.5 && cmpNum - solnNum <= 480)
						{
							ratioSoln = cmpSoln;
							solnNum = cmpNum;
							cout << "!!!!!!!!!find a route with time " << solnNum << endl;
						}
						else if (windratio > 13.7 && cmpNum - solnNum > 360)
						{	
							cout << "!!!!!!!!!that's too much drop with " << cmpNum - solnNum << endl;
							cout << "now stop the search for city" << i << endl;
							cout << endl;
							cout << endl;
							findSoln = true;
							break;
						}
						else if (windratio > 13.7 && cmpNum - solnNum <= 360)
						{
							ratioSoln = cmpSoln;
							solnNum = cmpNum;
							cout << "!!!!!!!!!find a route with time " << solnNum << endl;
						}
						else if (windratio > 12.3 && cmpNum - solnNum > 240)
						{	
							cout << "!!!!!!!!!that's too much drop with " << cmpNum - solnNum << endl;
							cout << "now stop the search for city" << i << endl;
							cout << endl;
							cout << endl;
							findSoln = true;
							break;
						}
						else if (windratio > 12.3 && cmpNum - solnNum <= 240)
						{
							ratioSoln = cmpSoln;
							solnNum = cmpNum;
							cout << "!!!!!!!!!find a route with time " << solnNum << endl;
						}
						else if (windratio > 10.0)
						{
							findSoln = true;
							break;
						}
					}
				}
				if (ratioSoln.empty())
				{
					break;
				}
			}

			while (ratioSoln.empty())
			{
				windratio += 0.5;
				cout << "!!!!!!!!!Let's start to allow " << windratio << endl;
				cout << "!!!!!!!!!Let's start to allow " << windratio << " fo city" << i << "(" << _desCityList[i]->getBlock()->getX() << "," << _desCityList[i]->getBlock()->getY() << ")!!!!!!!!!" << endl;
				cout << "!!!!!!!!!Let's start to allow " << windratio << endl;
				OperBlock * nullOper = NULL;
				for (auto & block : _blockList)
				{
					block->setSituation(0);
					block->setMyOperBlock(nullOper);
				}
				ratioSoln = bfs->solve_allow_windRatio_singleTarget(_desCityList[i]->getBlock(), windratio);
			}
			_desCityList[i]->setSoln(ratioSoln);
		}
		else
		{
			_desCityList[i]->setSoln(_multiSoln[i - 1]);//#city = 11; #soln = 10
		}

	}
	
*/

	//zhoulei debug
	//OperBlock* opb1 = new OperBlock(_blockList[0], 9, 30);
	//opb1->setSolnTime(9*60+30);//9:30
	//OperBlock* opb2 = new OperBlock(_blockList[1], 10, 4);
	//opb2->setSolnTime(10*60+4);//10:04
	//_soln.push_back(opb1);
	//_soln.push_back(opb2);

}

void PathSolver::solve_allR()
{
	for (auto & block : _blockList)
	{
		block->setSituation(0);
		block->setViolations(0);
	}

	BFS* bfs = new BFS(_origin);//should be origin and destination!!
								//_soln = bfs->solve_by_anyCases_singleTarget(_destination);
								//sort(_soln.begin(), _soln.end(), OperBlock::cmpBySolnTime);

	_multiSoln = bfs->solve_allR_multiTarget();

	//之前的从下往上的约束搜索
	for (int i = 1; i < _desCityList.size(); ++i)
	{
	if (_multiSoln[i - 1].empty())
	{
		int NumOf_littleWind = Util::NumOf_littleWindForAllR;
		vector<OperBlock *> ratioSoln;
		while (ratioSoln.empty())
		{
			NumOf_littleWind -= 1;
			cout << "!!!!!!!!!Let's start to allow " << NumOf_littleWind << endl;
			cout << "!!!!!!!!!Let's start to allow "<< NumOf_littleWind <<" for city" << i << "(" << _desCityList[i]->getBlock()->getX() << "," << _desCityList[i]->getBlock()->getY() << ")!!!!!!!!!" << endl;
			cout << "!!!!!!!!!Let's start to allow " << NumOf_littleWind << endl;
			OperBlock * nullOper = NULL;
			for (auto & block : _blockList)
			{
				block->setSituation(0);
				block->setViolations(0);
				block->setMyOperBlock(nullOper);
			}
			ratioSoln=bfs->solve_allR_singleTarget(_desCityList[i]->getBlock(), NumOf_littleWind);
		}
		_desCityList[i]->setSoln(ratioSoln);
	}
	else
	{
		_desCityList[i]->setSoln(_multiSoln[i - 1]);//#city = 11; #soln = 10
	}

	}


}
