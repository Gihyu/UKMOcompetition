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
	//用这个的时候得将initialRatio设置成0.0
	//for (int i = 1; i < _desCityList.size(); ++i)
	//{
	//	if (_multiSoln[i - 1].empty())
	//	{
	//		double windratio = 15.2;
	//		vector<OperBlock *> ratioSoln;
	//		int solnNum;
	//		bool findSoln = false;
	//		while (!findSoln)
	//		{
	//			windratio -=0.2;
	//			cout << "!!!!!!!!! " << windratio << endl;
	//			cout << "!!!!!!!!!Let's start to allow " << windratio << " for city" << i << "(" << _desCityList[i]->getBlock()->getX() << "," << _desCityList[i]->getBlock()->getY() << ")!!!!!!!!!" << endl;
	//			cout << "!!!!!!!!! " << windratio << endl;
	//			OperBlock * nullOper = NULL;
	//			for (auto & block : _blockList)
	//			{
	//				block->setSituation(0);
	//				block->setMyOperBlock(nullOper);
	//			}
	//			if (ratioSoln.empty())
	//			{
	//				ratioSoln = bfs->solve_allow_windRatio_singleTarget(_desCityList[i]->getBlock(), windratio);
	//				if (ratioSoln.empty())
	//				{
	//					break;
	//				}
	//				else
	//				{
	//					solnNum = ratioSoln[ratioSoln.size() - 1]->getSolnTime();
	//					cout << "!!!!!!!!!find a route with time " << solnNum << endl;
	//				}				
	//			}
	//			else
	//			{
	//				vector<OperBlock *> cmpSoln;
	//				cmpSoln = bfs->solve_allow_windRatio_singleTarget(_desCityList[i]->getBlock(), windratio);
	//				if (cmpSoln.empty())
	//				{							
	//					cout << "now stop the search for city" << i << endl;
	//					cout << endl;
	//					cout << endl;
	//					findSoln = true;
	//					break;
	//				}
	//				else
	//				{
	//					int cmpNum = cmpSoln[cmpSoln.size() - 1]->getSolnTime();
	//					//只找时间不一样的话
	//					//就算时间不一样，还可以找同时间不同路的
	//					if (cmpNum - solnNum > 0)
	//					{	
	//						ratioSoln = cmpSoln;
	//						cout << "!!!!!!!!!find a route with time " << ratioSoln[ratioSoln.size() - 1]->getSolnTime() << endl;
	//						cout << "now stop the search for city" << i << endl;
	//						cout << endl;
	//						cout << endl;
	//						findSoln = true;
	//						break;
	//					}
	//					else 
	//					{
	//						cout << "!!!!!!!!!also find a route with time " << cmpNum << endl;
	//						OperBlock * RS_final_OperBlock = ratioSoln[ratioSoln.size() - 1];
	//						OperBlock * CS_final_OperBlock = cmpSoln[cmpSoln.size() - 1];
	//						bool is_same = true;
	//						OperBlock * RS_front = RS_final_OperBlock->getFront();
	//						OperBlock * CS_front = CS_final_OperBlock->getFront();
	//						while(RS_front->getSolnTime() != Util::startTime_BFS)
	//						{
	//							if (RS_front->getX() != CS_front->getX() || RS_front->getY() != CS_front->getY() || RS_front->getSolnTime() != CS_front->getSolnTime())
	//							{
	//								is_same = false;
	//								break;
	//							}
	//							RS_front = RS_front->getFront();
	//							CS_front = CS_front->getFront();
	//						}
	//						if (!is_same)
	//						{
	//							ratioSoln = cmpSoln;
	//							cout << "!!!!!!!!!find a route with same time " << ratioSoln[ratioSoln.size() - 1]->getSolnTime() << " but use another route" <<endl;
	//							cout << "now stop the search for city" << i << endl;
	//							cout << endl;
	//							cout << endl;
	//							findSoln = true;
	//							break;
	//						}
	//						else
	//						{
	//							cout << "but also use the same route"  << endl;
	//						}
	//					}

	//					//太大了就没必要换路径了
	//					/*if (windratio > 14.5 && cmpNum - solnNum > 480)
	//					{
	//						cout << "!!!!!!!!!that's too much drop with " << cmpNum - solnNum << endl;
	//						cout << "now stop the search for city" << i << endl;
	//						cout << endl;
	//						cout << endl;
	//						findSoln = true;
	//						break;
	//					}
	//					else if (windratio > 14.5 && cmpNum - solnNum <= 480)
	//					{
	//						ratioSoln = cmpSoln;
	//						solnNum = cmpNum;
	//						cout << "!!!!!!!!!find a route with time " << solnNum << endl;
	//					}
	//					else if (windratio > 13.7 && cmpNum - solnNum > 360)
	//					{	
	//						cout << "!!!!!!!!!that's too much drop with " << cmpNum - solnNum << endl;
	//						cout << "now stop the search for city" << i << endl;
	//						cout << endl;
	//						cout << endl;
	//						findSoln = true;
	//						break;
	//					}
	//					else if (windratio > 13.7 && cmpNum - solnNum <= 360)
	//					{
	//						ratioSoln = cmpSoln;
	//						solnNum = cmpNum;
	//						cout << "!!!!!!!!!find a route with time " << solnNum << endl;
	//					}
	//					else if (windratio > 12.3 && cmpNum - solnNum > 240)
	//					{	
	//						cout << "!!!!!!!!!that's too much drop with " << cmpNum - solnNum << endl;
	//						cout << "now stop the search for city" << i << endl;
	//						cout << endl;
	//						cout << endl;
	//						findSoln = true;
	//						break;
	//					}
	//					else if (windratio > 12.3 && cmpNum - solnNum <= 240)
	//					{
	//						ratioSoln = cmpSoln;
	//						solnNum = cmpNum;
	//						cout << "!!!!!!!!!find a route with time " << solnNum << endl;
	//					}
	//					else if (windratio > 10.0)
	//					{
	//						findSoln = true;
	//						break;
	//					}*/
	//				}
	//			}
	//			if (ratioSoln.empty())
	//			{
	//				break;
	//			}
	//		}

	//		while (ratioSoln.empty())
	//		{
	//			windratio += 0.5;
	//			cout << "!!!!!!!!!Let's start to allow " << windratio << endl;
	//			cout << "!!!!!!!!!Let's start to allow " << windratio << " fo city" << i << "(" << _desCityList[i]->getBlock()->getX() << "," << _desCityList[i]->getBlock()->getY() << ")!!!!!!!!!" << endl;
	//			cout << "!!!!!!!!!Let's start to allow " << windratio << endl;
	//			OperBlock * nullOper = NULL;
	//			for (auto & block : _blockList)
	//			{
	//				block->setSituation(0);
	//				block->setMyOperBlock(nullOper);
	//			}
	//			ratioSoln = bfs->solve_allow_windRatio_singleTarget(_desCityList[i]->getBlock(), windratio);
	//		}
	//		_desCityList[i]->setSoln(ratioSoln);
	//	}
	//	else
	//	{
	//		_desCityList[i]->setSoln(_multiSoln[i - 1]);//#city = 11; #soln = 10
	//	}

	//}
	


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

	//开挂版 不用_origin
	//_desCityList[2]->getBlock()
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
			if (NumOf_littleWind == 0)
			{
				break;
			}
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
			
			//ratioSoln = bfs->solve_allR_singleTarget(getBlockByCoordinate(315,371), NumOf_littleWind, Util::initRatio_forAllR);
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


