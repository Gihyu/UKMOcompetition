#include "IO.h"


IO::IO()
{
}

void IO::input(Schedule * schedule)
{
	schedule->setCityList(readCity());
	tmpAnalyse();
	//schedule->setBlockList(readForecast(true));
	//readMeasure(schedule->getBlockList(),true);
}

vector<City*> IO::readCity()
{
	vector<City*> cities;

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

		City* city = new City(cities.size(), cityNo, x, y);
		cities.push_back(city);
	}
	file.close();

	cout << "# Cities:" << cities.size() << endl;
	for (auto&city : cities)
	{
		city->print();
	}
	cout << endl;

	return cities;
}

vector<Block*> IO::readForecast(bool isTraining)
{
	vector<Block*> blocks;

	string fileName;
	if (isTraining)
	{
		fileName = Util::InputPath + string("ForecastDataforTraining_test.csv");//!!
	}
	else
	{
		fileName = Util::InputPath + string("ForecastDataforTesting.csv");
	}
	
	cout << "* Read Forecast Data for Training from txt file:" << fileName << endl;
	ifstream file(fileName.c_str());

	string buf;
	char * token;
	char * tmp;

	getline(file, buf);//headline


	int x = 0;
	int y = 0;
	int date = 0;
	int hour = 0;
	int realization = 0;
	double wind = 0.0;

	while (getline(file, buf))
	{
		token = strtok_s((char *)buf.c_str(), ",", &tmp);
		x = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		y = atoi(token);

		Block* tmpBlock = NULL;
		bool found = false;
		for (auto&b : blocks)
		{
			if (b->getX() == x && b->getY()== y)
			{
				tmpBlock = b;
				found = true;
				break;
			}
		}
		if (!found)
		{
			tmpBlock = new Block(blocks.size(), x, y);
			blocks.push_back(tmpBlock);
		}

		token = strtok_s(NULL, ",", &tmp);
		date = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		hour = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		realization = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		wind = atof(token);

		BLOCKTYPE type = (BLOCKTYPE)realization;

		//BLOCKTYPE type = B_MODE1;
		//switch (realization)
		//{
		//case 1:
		//	type = B_MODE1;
		//	break;
		//case 2:
		//	type = B_MODE2;
		//	break;
		//case 3:
		//	type = B_MODE3;
		//	break;
		//case 4:
		//	type = B_MODE4;
		//	break;
		//case 5:
		//	type = B_MODE5;
		//	break;
		//case 6:
		//	type = B_MODE6;
		//	break;
		//case 7:
		//	type = B_MODE7;
		//	break;
		//case 8:
		//	type = B_MODE8;
		//	break;
		//case 9:
		//	type = B_MODE9;
		//	break;
		//case 10:
		//	type = B_MODE10;
		//	break;
		//default:
		//	cout << "error:realization not exist!" << endl;
		//	break;
		//}

		OperBlock* tmpOperBlock = new OperBlock(tmpBlock, date, hour, type, wind);
		tmpBlock->pushForecastOperBlock(tmpOperBlock);

	}
	file.close();

	cout << "# Blocks:" << blocks.size() << endl;

	return blocks;
}

void IO::readMeasure(vector<Block*> blocks,bool isTraining)
{
	string fileName;

	if (isTraining)
	{
		fileName = Util::InputPath + string("In-situMeasurementforTraining.csv");
	}
	else
	{
		cout << "error:measure file for test not exist!" << endl;
	}
	
	cout << "* Read Measurement for Training from txt file:" << fileName << endl;
	ifstream file(fileName.c_str());

	string buf;
	char * token;
	char * tmp;

	getline(file, buf);//headline

	int x = 0;
	int y = 0;
	int date = 0;
	int hour = 0;
	BLOCKTYPE type = B_MEASURE;
	double wind = 0.0;

	while (getline(file, buf))
	{
		token = strtok_s((char *)buf.c_str(), ",", &tmp);
		x = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		y = atoi(token);

		Block* tmpBlock = NULL;
		bool found = false;
		for (auto&b : blocks)
		{
			if (b->getX() == x&& b->getY() == y)
			{
				tmpBlock = b;
				found = true;
				break;
			}
		}
		if (!found)
		{
			cout << "error:block not found!" << endl;
		}
		else
		{
			token = strtok_s(NULL, ",", &tmp);
			date = atoi(token);
			
			token = strtok_s(NULL, ",", &tmp);
			hour = atoi(token);

			token = strtok_s(NULL, ",", &tmp);
			wind = atof(token);

			OperBlock* tmpOperBlock = new OperBlock(tmpBlock, date, hour, type, wind);
			tmpBlock->pushMeasureOperBlock(tmpOperBlock);
		}

	}
	file.close();

	cout << ">>> finish readMeasure" << endl;
}

