#include "BFS.h"

BFS::BFS(Block * sourceBlock):
_sourceBlock(sourceBlock)
{

}

void BFS::clearQueue(queue<OperBlock *>& q) {
	queue<OperBlock*> empty;
	swap(empty, q);
}

vector<vector<OperBlock *>> BFS::solve_by_anyCases_multiTarget()
{
	vector<vector<OperBlock *>> multiSoln;

	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);


	while (!_ingOperBlocks.empty()  && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime << " and SolnTime is " << ingOperBlock->getSolnTime() << endl;
			int allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock(cangoto, thisTime))
				{	

				    //顺序不能乱
					//回撞sourceBlock导致getFront为NULL
					//firstReach就是注释掉

					if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
					{				
						chooseLowestWind(ingOperBlock, cangoto, thisTime,Util::initRatio);
						//chooseHighestWind(ingOperBlock, cangoto, thisTime);
					}
					else if (cangoto->getSituation() == 0)

					//if (cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						cangoto->setViolations(getNum_Violations(ingOperBlock, cangoto, thisTime, Util::initRatio));
						OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						cangotoOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(cangotoOperBlock);
						_ingOperBlocks.push(cangotoOperBlock);
					}											
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{	
				if (cangoto->getSituation() == 0 )
				{
					allCangoToOper = false;
					break;
				}
			}

			if (allCangoToOper)
			{
				_vistedOperBlocks.push_back(ingOperBlock);
				ingOperBlock->getBlock()->setSituation(2);
				_ingOperBlocks.pop();
			}
			//如果原地停留 那么仍需要判断停留的时候是否风速大于initial ratio
			//若大于，则作为坠机处理(假visted处理）
			//所以output的时候对于第一种群式搜索的时候也没关系（因为soln time 对应的风速肯定是小于initial ratio的）
			else
			{	
				if (ingOperBlock->getWind(thisTime / 60) < Util::initRatio)
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				else
				{
					_vistedOperBlocks.push_back(ingOperBlock);
					ingOperBlock->getBlock()->setSituation(2);
					_ingOperBlocks.pop();
				}
				
			}

		}
		//若cangoto为空 那么也是变为visted
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	vector<pair<int, int>> destinations;
	pair<int,int> d1(84, 203);
	pair<int, int> d2(199, 371);
	pair<int, int> d3(140, 234);
	pair<int, int> d4(236, 241);
	pair<int, int> d5(315, 281);
	pair<int, int> d6(358, 207);
	pair<int, int> d7(363, 237);
	pair<int, int> d8(423, 266);
	pair<int, int> d9(125, 375);
	pair<int, int> d10(189, 274);

	destinations.push_back(d1);
	destinations.push_back(d2);
	destinations.push_back(d3);
	destinations.push_back(d4);
	destinations.push_back(d5);
	destinations.push_back(d6);
	destinations.push_back(d7);
	destinations.push_back(d8);
	destinations.push_back(d9);
	destinations.push_back(d10);

	for (int k = 0; k < 10; k++)
	{
		int destinationX = destinations[k].first;
		int destinationY = destinations[k].second;
		vector<OperBlock * > OperRoute;
		OperBlock * front = NULL;
		// maybe some is soln but it's ingTime is 1258 so it will be in the ingBlock
		bool findTheDes = false;
		for (auto & visted : _vistedOperBlocks)
		{
			if (visted->getBlock()->getX() == destinationX && visted->getBlock()->getY() == destinationY)
			{
				OperRoute.push_back(visted);
				front = visted->getFront();
				findTheDes = true;
				break;
			}			
		}
		if (!findTheDes)
		{
			int size = _ingOperBlocks.size();
			for (int i = 1; i <= size; i++)
			{
				OperBlock * testOperBlock = _ingOperBlocks.front();
				if (destinationX == testOperBlock->getX() && destinationY == testOperBlock->getY())
				{	
					OperRoute.push_back(testOperBlock);
					front = testOperBlock->getFront();
					findTheDes = true;
					break;
				}
				_ingOperBlocks.pop();
				_ingOperBlocks.push(testOperBlock);
			}
		}
		while (front != NULL)
		{
			OperRoute.push_back(front);
			front = front->getFront();
		}
		sort(OperRoute.begin(), OperRoute.end(), OperBlock::cmpBySolnTime);
		multiSoln.push_back(OperRoute);
	}

	//print function
	for (int k = 0;k<10;k++)
	{
		if (multiSoln[k].empty())
		{
			cout << endl;
			cout << "can not reach city" << k+1 << "(" << destinations[k].first << "," << destinations[k].second << endl;
			cout << endl;
		}
		else
		{
			cout << endl;
			cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << destinations[k].first << "," << destinations[k].second << ") is :" << endl;
			cout << endl;
			cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "Wind"  << "\t" << "violations" << endl;
			for (int i =0; i<multiSoln[k].size(); i++)
			{
				int absPlus = abs(multiSoln[k][i]->getBlock()->getX() - 142) + abs(multiSoln[k][i]->getBlock()->getY() - 328);
				/*if (absPlus * 2 + 180 < multiSoln[k][i]->getSolnTime())
				{
					cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
				}*/
				if (absPlus * 2 + Util::startTime_BFS > multiSoln[k][i]->getSolnTime())
				{
					cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
				}

				//cout << "(" << multiSoln[k][i]->getBlock()->getX() << "," << multiSoln[k][i]->getBlock()->getY() << ")Solntime is" << multiSoln[k][i]->getSolnTime() << "->" ;
				if (multiSoln[k][i]->getSolnTime()!=Util::startTime_BFS)
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getWind((multiSoln[k][i]->getSolnTime()-2) / 60) << "\t" << multiSoln[k][i]->getBlock()->getViolations()<< endl;
				}
				else
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getWind(multiSoln[k][i]->getSolnTime() / 60) << "\t" << multiSoln[k][i]->getBlock()->getViolations()<< endl;
				}
			}
			cout << endl;
			cout << endl;
		}
	}

	return multiSoln;
}


vector<OperBlock *> BFS::solve_allow_windRatio_singleTarget(Block * targetBlock,double windRatio)
{
	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();

	bool findTheTarget = false;
	int targetSolnTime = Util::maxTime;


	while (!_ingOperBlocks.empty() && _ingOperBlocks.front()->getIngTime()<targetSolnTime && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			//cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime << " and SolnTime is " << ingOperBlock->getSolnTime() << endl;
			bool allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock_allow_ratio(cangoto, thisTime, windRatio))
				{
					if (cangoto->getX() == targetX && cangoto->getY() == targetY && cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						cangoto->setViolations(getNum_Violations(ingOperBlock,cangoto,thisTime,windRatio));
						OperBlock * targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						targetOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(targetOperBlock);
						_ingOperBlocks.push(targetOperBlock);
						targetSolnTime = thisTime + Util::flyTime;
						findTheTarget = true;
					}
					else
					{	

						if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
						{
							chooseLowestWind(ingOperBlock, cangoto, thisTime,windRatio);
							//chooseHighestWind(ingOperBlock, cangoto, thisTime);
						}
						else if (cangoto->getSituation() == 0)
						
						//if (cangoto->getSituation() == 0)
						{
							cangoto->setSituation(1);
							cangoto->setViolations(getNum_Violations(ingOperBlock, cangoto, thisTime, windRatio));
							OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
							cangotoOperBlock->setFront(ingOperBlock);
							cangoto->setMyOperBlock(cangotoOperBlock);
							_ingOperBlocks.push(cangotoOperBlock);
						}

					}
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{
				if (cangoto->getSituation() == 0 )
				{
					allCangoToOper = false;
					break;
				}
			}

			if (allCangoToOper)
			{
				_vistedOperBlocks.push_back(ingOperBlock);
				ingOperBlock->getBlock()->setSituation(2);
				_ingOperBlocks.pop();
			}
			//判断停留的时候是否不坠机
			else
			{
				if (ingOperBlock->getWind(thisTime / 60) < windRatio)
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				else
				{
					_vistedOperBlocks.push_back(ingOperBlock);
					ingOperBlock->getBlock()->setSituation(2);
					_ingOperBlocks.pop();
				}
				
			}

		}
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	vector<OperBlock *> OperRoute;
	if (findTheTarget)
	{
		// when print don't forget to inverted order		
		OperBlock * targetOB = targetBlock->getMyOperBlock();
		OperRoute.push_back(targetOB);
		OperBlock * front = targetOB->getFront();
		while (front != NULL)
		{
			OperRoute.push_back(front);
			front = front->getFront();
		}
	}

	sort(OperRoute.begin(), OperRoute.end(), OperBlock::cmpBySolnTime);

	//print function
	if (OperRoute.empty())
	{
		cout << " shortest path failed ! Can't find the targetBlock when allow "<<windRatio<<" !" << endl;
	}
	else
	{
		cout << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << targetBlock->getX() << "," << targetBlock->getY() << ") is :" << endl;
		cout << endl;
		cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "Wind" <<  "\t" << "violations" << endl;
		for (int i = 0; i <OperRoute.size(); i++)
		{
			int absPlus = abs(OperRoute[i]->getBlock()->getX() - 142) + abs(OperRoute[i]->getBlock()->getY() - 328);
			/*if (absPlus * 2 + 180 < OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
			}*/
			if (absPlus * 2 + Util::startTime_BFS > OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
			}

			//cout << "(" << OperRoute[i]->getBlock()->getX() << "," << OperRoute[i]->getBlock()->getY() << ")Solntime is" << OperRoute[i]->getSolnTime() << " and the wind that time is" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << "->" << endl;
			
			if (OperRoute[i]->getSolnTime()!=Util::startTime_BFS)
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getWind((OperRoute[i]->getSolnTime()-2) / 60) << "\t" << OperRoute[i]->getBlock()->getViolations()<< endl;
			}
			else
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime()  / 60) << "\t" << OperRoute[i]->getBlock()->getViolations() << endl;
			}
			
		}
		cout << endl;
		cout << endl;
	}


	return OperRoute;
}


void BFS::chooseLowestWind(OperBlock * oper, Block * blo, int thistime,double windRatio)
{	
	if((thistime + Util::flyTime) == blo->getMyOperBlock()->getSolnTime())
	{
		if (getNum_Violations(oper, blo, thistime, windRatio) < blo->getViolations())
		{
			blo->getMyOperBlock()->setFront(oper);
			blo->setViolations(getNum_Violations(oper, blo, thistime, windRatio));
		}
		else if (getNum_Violations(oper, blo, thistime, windRatio) == blo->getViolations())
		{
			if (oper->getWind(thistime / 60) < blo->getMyOperBlock()->getFront()->getWind(thistime / 60))
			{
				//cout << "--------------------------change type2------------------------" << endl;
				blo->getMyOperBlock()->setFront(oper);
			}
		}
		
	}
}


void BFS::chooseHighestWind(OperBlock * oper, Block * blo, int thistime,double windRatio)
{
	if ((thistime + Util::flyTime) == blo->getMyOperBlock()->getSolnTime())
	{
		if (getNum_Violations(oper, blo, thistime, windRatio) < blo->getViolations())
		{
			blo->getMyOperBlock()->setFront(oper);
			blo->setViolations(getNum_Violations(oper, blo, thistime, windRatio));
		}
		else if (getNum_Violations(oper, blo, thistime, windRatio) == blo->getViolations())
		{
			if (oper->getWind(thistime / 60) > blo->getMyOperBlock()->getFront()->getWind(thistime / 60))
			{
				//cout << "--------------------------change type2------------------------" << endl;
				blo->getMyOperBlock()->setFront(oper);
			}
		}

	}
}

