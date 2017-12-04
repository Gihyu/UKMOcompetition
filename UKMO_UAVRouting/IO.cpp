#include "IO.h"

IO::IO()
{
	struct tm timeinfo;
	timeinfo.tm_year = 2017 - 1900;
	timeinfo.tm_mon = 11 - 1;
	timeinfo.tm_mday = 13;
	timeinfo.tm_hour = 0;
	timeinfo.tm_min = 0;
	timeinfo.tm_sec = 0;
	Util::startTime = mktime(&timeinfo);
	cout << "* Basic start time:" << Util::getTimeStr(Util::startTime) << endl;
}

void IO::input(Schedule* sche,int date)
{
	Util::printCurTime();

	string inFile;
	if (Util::allRealization)
	{
		inFile = "reProcess_day" + to_string(date) + "_R10";
		readForecastMatrix(sche, inFile);
	}
	else
	{
		if (date <= 5)//training
		{
			inFile= "Train_MergeLinreg_D_" + to_string(date);
		}
		else//testing
		{
			inFile = "compress_day" + to_string(date)+"R"+to_string(Util::realization);//MergeLinreg_D_//TrainByLinregDate2&4_D_
		}
		readForecast(sche, inFile);
	}
	Util::printCurTime();

	readCity(sche);
	Util::printCurTime();

	//if (date <= 5)//training
	//{
	//	inFile = "compress_reProcess_day"+ to_string(date);
	//	readMeasure(sche,inFile);
	//	Util::printCurTime();
	//}
}


void IO::readForecast(Schedule* sche,string inFile)
{
	vector<Block*> blocks;

	string fileName = Util::InputPath + inFile + ".csv";
	
	cout << "* Read forecast data for from txt file:" << fileName << endl;
	ifstream file(fileName.c_str());

	string buf;
	char * token;
	char * tmp;

	//getline(file, buf);//headline

	int x = 0;
	int y = 0;
	int date = 0;
	array<double, Util::hourCount> windArr;

	int testFlag = 0;

	while (getline(file, buf))
	{
		//testFlag++;
		//if (testFlag % 10000 == 0)
		//{
		//	cout << "cout:" << testFlag << endl;
		//}

		token = strtok_s((char *)buf.c_str(), ",", &tmp);
		x = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		y = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		date = atoi(token);

		for (int i = 0; i < Util::hourCount; ++i)
		{
			token = strtok_s(NULL, ",", &tmp);
			windArr[i] = atof(token);
		}

		Block* newBlock = new Block(x, y, date, windArr);
		blocks.push_back(newBlock);
	}
	file.close();

	sche->setBlockList(blocks);

	cout << "# Blocks:" << sche->getBlockList().size() << endl;
	cout << ">>> test print" << endl;
	blocks.front()->print();
	blocks.back()->print();
	cout << endl;
}

void IO::readForecastMatrix(Schedule * sche, string inFile)
{
	vector<Block*> blocks;
	int basicHour = 3;

	string fileName = Util::InputPath + inFile + ".csv";

	cout << "* Read forecast data for from txt file:" << fileName << endl;
	ifstream file(fileName.c_str());

	string buf;
	char * token;
	char * tmp;

	int x = 0;
	int y = 0;
	int date = 0;
	int hour = 0;
	array<array<double, 10>, Util::hourCount> windMatrix;
	int flag = 0;

	while (getline(file, buf))
	{
		token = strtok_s((char *)buf.c_str(), ",", &tmp);
		x = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		y = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		date = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		hour = atoi(token);

		if (date < 5)//training
		{
			token = strtok_s(NULL, ",", &tmp);//measure
		}

		for (int i = 0; i < Util::realizationCount; ++i)
		{
			token = strtok_s(NULL, ",", &tmp);
			windMatrix[hour - basicHour][i] = atof(token);
		}

		flag = 1;
		while (flag < Util::hourCount)
		{
			getline(file, buf);
			flag++;

			token = strtok_s((char *)buf.c_str(), ",", &tmp);
			//x = atoi(token);

			token = strtok_s(NULL, ",", &tmp);
			//y = atoi(token);

			token = strtok_s(NULL, ",", &tmp);
			//date = atoi(token);

			token = strtok_s(NULL, ",", &tmp);
			hour = atoi(token);

			if (date < 5)//training
			{
				token = strtok_s(NULL, ",", &tmp);//measure
			}

			for (int i = 0; i < Util::realizationCount; ++i)
			{
				token = strtok_s(NULL, ",", &tmp);
				windMatrix[hour - basicHour][i] = atof(token);
			}

		}

		Block* newBlock = new Block(x, y, date, windMatrix);
		blocks.push_back(newBlock);
	}
	file.close();

	sche->setBlockList(blocks);

	cout << "# Blocks:" << sche->getBlockList().size() << endl;
	cout << ">>> test print" << endl;
	blocks.front()->print();
	blocks.back()->print();
	cout << endl;
}

