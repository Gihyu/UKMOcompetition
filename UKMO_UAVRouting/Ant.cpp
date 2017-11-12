#include"Ant.h"


Ant::Ant()
{
	
}


Ant::~Ant()
{
	
}

//
//void Ant::search()
//{	
//	_path.clear();
//	stopCondition = false;
//	ChooseNextLeg();
//}
//
//
//void Ant::ChooseNextLeg()
//{	
//	
//	while(!stopCondition)
//	{	
//		int LegIndex = _path.size() ;		
//		vector<Leg *> canGoToLegs;
//		vector<Leg *> choicesLegs = _presentStation->getArrLegList();
//		if(_presentLeg->getDepTime ()>Util::ACO_STOPSIGNAL)
//		{						
//			for (int i = 0; i < choicesLegs.size(); i++)
//			{
//				if (!is_in_path(choicesLegs[i]) && choicesLegs[i]->getDepTime() > Util::ACO_STOPSIGNAL)
//				{
//					canGoToLegs.push_back(choicesLegs[i]);
//				}
//			}
//		}
//		else
//		{
//			for (auto & choice : choicesLegs)
//			{
//				if (!is_in_path(choice))
//				{
//					canGoToLegs.push_back(choice);
//				}
//			}
//		}
//		Leg * nextLeg = chooseOneLeg(canGoToLegs, LegIndex);
//		if (nextLeg != NULL)
//		{	
//			_presentLeg = nextLeg;	
//			//cout << "now the path size is " << _path.size() << endl;
//			//cout << "presentLeg is " << _presentLeg->getId() << endl;
//			_presentStation = _presentLeg->getArrStation();
//			_path.push_back(_presentLeg);
//		}
//		else
//		{
//			stopCondition = true;
//		}
//		
//		if ((_path.size()-_lockSize)== SUBLOFSIZE)
//		{
//			stopCondition = true;
//		}
//	}
//}
//
//
//
//Leg * Ant::chooseOneLeg(vector<Leg *> canGoToLegs,int LegIndex)
//{
//	if (canGoToLegs.size() == 0)
//	{
//		return NULL;
//	}
//	//¬÷≈Ã—°‘Ò
//	else
//	{			
//		double sumOfPheromone = 0.0;
//		int choiceSize = canGoToLegs.size();
//		for (int j = 0; j < choiceSize; j++)
//		{
//			sumOfPheromone += pow(canGoToLegs[j]->getPheromone(_threadIndex, LegIndex), Util::ALPHA)*pow(canGoToLegs[j]->getImportanceACO(), Util::BETA);
//		}
//		int IntSumOfPheromone = (int)(sumOfPheromone-1);
//		static default_random_engine f;
//		static uniform_int_distribution<time_t> x(0, IntSumOfPheromone);
//		double random = x(f) * 1.0;
//		//cout << "choice Size is"<< choiceSize <<" choice Leg are"<< "\t";
//		for (int k = 0; k < choiceSize; k++)
//		{	
//			//cout << canGoToLegs[k]->getId() << "\t" ;
//			if (k == choiceSize - 1)
//			{
//				//cout << endl;
//				//cout << "find it! " << canGoToLegs[k]->getId() << endl;
//				//Leg * nextLeg = canGoToLegs[k];
//				return canGoToLegs[k];
//			}
//			else
//			{
//				if (random - pow(canGoToLegs[k]->getPheromone(_threadIndex, LegIndex), Util::ALPHA)*pow(canGoToLegs[k]->getImportanceACO(), Util::BETA) < 0)
//				{
//					//cout << endl;
//					//cout << "find it! " << canGoToLegs[k]->getId() << endl;
//					//Leg * nextLeg = canGoToLegs[k];
//					return canGoToLegs[k];
//				}
//				else
//				{
//					random = random - pow(canGoToLegs[k]->getPheromone(_threadIndex, LegIndex), Util::ALPHA)*pow(canGoToLegs[k]->getImportanceACO(), Util::BETA);
//				}
//			}
//			
//			
//		}
//		
//		//return nextLeg;
//	}
//}
//
//bool Ant::is_in_path(Leg * wantoGo)
//{	
//	if (_path.size() == 0)
//	{
//		return false;
//	}
//	else
//	{
//		for (auto & have_gone : _path)
//		{
//			if (have_gone->getId() == wantoGo->getId())
//			{
//				return true;
//			}
//		}
//		return false;
//	}
//	
//}
//
//
//
//bool Ant::is_a_valid_choice(Leg * wantoGo)
//{
//
//}