void BFS::chooseBestWind_forAllR(OperBlock * oper, Block * blo, int thistime ,int allowNum,double windRatio)
{	
	//先判断时间是否一致
	//if (thistime == blo->getMyOperBlock()->getFront()->getSolnTime() )

	//有vialation的vote
	//if ( (thistime + Util::flyTime) == blo->getMyOperBlock()->getSolnTime())
	//{	
	//	//如果大于15.0的时候找点仍然找那些15.0之内rank更高的点，那么这里就不必填windRatio
	//	int thisNumof_littleWind = oper->getBlock()->getNumOf_littleWind(thistime / 60, windRatio);
	//	int nextNumof_littleWind = blo->getMyOperBlock()->getFront()->getBlock()->getNumOf_littleWind(thistime / 60, windRatio);
	//	if (thisNumof_littleWind > nextNumof_littleWind)
	//	{
	//		//cout << "--------------------------change level------------------------" << endl;
	//		blo->getMyOperBlock()->setFront(oper);
	//		updateViolations_exchange(oper, blo, thistime, allowNum,windRatio);
	//		
	//	}
	//	else if (thisNumof_littleWind == nextNumof_littleWind)
	//	{
	//		if (getNum_Violations_allR(oper, blo, thistime, allowNum, windRatio) < blo->getViolations())
	//		{	
	//			//cout << "--------------------------change violations------------------------" << endl;
	//			blo->getMyOperBlock()->setFront(oper);
	//			blo->setViolations(getNum_Violations_allR(oper, blo, thistime, allowNum, windRatio));
	//		}
	//		else if (getNum_Violations_allR(oper, blo, thistime, allowNum, windRatio) == blo->getViolations())
	//		{
	//			double thisAvgWind = oper->getBlock()->getAvgWind(thistime / 60);
	//			double nextAvgWind = blo->getMyOperBlock()->getFront()->getBlock()->getAvgWind(thistime / 60);

	//			//大风与小风的选择
	//			//if (thisAvgWind > nextAvgWind)
	//			if (thisAvgWind < nextAvgWind)
	//			{
	//				//cout << "--------------------------change wind------------------------" << endl;
	//				blo->getMyOperBlock()->setFront(oper);
	//			}
	//		}
	//	
	//	}
	//} 



	//only AvgWind
	//if ((thistime + 2) == blo->getMyOperBlock()->getSolnTime())
	//{
	//	double thisAvgWind = oper->getBlock()->getAvgWind(thistime / 60);
	//	double nextAvgWind = blo->getMyOperBlock()->getFront()->getBlock()->getAvgWind(thistime / 60);
	//	if (thisAvgWind < nextAvgWind)
	//	{
	//		//cout << "--------------------------change wind------------------------" << endl;
	//		blo->getMyOperBlock()->setFront(oper);
	//	}
	//}




	//没vialation的vote
	//1-14选了大风！票10之外的选大风  或者 小风 仔细看
	//if ((thistime + Util::flyTime) == blo->getMyOperBlock()->getSolnTime())
	//{
	//	//如果大于15.0的时候找点仍然找那些15.0之内rank更高的点，那么这里就不必填windRatio
	//	int thisNumof_littleWind = oper->getBlock()->getNumOf_littleWind(thistime / 60, windRatio);
	//	int nextNumof_littleWind = blo->getMyOperBlock()->getFront()->getBlock()->getNumOf_littleWind(thistime / 60, windRatio);
	//	if (thisNumof_littleWind > nextNumof_littleWind)
	//	{
	//		//cout << "--------------------------change level------------------------" << endl;
	//		blo->getMyOperBlock()->setFront(oper);
	//		updateViolations_exchange(oper, blo, thistime, allowNum, windRatio);

	//	}
	//	else if (thisNumof_littleWind == nextNumof_littleWind)
	//	{
	//		double thisAvgWind = oper->getBlock()->getAvgWind(thistime / 60);
	//		double nextAvgWind = blo->getMyOperBlock()->getFront()->getBlock()->getAvgWind(thistime / 60);

	//		//大风与小风的选择
	//		//if (thisAvgWind > nextAvgWind)
	//		if (thisNumof_littleWind == 10)
	//		{
	//			if (thisAvgWind < nextAvgWind)
	//			{
	//				//cout << "--------------------------change wind------------------------" << endl;
	//				blo->getMyOperBlock()->setFront(oper);
	//			}
	//		}
	//		else
	//		{
	//			if (thisAvgWind < nextAvgWind)
	//			{
	//				//cout << "--------------------------change wind------------------------" << endl;
	//				blo->getMyOperBlock()->setFront(oper);
	//			}
	//		}
	//	}
	//}

	//细分的vote 1-14改了大风！
	if ((thistime + Util::flyTime) == blo->getMyOperBlock()->getSolnTime())
	{
		int thisNumof_littleWind = oper->getBlock()->getNumOf_littleWind(thistime / 60, windRatio);
		int nextNumof_littleWind = blo->getMyOperBlock()->getFront()->getBlock()->getNumOf_littleWind(thistime / 60, windRatio);
		if (thisNumof_littleWind > nextNumof_littleWind)
		{
			blo->getMyOperBlock()->setFront(oper);
			updateViolations_exchange(oper, blo, thistime, allowNum, windRatio);

		}
		else if (thisNumof_littleWind == nextNumof_littleWind)
		{	
			double cmpWindRatio = windRatio;
			//int cmpTime = 0;
			while (thisNumof_littleWind == nextNumof_littleWind)
			{	
				//cmpTime += 1;
				cmpWindRatio = cmpWindRatio - 0.25;
				//cout << "cmpWindRatio is " << cmpWindRatio <<" cmpTime is "<<cmpTime<< endl;
				thisNumof_littleWind = oper->getBlock()->getNumOf_littleWind(thistime / 60, cmpWindRatio);
				nextNumof_littleWind = blo->getMyOperBlock()->getFront()->getBlock()->getNumOf_littleWind(thistime / 60, cmpWindRatio);
				if (thisNumof_littleWind > nextNumof_littleWind)
				{
					blo->getMyOperBlock()->setFront(oper);
					updateViolations_exchange(oper, blo, thistime, allowNum, windRatio);
				}
				else if (thisNumof_littleWind < nextNumof_littleWind)
				{
					break;
				}
				if (cmpWindRatio < 6.0)
				{
					double thisAvgWind = oper->getBlock()->getAvgWind(thistime / 60);
					double nextAvgWind = blo->getMyOperBlock()->getFront()->getBlock()->getAvgWind(thistime / 60);

					//大风与小风的选择
					//if (thisAvgWind > nextAvgWind)
					if (thisAvgWind < nextAvgWind)
					{
						//cout << "--------------------------change cause of cmpWindRatio<6.0 so use AvgWind------------------------" << endl;
						blo->getMyOperBlock()->setFront(oper);
					}
					else
					{
						break;
					}
					
				}
				//cout << endl;
			}
		}
	}

	//细分层10票之外14.0之内的选大风
//if ((thistime + Util::flyTime) == blo->getMyOperBlock()->getSolnTime())
//{
//	int thisNumof_littleWind = oper->getBlock()->getNumOf_littleWind(thistime / 60, windRatio);
//	int nextNumof_littleWind = blo->getMyOperBlock()->getFront()->getBlock()->getNumOf_littleWind(thistime / 60, windRatio);
//	if (thisNumof_littleWind > nextNumof_littleWind)
//	{
//		blo->getMyOperBlock()->setFront(oper);
//		updateViolations_exchange(oper, blo, thistime, allowNum, windRatio);
//
//	}
//	else if (thisNumof_littleWind == nextNumof_littleWind)
//	{
//		double cmpWindRatio = windRatio;
//		//int cmpTime = 0;
//		while (thisNumof_littleWind == nextNumof_littleWind)
//		{
//			//cmpTime += 1;
//			cmpWindRatio = cmpWindRatio - 0.25;
//			//cout << "cmpWindRatio is " << cmpWindRatio <<" cmpTime is "<<cmpTime<< endl;
//			thisNumof_littleWind = oper->getBlock()->getNumOf_littleWind(thistime / 60, cmpWindRatio);
//			nextNumof_littleWind = blo->getMyOperBlock()->getFront()->getBlock()->getNumOf_littleWind(thistime / 60, cmpWindRatio);
//			if (thisNumof_littleWind > nextNumof_littleWind)
//			{
//				blo->getMyOperBlock()->setFront(oper);
//				updateViolations_exchange(oper, blo, thistime, allowNum, windRatio);
//			}
//			else if (thisNumof_littleWind < nextNumof_littleWind)
//			{
//				break;
//			}
//
//			if (cmpWindRatio < 14.0 && thisNumof_littleWind==10)
//			{
//				double thisAvgWind = oper->getBlock()->getAvgWind(thistime / 60);
//				double nextAvgWind = blo->getMyOperBlock()->getFront()->getBlock()->getAvgWind(thistime / 60);
//
//				//大风与小风的选择
//				if (thisAvgWind > nextAvgWind)
//					//if (thisAvgWind < nextAvgWind)
//				{
//					//cout << "--------------------------change cause of cmpWindRatio<6.0 so use AvgWind------------------------" << endl;
//					blo->getMyOperBlock()->setFront(oper);
//				}
//				else
//				{
//					break;
//				}
//
//			}
//			else if (cmpWindRatio < 6.0)
//			{
//				double thisAvgWind = oper->getBlock()->getAvgWind(thistime / 60);
//				double nextAvgWind = blo->getMyOperBlock()->getFront()->getBlock()->getAvgWind(thistime / 60);
//
//				//大风与小风的选择
//				//if (thisAvgWind > nextAvgWind)
//				if (thisAvgWind < nextAvgWind)
//				{
//					//cout << "--------------------------change cause of cmpWindRatio<6.0 so use AvgWind------------------------" << endl;
//					blo->getMyOperBlock()->setFront(oper);
//				}
//				else
//				{
//					break;
//				}
//
//			}
//			//cout << endl;
//		}
//	}
//}

}

void BFS::chooseBestWind_CBNT(OperBlock * oper, Block * blo, int thistime, int allowNum, double windRatio)
{
	//先判断时间是否一致

	//only AvgWind
	if ((thistime + 2) == blo->getMyOperBlock()->getSolnTime())
	{
		double thisAvgWind = oper->getBlock()->getCBNTavgWind(thistime / 60);
		double nextAvgWind = blo->getMyOperBlock()->getFront()->getBlock()->getCBNTavgWind(thistime / 60);
		if (thisAvgWind < nextAvgWind)
		{
			//cout << "--------------------------change wind CBNT------------------------" << endl;
			blo->getMyOperBlock()->setFront(oper);
		}
	}




	//没vialation的vote
	//1-14选了大风！票10之外的选大风  或者 小风 仔细看
	//if ((thistime + Util::flyTime) == blo->getMyOperBlock()->getSolnTime())
	//{
	//	//如果大于15.0的时候找点仍然找那些15.0之内rank更高的点，那么这里就不必填windRatio
	//	int thisNumof_littleWind = oper->getBlock()->getCBNTvotes(thistime / 60, windRatio);
	//	int nextNumof_littleWind = blo->getMyOperBlock()->getFront()->getBlock()->getCBNTvotes(thistime / 60, windRatio);
	//	if (thisNumof_littleWind > nextNumof_littleWind)
	//	{
	//		//cout << "--------------------------change level CBNT------------------------" << endl;
	//		blo->getMyOperBlock()->setFront(oper);
	//		updateViolations_exchange(oper, blo, thistime, allowNum, windRatio);

	//	}
	//	else if (thisNumof_littleWind == nextNumof_littleWind)
	//	{
	//		double thisAvgWind = oper->getBlock()->getCBNTavgWind(thistime / 60);
	//		double nextAvgWind = blo->getMyOperBlock()->getFront()->getBlock()->getCBNTavgWind(thistime / 60);
	//		//大风与小风的选择
	//		//if (thisAvgWind > nextAvgWind)
	//		if (thisAvgWind < nextAvgWind)
	//		{
	//			//cout << "--------------------------change wind CBNT------------------------" << endl;
	//			blo->getMyOperBlock()->setFront(oper);
	//		}
	//		
	//	}
	//}


}

void BFS::chooseBest_backtrack_evabyallR(OperBlock * oper, Block * blo, int thistime, int allowNum, double windRatio, double singleRatio)
{
	//根据单一模型来选父辈
	if ((thistime + 2) == blo->getMyOperBlock()->getSolnTime())
	{
		double thisWind = oper->getBlock()->getWind(thistime / 60);
		double nextWind = blo->getMyOperBlock()->getFront()->getBlock()->getWind(thistime / 60);
		if (thisWind < nextWind)
		{
			//cout << "--------------------------change wind------------------------" << endl;
			blo->getMyOperBlock()->setFront(oper);
		}
	}

	//classifier
	//if ((thistime + 2) == blo->getMyOperBlock()->getSolnTime())
	//{
	//	double thisWind = oper->getBlock()->getWind(thistime / 60);
	//	double nextWind = blo->getMyOperBlock()->getFront()->getBlock()->getWind(thistime / 60);
	//	if (thisWind < nextWind)
	//	{
	//		//cout << "--------------------------change wind------------------------" << endl;
	//		blo->getMyOperBlock()->setFront(oper);
	//	}
	//	else if (thisWind == nextWind)
	//	{
	//		double thisAvgWind = oper->getBlock()->getAvgWind(thistime / 60);
	//		double nextAvgWind = blo->getMyOperBlock()->getFront()->getBlock()->getAvgWind(thistime / 60);
	//		if (thisAvgWind < nextAvgWind)
	//		{	
	//			//cout << "--------------------------change wind when equal------------------------" << endl;
	//			blo->getMyOperBlock()->setFront(oper);
	//		}
	//	}
	//}

	//根据avg来选父辈
	//if ((thistime + 2) == blo->getMyOperBlock()->getSolnTime())
	//{
	//	double thisAvgWind = oper->getBlock()->getAvgWind(thistime / 60);
	//	double nextAvgWind = blo->getMyOperBlock()->getFront()->getBlock()->getAvgWind(thistime / 60);
	//	if (thisAvgWind < nextAvgWind)
	//	{
	//		//cout << "--------------------------change wind------------------------" << endl;
	//		blo->getMyOperBlock()->setFront(oper);
	//	}
	//}

	//根据vote来选父辈,子层avg
	//if ((thistime + Util::flyTime) == blo->getMyOperBlock()->getSolnTime())
	//{
	//	//如果大于15.0的时候找点仍然找那些15.0之内rank更高的点，那么这里就不必填windRatio
	//	int thisNumof_littleWind = oper->getBlock()->getNumOf_littleWind(thistime / 60, windRatio);
	//	int nextNumof_littleWind = blo->getMyOperBlock()->getFront()->getBlock()->getNumOf_littleWind(thistime / 60, windRatio);
	//	if (thisNumof_littleWind > nextNumof_littleWind)
	//	{
	//		//cout << "--------------------------change level------------------------" << endl;
	//		blo->getMyOperBlock()->setFront(oper);
	//		updateViolations_exchange(oper, blo, thistime, allowNum, windRatio);

	//	}
	//	else if (thisNumof_littleWind == nextNumof_littleWind)
	//	{
	//		double thisAvgWind = oper->getBlock()->getAvgWind(thistime / 60);
	//		double nextAvgWind = blo->getMyOperBlock()->getFront()->getBlock()->getAvgWind(thistime / 60);

	//		//大风与小风的选择
	//		//if (thisAvgWind > nextAvgWind)
	//		if (thisAvgWind < nextAvgWind)
	//		{
	//			//cout << "--------------------------change wind------------------------" << endl;
	//			blo->getMyOperBlock()->setFront(oper);
	//		}
	//	}
	//}

	//根据vote来选父辈,子层还是看个人的模型
	//if ((thistime + Util::flyTime) == blo->getMyOperBlock()->getSolnTime())
	//{
	//	//如果大于15.0的时候找点仍然找那些15.0之内rank更高的点，那么这里就不必填windRatio
	//	int thisNumof_littleWind = oper->getBlock()->getNumOf_littleWind(thistime / 60, windRatio);
	//	int nextNumof_littleWind = blo->getMyOperBlock()->getFront()->getBlock()->getNumOf_littleWind(thistime / 60, windRatio);
	//	if (thisNumof_littleWind > nextNumof_littleWind)
	//	{
	//		//cout << "--------------------------change level------------------------" << endl;
	//		blo->getMyOperBlock()->setFront(oper);
	//		updateViolations_exchange(oper, blo, thistime, allowNum, windRatio);

	//	}
	//	else if (thisNumof_littleWind == nextNumof_littleWind)
	//	{
	//		double thisWind = oper->getBlock()->getWind(thistime / 60);
	//		double nextWind = blo->getMyOperBlock()->getFront()->getBlock()->getWind(thistime / 60);
	//		if (thisWind < nextWind)
	//		{
	//			//cout << "--------------------------change wind------------------------" << endl;
	//			blo->getMyOperBlock()->setFront(oper);
	//		}
	//	}
	//}
}