void IO::tmpAnalyse()
{
	vector<Block*> blocks;

	//string fileName = Util::InputPath + string("ForecastDataforTraining.csv");
	string fileName = Util::InputPath + string("In-situMeasurementforTraining.csv");

	cout << "* Read Data for Training from txt file:" << fileName << endl;
	ifstream file(fileName.c_str());

	string buf;
	char * token;
	char * tmp;

	getline(file, buf);//headline


	int x = 0;
	int y = 0;
	int date = 0;
	int hour = 0;
	int realization = 0;
	double wind = 0.0;

	while (getline(file, buf))
	{
		token = strtok_s((char *)buf.c_str(), ",", &tmp);
		x = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		y = atoi(token);

		Block* tmpBlock = NULL;
		bool found = false;
		for (auto&b : blocks)
		{
			if (b->getX() == x && b->getY() == y)
			{
				tmpBlock = b;
				found = true;
				break;
			}
		}
		if (!found)
		{
			tmpBlock = new Block(blocks.size(), x, y);
			blocks.push_back(tmpBlock);
		}
		if (blocks.size() % 1000 == 0)
		{
			cout << "cout:" << blocks.size() / 1000 << endl;
		}
	}
	file.close();

	cout << "# Blocks:" << blocks.size() << endl;
}

void IO::preProcess()
//数据预处理
{
	bool isForecastFile = false;
	devideFileByDay(isForecastFile);

	isForecastFile = true;
	devideFileByDay(isForecastFile);
}

void IO::devideFileByDay(bool isForecast)
{
	string fileType = ".csv";
	
	string in = string("ForecastDataforTraining_test");
	if (!isForecast)
	{
		in = string("In-situMeasurementforTraining_test");
	}
	
	//input
	string inFileName = Util::InputPath + in + fileType;
	cout << "* Read data to be devided from csv file:" << inFileName << endl;
	ifstream file(inFileName.c_str());

	//output
	string outFileDay1 = Util::OutputPath + in + "_day1" + fileType;
	ofstream outDay1(outFileDay1.c_str());

	string outFileDay2 = Util::OutputPath + in + "_day2" + fileType;
	ofstream outDay2(outFileDay2.c_str());

	string outFileDay3 = Util::OutputPath + in + "_day3" + fileType;
	ofstream outDay3(outFileDay3.c_str());

	string outFileDay4 = Util::OutputPath + in + "_day4" + fileType;
	ofstream outDay4(outFileDay4.c_str());

	string outFileDay5 = Util::OutputPath + in + "_day5" + fileType;
	ofstream outDay5(outFileDay5.c_str());

	string buf;
	char * token;
	char * tmp;

	getline(file, buf);//headline


	int x = 0;
	int y = 0;
	int date = 0;
	int hour = 0;
	double wind = 0.0;

	while (getline(file, buf))
	{
		string bufCopy = buf;
		token = strtok_s((char *)bufCopy.c_str(), ",", &tmp);
		//x = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		//y = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		date = atoi(token);

		switch (date)
		{
		case 1:
			outDay1 << buf << "\n";
			break;
		case 2:
			outDay2 << buf << "\n";
			break;
		case 3:
			outDay3 << buf << "\n";
			break;
		case 4:
			outDay4 << buf << "\n";
			break;
		case 5:
			outDay5 << buf << "\n";
			break;
		default:
			cout << "error: date cannot be "<< date << endl;
			break;
		}
	}
	
	file.close();
	outDay1.close();
	outDay2.close();
	outDay3.close();
	outDay4.close();
	outDay5.close();
}