void PathSolver::solve_allR_justAvg()
{
	for (auto & block : _blockList)
	{
		block->setSituation(0);
		block->setViolations(0);
	}

	BFS* bfs = new BFS(_origin);//should be origin and destination!!
								//_soln = bfs->solve_by_anyCases_singleTarget(_destination);
								//sort(_soln.begin(), _soln.end(), OperBlock::cmpBySolnTime);

	_multiSoln = bfs->solve_allRjustAvg_multi();

	//之前的从下往上的约束搜索

	for (int i = 1; i < _desCityList.size(); ++i)
	{
		if (_multiSoln[i - 1].empty())
		{
			//double windratio = 14.5;
			double windratio = Util::allRjustAvgRatio;
			vector<OperBlock *> ratioSoln;
			while (ratioSoln.empty())
			{	
				if (windratio < 15)
				{
					windratio += 0.05;
				}
				else
				{
					windratio += 0.5;
				}
				cout << "!!!!!!!!!Let's start to allow " << windratio << endl;
				cout << "!!!!!!!!!Let's start to allow " << windratio << " for city" << i << "(" << _desCityList[i]->getBlock()->getX() << "," << _desCityList[i]->getBlock()->getY() << ")!!!!!!!!!" << endl;
				cout << "!!!!!!!!!Let's start to allow " << windratio << endl;
				OperBlock * nullOper = NULL;
				for (auto & block : _blockList)
				{
					block->setSituation(0);
					block->setViolations(0);
					block->setMyOperBlock(nullOper);
				}
				ratioSoln = bfs->solve_allRjustAvg_single(_desCityList[i]->getBlock(), windratio);
			}
			_desCityList[i]->setSoln(ratioSoln);
		}
		else
		{
			_desCityList[i]->setSoln(_multiSoln[i - 1]);//#city = 11; #soln = 10
		}

	}
}