vector<vector<OperBlock *>> BFS::solve_allR_multiTarget()
{
	vector<vector<OperBlock *>> multiSoln;

	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock,Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	while (!_ingOperBlocks.empty()  && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime << " and SolnTime is " << ingOperBlock->getSolnTime() << endl;
			int allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{	
				//如果是fixNum-changeWind，那么一般intial的Num不是10，所以得手动改这里，让它变成10
				//Util::NumOf_littleWindForAllR
				if (ingOperBlock->cangotoThisBlock_allR(cangoto, thisTime ,4 ,Util::initRatio_forAllR))
				{

					//顺序不能乱
					//回撞sourceBlock导致getFront为NULL
					//firstReach就是注释掉

					if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
					{	
						//写11为了在10的时候计数不乱加(fixWind,changeNum的)
						chooseBestWind_forAllR(ingOperBlock, cangoto, thisTime,11,Util::initRatio_forAllR);
					}
					else if (cangoto->getSituation() == 0)
					//if (cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						cangotoOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(cangotoOperBlock);
						_ingOperBlocks.push(cangotoOperBlock);
					}
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{
				if (cangoto->getSituation() == 0)
				{
					allCangoToOper = false;
					break;
				}
			}

			if (allCangoToOper)
			{
				_vistedOperBlocks.push_back(ingOperBlock);
				ingOperBlock->getBlock()->setSituation(2);
				_ingOperBlocks.pop();
			}
			//如果原地停留 那么仍需要判断停留的时候是否风速大于initial ratio
			//若大于，则作为坠机处理(假visted处理）
			//所以output的时候对于第一种群式搜索的时候也没关系（因为soln time 对应的风速肯定是小于initial ratio的）
			else
			{
				if (ingOperBlock->getBlock()->getNumOf_littleWind(thisTime / 60, Util::initRatio_forAllR) >=Util::NumOf_littleWindForAllR)
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				else
				{
					_vistedOperBlocks.push_back(ingOperBlock);
					ingOperBlock->getBlock()->setSituation(2);
					_ingOperBlocks.pop();
				}

			}

		}
		//若cangoto为空 那么也是变为visted
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	vector<pair<int, int>> destinations;
	pair<int, int> d1(84, 203);
	pair<int, int> d2(199, 371);
	pair<int, int> d3(140, 234);
	pair<int, int> d4(236, 241);
	pair<int, int> d5(315, 281);
	pair<int, int> d6(358, 207);
	pair<int, int> d7(363, 237);
	pair<int, int> d8(423, 266);
	pair<int, int> d9(125, 375);
	pair<int, int> d10(189, 274);

	destinations.push_back(d1);
	destinations.push_back(d2);
	destinations.push_back(d3);
	destinations.push_back(d4);
	destinations.push_back(d5);
	destinations.push_back(d6);
	destinations.push_back(d7);
	destinations.push_back(d8);
	destinations.push_back(d9);
	destinations.push_back(d10);

	for (int k = 0; k < 10; k++)
	{
		int destinationX = destinations[k].first;
		int destinationY = destinations[k].second;
		vector<OperBlock * > OperRoute;
		OperBlock * front = NULL;
		// maybe some is soln but it's ingTime is 1258 so it will be in the ingBlock
		bool findTheDes = false;
		for (auto & visted : _vistedOperBlocks)
		{
			if (visted->getBlock()->getX() == destinationX && visted->getBlock()->getY() == destinationY)
			{
				OperRoute.push_back(visted);
				front = visted->getFront();
				findTheDes = true;
				break;
			}
		}
		if (!findTheDes)
		{
			int size = _ingOperBlocks.size();
			for (int i = 1; i <= size; i++)
			{
				OperBlock * testOperBlock = _ingOperBlocks.front();
				if (destinationX == testOperBlock->getX() && destinationY == testOperBlock->getY())
				{
					OperRoute.push_back(testOperBlock);
					front = testOperBlock->getFront();
					findTheDes = true;
					break;
				}
				_ingOperBlocks.pop();
				_ingOperBlocks.push(testOperBlock);
			}
		}
		while (front != NULL)
		{
			OperRoute.push_back(front);
			front = front->getFront();
		}
		sort(OperRoute.begin(), OperRoute.end(), OperBlock::cmpBySolnTime);
		multiSoln.push_back(OperRoute);
	}

	//print function
	for (int k = 0; k<10; k++)
	{
		if (multiSoln[k].empty())
		{
			cout << endl;
			cout << "can not reach city" << k + 1 << "(" << destinations[k].first << "," << destinations[k].second << endl;
			cout << endl;
		}
		else
		{
			cout << endl;
			cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << destinations[k].first << "," << destinations[k].second << ") is :" << endl;
			cout << endl;
			cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "AvgWind" << "\t" << "rank" << "\t" <<"violations" <<endl;
			for (int i = 0; i<multiSoln[k].size(); i++)
			{	
				//开挂的时候就不是 142  328 了
				int absPlus = abs(multiSoln[k][i]->getBlock()->getX() - _sourceBlock->getX()) + abs(multiSoln[k][i]->getBlock()->getY() - _sourceBlock->getY());
				/*if (absPlus * 2 + 180 < multiSoln[k][i]->getSolnTime())
				{
				cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
				}*/
				if (absPlus * 2 + Util::startTime_BFS > multiSoln[k][i]->getSolnTime())
				{
					cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
				}

				if (multiSoln[k][i]->getSolnTime() != Util::startTime_BFS)
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getAvgWind((multiSoln[k][i]->getSolnTime()- Util::flyTime) / 60) << "\t" << multiSoln[k][i]->getBlock()->getNumOf_littleWind((multiSoln[k][i]->getSolnTime()-Util::flyTime) / 60, Util::initRatio_forAllR) << "\t" << multiSoln[k][i]->getBlock()->getViolations()<< endl;
				}
				else
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getAvgWind(multiSoln[k][i]->getSolnTime() / 60) << "\t" << multiSoln[k][i]->getBlock()->getNumOf_littleWind(multiSoln[k][i]->getSolnTime()  / 60, Util::initRatio_forAllR) << "\t" << multiSoln[k][i]->getBlock()->getViolations()<< endl;
				}
				
			}
			cout << endl;
			cout << endl;
		}
	}

	return multiSoln;
}

vector<OperBlock *> BFS::solve_allR_singleTarget(Block * targetBlock, int allowNumOf_littleWind , double allRratio)
{
	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();

	int targetSolnTime = Util::maxTime;
	bool findTheTarget = false;


	while (!_ingOperBlocks.empty() && _ingOperBlocks.front()->getIngTime()<targetSolnTime && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			//cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime << " and SolnTime is " << ingOperBlock->getSolnTime() << endl;
			bool allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock_allR(cangoto, thisTime, allowNumOf_littleWind, allRratio))
				{
					if (cangoto->getX() == targetX && cangoto->getY() == targetY && cangoto->getSituation() == 0)
					{							
						cangoto->setSituation(1);
						cangoto->setViolations(getNum_Violations_allR(ingOperBlock, cangoto, thisTime, allowNumOf_littleWind,allRratio));
						OperBlock * targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						targetOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(targetOperBlock);
						_ingOperBlocks.push(targetOperBlock);
						targetSolnTime = thisTime + Util::flyTime;
						findTheTarget = true;										
					}
					else
					{

						if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
						{
							chooseBestWind_forAllR(ingOperBlock, cangoto, thisTime, allowNumOf_littleWind,allRratio);
						}
						else if (cangoto->getSituation() == 0)
							//if (cangoto->getSituation() == 0)
						{
							cangoto->setSituation(1);
							cangoto->setViolations(getNum_Violations_allR(ingOperBlock, cangoto, thisTime, allowNumOf_littleWind, allRratio));
							OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
							cangotoOperBlock->setFront(ingOperBlock);
							cangoto->setMyOperBlock(cangotoOperBlock);
							_ingOperBlocks.push(cangotoOperBlock);
						}

					}
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{
				if (cangoto->getSituation() == 0)
				{
					allCangoToOper = false;
					break;
				}
			}

			if (allCangoToOper)
			{
				_vistedOperBlocks.push_back(ingOperBlock);
				ingOperBlock->getBlock()->setSituation(2);
				_ingOperBlocks.pop();
			}
			//判断停留的时候是否不坠机
			else
			{
				if (ingOperBlock->getBlock()->getNumOf_littleWind(thisTime / 60, allRratio) >= allowNumOf_littleWind)
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				else
				{
					_vistedOperBlocks.push_back(ingOperBlock);
					ingOperBlock->getBlock()->setSituation(2);
					_ingOperBlocks.pop();
				}

			}

		}
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	vector<OperBlock *> OperRoute;
	if (findTheTarget)
	{
		// when print don't forget to inverted order
		OperBlock * targetOB = targetBlock->getMyOperBlock();
		OperRoute.push_back(targetOB);
		OperBlock * front = targetOB->getFront();
		while (front != NULL)
		{
			OperRoute.push_back(front);
			front = front->getFront();
		}
	}

	sort(OperRoute.begin(), OperRoute.end(), OperBlock::cmpBySolnTime);

	//print function
	if (OperRoute.empty())
	{
		cout << " shortest path failed ! Can't find the targetBlock when allow " << allowNumOf_littleWind << " !" << endl;
	}
	else
	{
		cout << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << targetBlock->getX() << "," << targetBlock->getY() << ") is :" << endl;
		cout << endl;
		cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "AvgWind" << "\t" << "rank" << "\t" <<"violations"<< endl;
		for (int i = 0; i <OperRoute.size(); i++)
		{	
			//开挂的时候可能不是142 328
			int absPlus = abs(OperRoute[i]->getBlock()->getX() - _sourceBlock->getX()) + abs(OperRoute[i]->getBlock()->getY() - _sourceBlock->getY());
			/*if (absPlus * 2 + 180 < OperRoute[i]->getSolnTime())
			{
			cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
			}*/
			if (absPlus * 2 + Util::startTime_BFS > OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
			}

			//cout << "(" << OperRoute[i]->getBlock()->getX() << "," << OperRoute[i]->getBlock()->getY() << ")Solntime is" << OperRoute[i]->getSolnTime() << " and the wind that time is" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << "->" << endl;

			if (OperRoute[i]->getSolnTime() != Util::startTime_BFS)
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getAvgWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60) << "\t" << OperRoute[i]->getBlock()->getNumOf_littleWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60, allRratio) << "\t" << OperRoute[i]->getBlock()->getViolations() ;
				if (OperRoute[i]->getBlock()->getNumOf_littleWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60, allRratio) == 8 || OperRoute[i]->getBlock()->getNumOf_littleWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60, allRratio) == 9)
				{
					vector<int > sayNoIndex = OperRoute[i]->getBlock()->getSayNoModelId((OperRoute[i]->getSolnTime() - Util::flyTime) / 60, allRratio);
					for (int sni = 0;sni<sayNoIndex.size(); sni++)
					{
						cout << "\t" << sayNoIndex[sni];
					}
				}
				cout << endl;
			}
			else
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getAvgWind(OperRoute[i]->getSolnTime() / 60) << "\t" << OperRoute[i]->getBlock()->getNumOf_littleWind(OperRoute[i]->getSolnTime() / 60, allRratio) << "\t" << OperRoute[i]->getBlock()->getViolations() << endl;
			}

		}
		cout << endl;
		cout << endl;
	}


	return OperRoute;
}

void BFS::updateViolations_exchange(OperBlock * ingOB, Block* cgt, int thisTime, int allowNum,double windRatio)
{
	if (ingOB->getBlock()->getNumOf_littleWind(thisTime / 60, windRatio) ==allowNum || cgt->getNumOf_littleWind(thisTime / 60, windRatio) ==allowNum)
	{
		if (ingOB->getBlock()->getViolations() == cgt->getViolations())
		{	
			//cout << "change type 1" << endl;
			cgt->setViolations(ingOB->getBlock()->getViolations() + 1);
		}
	}
	else
	{
		if (ingOB->getBlock()->getViolations() != cgt->getViolations())
		{	
			//cout << "change type 2" << endl;
			cgt->setViolations(ingOB->getBlock()->getViolations());
		}
	}
}

int BFS::getNum_Violations_allR(OperBlock * ingOB, Block* cgt, int thisTime, int allowNum,double windRatio)
{
	if (ingOB->getBlock()->getNumOf_littleWind(thisTime / 60, windRatio) == allowNum || cgt->getNumOf_littleWind(thisTime / 60, windRatio) == allowNum)
	{
		return (ingOB->getBlock()->getViolations() + 1);
	}
	else
	{
		return ingOB->getBlock()->getViolations();
	}
}

int BFS::getNum_Violations(OperBlock * ingOB, Block* cgt, int thisTime,double windRatio)
{
	if (ingOB->getBlock()->getWind(thisTime / 60) <=(windRatio-0.5) && cgt->getWind(thisTime / 60) <= (windRatio - 0.5))
	{
		return ingOB->getBlock()->getViolations();
	}
	else
	{	
		return (ingOB->getBlock()->getViolations() + 1);	
	}
}

