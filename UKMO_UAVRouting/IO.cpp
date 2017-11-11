#include "IO.h"

void IO::input(Schedule* sche, bool isTraining)
{
	readForecast(sche, isTraining);
	Util::printCurTime();

	readCity(sche);
	Util::printCurTime();

	if (isTraining)
	{
		readMeasure(sche);
		Util::printCurTime();
	}
}


void IO::readForecast(Schedule* sche, bool isTraining)
{
	vector<Block*> blocks;

	string fileName;
	if (isTraining)
	{
		fileName = Util::InputPath + string("input_ForecastDataforTraining.csv");
	}
	else
	{
		fileName = Util::InputPath + string("input_ForecastDataforTesting.csv");
	}
	
	cout << "* Read forecast data for from txt file:" << fileName << endl;
	ifstream file(fileName.c_str());

	string buf;
	char * token;
	char * tmp;

	//getline(file, buf);//headline

	int x = 0;
	int y = 0;
	int date = 0;
	array<double, 12> windArr;

	int testFlag = 0;

	while (getline(file, buf))
	{
		testFlag++;
		if (testFlag % 10000 == 0)
		{
			cout << "cout:" << testFlag << endl;
		}

		token = strtok_s((char *)buf.c_str(), ",", &tmp);
		x = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		y = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		date = atoi(token);

		for (int i = 0; i < 12; ++i)
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
	blocks.front()->print();
	blocks.back()->print();

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

void IO::readMeasure(Schedule* sche)
{
	vector<Block*> blocks = sche->getBlockList();

	string fileName = Util::InputPath + string("input_In-situMeasurementforTraining.csv");
	cout << "* Read Measurement for Training from txt file:" << fileName << endl;
	ifstream file(fileName.c_str());

	string buf;
	char * token;
	char * tmp;

	//getline(file, buf);//headline

	int x = 0;
	int y = 0;
	int date = 0;
	array<double, 12> windArr;

	int testFlag = 0;

	while (getline(file, buf))
	{
		testFlag++;
		if (testFlag % 10000 == 0)
		{
			cout << "cout:" << testFlag << endl;
		}

		token = strtok_s((char *)buf.c_str(), ",", &tmp);
		x = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		y = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		date = atoi(token);

		for (int i = 0; i < 12; ++i)
		{
			token = strtok_s(NULL, ",", &tmp);
			windArr[i] = atof(token);
		}

		bool found = false;
		for (auto&tmpBlock : blocks)
		{
			if (tmpBlock->equal(x, y))
			{
				tmpBlock->setMeasureWindArr(windArr);
				found = true;
				break;
			}
		}
		if (!found)
		{
			cout << "error:block not found!" << endl;
			cout << "\tx:" << x << "\ty:" << y << endl;
			exit(0);
		}
	}
	file.close();

	cout << ">>> finish readMeasure" << endl;
}