void IO::readCity(Schedule* sche)
{
	vector<City*> cities;
	vector<Block*> blocks = sche->getBlockList();

	string fileName = Util::InputPath + string("CityData.csv");
	cout << "* Read cities from txt file:" << fileName << endl;
	ifstream file(fileName.c_str());

	string buf;
	char * token;
	char * tmp;

	getline(file, buf);//headline

	int cityNo = 0;
	int x = 0;
	int y = 0;
	while (getline(file, buf))
	{
		token = strtok_s((char *)buf.c_str(), ",", &tmp);
		cityNo = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		x = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		y = atoi(token);

		for (auto&tmpBlock : blocks)
		{
			if (tmpBlock->equal(x, y))
			{
				City* city = new City(cityNo,tmpBlock);
				cities.push_back(city);
				break;
			}
		}

	}
	file.close();

	cout << "# Cities:" << cities.size() << endl;
	for (auto&city : cities)
	{
		city->print();
	}
	cout << endl;

	sche->setCityList(cities);
}

void IO::readMeasure(Schedule* sche,string inFile)
{
	vector<Block*> blocks = sche->getBlockList();

	string fileName = Util::InputPath + inFile + ".csv"; 
	cout << "* Read Measurement for Training from txt file:" << fileName << endl;
	ifstream file(fileName.c_str());

	string buf;
	char * token;
	char * tmp;

	//getline(file, buf);//headline

	int x = 0;
	int y = 0;
	int date = 0;
	array<double, Util::hourCount> windArr;

	int testFlag = 0;

	while (getline(file, buf))
	{
		//testFlag++;
		//if (testFlag % 10000 == 0)
		//{
		//	cout << "cout:" << testFlag << endl;
		//}

		token = strtok_s((char *)buf.c_str(), ",", &tmp);
		x = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		y = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		date = atoi(token);

		for (int i = 0; i < Util::hourCount; ++i)
		{
			token = strtok_s(NULL, ",", &tmp);
			windArr[i] = atof(token);
		}

		int index = Schedule::getBlockIndex(x, y);
		if (index > -1 && index < blocks.size())
		{
			Block* tmpBlock = blocks[index];
			if (tmpBlock->equal(x, y))
			{
				tmpBlock->setMeasureWindArr(windArr);
			}
			else
			{
				cout << "error:mismatch"<< endl;
				cout << "\tx:" << x << "\ty:" << y << endl;
				exit(0);
			}
		}
		else
		{
			cout << "error:wrong!:" << index << endl;
			exit(0);
		}
	}
	file.close();

	cout << ">>> finish readMeasure" << endl;
}

void IO::printSoln(int destinationCityNo, Block * origin, vector<OperBlock*> soln)
{
	cout << ">>> PRINT SOLN: city_"<< destinationCityNo << endl;
	for (auto&tmp : soln)
	{
		tmp->print();
	}
}

void IO::outputSoln(int destinationCityNo, Block* origin, vector<OperBlock*> soln)
{	
	printSoln(destinationCityNo, origin, soln);
	
	int date = soln.front()->getBlock()->getDate();

	string outFileName = Util::OutputPath + "Soln_city"+to_string(destinationCityNo)+"_day"+to_string(date) + ".csv";
	cout << "* Soln output:" << outFileName << endl;
	ofstream out(outFileName.c_str());

	time_t curTime = Util::getTime(3,0);//3:00
	time_t step = 2 * 60;//2mins
	Block* last = origin;
	int lastX = last->getX();
	int lastY = last->getY();
	for (auto&tmp : soln)
	{
		time_t tmpT = Util::getTime(tmp->getSolnTime());
		while (curTime < tmpT)
		{
			out << destinationCityNo << "," << date << ","
				<< Util::getTimeStr(curTime) << ","
				<< lastX << "," << lastY << endl;
			curTime += step;
		}
		last = tmp->getBlock();
		lastX = last->getX();
		lastY = last->getY();
		out << destinationCityNo << "," << date << ","
			<< Util::getTimeStr(curTime) << ","
			<< lastX << "," << lastY << endl;
		curTime += step;
	}
}

void IO::outputMultiSoln(vector<City*> cities, Block * origin, vector<vector<OperBlock*>> multiSoln)
{
	int date = origin->getDate();

	string outFileName = Util::OutputPath + "Soln_multi_day" + to_string(date) + ".csv";
	cout << "* Soln output:" << outFileName << endl;
	ofstream out(outFileName.c_str());

	for (auto&city : cities)
	{
		if (city->getBlock() == origin)
		{
			continue;
		}

		vector<OperBlock*> soln = city->getSoln();
		int destinationCityNo = city->getNo();
		printSoln(destinationCityNo, origin, soln);


		time_t curTime = Util::getTime(3, 0);//3:00
		time_t step = 2 * 60;//2mins
		Block* last = origin;
		int lastX = last->getX();
		int lastY = last->getY();
		for (auto&tmp : soln)
		{
			time_t tmpT = Util::getTime(tmp->getSolnTime());
			while (curTime < tmpT)
			{
				out << destinationCityNo << "," << date << ","
					<< Util::getTimeStr(curTime) << ","
					<< lastX << "," << lastY << endl;
				curTime += step;
			}
			last = tmp->getBlock();
			lastX = last->getX();
			lastY = last->getY();
			out << destinationCityNo << "," << date << ","
				<< Util::getTimeStr(curTime) << ","
				<< lastX << "," << lastY << endl;
			curTime += step;
		}
	}
}