// valued by allR , added 12-23
vector<vector<OperBlock *>> BFS::solve_multiTarget_valued_by_allR()
{
	vector<vector<OperBlock *>> multiSoln;

	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);


	while (!_ingOperBlocks.empty() && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime << " and SolnTime is " << ingOperBlock->getSolnTime() << endl;
			int allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock_valued_by_allR(cangoto, thisTime,Util::initRatio,Util::NumOf_littleWindForAllR))
				{

					//顺序不能乱
					//回撞sourceBlock导致getFront为NULL
					//firstReach就是注释掉

					if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
					{
						chooseLowestWind(ingOperBlock, cangoto, thisTime, Util::initRatio);
					}
					else if (cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						cangoto->setViolations(getNum_Violations(ingOperBlock, cangoto, thisTime, Util::initRatio));
						OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						cangotoOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(cangotoOperBlock);
						_ingOperBlocks.push(cangotoOperBlock);
					}
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{
				if (cangoto->getSituation() == 0)
				{
					allCangoToOper = false;
					break;
				}
			}

			if (allCangoToOper)
			{
				_vistedOperBlocks.push_back(ingOperBlock);
				ingOperBlock->getBlock()->setSituation(2);
				_ingOperBlocks.pop();
			}
			//如果原地停留 那么仍需要判断停留的时候是否风速大于initial ratio
			//若大于，则作为坠机处理(假visted处理）
			//所以output的时候对于第一种群式搜索的时候也没关系（因为soln time 对应的风速肯定是小于initial ratio的）
			else
			{
				if (ingOperBlock->getWind(thisTime / 60) < Util::initRatio)
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				else
				{
					_vistedOperBlocks.push_back(ingOperBlock);
					ingOperBlock->getBlock()->setSituation(2);
					_ingOperBlocks.pop();
				}

			}

		}
		//若cangoto为空 那么也是变为visted
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	vector<pair<int, int>> destinations;
	pair<int, int> d1(84, 203);
	pair<int, int> d2(199, 371);
	pair<int, int> d3(140, 234);
	pair<int, int> d4(236, 241);
	pair<int, int> d5(315, 281);
	pair<int, int> d6(358, 207);
	pair<int, int> d7(363, 237);
	pair<int, int> d8(423, 266);
	pair<int, int> d9(125, 375);
	pair<int, int> d10(189, 274);

	destinations.push_back(d1);
	destinations.push_back(d2);
	destinations.push_back(d3);
	destinations.push_back(d4);
	destinations.push_back(d5);
	destinations.push_back(d6);
	destinations.push_back(d7);
	destinations.push_back(d8);
	destinations.push_back(d9);
	destinations.push_back(d10);

	for (int k = 0; k < 10; k++)
	{
		int destinationX = destinations[k].first;
		int destinationY = destinations[k].second;
		vector<OperBlock * > OperRoute;
		OperBlock * front = NULL;
		// maybe some is soln but it's ingTime is 1258 so it will be in the ingBlock
		bool findTheDes = false;
		for (auto & visted : _vistedOperBlocks)
		{
			if (visted->getBlock()->getX() == destinationX && visted->getBlock()->getY() == destinationY)
			{
				OperRoute.push_back(visted);
				front = visted->getFront();
				findTheDes = true;
				break;
			}
		}
		if (!findTheDes)
		{
			int size = _ingOperBlocks.size();
			for (int i = 1; i <= size; i++)
			{
				OperBlock * testOperBlock = _ingOperBlocks.front();
				if (destinationX == testOperBlock->getX() && destinationY == testOperBlock->getY())
				{
					OperRoute.push_back(testOperBlock);
					front = testOperBlock->getFront();
					findTheDes = true;
					break;
				}
				_ingOperBlocks.pop();
				_ingOperBlocks.push(testOperBlock);
			}
		}
		while (front != NULL)
		{
			OperRoute.push_back(front);
			front = front->getFront();
		}
		sort(OperRoute.begin(), OperRoute.end(), OperBlock::cmpBySolnTime);
		multiSoln.push_back(OperRoute);
	}

	//print function
	for (int k = 0; k<10; k++)
	{
		if (multiSoln[k].empty())
		{
			cout << endl;
			cout << "can not reach city" << k + 1 << "(" << destinations[k].first << "," << destinations[k].second << endl;
			cout << endl;
		}
		else
		{
			cout << endl;
			cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << destinations[k].first << "," << destinations[k].second << ") is :" << endl;
			cout << endl;
			cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "Wind" << "\t" << "violations" <<"\t"<<"votes"<< endl;
			for (int i = 0; i<multiSoln[k].size(); i++)
			{
				int absPlus = abs(multiSoln[k][i]->getBlock()->getX() - 142) + abs(multiSoln[k][i]->getBlock()->getY() - 328);
				/*if (absPlus * 2 + 180 < multiSoln[k][i]->getSolnTime())
				{
				cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
				}*/
				if (absPlus * 2 + Util::startTime_BFS > multiSoln[k][i]->getSolnTime())
				{
					cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
				}

				//cout << "(" << multiSoln[k][i]->getBlock()->getX() << "," << multiSoln[k][i]->getBlock()->getY() << ")Solntime is" << multiSoln[k][i]->getSolnTime() << "->" ;
				if (multiSoln[k][i]->getSolnTime() != Util::startTime_BFS)
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getWind((multiSoln[k][i]->getSolnTime() - 2) / 60) << "\t" << multiSoln[k][i]->getBlock()->getViolations() << "\t" << multiSoln[k][i]->getBlock()->getNumOf_littleWind((multiSoln[k][i]->getSolnTime() - 2) / 60,Util::initRatio) << endl;
				}
				else
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getWind(multiSoln[k][i]->getSolnTime() / 60) << "\t" << multiSoln[k][i]->getBlock()->getViolations() << "\t" << multiSoln[k][i]->getBlock()->getNumOf_littleWind(multiSoln[k][i]->getSolnTime() / 60,Util::initRatio) << endl;
				}
			}
			cout << endl;
			cout << endl;
		}
	}

	return multiSoln;
}

vector<OperBlock *> BFS::solve_singleTarget_valued_by_allR(Block * targetBlock, double windRatio)
{
	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();

	bool findTheTarget = false;
	int targetSolnTime = Util::maxTime;


	while (!_ingOperBlocks.empty() && _ingOperBlocks.front()->getIngTime()<targetSolnTime && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			//cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime << " and SolnTime is " << ingOperBlock->getSolnTime() << endl;
			bool allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock_valued_by_allR(cangoto, thisTime, windRatio,Util::NumOf_littleWindForAllR))
				{
					if (cangoto->getX() == targetX && cangoto->getY() == targetY && cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						cangoto->setViolations(getNum_Violations(ingOperBlock, cangoto, thisTime, windRatio));
						OperBlock * targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						targetOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(targetOperBlock);
						_ingOperBlocks.push(targetOperBlock);
						targetSolnTime = thisTime + Util::flyTime;
						findTheTarget = true;
					}
					else
					{

						if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
						{
							chooseLowestWind(ingOperBlock, cangoto, thisTime, windRatio);
							//chooseHighestWind(ingOperBlock, cangoto, thisTime);
						}
						else if (cangoto->getSituation() == 0)

							//if (cangoto->getSituation() == 0)
						{
							cangoto->setSituation(1);
							cangoto->setViolations(getNum_Violations(ingOperBlock, cangoto, thisTime, windRatio));
							OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
							cangotoOperBlock->setFront(ingOperBlock);
							cangoto->setMyOperBlock(cangotoOperBlock);
							_ingOperBlocks.push(cangotoOperBlock);
						}

					}
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{
				if (cangoto->getSituation() == 0)
				{
					allCangoToOper = false;
					break;
				}
			}

			if (allCangoToOper)
			{
				_vistedOperBlocks.push_back(ingOperBlock);
				ingOperBlock->getBlock()->setSituation(2);
				_ingOperBlocks.pop();
			}
			//判断停留的时候是否不坠机
			else
			{
				if (ingOperBlock->getWind(thisTime / 60) < windRatio)
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				else
				{
					_vistedOperBlocks.push_back(ingOperBlock);
					ingOperBlock->getBlock()->setSituation(2);
					_ingOperBlocks.pop();
				}

			}

		}
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	vector<OperBlock *> OperRoute;
	if (findTheTarget)
	{
		// when print don't forget to inverted order		
		OperBlock * targetOB = targetBlock->getMyOperBlock();
		OperRoute.push_back(targetOB);
		OperBlock * front = targetOB->getFront();
		while (front != NULL)
		{
			OperRoute.push_back(front);
			front = front->getFront();
		}
	}

	sort(OperRoute.begin(), OperRoute.end(), OperBlock::cmpBySolnTime);

	//print function
	if (OperRoute.empty())
	{
		cout << " shortest path failed ! Can't find the targetBlock when allow " << windRatio << " !" << endl;
	}
	else
	{
		cout << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << targetBlock->getX() << "," << targetBlock->getY() << ") is :" << endl;
		cout << endl;
		cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "Wind" << "\t" << "violations" << "\t"<<"votes"<<endl;
		for (int i = 0; i <OperRoute.size(); i++)
		{
			int absPlus = abs(OperRoute[i]->getBlock()->getX() - 142) + abs(OperRoute[i]->getBlock()->getY() - 328);
			/*if (absPlus * 2 + 180 < OperRoute[i]->getSolnTime())
			{
			cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
			}*/
			if (absPlus * 2 + Util::startTime_BFS > OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
			}

			//cout << "(" << OperRoute[i]->getBlock()->getX() << "," << OperRoute[i]->getBlock()->getY() << ")Solntime is" << OperRoute[i]->getSolnTime() << " and the wind that time is" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << "->" << endl;

			if (OperRoute[i]->getSolnTime() != Util::startTime_BFS)
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getWind((OperRoute[i]->getSolnTime() - 2) / 60) << "\t" << OperRoute[i]->getBlock()->getViolations() << "\t" << OperRoute[i]->getBlock()->getNumOf_littleWind((OperRoute[i]->getSolnTime() - 2) / 60,windRatio) << endl;
			}
			else
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << "\t" << OperRoute[i]->getBlock()->getViolations() << "\t" << OperRoute[i]->getBlock()->getNumOf_littleWind(OperRoute[i]->getSolnTime() / 60,windRatio) << endl;
			}

		}
		cout << endl;
		cout << endl;
	}


	return OperRoute;
}

void BFS::chooseLowestForAllRjustAvg(OperBlock * oper, Block * blo, int thistime,double allRratio)
{

	/*if ((thistime + Util::flyTime) == blo->getMyOperBlock()->getSolnTime())
	{
		int thisNumof_littleWind = oper->getBlock()->getNumOf_littleWind(thistime / 60, allRratio);
		int nextNumof_littleWind = blo->getMyOperBlock()->getFront()->getBlock()->getNumOf_littleWind(thistime / 60, allRratio);
		if (thisNumof_littleWind > nextNumof_littleWind)
		{
			blo->getMyOperBlock()->setFront(oper);

		}
		else if (thisNumof_littleWind == nextNumof_littleWind)
		{	
			double thisAvgWind = oper->getBlock()->getAvgWind(thistime / 60);
			double nextAvgWind = blo->getMyOperBlock()->getFront()->getBlock()->getAvgWind(thistime / 60);
			if (thisAvgWind < nextAvgWind)
			{
				blo->getMyOperBlock()->setFront(oper);
			}
		}
	}*/



	//only AvgWind
	if ((thistime + 2) == blo->getMyOperBlock()->getSolnTime())
	{
		double thisAvgWind = oper->getBlock()->getAvgWind(thistime / 60);
		double nextAvgWind = blo->getMyOperBlock()->getFront()->getBlock()->getAvgWind(thistime / 60);
		if (thisAvgWind < nextAvgWind)
		{
			//cout << "--------------------------change wind------------------------" << endl;
			blo->getMyOperBlock()->setFront(oper);
		}
	}
}

vector<vector<OperBlock *>> BFS::solve_allRjustAvg_multi()
{
	vector<vector<OperBlock *>> multiSoln;

	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	while (!_ingOperBlocks.empty() && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime << " and SolnTime is " << ingOperBlock->getSolnTime() << endl;
			int allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock_allRjustAvg(cangoto, thisTime, Util::allRjustAvgRatio))
				{
					if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
					{
						chooseLowestForAllRjustAvg(ingOperBlock, cangoto, thisTime,Util::allRjustAvgRatio);
					}
					else if (cangoto->getSituation() == 0)
						//if (cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						cangotoOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(cangotoOperBlock);
						_ingOperBlocks.push(cangotoOperBlock);
					}
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{
				if (cangoto->getSituation() == 0)
				{
					allCangoToOper = false;
					break;
				}
			}

			if (allCangoToOper)
			{
				_vistedOperBlocks.push_back(ingOperBlock);
				ingOperBlock->getBlock()->setSituation(2);
				_ingOperBlocks.pop();
			}
			//如果原地停留 那么仍需要判断停留的时候是否风速大于initial ratio
			//若大于，则作为坠机处理(假visted处理）
			//所以output的时候对于第一种群式搜索的时候也没关系（因为soln time 对应的风速肯定是小于initial ratio的）
			else
			{
				if (ingOperBlock->getBlock()->getAvgWind(thisTime/60)<Util::allRjustAvgRatio)
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				else
				{
					_vistedOperBlocks.push_back(ingOperBlock);
					ingOperBlock->getBlock()->setSituation(2);
					_ingOperBlocks.pop();
				}

			}

		}
		//若cangoto为空 那么也是变为visted
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	vector<pair<int, int>> destinations;
	pair<int, int> d1(84, 203);
	pair<int, int> d2(199, 371);
	pair<int, int> d3(140, 234);
	pair<int, int> d4(236, 241);
	pair<int, int> d5(315, 281);
	pair<int, int> d6(358, 207);
	pair<int, int> d7(363, 237);
	pair<int, int> d8(423, 266);
	pair<int, int> d9(125, 375);
	pair<int, int> d10(189, 274);

	destinations.push_back(d1);
	destinations.push_back(d2);
	destinations.push_back(d3);
	destinations.push_back(d4);
	destinations.push_back(d5);
	destinations.push_back(d6);
	destinations.push_back(d7);
	destinations.push_back(d8);
	destinations.push_back(d9);
	destinations.push_back(d10);

	for (int k = 0; k < 10; k++)
	{
		int destinationX = destinations[k].first;
		int destinationY = destinations[k].second;
		vector<OperBlock * > OperRoute;
		OperBlock * front = NULL;
		// maybe some is soln but it's ingTime is 1258 so it will be in the ingBlock
		bool findTheDes = false;
		for (auto & visted : _vistedOperBlocks)
		{
			if (visted->getBlock()->getX() == destinationX && visted->getBlock()->getY() == destinationY)
			{
				OperRoute.push_back(visted);
				front = visted->getFront();
				findTheDes = true;
				break;
			}
		}
		if (!findTheDes)
		{
			int size = _ingOperBlocks.size();
			for (int i = 1; i <= size; i++)
			{
				OperBlock * testOperBlock = _ingOperBlocks.front();
				if (destinationX == testOperBlock->getX() && destinationY == testOperBlock->getY())
				{
					OperRoute.push_back(testOperBlock);
					front = testOperBlock->getFront();
					findTheDes = true;
					break;
				}
				_ingOperBlocks.pop();
				_ingOperBlocks.push(testOperBlock);
			}
		}
		while (front != NULL)
		{
			OperRoute.push_back(front);
			front = front->getFront();
		}
		sort(OperRoute.begin(), OperRoute.end(), OperBlock::cmpBySolnTime);
		multiSoln.push_back(OperRoute);
	}

	//print function
	for (int k = 0; k<10; k++)
	{
		if (multiSoln[k].empty())
		{
			cout << endl;
			cout << "can not reach city" << k + 1 << "(" << destinations[k].first << "," << destinations[k].second << endl;
			cout << endl;
		}
		else
		{
			cout << endl;
			cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << destinations[k].first << "," << destinations[k].second << ") is :" << endl;
			cout << endl;
			cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "AvgWind" << "\t" << "rank" << "\t" << "violations" << endl;
			for (int i = 0; i<multiSoln[k].size(); i++)
			{
				//开挂的时候就不是 142  328 了
				int absPlus = abs(multiSoln[k][i]->getBlock()->getX() - _sourceBlock->getX()) + abs(multiSoln[k][i]->getBlock()->getY() - _sourceBlock->getY());
				/*if (absPlus * 2 + 180 < multiSoln[k][i]->getSolnTime())
				{
				cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
				}*/
				if (absPlus * 2 + Util::startTime_BFS > multiSoln[k][i]->getSolnTime())
				{
					cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
				}

				if (multiSoln[k][i]->getSolnTime() != Util::startTime_BFS)
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getAvgWind((multiSoln[k][i]->getSolnTime() - Util::flyTime) / 60) << "\t" << multiSoln[k][i]->getBlock()->getNumOf_littleWind((multiSoln[k][i]->getSolnTime() - Util::flyTime) / 60, Util::initRatio_forAllR) << "\t" << multiSoln[k][i]->getBlock()->getViolations() << endl;
				}
				else
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getAvgWind(multiSoln[k][i]->getSolnTime() / 60) << "\t" << multiSoln[k][i]->getBlock()->getNumOf_littleWind(multiSoln[k][i]->getSolnTime() / 60, Util::initRatio_forAllR) << "\t" << multiSoln[k][i]->getBlock()->getViolations() << endl;
				}

			}
			cout << endl;
			cout << endl;
		}
	}

	return multiSoln;
}

