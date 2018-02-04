#include "Block.h"
int Block::_count = 0;

Block::Block(int x, int y, int date, array<double, Util::hourCount> windArr):
	_x(x),_y(y),_date(date),_windArr(windArr)
{
	_id = _count;
	_count++;
}

Block::Block(int x, int y, int date, array<array<double, 10>, Util::hourCount> windMatrix):
	_x(x), _y(y), _date(date), _windMatrix(windMatrix)
{
	_id = _count;
	_count++;
}

pair<int, int> Block::getCoordinate()
{
	pair<int, int> p;
	p.first = _x;
	p.second = _y;
	return p;
}

bool Block::equal(Block * block)
{
	if (block->getX() != _x)
	{
		return false;
	}

	if (block->getY() != _y)
	{
		return false;
	}

	return true;
}

bool Block::equal(int x, int y)
{
	if ( x != _x)
	{
		return false;
	}

	if ( y != _y)
	{
		return false;
	}

	return true;
}

void Block::print()
{
	cout << "Block_" << _id << "\tx:" << _x << "\ty:" << _y << "\tdate:" << _date << endl;
	switch (Util::inputMode)
	{
	case M_Single:
		printWindArr();
		printRainArr();
		break;
	case M_Multi:
		printWindMatrix();
		printRainMatrix();
		break;
	case M_SingleAndMulti:
		printWindArr();
		printRainArr();

		printWindMatrix();
		printRainMatrix();

		break;
	}
}

void Block::printWindArr()
{
	cout << "windArr:";
	for (auto&wind : _windArr)
	{
		cout << "\t" << wind;
	}
	cout << endl;
}

void Block::printWindMatrix()
{
	cout << "windMatrix:\n";
	for (auto&arr : _windMatrix)
	{
		for (auto&wind : arr)
		{
			cout << "\t" << wind;
		}
		cout << endl;
	}
}

void Block::printRainArr()
{
	cout << "rainArr:";
	for (auto&rain : _rainArr)
	{
		cout << "\t" << rain;
	}
	cout << endl;
}

void Block::printRainMatrix()
{
	cout << "rainMatrix:\n";
	for (auto&arr : _rainMatrix)
	{
		for (auto&rain : arr)
		{
			cout << "\t" << rain;
		}
		cout << endl;
	}
}

double Block::testgetWind(int thisTime)
{
	if(_time_wind.empty())
	{
		return 0.0;
	}
	else
	{	
		for (auto & time_wind : _time_wind)
		{
			if (thisTime == time_wind.first)
			{
				return time_wind.second;
			}
		}
		return 0.0;
	}
}



double Block::getAvgWind(int hour)
{
	array<double, 10> thisTimeWindArr = getWindAllRealization(hour);
	double sum = 0.0;
	double max = 0.0;
	double min = 100.0;
	for (int i = 0; i < 10; i++)
	{
		sum += thisTimeWindArr[i];
		if (thisTimeWindArr[i] > max)
		{
			max = thisTimeWindArr[i];
		}
		else if (thisTimeWindArr[i] < min)
		{
			min = thisTimeWindArr[i];
		}
	}
	double windAvg = (sum - max - min) / 8;
	return windAvg;
}

double Block::getWeightedAvgWind(int hour)
{	
	array<double, 10> thisTimeWindArr = getWindAllRealization(hour);

	double max = 0.0;
	double min = 100.0;
	int maxIndex;
	int minIndex;
	for (int i = 0; i < 10; i++)
	{
		if (thisTimeWindArr[i] > max)
		{
			max = thisTimeWindArr[i];
			maxIndex = i;
		}
		else if (thisTimeWindArr[i] < min)
		{
			min = thisTimeWindArr[i];
			minIndex = i;
		}
	}
	double sum = 0.0;
	double weightSum = 0.0;
	for (int j = 0; j < 10; j++)
	{
		if (j != maxIndex && j != minIndex)
		{
			sum += thisTimeWindArr[j] * Util::WeightOfModels[j];
			weightSum+= Util::WeightOfModels[j];
		}
	}
	double weightedAvgWind = sum / weightSum;
	return weightedAvgWind;
}