void PathSolver::solve_allR_backtrack()
{
	for (auto & block : _blockList)
	{
		block->setSituation(0);
		block->setViolations(0);
	}

	//_origin
	//_desCityList[2]->getBlock()
	BFS* bfs = new BFS(_origin);
	
	/*double wind[11];
	wind[1] =14.15 ;
	wind[2] = 13.35;
	wind[3] = 16.3;
	wind[4] =17.3 ;
	wind[5] = 17.3;
	wind[6] = 17.3;
	wind[7] = 17.3;
	wind[8] = 17.3;
	wind[9] = 13.35;
	wind[10] = 17.3;*/


	for (int i = 1; i < _desCityList.size(); ++i)
	{
		int NumOf_littleWind = Util::NumOf_littleWindForAllR;
		double allRwind = Util::initRatio_forAllR;
		vector<OperBlock *> ratioSoln;
		while (ratioSoln.empty())
		{	
			//allRwind = wind[i];


			if (NumOf_littleWind == 0)
			{
				break;
			}
			NumOf_littleWind -= 1;

			/*if (allRwind < 15.0)
			{
				allRwind += 0.25;
			}
			else
			{	
				if (allRwind > 16.0)
				{
					break;
				}
				allRwind += 1.0;
			}*/

			cout << "!!!!!!!!!Let's start to allow " << allRwind <<" and allow vote "<< NumOf_littleWind<< endl;
			cout << "!!!!!!!!!Let's start to allow " << allRwind << " for city" << i << "(" << _desCityList[i]->getBlock()->getX() << "," << _desCityList[i]->getBlock()->getY() << ")!!!!!!!!!" << endl;
			cout << "!!!!!!!!!Let's start to allow " << allRwind << " and allow vote " << NumOf_littleWind << endl;
			OperBlock * nullOper = NULL;
			for (auto & block : _blockList)
			{
				block->setSituation(0);
				block->setViolations(0);
				block->setMyOperBlock(nullOper);
			}
			ratioSoln = bfs->solve_backtrack_allR(_desCityList[i]->getBlock(), NumOf_littleWind, allRwind);

		}
		_desCityList[i]->setSoln(ratioSoln);
	}

}