vector<OperBlock *> BFS::solve_allRjustAvg_single(Block * targetBlock, double allRratio)
{
	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();

	int targetSolnTime = Util::maxTime;
	bool findTheTarget = false;


	while (!_ingOperBlocks.empty() && _ingOperBlocks.front()->getIngTime()<targetSolnTime && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			//cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime << " and SolnTime is " << ingOperBlock->getSolnTime() << endl;
			bool allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock_allRjustAvg(cangoto, thisTime, allRratio))
				{
					if (cangoto->getX() == targetX && cangoto->getY() == targetY && cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						OperBlock * targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						targetOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(targetOperBlock);
						_ingOperBlocks.push(targetOperBlock);
						targetSolnTime = thisTime + Util::flyTime;
						findTheTarget = true;
					}
					else
					{

						if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
						{
							chooseLowestForAllRjustAvg(ingOperBlock, cangoto, thisTime,allRratio);
						}
						else if (cangoto->getSituation() == 0)
							//if (cangoto->getSituation() == 0)
						{
							cangoto->setSituation(1);
							OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
							cangotoOperBlock->setFront(ingOperBlock);
							cangoto->setMyOperBlock(cangotoOperBlock);
							_ingOperBlocks.push(cangotoOperBlock);
						}

					}
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{
				if (cangoto->getSituation() == 0)
				{
					allCangoToOper = false;
					break;
				}
			}

			if (allCangoToOper)
			{
				_vistedOperBlocks.push_back(ingOperBlock);
				ingOperBlock->getBlock()->setSituation(2);
				_ingOperBlocks.pop();
			}
			//判断停留的时候是否不坠机
			else
			{
				if (ingOperBlock->getBlock()->getAvgWind(thisTime / 60)<allRratio)
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				else
				{
					_vistedOperBlocks.push_back(ingOperBlock);
					ingOperBlock->getBlock()->setSituation(2);
					_ingOperBlocks.pop();
				}

			}

		}
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	vector<OperBlock *> OperRoute;
	if (findTheTarget)
	{
		// when print don't forget to inverted order
		OperBlock * targetOB = targetBlock->getMyOperBlock();
		OperRoute.push_back(targetOB);
		OperBlock * front = targetOB->getFront();
		while (front != NULL)
		{
			OperRoute.push_back(front);
			front = front->getFront();
		}
	}

	sort(OperRoute.begin(), OperRoute.end(), OperBlock::cmpBySolnTime);

	//print function
	if (OperRoute.empty())
	{
		cout << " shortest path failed ! Can't find the targetBlock when allow " << allRratio << " !" << endl;
	}
	else
	{
		cout << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << targetBlock->getX() << "," << targetBlock->getY() << ") is :" << endl;
		cout << endl;
		cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "AvgWind" << "\t" << "rank" << "\t" << "violations" << endl;
		for (int i = 0; i <OperRoute.size(); i++)
		{
			//开挂的时候可能不是142 328
			int absPlus = abs(OperRoute[i]->getBlock()->getX() - _sourceBlock->getX()) + abs(OperRoute[i]->getBlock()->getY() - _sourceBlock->getY());
			/*if (absPlus * 2 + 180 < OperRoute[i]->getSolnTime())
			{
			cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
			}*/
			if (absPlus * 2 + Util::startTime_BFS > OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
			}

			//cout << "(" << OperRoute[i]->getBlock()->getX() << "," << OperRoute[i]->getBlock()->getY() << ")Solntime is" << OperRoute[i]->getSolnTime() << " and the wind that time is" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << "->" << endl;

			if (OperRoute[i]->getSolnTime() != Util::startTime_BFS)
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getAvgWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60) << "\t" << OperRoute[i]->getBlock()->getNumOf_littleWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60, allRratio) << "\t" << OperRoute[i]->getBlock()->getViolations();
				if (OperRoute[i]->getBlock()->getNumOf_littleWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60, allRratio) == 8 || OperRoute[i]->getBlock()->getNumOf_littleWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60, allRratio) == 9)
				{
					vector<int > sayNoIndex = OperRoute[i]->getBlock()->getSayNoModelId((OperRoute[i]->getSolnTime() - Util::flyTime) / 60, allRratio);
					for (int sni = 0; sni<sayNoIndex.size(); sni++)
					{
						cout << "\t" << sayNoIndex[sni];
					}
				}
				cout << endl;
			}
			else
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getAvgWind(OperRoute[i]->getSolnTime() / 60) << "\t" << OperRoute[i]->getBlock()->getNumOf_littleWind(OperRoute[i]->getSolnTime() / 60, allRratio) << "\t" << OperRoute[i]->getBlock()->getViolations() << endl;
			}

		}
		cout << endl;
		cout << endl;
	}


	return OperRoute;
}


//backtrack
vector<OperBlock *> BFS::solve_backtrack_allR(Block * targetBlock, int allowNumOf_littleWind, double allRratio)
{
	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_sourceBlock->setMyOperBlock(sourceOperBlock);
	_ingOperBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();

	int targetSolnTime = Util::maxTime;
	bool findTheTarget = false;


	while (!_ingOperBlocks.empty() && _ingOperBlocks.front()->getIngTime()<targetSolnTime && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			//cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime << " and SolnTime is " << ingOperBlock->getSolnTime() << endl;
			bool allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock_allR(cangoto, thisTime, allowNumOf_littleWind, allRratio))
				{
					if (cangoto->getX() == targetX && cangoto->getY() == targetY && cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						//cangoto->setViolations(getNum_Violations_allR(ingOperBlock, cangoto, thisTime, allowNumOf_littleWind, allRratio));
						OperBlock * targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						targetOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(targetOperBlock);
						_ingOperBlocks.push(targetOperBlock);
						targetSolnTime = thisTime + Util::flyTime;
						findTheTarget = true;
					}
					else
					{

						if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
						{
							chooseBestWind_forAllR(ingOperBlock, cangoto, thisTime, allowNumOf_littleWind, allRratio);
						}
						else if (cangoto->getSituation() == 0)
							//if (cangoto->getSituation() == 0)
						{
							cangoto->setSituation(1);
							//cangoto->setViolations(getNum_Violations_allR(ingOperBlock, cangoto, thisTime, allowNumOf_littleWind, allRratio));
							OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
							cangotoOperBlock->setFront(ingOperBlock);
							cangoto->setMyOperBlock(cangotoOperBlock);
							_ingOperBlocks.push(cangotoOperBlock);
						}

					}
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{
				if (cangoto->getSituation() == 0)
				{
					allCangoToOper = false;
					break;
				}
			}

			if (allCangoToOper)
			{
				_vistedOperBlocks.push_back(ingOperBlock);
				ingOperBlock->getBlock()->setSituation(2);
				_ingOperBlocks.pop();
			}
			//判断停留的时候是否不坠机
			else
			{
				//if (ingOperBlock->getBlock()->getNumOf_littleWind(thisTime / 60, allRratio) >= allowNumOf_littleWind )
				if (ingOperBlock->cangotoThisBlock_allR(ingOperBlock->getBlock(), thisTime, allowNumOf_littleWind, allRratio))
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				//backtrack的重点！
				else
				{	
					//如果原点延迟太多，就应该坠毁了，无法再回溯
					if (ingOperBlock->getX() == _sourceBlock->getX() && ingOperBlock->getY() == _sourceBlock->getY())
					{
						cout << "!!!!!   souceBlock can't start from the hour at this " << thisTime << " time!!!!!!!" << endl;
						_ingOperBlocks.pop();
					}
					else
					{
						//首先拿到前一个operBlock的状态
						int frontSituation = ingOperBlock->getFront()->getBlock()->getSituation();
						//分析过了，如果是ing或者0，0的之前回溯过了，ing的待会自然会轮到它的自身判断，因此都不用理
						if (frontSituation == 2)
						{
							OperBlock * frontOpBlock = ingOperBlock->getFront();
							bool backTrackStop = false;
							while (!backTrackStop && frontOpBlock->getBlock()->getSituation() ==2 )
							{
								//cout << "  !!!!!!!!! back track to (" << frontOpBlock->getX() << "," << frontOpBlock->getY() << ") !!!!!!!!! " << endl;
								
								//if (frontOpBlock->getBlock()->getNumOf_littleWind(thisTime / 60, allRratio) >= allowNumOf_littleWind)
								if (frontOpBlock->cangotoThisBlock_allR(frontOpBlock->getBlock(), thisTime, allowNumOf_littleWind, allRratio))
								{
									frontOpBlock->getBlock()->setSituation(1);
									frontOpBlock->setIngTime(thisTime + Util::flyTime);
									_ingOperBlocks.push(frontOpBlock);
									backTrackStop = true;
								}
								//如果前者还是因为停留坠机
								else
								{
									//由于原点出发的时候是特赦的 ，所以这个地方容易死锁，若是原点，则直接允许变成ing（有没有必要？认真想想
									if (frontOpBlock->getX() == _sourceBlock->getX() && frontOpBlock->getY() == _sourceBlock->getY())
									{
										frontOpBlock->getBlock()->setSituation(1);
										frontOpBlock->setIngTime(thisTime + Util::flyTime);
										_ingOperBlocks.push(frontOpBlock);
										backTrackStop = true;
									}
									else
									{
										frontOpBlock->getBlock()->setSituation(0);
										OperBlock * nullOper = NULL;
										frontOpBlock->getBlock()->setMyOperBlock(nullOper);
										//循环下去,
										frontOpBlock = frontOpBlock->getFront();
										if (frontOpBlock == NULL)
										{
											cout << " !!!!!! a bug exists in backtrack !!!!!!!" << endl;
										}
										//用完之后不能再设为空，否则二对一的时候会报错
										//frontOper = NULL;
									}

								}
							}
						}

						//回溯完毕之后将自己变成未访问，Oper关系解除，但oper不能delete！这个回溯算法就是一对一，因为中间所有点的solnTime都丢失了
						//回溯算法的话中间的约束规则要比正常的BFS要紧，不然BFS总是先到，这个就没用了
						OperBlock * nullOper = NULL;
						ingOperBlock->getBlock()->setSituation(0);
						ingOperBlock->getBlock()->setMyOperBlock(nullOper);
						//ingOperBlock = NULL;
						_ingOperBlocks.pop();
					}
					
				}

			}

		}
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	vector<OperBlock *> OperRoute;
	if (findTheTarget)
	{
		// when print don't forget to inverted order
		OperBlock * targetOB = targetBlock->getMyOperBlock();
		OperRoute.push_back(targetOB);
		OperBlock * front = targetOB->getFront();
		while (front != NULL)
		{
			OperRoute.push_back(front);
			front = front->getFront();
		}
	}

	sort(OperRoute.begin(), OperRoute.end(), OperBlock::cmpBySolnTime);

	//print function
	if (OperRoute.empty())
	{
		cout << " shortest path failed ! Can't find the targetBlock when allow " << allowNumOf_littleWind << " !" << endl;
	}
	else
	{
		cout << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << targetBlock->getX() << "," << targetBlock->getY() << ") is :" << endl;
		cout << endl;
		cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "AvgWind" << "\t" << "rank" << "\t" << "violations" << endl;
		for (int i = 0; i <OperRoute.size(); i++)
		{
			//开挂的时候可能不是142 328
			int absPlus = abs(OperRoute[i]->getBlock()->getX() - _sourceBlock->getX()) + abs(OperRoute[i]->getBlock()->getY() - _sourceBlock->getY());
			/*if (absPlus * 2 + 180 < OperRoute[i]->getSolnTime())
			{
			cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
			}*/
			if (absPlus * 2 + Util::startTime_BFS > OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
			}

			//cout << "(" << OperRoute[i]->getBlock()->getX() << "," << OperRoute[i]->getBlock()->getY() << ")Solntime is" << OperRoute[i]->getSolnTime() << " and the wind that time is" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << "->" << endl;

			if (OperRoute[i]->getSolnTime() != Util::startTime_BFS)
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getAvgWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60) << "\t" << OperRoute[i]->getBlock()->getNumOf_littleWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60, allRratio) << "\t" << OperRoute[i]->getBlock()->getViolations();
				if (OperRoute[i]->getBlock()->getNumOf_littleWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60, allRratio) == 8 || OperRoute[i]->getBlock()->getNumOf_littleWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60, allRratio) == 9)
				{
					vector<int > sayNoIndex = OperRoute[i]->getBlock()->getSayNoModelId((OperRoute[i]->getSolnTime() - Util::flyTime) / 60,allRratio);
					for (int sni = 0; sni<sayNoIndex.size(); sni++)
					{
						cout << "\t" << sayNoIndex[sni];
					}
				}
				cout << endl;
			}
			else
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getAvgWind(OperRoute[i]->getSolnTime() / 60) << "\t" << OperRoute[i]->getBlock()->getNumOf_littleWind(OperRoute[i]->getSolnTime() / 60, allRratio) << "\t" << OperRoute[i]->getBlock()->getViolations() << endl;
			}

		}
		cout << endl;
		cout << endl;
	}


	return OperRoute;
}