int Block::getNumOf_littleWind(int hour,double allRratio)
{
	int countNumOf_littleWind = 0;
	array<double, 10> thisTimeWindArr = getWindAllRealization(hour);
	
	
	for (int i = 0; i < 10; i++)
	{
		if (thisTimeWindArr[i] <allRratio)
		{
			countNumOf_littleWind++;
		}
	}

	//如果是特需 优选的model号
	/*int modelsIndex[4];
	modelsIndex[0] = 1;
	modelsIndex[1] = 3;
	modelsIndex[2] = 9;
	modelsIndex[3] = 10;*/
	//modelsIndex[4] = 8;
	//modelsIndex[5] = 10;

	/*for (int i = 0; i < 4; i++)
	{
		if (thisTimeWindArr[modelsIndex[i]-1] <allRratio)
		{
			countNumOf_littleWind++;
		}
	}*/



	return countNumOf_littleWind;
}

vector<int > Block::getSayNoModelId(int hour,double windRatio)
{
	vector<int > vioIndex;
	array<double, 10> thisTimeWindArr = getWindAllRealization(hour);
	for (int i = 0; i < 10; i++)
	{
		if (thisTimeWindArr[i] >= windRatio)
		{	
			//加1转换为真实的model号
			int  index = i+1;
			vioIndex.push_back(index);
		}
	}
	return vioIndex;
}


bool Block::isItAbadChoice(int thisTime, double allRratio)
{
	int numOfLittle = getNumOf_littleWind(thisTime / 60, allRratio);

	//double avoidBadCase = 30.0;
	//if (numOfLittle == 10)
	//{
	//	avoidBadCase = Util::vote10ratio;
	//}
	//else if (numOfLittle == 9)
	//{
	//	avoidBadCase = Util::vote9ratio;
	//}
	//else if (numOfLittle == 8)
	//{
	//	avoidBadCase = Util::vote8ratio;
	//}
	//else if (numOfLittle == 7)
	//{
	//	avoidBadCase = Util::vote7ratio;
	//}
	//else if (numOfLittle == 6)
	//{
	//	avoidBadCase = Util::vote6ratio;
	//}
	//else if (numOfLittle == 5)
	//{
	//	avoidBadCase = Util::vote5ratio;
	//}


	//if (getAvgWind(thisTime / 60) < avoidBadCase)
	//{
	//	return true;
	//}
	//else
	//{
	//	return false;
	//}

	array<double, 10> thisTimeWindArr = getWindAllRealization(thisTime / 60);
	if (numOfLittle == 8 && thisTimeWindArr[5] >= allRratio && thisTimeWindArr[9] >= allRratio)
	{
		return false;
	}
	else if (numOfLittle == 8 && thisTimeWindArr[5] >= allRratio && thisTimeWindArr[7] >= allRratio)
	{
		return false;
	}
	else
		return true;

}