void PathSolver::solve_CBNT_backtrack()
{
	for (auto & block : _blockList)
	{
		block->setSituation(0);
		block->setViolations(0);
	}

	BFS* bfs = new BFS(_origin);

	for (int i = 1; i < _desCityList.size(); ++i)
	{
		int NumOf_littleWind = Util::NumOf_littleWindForAllR;
		double allRwind = Util::initRatio_forAllR;
		vector<OperBlock *> ratioSoln;
		while (ratioSoln.empty())
		{
			/*if (NumOf_littleWind == 0)
			{
				break;
			}
			NumOf_littleWind -= 1;*/

			if (allRwind < 15.0)
			{
			allRwind += 0.05;
			}
			else 
			{
			if (allRwind > 15.2)
			{
				break;
			}
			allRwind += 0.1;
			}

			cout << "!!!!!!!!!Let's start to allow " << allRwind << " and allow vote " << NumOf_littleWind << endl;
			cout << "!!!!!!!!!Let's start to allow " << allRwind << " for city" << i << "(" << _desCityList[i]->getBlock()->getX() << "," << _desCityList[i]->getBlock()->getY() << ")!!!!!!!!!" << endl;
			cout << "!!!!!!!!!Let's start to allow " << allRwind << " and allow vote " << NumOf_littleWind << endl;
			OperBlock * nullOper = NULL;
			for (auto & block : _blockList)
			{
				block->setSituation(0);
				block->setViolations(0);
				block->setMyOperBlock(nullOper);
			}
			ratioSoln = bfs->solve_backtrack_CBNT(_desCityList[i]->getBlock(), NumOf_littleWind, allRwind);

		}
		_desCityList[i]->setSoln(ratioSoln);
	}
}


void PathSolver::solve_singleModel_evabyallR_backtrack()
{
	for (auto & block : _blockList)
	{
		block->setSituation(0);
		block->setViolations(0);
	}

	//_origin
	//_desCityList[2]->getBlock()
	BFS* bfs = new BFS(_origin);


	for (int i = 1; i < _desCityList.size(); ++i)
	{
		int NumOf_littleWind = Util::NumOf_littleWindForAllR;
		double allRwind = Util::initRatio_forAllR;
		double singleWind = Util::initRatio;
		vector<OperBlock *> ratioSoln;
		while (ratioSoln.empty())
		{
			if (singleWind < 15.0)
			{
				singleWind += 0.05;
			}
			else
			{	
				if (singleWind > 16.0)
				{	
					cout << "break!" << endl;
					cout << endl;
					break;
				}
				singleWind += 0.5;
			}
			cout << "!!!!!!!!!Let's start to allow " << singleWind << " and allow vote " << NumOf_littleWind << " for city" << i << "(" << _desCityList[i]->getBlock()->getX() << "," << _desCityList[i]->getBlock()->getY() << ")!!!!!!!!!" << endl;
			OperBlock * nullOper = NULL;
			for (auto & block : _blockList)
			{
				block->setSituation(0);
				block->setViolations(0);
				block->setMyOperBlock(nullOper);
			}
			ratioSoln = bfs->solve_backtrack_singleModel_evabyallR(_desCityList[i]->getBlock(), NumOf_littleWind, allRwind,singleWind);

		}
		_desCityList[i]->setSoln(ratioSoln);
	}

}


// season 2 