vector<OperBlock *> BFS::solve_backtrack_CBNT(Block * targetBlock, int allowNumOf_littleWind, double allRratio)
{
	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_sourceBlock->setMyOperBlock(sourceOperBlock);
	_ingOperBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();

	int targetSolnTime = Util::maxTime;
	bool findTheTarget = false;


	while (!_ingOperBlocks.empty() && _ingOperBlocks.front()->getIngTime()<targetSolnTime && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			//cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime << " and SolnTime is " << ingOperBlock->getSolnTime() << endl;
			bool allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock_CBNT(cangoto, thisTime, allowNumOf_littleWind, allRratio))
				{
					if (cangoto->getX() == targetX && cangoto->getY() == targetY && cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						OperBlock * targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						targetOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(targetOperBlock);
						_ingOperBlocks.push(targetOperBlock);
						targetSolnTime = thisTime + Util::flyTime;
						findTheTarget = true;
					}
					else
					{

						if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
						{
							chooseBestWind_CBNT(ingOperBlock, cangoto, thisTime, allowNumOf_littleWind, allRratio);
						}
						else if (cangoto->getSituation() == 0)
						{
							cangoto->setSituation(1);
							OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
							cangotoOperBlock->setFront(ingOperBlock);
							cangoto->setMyOperBlock(cangotoOperBlock);
							_ingOperBlocks.push(cangotoOperBlock);
						}

					}
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{
				if (cangoto->getSituation() == 0)
				{
					allCangoToOper = false;
					break;
				}
			}

			if (allCangoToOper)
			{
				_vistedOperBlocks.push_back(ingOperBlock);
				ingOperBlock->getBlock()->setSituation(2);
				_ingOperBlocks.pop();
			}
			//判断停留的时候是否不坠机
			else
			{
				if (ingOperBlock->cangotoThisBlock_CBNT(ingOperBlock->getBlock(), thisTime, allowNumOf_littleWind, allRratio))
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				//backtrack的重点！
				else
				{
					//如果原点延迟太多，就应该坠毁了，无法再回溯
					if (ingOperBlock->getX() == _sourceBlock->getX() && ingOperBlock->getY() == _sourceBlock->getY())
					{
						cout << "!!!!!   souceBlock can't start from the hour at this " << thisTime << " time!!!!!!!" << endl;
						_ingOperBlocks.pop();
					}
					else
					{
						//首先拿到前一个operBlock的状态
						int frontSituation = ingOperBlock->getFront()->getBlock()->getSituation();
						//分析过了，如果是ing或者0，0的之前回溯过了，ing的待会自然会轮到它的自身判断，因此都不用理
						if (frontSituation == 2)
						{
							OperBlock * frontOpBlock = ingOperBlock->getFront();
							bool backTrackStop = false;
							while (!backTrackStop && frontOpBlock->getBlock()->getSituation() == 2)
							{
								//cout << "  !!!!!!!!! back track to (" << frontOpBlock->getX() << "," << frontOpBlock->getY() << ") !!!!!!!!! " << endl;

								if (frontOpBlock->cangotoThisBlock_CBNT(frontOpBlock->getBlock(), thisTime, allowNumOf_littleWind, allRratio))
								{
									frontOpBlock->getBlock()->setSituation(1);
									frontOpBlock->setIngTime(thisTime + Util::flyTime);
									_ingOperBlocks.push(frontOpBlock);
									backTrackStop = true;
								}
								//如果前者还是因为停留坠机
								else
								{
									//由于原点出发的时候是特赦的 ，所以这个地方容易死锁，若是原点，则直接允许变成ing（有没有必要？认真想想
									if (frontOpBlock->getX() == _sourceBlock->getX() && frontOpBlock->getY() == _sourceBlock->getY())
									{
										frontOpBlock->getBlock()->setSituation(1);
										frontOpBlock->setIngTime(thisTime + Util::flyTime);
										_ingOperBlocks.push(frontOpBlock);
										backTrackStop = true;
									}
									else
									{
										frontOpBlock->getBlock()->setSituation(0);
										OperBlock * nullOper = NULL;
										frontOpBlock->getBlock()->setMyOperBlock(nullOper);
										//循环下去,
										frontOpBlock = frontOpBlock->getFront();
										if (frontOpBlock == NULL)
										{
											cout << " !!!!!! a bug exists in backtrack !!!!!!!" << endl;
										}
										//用完之后不能再设为空，否则二对一的时候会报错
										//frontOper = NULL;
									}

								}
							}
						}

						//回溯完毕之后将自己变成未访问，Oper关系解除，但oper不能delete！这个回溯算法就是一对一，因为中间所有点的solnTime都丢失了
						//回溯算法的话中间的约束规则要比正常的BFS要紧，不然BFS总是先到，这个就没用了
						OperBlock * nullOper = NULL;
						ingOperBlock->getBlock()->setSituation(0);
						ingOperBlock->getBlock()->setMyOperBlock(nullOper);
						//ingOperBlock = NULL;
						_ingOperBlocks.pop();
					}

				}

			}

		}
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	vector<OperBlock *> OperRoute;
	if (findTheTarget)
	{
		// when print don't forget to inverted order
		OperBlock * targetOB = targetBlock->getMyOperBlock();
		OperRoute.push_back(targetOB);
		OperBlock * front = targetOB->getFront();
		while (front != NULL)
		{
			OperRoute.push_back(front);
			front = front->getFront();
		}
	}

	sort(OperRoute.begin(), OperRoute.end(), OperBlock::cmpBySolnTime);

	//print function
	if (OperRoute.empty())
	{
		cout << " shortest path failed ! Can't find the targetBlock when allow " << allowNumOf_littleWind << " !" << endl;
	}
	else
	{
		cout << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << targetBlock->getX() << "," << targetBlock->getY() << ") is :" << endl;
		cout << endl;
		cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "AvgWind" << "\t" << "rank" << "\t" << "violations" << endl;
		for (int i = 0; i <OperRoute.size(); i++)
		{
			//开挂的时候可能不是142 328
			int absPlus = abs(OperRoute[i]->getBlock()->getX() - _sourceBlock->getX()) + abs(OperRoute[i]->getBlock()->getY() - _sourceBlock->getY());
			/*if (absPlus * 2 + 180 < OperRoute[i]->getSolnTime())
			{
			cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
			}*/
			if (absPlus * 2 + Util::startTime_BFS > OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
			}

			//cout << "(" << OperRoute[i]->getBlock()->getX() << "," << OperRoute[i]->getBlock()->getY() << ")Solntime is" << OperRoute[i]->getSolnTime() << " and the wind that time is" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << "->" << endl;

			if (OperRoute[i]->getSolnTime() != Util::startTime_BFS)
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getCBNTavgWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60) << "\t" << OperRoute[i]->getBlock()->getCBNTvotes((OperRoute[i]->getSolnTime() - Util::flyTime) / 60, allRratio) << "\t" << OperRoute[i]->getBlock()->getViolations() << endl;				
			}
			else
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getCBNTavgWind(OperRoute[i]->getSolnTime() / 60) << "\t" << OperRoute[i]->getBlock()->getCBNTvotes(OperRoute[i]->getSolnTime() / 60, allRratio) << "\t" << OperRoute[i]->getBlock()->getViolations() << endl;
			}

		}
		cout << endl;
		cout << endl;
	}


	return OperRoute;
}

vector<OperBlock *> BFS::solve_backtrack_singleModel_evabyallR(Block * targetBlock, int allowNumOf_littleWind, double allRratio,double singleRatio)
{
	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_sourceBlock->setMyOperBlock(sourceOperBlock);
	_ingOperBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();

	int targetSolnTime = Util::maxTime;
	bool findTheTarget = false;


	while (!_ingOperBlocks.empty() && _ingOperBlocks.front()->getIngTime()<targetSolnTime && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			//cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime << " and SolnTime is " << ingOperBlock->getSolnTime() << endl;
			bool allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock_backtrack_evabyallR(cangoto, thisTime, allowNumOf_littleWind, allRratio,singleRatio))
				{
					if (cangoto->getX() == targetX && cangoto->getY() == targetY && cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						//cangoto->setViolations(getNum_Violations_allR(ingOperBlock, cangoto, thisTime, allowNumOf_littleWind, allRratio));
						OperBlock * targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						targetOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(targetOperBlock);
						_ingOperBlocks.push(targetOperBlock);
						targetSolnTime = thisTime + Util::flyTime;
						findTheTarget = true;
					}
					else
					{

						if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
						{
							chooseBest_backtrack_evabyallR(ingOperBlock, cangoto, thisTime, allowNumOf_littleWind, allRratio,singleRatio);
						}
						else if (cangoto->getSituation() == 0)
							//if (cangoto->getSituation() == 0)
						{
							cangoto->setSituation(1);
							//cangoto->setViolations(getNum_Violations_allR(ingOperBlock, cangoto, thisTime, allowNumOf_littleWind, allRratio));
							OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
							cangotoOperBlock->setFront(ingOperBlock);
							cangoto->setMyOperBlock(cangotoOperBlock);
							_ingOperBlocks.push(cangotoOperBlock);
						}

					}
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{
				if (cangoto->getSituation() == 0)
				{
					allCangoToOper = false;
					break;
				}
			}

			if (allCangoToOper)
			{
				_vistedOperBlocks.push_back(ingOperBlock);
				ingOperBlock->getBlock()->setSituation(2);
				_ingOperBlocks.pop();
			}
			//判断停留的时候是否不坠机
			else
			{
				if (ingOperBlock->cangotoThisBlock_backtrack_evabyallR(ingOperBlock->getBlock(), thisTime, allowNumOf_littleWind, allRratio,singleRatio))
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				//backtrack的重点！
				else
				{
					//如果原点延迟太多，就应该坠毁了，无法再回溯
					if (ingOperBlock->getX() == _sourceBlock->getX() && ingOperBlock->getY() == _sourceBlock->getY())
					{
						cout << "!!!!!   souceBlock can't start from the hour at this " << thisTime << " time!!!!!!!" << endl;
						_ingOperBlocks.pop();
					}
					else
					{
						//首先拿到前一个operBlock的状态
						int frontSituation = ingOperBlock->getFront()->getBlock()->getSituation();
						//分析过了，如果是ing或者0，0的之前回溯过了，ing的待会自然会轮到它的自身判断，因此都不用理
						if (frontSituation == 2)
						{
							OperBlock * frontOpBlock = ingOperBlock->getFront();
							bool backTrackStop = false;
							while (!backTrackStop && frontOpBlock->getBlock()->getSituation() == 2)
							{
								
								if (frontOpBlock->cangotoThisBlock_backtrack_evabyallR(frontOpBlock->getBlock(), thisTime, allowNumOf_littleWind, allRratio,singleRatio))
								{
									frontOpBlock->getBlock()->setSituation(1);
									frontOpBlock->setIngTime(thisTime + Util::flyTime);
									_ingOperBlocks.push(frontOpBlock);
									backTrackStop = true;
								}
								//如果前者还是因为停留坠机
								else
								{
									//由于原点出发的时候是特赦的 ，所以这个地方容易死锁，若是原点，则直接允许变成ing（有没有必要？认真想想
									if (frontOpBlock->getX() == _sourceBlock->getX() && frontOpBlock->getY() == _sourceBlock->getY())
									{
										frontOpBlock->getBlock()->setSituation(1);
										frontOpBlock->setIngTime(thisTime + Util::flyTime);
										_ingOperBlocks.push(frontOpBlock);
										backTrackStop = true;
									}
									else
									{
										frontOpBlock->getBlock()->setSituation(0);
										OperBlock * nullOper = NULL;
										frontOpBlock->getBlock()->setMyOperBlock(nullOper);
										//循环下去,
										frontOpBlock = frontOpBlock->getFront();
										if (frontOpBlock == NULL)
										{
											cout << " !!!!!! a bug exists in backtrack !!!!!!!" << endl;
										}
										//用完之后不能再设为空，否则二对一的时候会报错
										//frontOper = NULL;
									}

								}
							}
						}

						//回溯完毕之后将自己变成未访问，Oper关系解除，但oper不能delete！这个回溯算法就是一对一，因为中间所有点的solnTime都丢失了
						//回溯算法的话中间的约束规则要比正常的BFS要紧，不然BFS总是先到，这个就没用了
						OperBlock * nullOper = NULL;
						ingOperBlock->getBlock()->setSituation(0);
						ingOperBlock->getBlock()->setMyOperBlock(nullOper);
						//ingOperBlock = NULL;
						_ingOperBlocks.pop();
					}

				}

			}

		}
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	vector<OperBlock *> OperRoute;
	if (findTheTarget)
	{
		// when print don't forget to inverted order
		OperBlock * targetOB = targetBlock->getMyOperBlock();
		OperRoute.push_back(targetOB);
		OperBlock * front = targetOB->getFront();
		while (front != NULL)
		{
			OperRoute.push_back(front);
			front = front->getFront();
		}
	}

	sort(OperRoute.begin(), OperRoute.end(), OperBlock::cmpBySolnTime);

	//print function
	if (OperRoute.empty())
	{
		cout << " shortest path failed ! Can't find the targetBlock when allow " << allowNumOf_littleWind << " !" << endl;
	}
	else
	{
		cout << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << targetBlock->getX() << "," << targetBlock->getY() << ") is :" << endl;
		cout << endl;
		cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "AvgWind" << "\t" << "vote" << "\t" << "singleWind" << endl;
		for (int i = 0; i <OperRoute.size(); i++)
		{
			//开挂的时候可能不是142 328
			int absPlus = abs(OperRoute[i]->getBlock()->getX() - _sourceBlock->getX()) + abs(OperRoute[i]->getBlock()->getY() - _sourceBlock->getY());
			/*if (absPlus * 2 + 180 < OperRoute[i]->getSolnTime())
			{
			cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
			}*/
			if (absPlus * 2 + Util::startTime_BFS > OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
			}

			//cout << "(" << OperRoute[i]->getBlock()->getX() << "," << OperRoute[i]->getBlock()->getY() << ")Solntime is" << OperRoute[i]->getSolnTime() << " and the wind that time is" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << "->" << endl;

			if (OperRoute[i]->getSolnTime() != Util::startTime_BFS)
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getAvgWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60) << "\t" << OperRoute[i]->getBlock()->getNumOf_littleWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60, allRratio) << "\t" << OperRoute[i]->getBlock()->getWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60) << endl;
			}
			else
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getAvgWind(OperRoute[i]->getSolnTime() / 60) << "\t" << OperRoute[i]->getBlock()->getNumOf_littleWind(OperRoute[i]->getSolnTime() / 60, allRratio) << "\t" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << endl;
			}

		}
		cout << endl;
		cout << endl;
	}


	return OperRoute;
}




// season 2 
void BFS::chooseBest_backtrack_single_rain(OperBlock * ingOp, Block *cango, int thistime)
{
	//先判断时间是否对的上
	if ((thistime + 2) == cango->getMyOperBlock()->getSolnTime())
	{
		double thisWind = ingOp->getBlock()->getWind(thistime / 60);
		double nextWind = cango->getMyOperBlock()->getFront()->getBlock()->getWind(thistime / 60);

		double thisRain = ingOp->getBlock()->getRain(thistime / 60);
		double nextRain = cango->getMyOperBlock()->getFront()->getBlock()->getRain(thistime / 60);

		double thisCmpRatio = thisWind / 15.0 + thisRain / 4.0;
		double nextCmpRatio = nextWind / 15.0 + nextRain / 4.0;

		if (thisCmpRatio < nextCmpRatio)
		{
			//cout << "--------------------------change wind byCmpRatio------------------------" << endl;
			cango->getMyOperBlock()->setFront(ingOp);
		}
	}
}

