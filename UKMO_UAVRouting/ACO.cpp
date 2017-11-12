#include"ACO.h"

ACO::ACO(Block * sourceBlock) :
	_sourceBlock(sourceBlock),_iteration(Util::maxIterationACO)
{

}




vector<OperBlock *> ACO::solve(Block * targetBlock)
{
	//int targetX = targetBlock->getX();
	//int targetY = targetBlock->getY();

	////初始化费洛蒙浓度 	
	//// maybe do this here maybe in schedule


	////生成蚁群且初始化colony
	//vector<Ant *>  colony;
	//for (int i = 0; i < ANTAMOUNT; i++)
	//{	
	//	Ant * ant = new Ant();
	//	ant->setLockSize(_lockLeg.size());
	//	ant->setThreadIndex(_thread);
	//	colony.push_back(ant);
	//}
	//_iteration = 0;
	//while(_iteration < Util::maxIterationForACO)
	//{	
	//	//cout << "now iteration is " << _iteration << endl;
	//	for(int i = 0; i<ANTAMOUNT; i++)
	//	{	
	//		//cout << "now iteration is " << _iteration << " and ant " << i << " is running" << endl;
	//		colony[i]->setBad();
	//		colony[i]->setPresentLeg (_startLeg);
	//		colony[i]->setPresentStation ( _startLeg->getArrStation());
	//		colony[i]->search();
	//		if(is_in_canGoTo(colony[i]->getPresentStation()))
	//		{
	//			//cout << "haha the size is "<<colony[i]->getPath().size() << endl;				
	//			vector<Leg *> tempLegList;
	//			if (has_lockLeg)
	//			{
	//				for (int k = 0; k < _lockLeg.size(); k++)
	//				{
	//					tempLegList.push_back(_lockLeg[k]);
	//				}
	//			}
	//			for(int j = 0; j <colony[i]->getPath().size(); j++)
	//			{
	//				tempLegList.push_back(colony[i]->getPath()[j]);
	//			}
	//			//七项check
	//			if (vaildToReTime(tempLegList))
	//			{	
	//				cout << "haha the size is "<<colony[i]->getPath().size() << endl;	
	//				Lof * rosterLof = _reTime->reTiming(_aircraft, tempLegList);
	//				if (rosterLof != NULL)
	//				{
	//					if (rosterLof->getReducedCost() < -0.00001)
	//					{	
	//						colony[i]->setGood();
	//						if (rosterLof->getReducedCost() < _avoidRepeat)
	//						{	
	//							colony[i]->setBest();
	//							rosterLof->setFoundBy(F_GA);
	//							_validLofList.push_back(rosterLof);
	//							_avoidRepeat = rosterLof->getReducedCost();
	//						}							
	//					}
	//				}
	//			}				
	//		}
	//	}

	//	updatePheromone( colony);
	//	_iteration++;
	//}	
	//return _validLofList;	
}




void ACO::updatePheromone(vector<Ant *> colony)
{
	for (int i = 0; i < ANTAMOUNT; i++)
	{	
		vector<Leg *> antPath = colony[i]->getPath();
		if (colony[i]->getRouteGrade() == 0)
		{
			for (int j = 0; j < antPath.size(); j++)
			{
				antPath[j]->setPheromone(_thread, j, antPath[j]->getPheromone(_thread, j) - Util::antPlusPheromoneGood);
			}
		}		
					
		else if (colony[i]->getRouteGrade() == 2)
		{
			for (int j = 0; j < antPath.size(); j++)
			{
				antPath[j]->setPheromone(_thread, j, antPath[j]->getPheromone(_thread, j) + Util::antPlusPheromoneBest);
			}
		}
		else if (colony[i]->getRouteGrade() == 1)
		{
			for (int j = 0; j < antPath.size(); j++)
			{
				antPath[j]->setPheromone(_thread, j, antPath[j]->getPheromone(_thread, j) + Util::antPlusPheromoneGood);
			}
		}
		
			
	}

	for (auto leg : _allLegList)
	{
		for (int legIndex = 0; legIndex < SUBLOFSIZE; legIndex++)
		{	
			double pastPheromone = leg->getPheromone(_thread, legIndex);
			//没去过的就不降低浓度了，保留他的吸引程度
			if (!pastPheromone == Util::initPheromone)
			{
				if (pastPheromone * Util::ROU > Util::maxPheromone)
				{
					leg->setPheromone(_thread, legIndex, Util::maxPheromone);
				}
				else if (pastPheromone * Util::ROU < Util::minPheromone)
				{
					leg->setPheromone(_thread, legIndex, Util::minPheromone);
				}
				else
				{
					leg->setPheromone(_thread, legIndex, pastPheromone * Util::ROU);
				}

			}
			
		}
	}
}


bool ACO::isInOperVector(Block * test, vector<OperBlock *> testVector)
{
	bool whetherIn = false;
	if (!testVector.empty())
	{
		int x = test->getX();
		int y = test->getY();
		for (auto & operBlock : testVector)
		{
			if (x == operBlock->getX() && y == operBlock->getY())
			{
				whetherIn = true;
				break;
			}
		}
	}
	return whetherIn;
}


//bool ACO::checkRepeat(vector<Leg *> legList)
//{
//	for (int i = 0; i < legList.size(); i++)
//	{
//		for (int j = i + 1; j < legList.size(); j++)
//		{
//			if (legList[i] == legList[j])
//			{
//				return false;
//			}
//		}
//	}
//
//	return true;
//}

