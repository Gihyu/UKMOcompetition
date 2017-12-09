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

	// 540 means start from 9:00
	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, 180);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	bool findTheFarmost = false;

	while (!_ingOperBlocks.empty() && !findTheFarmost && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
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

				    //˳������
					//��ײsourceBlock����getFrontΪNULL
					//firstReach����ע�͵�

					if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
					{				
						chooseLowestWind(ingOperBlock, cangoto, thisTime);
						//chooseHighestWind(ingOperBlock, cangoto, thisTime);
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
			//���ԭ��ͣ�� ��ô����Ҫ�ж�ͣ����ʱ���Ƿ���ٴ���initial ratio
			//�����ڣ�����Ϊ׹������(��visted����
			//����output��ʱ����ڵ�һ��Ⱥʽ������ʱ��Ҳû��ϵ����Ϊsoln time ��Ӧ�ķ��ٿ϶���С��initial ratio�ģ�
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
		//��cangotoΪ�� ��ôҲ�Ǳ�Ϊvisted
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

			for (int i =0; i<multiSoln[k].size(); i++)
			{
				int absPlus = abs(multiSoln[k][i]->getBlock()->getX() - 142) + abs(multiSoln[k][i]->getBlock()->getY() - 328);
				/*if (absPlus * 2 + 180 < multiSoln[k][i]->getSolnTime())
				{
					cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
				}*/
				if (absPlus * 2 + 180 > multiSoln[k][i]->getSolnTime())
				{
					cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
				}

				//cout << "(" << multiSoln[k][i]->getBlock()->getX() << "," << multiSoln[k][i]->getBlock()->getY() << ")Solntime is" << multiSoln[k][i]->getSolnTime() << "->" ;
				if (multiSoln[k][i]->getSolnTime()<1260)
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getWind(multiSoln[k][i]->getSolnTime() / 60) << endl;
				}
				else
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << endl;
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

	// 540 means start from 9:00
	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, 180);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();

	bool findTheTarget = false;
	OperBlock * targetOperBlock = NULL;


	while (!_ingOperBlocks.empty() && !findTheTarget && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
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
					if (cangoto->getX() == targetX && cangoto->getY() == targetY)
					{
						cangoto->setSituation(2);
						targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						targetOperBlock->setFront(ingOperBlock);
						_vistedOperBlocks.push_back(targetOperBlock);
						findTheTarget = true;
						break;
					}
					else
					{	

						if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
						{
							chooseLowestWind(ingOperBlock, cangoto, thisTime);
							//chooseHighestWind(ingOperBlock, cangoto, thisTime);
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
			//�ж�ͣ����ʱ���Ƿ�׹��
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
		OperRoute.push_back(targetOperBlock);
		OperBlock * front = targetOperBlock->getFront();
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

		for (int i = 0; i <OperRoute.size(); i++)
		{
			int absPlus = abs(OperRoute[i]->getBlock()->getX() - 142) + abs(OperRoute[i]->getBlock()->getY() - 328);
			/*if (absPlus * 2 + 180 < OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
			}*/
			if (absPlus * 2 + 180 > OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
			}

			//cout << "(" << OperRoute[i]->getBlock()->getX() << "," << OperRoute[i]->getBlock()->getY() << ")Solntime is" << OperRoute[i]->getSolnTime() << " and the wind that time is" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << "->" << endl;
			
			if (OperRoute[i]->getSolnTime()<1260)
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << endl;
			}
			else
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << endl;
			}
			
		}
		cout << endl;
		cout << endl;
	}


	return OperRoute;
}


void BFS::chooseLowestWind(OperBlock * oper, Block * blo, int thistime)
{	
	if (oper->getWind(thistime / 60) < blo->getMyOperBlock()->getFront()->getWind(thistime / 60))
	{	
		//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ing solntime " << oper->getSolnTime() << " myOperSolnTime " << blo->getMyOperBlock()->getFront()->getSolnTime() << " !!!!!!!!!!!!!!!" << endl;
		if (thistime == blo->getMyOperBlock()->getFront()->getSolnTime())
		{
			//cout << "--------------------------change------------------------" << endl;
			blo->getMyOperBlock()->setFront(oper);
		}

	}
}