vector<OperBlock *> BFS::solve_backtrack_single_rain(Block * targetBlock, double singleWindRatio, double singleRainRatio)
{
	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_sourceBlock->setMyOperBlock(sourceOperBlock);
	_ingOperBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();

	int targetSolnTime = Util::maxTime;
	bool findTheTarget = false;


	while (!_ingOperBlocks.empty() && _ingOperBlocks.front()->getIngTime()<targetSolnTime && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			//cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime << " and SolnTime is " << ingOperBlock->getSolnTime() << endl;
			bool allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock_backtrack_single_rain(cangoto, thisTime,singleWindRatio, singleRainRatio))
				{
					if (cangoto->getX() == targetX && cangoto->getY() == targetY && cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						OperBlock * targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						targetOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(targetOperBlock);
						_ingOperBlocks.push(targetOperBlock);
						targetSolnTime = thisTime + Util::flyTime;
						findTheTarget = true;
					}
					else
					{

						if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
						{
							chooseBest_backtrack_single_rain(ingOperBlock, cangoto, thisTime);
						}
						else if (cangoto->getSituation() == 0)
						{
							cangoto->setSituation(1);
							OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
							cangotoOperBlock->setFront(ingOperBlock);
							cangoto->setMyOperBlock(cangotoOperBlock);
							_ingOperBlocks.push(cangotoOperBlock);
						}

					}
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{
				if (cangoto->getSituation() == 0)
				{
					allCangoToOper = false;
					break;
				}
			}

			if (allCangoToOper)
			{
				_vistedOperBlocks.push_back(ingOperBlock);
				ingOperBlock->getBlock()->setSituation(2);
				_ingOperBlocks.pop();
			}
			//判断停留的时候是否不坠机
			else
			{
				if (ingOperBlock->cangotoThisBlock_backtrack_single_rain(ingOperBlock->getBlock(), thisTime, singleWindRatio, singleRainRatio))
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				//backtrack的重点！
				else
				{
					//如果原点延迟太多，就应该坠毁了，无法再回溯
					if (ingOperBlock->getX() == _sourceBlock->getX() && ingOperBlock->getY() == _sourceBlock->getY())
					{
						cout << "!!!!!   souceBlock can't start from the hour at this " << thisTime << " time!!!!!!!" << endl;
						_ingOperBlocks.pop();
					}
					else
					{
						//首先拿到前一个operBlock的状态
						int frontSituation = ingOperBlock->getFront()->getBlock()->getSituation();
						//分析过了，如果是ing或者0，0的之前回溯过了，ing的待会自然会轮到它的自身判断，因此都不用理
						if (frontSituation == 2)
						{
							OperBlock * frontOpBlock = ingOperBlock->getFront();
							bool backTrackStop = false;
							while (!backTrackStop && frontOpBlock->getBlock()->getSituation() == 2)
							{

								if (frontOpBlock->cangotoThisBlock_backtrack_single_rain(frontOpBlock->getBlock(), thisTime, singleWindRatio, singleRainRatio))
								{
									frontOpBlock->getBlock()->setSituation(1);
									frontOpBlock->setIngTime(thisTime + Util::flyTime);
									_ingOperBlocks.push(frontOpBlock);
									backTrackStop = true;
								}
								//如果前者还是因为停留坠机
								else
								{
									//由于原点出发的时候是特赦的 ，所以这个地方容易死锁，若是原点，则直接允许变成ing（有没有必要？认真想想
									if (frontOpBlock->getX() == _sourceBlock->getX() && frontOpBlock->getY() == _sourceBlock->getY())
									{
										frontOpBlock->getBlock()->setSituation(1);
										frontOpBlock->setIngTime(thisTime + Util::flyTime);
										_ingOperBlocks.push(frontOpBlock);
										backTrackStop = true;
									}
									else
									{	
										//cout << "backtrack!!" << endl;
										frontOpBlock->getBlock()->setSituation(0);
										OperBlock * nullOper = NULL;
										frontOpBlock->getBlock()->setMyOperBlock(nullOper);
										//循环下去,
										frontOpBlock = frontOpBlock->getFront();
										if (frontOpBlock == NULL)
										{
											cout << " !!!!!! a bug exists in backtrack !!!!!!!" << endl;
										}
										//用完之后不能再设为空，否则二对一的时候会报错
										//frontOper = NULL;
									}

								}
							}
						}

						//回溯完毕之后将自己变成未访问，Oper关系解除，但oper不能delete！这个回溯算法就是一对一，因为中间所有点的solnTime都丢失了
						//回溯算法的话中间的约束规则要比正常的BFS要紧，不然BFS总是先到，这个就没用了
						OperBlock * nullOper = NULL;
						ingOperBlock->getBlock()->setSituation(0);
						ingOperBlock->getBlock()->setMyOperBlock(nullOper);
						//ingOperBlock = NULL;
						_ingOperBlocks.pop();
					}

				}

			}

		}
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	vector<OperBlock *> OperRoute;
	if (findTheTarget)
	{
		// when print don't forget to inverted order
		OperBlock * targetOB = targetBlock->getMyOperBlock();
		OperRoute.push_back(targetOB);
		OperBlock * front = targetOB->getFront();
		while (front != NULL)
		{
			OperRoute.push_back(front);
			front = front->getFront();
		}
	}

	sort(OperRoute.begin(), OperRoute.end(), OperBlock::cmpBySolnTime);

	//print function
	if (OperRoute.empty())
	{
		cout << " shortest path failed ! Can't find the targetBlock when allow wind " << singleWindRatio << " and " << " rain "<<singleRainRatio<<endl;
	}
	else
	{
		cout << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << targetBlock->getX() << "," << targetBlock->getY() << ") is :" << endl;
		cout << endl;
		cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "wind" << "\t" << "rain" << endl;
		for (int i = 0; i <OperRoute.size(); i++)
		{
			int absPlus = abs(OperRoute[i]->getBlock()->getX() - _sourceBlock->getX()) + abs(OperRoute[i]->getBlock()->getY() - _sourceBlock->getY());
			if (absPlus * 2 + Util::startTime_BFS > OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
			}

			if (OperRoute[i]->getSolnTime() != Util::startTime_BFS)
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60) << "\t" << OperRoute[i]->getBlock()->getRain((OperRoute[i]->getSolnTime() - Util::flyTime) / 60) <<  endl;
			}
			else
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << "\t" << OperRoute[i]->getBlock()->getRain(OperRoute[i]->getSolnTime() / 60) <<  endl;
			}

		}
		cout << endl;
		cout << endl;
	}


	return OperRoute;
	
}

bool BFS::solve_backtrack_single_rain_logs(Block * targetBlock, double singleWindRatio, double singleRainRatio,int cityId)
{
	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_sourceBlock->setMyOperBlock(sourceOperBlock);
	_ingOperBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();

	int targetSolnTime = Util::maxTime;
	bool findTheTarget = false;


	while (!_ingOperBlocks.empty() && _ingOperBlocks.front()->getIngTime()<targetSolnTime && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			//cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime << " and SolnTime is " << ingOperBlock->getSolnTime() << endl;
			bool allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock_backtrack_single_rain(cangoto, thisTime, singleWindRatio, singleRainRatio))
				{
					if (cangoto->getX() == targetX && cangoto->getY() == targetY && cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						OperBlock * targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						targetOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(targetOperBlock);
						_ingOperBlocks.push(targetOperBlock);
						targetSolnTime = thisTime + Util::flyTime;
						findTheTarget = true;
					}
					else
					{

						if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
						{
							chooseBest_backtrack_single_rain(ingOperBlock, cangoto, thisTime);
						}
						else if (cangoto->getSituation() == 0)
						{
							cangoto->setSituation(1);
							OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
							cangotoOperBlock->setFront(ingOperBlock);
							cangoto->setMyOperBlock(cangotoOperBlock);
							_ingOperBlocks.push(cangotoOperBlock);
						}

					}
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{
				if (cangoto->getSituation() == 0)
				{
					allCangoToOper = false;
					break;
				}
			}

			if (allCangoToOper)
			{
				_vistedOperBlocks.push_back(ingOperBlock);
				ingOperBlock->getBlock()->setSituation(2);
				_ingOperBlocks.pop();
			}
			//判断停留的时候是否不坠机
			else
			{
				if (ingOperBlock->cangotoThisBlock_backtrack_single_rain(ingOperBlock->getBlock(), thisTime, singleWindRatio, singleRainRatio))
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				//backtrack的重点！
				else
				{
					//如果原点延迟太多，就应该坠毁了，无法再回溯
					if (ingOperBlock->getX() == _sourceBlock->getX() && ingOperBlock->getY() == _sourceBlock->getY())
					{
						//cout << "!!!!!   souceBlock can't start from the hour at this " << thisTime << " time!!!!!!!" << endl;
						_ingOperBlocks.pop();
					}
					else
					{
						//首先拿到前一个operBlock的状态
						int frontSituation = ingOperBlock->getFront()->getBlock()->getSituation();
						//分析过了，如果是ing或者0，0的之前回溯过了，ing的待会自然会轮到它的自身判断，因此都不用理
						if (frontSituation == 2)
						{
							OperBlock * frontOpBlock = ingOperBlock->getFront();
							bool backTrackStop = false;
							while (!backTrackStop && frontOpBlock->getBlock()->getSituation() == 2)
							{

								if (frontOpBlock->cangotoThisBlock_backtrack_single_rain(frontOpBlock->getBlock(), thisTime, singleWindRatio, singleRainRatio))
								{
									frontOpBlock->getBlock()->setSituation(1);
									frontOpBlock->setIngTime(thisTime + Util::flyTime);
									_ingOperBlocks.push(frontOpBlock);
									backTrackStop = true;
								}
								//如果前者还是因为停留坠机
								else
								{
									//由于原点出发的时候是特赦的 ，所以这个地方容易死锁，若是原点，则直接允许变成ing（有没有必要？认真想想
									if (frontOpBlock->getX() == _sourceBlock->getX() && frontOpBlock->getY() == _sourceBlock->getY())
									{
										frontOpBlock->getBlock()->setSituation(1);
										frontOpBlock->setIngTime(thisTime + Util::flyTime);
										_ingOperBlocks.push(frontOpBlock);
										backTrackStop = true;
									}
									else
									{
										//cout << "backtrack!!" << endl;
										frontOpBlock->getBlock()->setSituation(0);
										OperBlock * nullOper = NULL;
										frontOpBlock->getBlock()->setMyOperBlock(nullOper);
										//循环下去,
										frontOpBlock = frontOpBlock->getFront();
										if (frontOpBlock == NULL)
										{
											cout << " !!!!!! a bug exists in backtrack !!!!!!!" << endl;
										}
										//用完之后不能再设为空，否则二对一的时候会报错
										//frontOper = NULL;
									}

								}
							}
						}

						//回溯完毕之后将自己变成未访问，Oper关系解除，但oper不能delete！这个回溯算法就是一对一，因为中间所有点的solnTime都丢失了
						//回溯算法的话中间的约束规则要比正常的BFS要紧，不然BFS总是先到，这个就没用了
						OperBlock * nullOper = NULL;
						ingOperBlock->getBlock()->setSituation(0);
						ingOperBlock->getBlock()->setMyOperBlock(nullOper);
						//ingOperBlock = NULL;
						_ingOperBlocks.pop();
					}

				}

			}

		}
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	int solnTime;

	//高危风个数
	int Penalty1 = 0;
	//高危雨个数
	int Penalty2 = 0;
	//参数惩罚
	double Penalty3=singleWindRatio-14.5+singleRainRatio-3.75;
	//平均风
	double Penalty4 = 0.0;
	//平均雨
	double Penalty5 = 0.0;

	double sum_wind = 0.0;
	double sum_rain = 0.0;

	if (findTheTarget)
	{
		// when print don't forget to inverted order
		OperBlock * targetOB = targetBlock->getMyOperBlock();
		solnTime = targetOB->getSolnTime();
		if (targetOB->getBlock()->getWind((targetOB->getSolnTime()-Util::flyTime) / 60) > (singleWindRatio-0.5))
		{
			Penalty1++;
		}
		if (targetOB->getBlock()->getRain((targetOB->getSolnTime() - Util::flyTime) / 60) > (singleRainRatio - 0.25))
		{
			Penalty2++;
		}
		OperBlock * front = targetOB->getFront();
		while (front != NULL)
		{		
			if (front->getSolnTime() != Util::startTime_BFS)
			{
				if (front->getBlock()->getWind((front->getSolnTime()-Util::flyTime) / 60) > (singleWindRatio - 0.5))
				{
					Penalty1++;
				}
				if (front->getBlock()->getRain((front->getSolnTime() - Util::flyTime) / 60) > (singleRainRatio - 0.25))
				{
					Penalty2++;
				}
				sum_wind += targetOB->getBlock()->getWind((front->getSolnTime() - Util::flyTime) / 60);
				sum_rain += targetOB->getBlock()->getRain((front->getSolnTime() - Util::flyTime) / 60);
			}
			else
			{
				if (front->getBlock()->getWind(front->getSolnTime()  / 60) > (singleWindRatio - 0.5))
				{
					Penalty1++;
				}
				if (front->getBlock()->getRain(front->getSolnTime() / 60) > (singleRainRatio - 0.25))
				{
					Penalty2++;
				}
				sum_wind += targetOB->getBlock()->getWind(front->getSolnTime() / 60);
				sum_rain += targetOB->getBlock()->getRain(front->getSolnTime() / 60);
			}

			front = front->getFront();
			
		}
		Penalty4 = 2*sum_wind  / (solnTime - Util::startTime_BFS);
		Penalty5 = 2*sum_rain  / (solnTime - Util::startTime_BFS);

		cout << cityId << "\t" << Util::startTime_BFS << "\t" << solnTime - Util::startTime_BFS << "\t" << Penalty1 << "\t" << Penalty2 << "\t" << Penalty3 << "\t" << Penalty4 << "\t" << Penalty5 << endl;
		return true;
	}
	else
	{
		return false;
	}
	

}


void BFS::chooseBest_backtrack_all_rain(OperBlock * ingOp, Block *cango, int thistime)
{
	//先判断时间是否对的上,纯看avg的模式
	if ((thistime + 2) == cango->getMyOperBlock()->getSolnTime())
	{	
		int thisWindvote = ingOp->getBlock()->getWindVote(thistime / 60,15.0);
		int nextWindvote = cango->getMyOperBlock()->getFront()->getBlock()->getWindVote(thistime / 60,15.0);

		int thisRainvote = ingOp->getBlock()->getRainVote(thistime / 60,4.0);
		int nextRainvote = cango->getMyOperBlock()->getFront()->getBlock()->getRainVote(thistime / 60,4.0);

		int thisCmpvote = thisWindvote + thisRainvote ;
		int nextCmpvote = nextWindvote + nextRainvote ;

		if (thisCmpvote > nextCmpvote)
		{
			//cout << "--------------------------change wind byVote------------------------" << endl;
			cango->getMyOperBlock()->setFront(ingOp);
		}

		else if (thisCmpvote == nextCmpvote)
		{
			double thisWindAvg = ingOp->getBlock()->getWind(thistime / 60);
			double nextWindAvg = cango->getMyOperBlock()->getFront()->getBlock()->getWind(thistime / 60);

			double thisRainAvg = ingOp->getBlock()->getRain(thistime / 60);
			double nextRainAvg = cango->getMyOperBlock()->getFront()->getBlock()->getRain(thistime / 60);

			double thisCmpRatio = thisWindAvg + thisRainAvg * 15 / 4;
			double nextCmpRatio = nextWindAvg + nextRainAvg * 15 / 4;

			if (thisCmpRatio < nextCmpRatio)
			{
				cout << "--------------------------change wind byRatio------------------------" << endl;
				cango->getMyOperBlock()->setFront(ingOp);
			}
		}
	
	}
}