void PathSolver::solve_backtrack_single_rain()
{
	for (auto & block : _blockList)
	{
		block->setSituation(0);
		block->setViolations(0);
	}

	BFS* bfs = new BFS(_origin);

	int start[11];
	start[1] = 210;
	start[2] = 220;
	start[3] = 230;
	start[4] = 240;
	start[5] = 250;
	start[6] = 260;
	start[7] = 270;
	start[8] = 280;
	start[9] = 290;
	start[10] = 300;


	for (int i = 1; i < _desCityList.size(); ++i)
	{
		int NumOf_littleWind = Util::NumOf_littleWindForAllR;
		double allRwind = Util::initRatio_forAllR;
		vector<OperBlock *> ratioSoln;
		while (ratioSoln.empty())
		{
			Util::startTime_BFS = start[i];

			cout << "!!!!!!!!!Let's start to allow wind " << Util::singleWindratio << " and allow rain " << Util::singleRainratio << endl;
			cout << "!!!!!!!!!Let's start to allow wind " << Util::singleWindratio << " and allow rain " << Util::singleRainratio << " for city" << i << "(" << _desCityList[i]->getBlock()->getX() << "," << _desCityList[i]->getBlock()->getY() << ")!!!!!!!!!" << endl;
			cout << "!!!!!!!!!Let's start to allow wind " << Util::singleWindratio << " and allow rain " << Util::singleRainratio << endl;
			OperBlock * nullOper = NULL;
			for (auto & block : _blockList)
			{
				block->setSituation(0);
				block->setViolations(0);
				block->setMyOperBlock(nullOper);
			}
			ratioSoln = bfs->solve_backtrack_single_rain(_desCityList[i]->getBlock(), Util::singleWindratio, Util::singleRainratio);

		}
		_desCityList[i]->setSoln(ratioSoln);
	}
}

void PathSolver::solve_backtrack_single_rain_logs()
{
	for (auto & block : _blockList)
	{
		block->setSituation(0);
		block->setViolations(0);
	}

	BFS* bfs = new BFS(_origin);

	int start[11];
	start[1] = 180;
	start[2] = 180;
	start[3] = 180;
	start[4] = 180;
	start[5] = 180;
	start[6] = 180;
	start[7] = 180;
	start[8] = 180;
	start[9] = 180;
	start[10] = 180;


	for (int i = 1; i < _desCityList.size(); ++i)
	{
		int NumOf_littleWind = Util::NumOf_littleWindForAllR;
		double allRwind = Util::initRatio_forAllR;
		vector<OperBlock *> ratioSoln;

		Util::startTime_BFS = start[i]-10;
		bool stopCondition = false;

		Util::singleWindratio = 14.5;
		Util::singleRainratio = 14.0;

		bool thisRatioCanFind = false;

		while (!stopCondition)
		{
			Util::startTime_BFS = Util::startTime_BFS+10;

			int absPlus = abs(_desCityList[i]->getBlock()->getX() - _desCityList[0]->getBlock()->getX()) + abs(_desCityList[i]->getBlock()->getY() - _desCityList[0]->getBlock()->getY());
			if (absPlus * 2 + Util::startTime_BFS > Util::maxTime)
			{				
				if (!thisRatioCanFind)
				{	
					cout << "plus" << endl;
					Util::startTime_BFS = 180;
					if (Util::singleWindratio < 15.0)
					{
						Util::singleWindratio = Util::singleWindratio + 0.1;
						Util::singleRainratio = Util::singleRainratio + 0.1;
					}
					else
					{
						break;
					}
					
				}
				else
				{
					stopCondition = true;
				}			
			}

			OperBlock * nullOper = NULL;
			for (auto & block : _blockList)
			{
				block->setSituation(0);
				block->setViolations(0);
				block->setMyOperBlock(nullOper);
			}
			if (bfs->solve_backtrack_single_rain_logs(_desCityList[i]->getBlock(), Util::singleWindratio, Util::singleRainratio, i))
			{
				thisRatioCanFind = true;
			}

		}


		_desCityList[i]->setSoln(ratioSoln);
	}
}



Block * PathSolver::getBlockByCoordinate(int x, int y)
 {
	int index = Schedule::getBlockIndex(x, y);
	if (index < 0)
		 {
		cout << "error:PathSolver::getBlockByCoordinate:index < 0 " << endl;
		return NULL;
		}
	return _blockList[index];
	}