bool Block::isItSatisfyMultiLevel(int thisTime, double allRratio)
{
	int numOfLittle = getNumOf_littleWind(thisTime / 60, allRratio);
	int hour = thisTime / 60;
	
	//day8的
	/*if (numOfLittle == 10)
	{
		if (hour == 3)
		{
			if (getAvgWind(hour) > 11.1487)
			{
				return false;
			}
			else
			{
				return true;
			}				
		}
		else if (hour == 4)
		{
			if (getAvgWind(hour) > 10.0325)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (hour == 5)
		{
			if (getAvgWind(hour) > 11.3212)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (hour == 6)
		{
			if (getAvgWind(hour) > 12.075)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (hour == 7)
		{
			if (getAvgWind(hour) > 13.0375)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (hour == 8)
		{
			if (getAvgWind(hour) > 10.4288)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (hour == 9)
		{
			if (getAvgWind(hour) > 9.77375)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (hour == 10)
		{
			if (getAvgWind(hour) > 12.85)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (hour == 11)
		{
			if (getAvgWind(hour) > 9.845)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (hour == 12)
		{
			if (getAvgWind(hour) > 10.2725)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (hour == 13)
		{
			if (getAvgWind(hour) > 10.4765)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (hour == 14)
		{
			if (getAvgWind(hour) > 11.875)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	else if (numOfLittle == 9)*/
	if (numOfLittle == 9)
	{
		if (hour == 3)
		{
			if (getAvgWind(hour) > 13.3875)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (hour == 5)
		{
			if (getAvgWind(hour) > 11.225)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (hour == 6)
		{
			if (getAvgWind(hour) > 12.1625)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (hour == 7)
		{
			if (getAvgWind(hour) > 13.8875)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (hour == 10)
		{
			if (getAvgWind(hour) > 11.875)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (hour == 13)
		{
			if (getAvgWind(hour) > 14.3)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	else if (numOfLittle == 8)
	{
		if (hour == 7)
		{
			if (getAvgWind(hour) > 14.6)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (hour == 13)
		{
			if (getAvgWind(hour) > 14.35)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return true;
		}
			
	}
	else if (numOfLittle == 7)
	{
		if (hour == 3)
		{
			if (getAvgWind(hour) > 13.775)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (hour == 7)
		{
			if (getAvgWind(hour) > 14.45)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (hour == 13)
		{
			if (getAvgWind(hour) > 14.625)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return true;
		}

	}
	else if (numOfLittle == 6)
	{
		if (hour == 7)
		{
			if (getAvgWind(hour) > 14.4875)
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		else if (hour == 13)
		{
			if (getAvgWind(hour) > 14.35)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	else
	{	
		return true;
	}


	//day6的
	//if (numOfLittle == 10)
	//{
	//	if (hour == 3)
	//	{
	//		if (getAvgWind(hour) > 13.7375)
	//		{
	//			return false;
	//		}
	//		else
	//		{
	//			return true;
	//		}
	//	}
	//	else if (hour == 4)
	//	{
	//		if (getAvgWind(hour) > 14.3125)
	//		{
	//			return false;
	//		}
	//		else
	//		{
	//			return true;
	//		}
	//	}
	//	else if (hour == 5)
	//	{
	//		if (getAvgWind(hour) > 14.1625)
	//		{
	//			return false;
	//		}
	//		else
	//		{
	//			return true;
	//		}
	//	}
	//	else if (hour == 6)
	//	{
	//		if (getAvgWind(hour) > 13.6625)
	//		{
	//			return false;
	//		}
	//		else
	//		{
	//			return true;
	//		}
	//	}		
	//	else
	//	{
	//		return true;
	//	}
	//}
	//else if (numOfLittle == 9)
	//	//if (numOfLittle == 9)
	//{
	//	if (hour == 3)
	//	{
	//		if (getAvgWind(hour) > 13.85)
	//		{
	//			return false;
	//		}
	//		else
	//		{
	//			return true;
	//		}
	//	}
	//	else if (hour == 4)
	//	{
	//		if (getAvgWind(hour) > 14.3)
	//		{
	//			return false;
	//		}
	//		else
	//		{
	//			return true;
	//		}
	//	}
	//	else if (hour == 5)
	//	{
	//		if (getAvgWind(hour) > 14.0625)
	//		{
	//			return false;
	//		}
	//		else
	//		{
	//			return true;
	//		}
	//	}
	//	else
	//	{
	//		return true;
	//	}
	//}
	//else if (numOfLittle == 8)
	//{
	//	if (hour == 3)
	//	{
	//		if (getAvgWind(hour) > 14.175)
	//		{
	//			return false;
	//		}
	//		else
	//		{
	//			return true;
	//		}
	//	}
	//	else if (hour == 4)
	//	{
	//		if (getAvgWind(hour) > 14.5125)
	//		{
	//			return false;
	//		}
	//		else
	//		{
	//			return true;
	//		}
	//	}
	//	else if (hour == 5)
	//	{
	//		if (getAvgWind(hour) > 14.4375)
	//		{
	//			return false;
	//		}
	//		else
	//		{
	//			return true;
	//		}
	//	}
	//	else
	//	{
	//		return true;
	//	}

	//}
	//else if (numOfLittle == 7)
	//{
	//	if (hour == 4)
	//	{
	//		if (getAvgWind(hour) > 14.6625)
	//		{
	//			return false;
	//		}
	//		else
	//		{
	//			return true;
	//		}
	//	}
	//	else if (hour == 5)
	//	{
	//		if (getAvgWind(hour) > 14.525)
	//		{
	//			return false;
	//		}
	//		else
	//		{
	//			return true;
	//		}
	//	}
	//	else
	//	{
	//		return true;
	//	}

	//}
	//else if (numOfLittle == 6)
	//{
	//	if (hour == 5)
	//	{
	//		if (getAvgWind(hour) > 14.975)
	//		{
	//			return false;
	//		}
	//		else
	//		{
	//			return true;
	//		}
	//	}

	//	else
	//	{
	//		return true;
	//	}
	//}
	//else
	//{
	//	return true;
	//}
}


double Block::getCBNTavgWind(int hour)
{
	array<double, 10> wa = getWindAllRealization(hour);
	//double windAvg = (wa[1]+ wa[2] + wa[3] + wa[4] + wa[5] + wa[8]) / 6;

	//去掉mlp和model9
	//double windAvg = (wa[1] + wa[2] + wa[3] + wa[4] ) / 4;

	//day6的权重xgb lgbm mlp 9大一些
	//double windAvg = wa[1] * 0.05 + wa[2] * 0.05 + (wa[3]+0.5) * 0.3 + (wa[4]+0.5) * 0.2 + wa[5] * 0.2 + wa[8] * 0.2;

	//day8的权重2 3 lgbm大一些
	//double windAvg = wa[1] * 0.2 + wa[2] * 0.3 + (wa[3] + 0.5) * 0.1 + (wa[4] + 0.5) * 0.2 + wa[5] * 0.1 + wa[8] * 0.1;

	//day8的权重2 3 lgbm大一些 
	//double windAvg = wa[1] * 0.2 + wa[2] * 0.4 + (wa[3] + 0.5) * 0.1 + (wa[4] + 0.5) * 0.2 + wa[5] * 0.05 + wa[8] * 0.05;

	//如果只要wa2是model3   wa3是lgbmd15  wa4是mlp   wa6是xgbd15
	double windAvg = (wa[3]+0.5)*0.95+(wa[6]+0.5)*0.025+wa[4]*0.025;
	return windAvg;
}

int Block::getCBNTvotes(int hour,double allRratio)
{
	array<double, 10> wa = getWindAllRealization(hour);
	/*int specialIndex[6];
	specialIndex[0] = 1;
	specialIndex[1] = 2;
	specialIndex[2] = 3;
	specialIndex[3] = 4;
	specialIndex[4] = 5;
	specialIndex[5] = 8;*/

	//去掉model9和mlp
	/*int specialIndex[4];
	specialIndex[0] = 1;
	specialIndex[1] = 2;
	specialIndex[2] = 3;
	specialIndex[3] = 4;*/

	//只要xgb,mlp,3
	int specialIndex[3];
	specialIndex[0] = 3;
	specialIndex[1] = 4;
	specialIndex[2] = 6;

	int vote = 0;
	for (int i = 0; i < 3; i++)
	{
		if (wa[specialIndex[i]] <allRratio)
		{
			vote++;
		}
	}
	return vote;
}