void BFS::chooseHighestWind(OperBlock * oper, Block * blo, int thistime)
{
	if (oper->getWind(thistime / 60) > blo->getMyOperBlock()->getFront()->getWind(thistime / 60))
	{
		//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ing solntime " << oper->getSolnTime() << " myOperSolnTime " << blo->getMyOperBlock()->getFront()->getSolnTime() << " !!!!!!!!!!!!!!!" << endl;
		if (thistime == blo->getMyOperBlock()->getFront()->getSolnTime())
		{
			//cout << "--------------------------change------------------------" << endl;
			blo->getMyOperBlock()->setFront(oper);
		}

	}
}

void BFS::chooseBestWind_forAllR(OperBlock * oper, Block * blo, int thistime ,int allowNum)
{	
	//���ж�ʱ���Ƿ�һ��
	//if (thistime == blo->getMyOperBlock()->getFront()->getSolnTime() )
	if ( (thistime + 2) == blo->getMyOperBlock()->getSolnTime())
	{	
		int thisNumof_littleWind = oper->getBlock()->getNumOf_littleWind(thistime / 60);
		int nextNumof_littleWind = blo->getMyOperBlock()->getFront()->getBlock()->getNumOf_littleWind(thistime / 60);
		if (thisNumof_littleWind > nextNumof_littleWind)
		{
			//cout << "--------------------------change level------------------------" << endl;
			blo->getMyOperBlock()->setFront(oper);
			updateViolations_exchange(oper, blo, thistime, allowNum);
			
		}
		else if (thisNumof_littleWind == nextNumof_littleWind)
		{
			if (getNum_Violations(oper, blo, thistime, allowNum) < blo->getViolations())
			{	
				//cout << "--------------------------change violations------------------------" << endl;
				blo->getMyOperBlock()->setFront(oper);
				updateViolations_exchange(oper, blo, thistime, allowNum);
			}
			else if (getNum_Violations(oper, blo, thistime, allowNum) == blo->getViolations())
			{
				double thisAvgWind = oper->getBlock()->getAvgWind(thistime / 60);
				double nextAvgWind = blo->getMyOperBlock()->getFront()->getBlock()->getAvgWind(thistime / 60);
				if (thisAvgWind < nextAvgWind)
				{
					//cout << "--------------------------change wind------------------------" << endl;
					blo->getMyOperBlock()->setFront(oper);
					updateViolations_exchange(oper, blo, thistime, allowNum);
				}
			}
		
		}
	} 

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
				if (ingOperBlock->cangotoThisBlock_allR(cangoto, thisTime , Util::NumOf_littleWindForAllR))
				{

					//˳������
					//��ײsourceBlock����getFrontΪNULL
					//firstReach����ע�͵�

					if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
					{	
						//д11Ϊ����10��ʱ��������Ҽ�
						chooseBestWind_forAllR(ingOperBlock, cangoto, thisTime,11);
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
			//���ԭ��ͣ�� ��ô����Ҫ�ж�ͣ����ʱ���Ƿ���ٴ���initial ratio
			//�����ڣ�����Ϊ׹������(��visted����
			//����output��ʱ����ڵ�һ��Ⱥʽ������ʱ��Ҳû��ϵ����Ϊsoln time ��Ӧ�ķ��ٿ϶���С��initial ratio�ģ�
			else
			{
				if (ingOperBlock->getBlock()->getNumOf_littleWind(thisTime / 60) >=Util::NumOf_littleWindForAllR)
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
		//��cangotoΪ�� ��ôҲ�Ǳ�Ϊvisted
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
				int absPlus = abs(multiSoln[k][i]->getBlock()->getX() - 142) + abs(multiSoln[k][i]->getBlock()->getY() - 328);
				/*if (absPlus * 2 + 180 < multiSoln[k][i]->getSolnTime())
				{
				cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
				}*/
				if (absPlus * 2 + 180 > multiSoln[k][i]->getSolnTime())
				{
					cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
				}

				if (multiSoln[k][i]->getSolnTime() != Util::startTime_BFS)
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getAvgWind((multiSoln[k][i]->getSolnTime()- Util::flyTime) / 60) << "\t" << multiSoln[k][i]->getBlock()->getNumOf_littleWind((multiSoln[k][i]->getSolnTime()-Util::flyTime) / 60) << "\t" << multiSoln[k][i]->getBlock()->getViolations()<< endl;
				}
				else
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getAvgWind(multiSoln[k][i]->getSolnTime() / 60) << "\t" << multiSoln[k][i]->getBlock()->getNumOf_littleWind(multiSoln[k][i]->getSolnTime()  / 60) << "\t" << multiSoln[k][i]->getBlock()->getViolations()<< endl;
				}
				
			}
			cout << endl;
			cout << endl;
		}
	}

	return multiSoln;
}

