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
/*	
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
*/

	//现在的从15.0两分的形式搜索
	//用这个的时候得将initialRatio设置成0.0
	for (int i = 1; i < _desCityList.size(); ++i)
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
						//只找时间不一样的话
						//就算时间不一样，还可以找同时间不同路的
						if (cmpNum - solnNum > 0)
						{	
							ratioSoln = cmpSoln;
							cout << "!!!!!!!!!find a route with time " << ratioSoln[ratioSoln.size() - 1]->getSolnTime() << endl;
							cout << "now stop the search for city" << i << endl;
							cout << endl;
							cout << endl;
							findSoln = true;
							break;
						}
						else 
						{
							cout << "!!!!!!!!!also find a route with time " << cmpNum << endl;
							OperBlock * RS_final_OperBlock = ratioSoln[ratioSoln.size() - 1];
							OperBlock * CS_final_OperBlock = cmpSoln[cmpSoln.size() - 1];
							bool is_same = true;
							OperBlock * RS_front = RS_final_OperBlock->getFront();
							OperBlock * CS_front = CS_final_OperBlock->getFront();
							while(RS_front->getSolnTime() != Util::startTime_BFS)
							{
								if (RS_front->getX() != CS_front->getX() || RS_front->getY() != CS_front->getY() || RS_front->getSolnTime() != CS_front->getSolnTime())
								{
									is_same = false;
									break;
								}
								RS_front = RS_front->getFront();
								CS_front = CS_front->getFront();
							}
							if (!is_same)
							{
								ratioSoln = cmpSoln;
								cout << "!!!!!!!!!find a route with same time " << ratioSoln[ratioSoln.size() - 1]->getSolnTime() << " but use another route" <<endl;
								cout << "now stop the search for city" << i << endl;
								cout << endl;
								cout << endl;
								findSoln = true;
								break;
							}
							else
							{
								cout << "but also use the same route"  << endl;
							}
						}

						//太大了就没必要换路径了
						/*if (windratio > 14.5 && cmpNum - solnNum > 480)
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
						}*/
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
	


	//zhoulei debug
	//OperBlock* opb1 = new OperBlock(_blockList[0], 9, 30);
	//opb1->setSolnTime(9*60+30);//9:30
	//OperBlock* opb2 = new OperBlock(_blockList[1], 10, 4);
	//opb2->setSolnTime(10*60+4);//10:04
	//_soln.push_back(opb1);
	//_soln.push_back(opb2);

}

void PathSolver::solve_allR_fixWind_changeNum()
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
			ratioSoln=bfs->solve_allR_singleTarget(_desCityList[i]->getBlock(), NumOf_littleWind,Util ::initRatio_forAllR);
		}
		_desCityList[i]->setSoln(ratioSoln);
	}
	else
	{
		_desCityList[i]->setSoln(_multiSoln[i - 1]);//#city = 11; #soln = 10
	}

	}


}

void PathSolver::solve_allR_fixNum_changeWind()
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
			double windforAllR = Util::initRatio_forAllR - 0.5;
			vector<OperBlock *> ratioSoln;
			while (ratioSoln.empty())
			{
				windforAllR += 0.5;
				cout << "!!!!!!!!!Let's start to allow " << windforAllR << endl;
				cout << "!!!!!!!!!Let's start to allow " << windforAllR << " for city" << i << "(" << _desCityList[i]->getBlock()->getX() << "," << _desCityList[i]->getBlock()->getY() << ")!!!!!!!!!" << endl;
				cout << "!!!!!!!!!Let's start to allow " << windforAllR << endl;
				OperBlock * nullOper = NULL;
				for (auto & block : _blockList)
				{
					block->setSituation(0);
					block->setViolations(0);
					block->setMyOperBlock(nullOper);
				}
				ratioSoln = bfs->solve_allR_singleTarget(_desCityList[i]->getBlock(), Util::NumOf_littleWindForAllR, windforAllR);
			}
			_desCityList[i]->setSoln(ratioSoln);
		}
		else
		{
			_desCityList[i]->setSoln(_multiSoln[i - 1]);//#city = 11; #soln = 10
		}

	}


}

void PathSolver::solve_valued_by_allR()
{
	for (auto & block : _blockList)
	{
		block->setSituation(0);
		block->setViolations(0);
	}

	BFS * bfs = new BFS(_origin);

	_multiSoln = bfs->solve_multiTarget_valued_by_allR();

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
				ratioSoln=bfs->solve_singleTarget_valued_by_allR(_desCityList[i]->getBlock(), windratio);
			}
				_desCityList[i]->setSoln(ratioSoln);
		}
		else
		{
		_desCityList[i]->setSoln(_multiSoln[i - 1]);//#city = 11; #soln = 10
		}
	}
	

}