vector<OperBlock *> BFS::solve_backtrack_all_rain(Block * targetBlock, double allWindRatio, double allRainRatio, int windvote, int rainvote)
{
	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_sourceBlock->setMyOperBlock(sourceOperBlock);
	_ingOperBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();

	int targetSolnTime = Util::maxTime;
	bool findTheTarget = false;


	while (!_ingOperBlocks.empty() && _ingOperBlocks.front()->getIngTime()<targetSolnTime && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			//cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime << " and SolnTime is " << ingOperBlock->getSolnTime() << endl;
			bool allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock_backtrack_all_rain(cangoto, thisTime, allWindRatio, allRainRatio, windvote, rainvote))
				{
					if (cangoto->getX() == targetX && cangoto->getY() == targetY && cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						OperBlock * targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						targetOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(targetOperBlock);
						_ingOperBlocks.push(targetOperBlock);
						targetSolnTime = thisTime + Util::flyTime;
						findTheTarget = true;
					}
					else
					{

						if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
						{
							chooseBest_backtrack_all_rain(ingOperBlock, cangoto, thisTime);
						}
						else if (cangoto->getSituation() == 0)
						{
							cangoto->setSituation(1);
							OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
							cangotoOperBlock->setFront(ingOperBlock);
							cangoto->setMyOperBlock(cangotoOperBlock);
							_ingOperBlocks.push(cangotoOperBlock);
						}

					}
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{
				if (cangoto->getSituation() == 0)
				{
					allCangoToOper = false;
					break;
				}
			}

			if (allCangoToOper)
			{
				_vistedOperBlocks.push_back(ingOperBlock);
				ingOperBlock->getBlock()->setSituation(2);
				_ingOperBlocks.pop();
			}
			//判断停留的时候是否不坠机
			else
			{
				if (ingOperBlock->cangotoThisBlock_backtrack_all_rain(ingOperBlock->getBlock(), thisTime, allWindRatio, allRainRatio, windvote,  rainvote))
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				//backtrack的重点！
				else
				{
					//如果原点延迟太多，就应该坠毁了，无法再回溯
					if (ingOperBlock->getX() == _sourceBlock->getX() && ingOperBlock->getY() == _sourceBlock->getY())
					{
						cout << "!!!!!   souceBlock can't start from the hour at this " << thisTime << " time!!!!!!!" << endl;
						_ingOperBlocks.pop();
					}
					else
					{
						//首先拿到前一个operBlock的状态
						int frontSituation = ingOperBlock->getFront()->getBlock()->getSituation();
						//分析过了，如果是ing或者0，0的之前回溯过了，ing的待会自然会轮到它的自身判断，因此都不用理
						if (frontSituation == 2)
						{
							OperBlock * frontOpBlock = ingOperBlock->getFront();
							bool backTrackStop = false;
							while (!backTrackStop && frontOpBlock->getBlock()->getSituation() == 2)
							{

								if (frontOpBlock->cangotoThisBlock_backtrack_all_rain(frontOpBlock->getBlock(), thisTime, allWindRatio, allRainRatio, windvote, rainvote))
								{
									frontOpBlock->getBlock()->setSituation(1);
									frontOpBlock->setIngTime(thisTime + Util::flyTime);
									_ingOperBlocks.push(frontOpBlock);
									backTrackStop = true;
								}
								//如果前者还是因为停留坠机
								else
								{
									//由于原点出发的时候是特赦的 ，所以这个地方容易死锁，若是原点，则直接允许变成ing（有没有必要？认真想想
									if (frontOpBlock->getX() == _sourceBlock->getX() && frontOpBlock->getY() == _sourceBlock->getY())
									{
										frontOpBlock->getBlock()->setSituation(1);
										frontOpBlock->setIngTime(thisTime + Util::flyTime);
										_ingOperBlocks.push(frontOpBlock);
										backTrackStop = true;
									}
									else
									{
										//cout << "backtrack!!" << endl;
										frontOpBlock->getBlock()->setSituation(0);
										OperBlock * nullOper = NULL;
										frontOpBlock->getBlock()->setMyOperBlock(nullOper);
										//循环下去,
										frontOpBlock = frontOpBlock->getFront();
										if (frontOpBlock == NULL)
										{
											cout << " !!!!!! a bug exists in backtrack !!!!!!!" << endl;
										}
										//用完之后不能再设为空，否则二对一的时候会报错
										//frontOper = NULL;
									}

								}
							}
						}

						//回溯完毕之后将自己变成未访问，Oper关系解除，但oper不能delete！这个回溯算法就是一对一，因为中间所有点的solnTime都丢失了
						//回溯算法的话中间的约束规则要比正常的BFS要紧，不然BFS总是先到，这个就没用了
						OperBlock * nullOper = NULL;
						ingOperBlock->getBlock()->setSituation(0);
						ingOperBlock->getBlock()->setMyOperBlock(nullOper);
						//ingOperBlock = NULL;
						_ingOperBlocks.pop();
					}

				}

			}

		}
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	vector<OperBlock *> OperRoute;
	if (findTheTarget)
	{
		// when print don't forget to inverted order
		OperBlock * targetOB = targetBlock->getMyOperBlock();
		OperRoute.push_back(targetOB);
		OperBlock * front = targetOB->getFront();
		while (front != NULL)
		{
			OperRoute.push_back(front);
			front = front->getFront();
		}
	}

	sort(OperRoute.begin(), OperRoute.end(), OperBlock::cmpBySolnTime);

	//print function
	if (OperRoute.empty())
	{
		cout << " shortest path failed ! Can't find the targetBlock when allow wind " << allWindRatio << " and " << " rain " << allRainRatio << endl;
	}
	else
	{
		cout << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << targetBlock->getX() << "," << targetBlock->getY() << ") is :" << endl;
		cout << endl;
		cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "windavg" << "\t" << "windvote" << "\t" << "rainavg" << "\t" << "rainvote" << endl;
		for (int i = 0; i <OperRoute.size(); i++)
		{
			int absPlus = abs(OperRoute[i]->getBlock()->getX() - _sourceBlock->getX()) + abs(OperRoute[i]->getBlock()->getY() - _sourceBlock->getY());
			if (absPlus * 2 + Util::startTime_BFS > OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
			}

			if (OperRoute[i]->getSolnTime() != Util::startTime_BFS)
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getWindAvg((OperRoute[i]->getSolnTime() - Util::flyTime) / 60) << "\t"<< OperRoute[i]->getBlock()->getWindVote((OperRoute[i]->getSolnTime() - Util::flyTime) / 60,15.0) << "\t" << OperRoute[i]->getBlock()->getRainAvg((OperRoute[i]->getSolnTime() - Util::flyTime) / 60) << "\t" << OperRoute[i]->getBlock()->getRainVote((OperRoute[i]->getSolnTime() - Util::flyTime) / 60,4.0)<<endl;
			}
			else
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getWindAvg(OperRoute[i]->getSolnTime()  / 60) << "\t" << OperRoute[i]->getBlock()->getWindVote(OperRoute[i]->getSolnTime()  / 60, 15.0) << "\t" << OperRoute[i]->getBlock()->getRainAvg(OperRoute[i]->getSolnTime()  / 60) << "\t" << OperRoute[i]->getBlock()->getRainVote(OperRoute[i]->getSolnTime()  / 60, 4.0) << endl;
			}

		}
		cout << endl;
		cout << endl;
	}


	return OperRoute;

}

bool BFS::solve_backtrack_all_rain_logs(Block * targetBlock, double allWindRatio, double allRainRatio, int cityId, int windvote, int rainvote)
{
	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_sourceBlock->setMyOperBlock(sourceOperBlock);
	_ingOperBlocks.push(sourceOperBlock);

	//cout << Util::startTime_BFS << "windvote" << windvote << "rainvote" << endl;

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();

	int targetSolnTime = Util::maxTime;
	bool findTheTarget = false;


	while (!_ingOperBlocks.empty() && _ingOperBlocks.front()->getIngTime()<targetSolnTime && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			//cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime << " and SolnTime is " << ingOperBlock->getSolnTime() << endl;
			bool allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock_backtrack_all_rain(cangoto, thisTime, allWindRatio, allRainRatio, windvote, rainvote))
				{
					if (cangoto->getX() == targetX && cangoto->getY() == targetY && cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						OperBlock * targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						targetOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(targetOperBlock);
						_ingOperBlocks.push(targetOperBlock);
						targetSolnTime = thisTime + Util::flyTime;
						findTheTarget = true;
					}
					else
					{

						if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
						{
							chooseBest_backtrack_all_rain(ingOperBlock, cangoto, thisTime);
						}
						else if (cangoto->getSituation() == 0)
						{
							cangoto->setSituation(1);
							OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
							cangotoOperBlock->setFront(ingOperBlock);
							cangoto->setMyOperBlock(cangotoOperBlock);
							_ingOperBlocks.push(cangotoOperBlock);
						}

					}
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{
				if (cangoto->getSituation() == 0)
				{
					allCangoToOper = false;
					break;
				}
			}

			if (allCangoToOper)
			{
				_vistedOperBlocks.push_back(ingOperBlock);
				ingOperBlock->getBlock()->setSituation(2);
				_ingOperBlocks.pop();
			}
			//判断停留的时候是否不坠机
			else
			{
				if (ingOperBlock->cangotoThisBlock_backtrack_all_rain(ingOperBlock->getBlock(), thisTime, allWindRatio, allRainRatio, windvote, rainvote))
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				//backtrack的重点！
				else
				{
					//如果原点延迟太多，就应该坠毁了，无法再回溯
					if (ingOperBlock->getX() == _sourceBlock->getX() && ingOperBlock->getY() == _sourceBlock->getY())
					{
						//cout << "!!!!!   souceBlock can't start from the hour at this " << thisTime << " time!!!!!!!" << endl;
						_ingOperBlocks.pop();
					}
					else
					{
						//首先拿到前一个operBlock的状态
						int frontSituation = ingOperBlock->getFront()->getBlock()->getSituation();
						//分析过了，如果是ing或者0，0的之前回溯过了，ing的待会自然会轮到它的自身判断，因此都不用理
						if (frontSituation == 2)
						{
							OperBlock * frontOpBlock = ingOperBlock->getFront();
							bool backTrackStop = false;
							while (!backTrackStop && frontOpBlock->getBlock()->getSituation() == 2)
							{

								if (frontOpBlock->cangotoThisBlock_backtrack_all_rain(frontOpBlock->getBlock(), thisTime, allWindRatio, allRainRatio, windvote, rainvote))
								{
									frontOpBlock->getBlock()->setSituation(1);
									frontOpBlock->setIngTime(thisTime + Util::flyTime);
									_ingOperBlocks.push(frontOpBlock);
									backTrackStop = true;
								}
								//如果前者还是因为停留坠机
								else
								{
									//由于原点出发的时候是特赦的 ，所以这个地方容易死锁，若是原点，则直接允许变成ing（有没有必要？认真想想
									if (frontOpBlock->getX() == _sourceBlock->getX() && frontOpBlock->getY() == _sourceBlock->getY())
									{
										frontOpBlock->getBlock()->setSituation(1);
										frontOpBlock->setIngTime(thisTime + Util::flyTime);
										_ingOperBlocks.push(frontOpBlock);
										backTrackStop = true;
									}
									else
									{
										//cout << "backtrack!!" << endl;
										frontOpBlock->getBlock()->setSituation(0);
										OperBlock * nullOper = NULL;
										frontOpBlock->getBlock()->setMyOperBlock(nullOper);
										//循环下去,
										frontOpBlock = frontOpBlock->getFront();
										if (frontOpBlock == NULL)
										{
											cout << " !!!!!! a bug exists in backtrack !!!!!!!" << endl;
										}
										//用完之后不能再设为空，否则二对一的时候会报错
										//frontOper = NULL;
									}

								}
							}
						}

						//回溯完毕之后将自己变成未访问，Oper关系解除，但oper不能delete！这个回溯算法就是一对一，因为中间所有点的solnTime都丢失了
						//回溯算法的话中间的约束规则要比正常的BFS要紧，不然BFS总是先到，这个就没用了
						OperBlock * nullOper = NULL;
						ingOperBlock->getBlock()->setSituation(0);
						ingOperBlock->getBlock()->setMyOperBlock(nullOper);
						//ingOperBlock = NULL;
						_ingOperBlocks.pop();
					}

				}

			}

		}
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	int solnTime;

	//高危风个数
	int Penalty1 = 0;
	//高危雨个数
	int Penalty2 = 0;
	//投票惩罚
	int Penalty3 = 10 - windvote + 10 - rainvote;
	//平均风
	double Penalty4 = 0.0;
	//平均雨
	double Penalty5 = 0.0;

	double sum_wind = 0.0;
	double sum_rain = 0.0;


	if (findTheTarget)
	{
		OperBlock * targetOB = targetBlock->getMyOperBlock();
		solnTime = targetOB->getSolnTime();
		if (targetOB->getBlock()->getWindVote((targetOB->getSolnTime()-Util::flyTime) / 60,15.0) <10)
		{
			Penalty1++;
		}
		if (targetOB->getBlock()->getRainVote((targetOB->getSolnTime() - Util::flyTime) / 60,4.0) <10)
		{
			Penalty2++;
		}
		OperBlock * front = targetOB->getFront();
		while (front != NULL)
		{	
			if (front->getSolnTime() != Util::startTime_BFS)
			{
				if (front->getBlock()->getWindVote((front->getSolnTime()-Util::flyTime) / 60, 15.0) <10)
				{
					Penalty1++;
				}
				if (front->getBlock()->getRainVote((front->getSolnTime() - Util::flyTime) / 60, 4.0) <10)
				{
					Penalty2++;
				}
				sum_wind += targetOB->getBlock()->getWindAvg((front->getSolnTime() - Util::flyTime) / 60);
				sum_rain += targetOB->getBlock()->getRainAvg((front->getSolnTime() - Util::flyTime) / 60);
			}
			else
			{
				if (front->getBlock()->getWindVote(front->getSolnTime() / 60, 15.0) <10)
				{
					Penalty1++;
				}
				if (front->getBlock()->getRainVote(front->getSolnTime() / 60, 4.0) <10)
				{
					Penalty2++;
				}
				sum_wind += targetOB->getBlock()->getWindAvg(front->getSolnTime()  / 60);
				sum_rain += targetOB->getBlock()->getRainAvg(front->getSolnTime() / 60);
			}
					
			front = front->getFront();
			
		}
		Penalty4 = 2 * sum_wind / (solnTime - Util::startTime_BFS);
		Penalty5 = 2 * sum_rain / (solnTime - Util::startTime_BFS);

		cout << cityId << "\t" << Util::startTime_BFS << "\t" << solnTime - Util::startTime_BFS << "\t" << Penalty1 << "\t" << Penalty2 << "\t" << Penalty3 << "\t" << Penalty4 << "\t" << Penalty5 << endl;
		return true;
	}
	else
	{
		return false;
	}


}