vector<OperBlock *> BFS::solve_allR_singleTarget(Block * targetBlock, int allowNumOf_littleWind)
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
				if (ingOperBlock->cangotoThisBlock_allR(cangoto, thisTime, allowNumOf_littleWind))
				{
					if (cangoto->getX() == targetX && cangoto->getY() == targetY && cangoto->getSituation() == 0)
					{							
						cangoto->setSituation(1);
						cangoto->setViolations(getNum_Violations(ingOperBlock, cangoto, thisTime, allowNumOf_littleWind));
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
							chooseBestWind_forAllR(ingOperBlock, cangoto, thisTime, allowNumOf_littleWind);
						}
						else if (cangoto->getSituation() == 0)
							//if (cangoto->getSituation() == 0)
						{
							cangoto->setSituation(1);
							cangoto->setViolations(getNum_Violations(ingOperBlock, cangoto, thisTime, allowNumOf_littleWind));
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
			//�ж�ͣ����ʱ���Ƿ�׹��
			else
			{
				if (ingOperBlock->getBlock()->getNumOf_littleWind(thisTime / 60) >= allowNumOf_littleWind)
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
			int absPlus = abs(OperRoute[i]->getBlock()->getX() - 142) + abs(OperRoute[i]->getBlock()->getY() - 328);
			/*if (absPlus * 2 + 180 < OperRoute[i]->getSolnTime())
			{
			cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
			}*/
			if (absPlus * 2 + 180 > OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
			}

			//cout << "(" << OperRoute[i]->getBlock()->getX() << "," << OperRoute[i]->getBlock()->getY() << ")Solntime is" << OperRoute[i]->getSolnTime() << " and the wind that time is" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << "->" << endl;

			if (OperRoute[i]->getSolnTime() != Util::startTime_BFS)
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getAvgWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60) << "\t" << OperRoute[i]->getBlock()->getNumOf_littleWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60) << "\t" << OperRoute[i]->getBlock()->getViolations() << endl;
			}
			else
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getAvgWind(OperRoute[i]->getSolnTime() / 60) << "\t" << OperRoute[i]->getBlock()->getNumOf_littleWind(OperRoute[i]->getSolnTime() / 60) << "\t" << OperRoute[i]->getBlock()->getViolations() << endl;
			}

		}
		cout << endl;
		cout << endl;
	}


	return OperRoute;
}


void BFS::updateViolations_exchange(OperBlock * ingOB, Block* cgt, int thisTime, int allowNum)
{
	if (ingOB->getBlock()->getNumOf_littleWind(thisTime / 60) ==allowNum || cgt->getNumOf_littleWind(thisTime / 60) ==allowNum)
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

int BFS::getNum_Violations(OperBlock * ingOB, Block* cgt, int thisTime, int allowNum)
{
	if (ingOB->getBlock()->getNumOf_littleWind(thisTime / 60) == allowNum || cgt->getNumOf_littleWind(thisTime / 60) == allowNum)
	{
		return (ingOB->getBlock()->getViolations() + 1);
	}
	else
	{
		return ingOB->getBlock()->getViolations();
	}
}