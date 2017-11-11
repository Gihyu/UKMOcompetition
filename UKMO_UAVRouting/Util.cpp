#include "Util.h"

string Util::OutputPath = "C:/Users/zhoulei/Documents/VisualStudio2015/Projects/UKMO_UAVRouting/output/";
string Util::InputPath = "C:/Users/zhoulei/Documents/VisualStudio2015/Projects/UKMO_UAVRouting/input/";

double Util::Epsilon = 0.0001;//0.0001

int Util::flyTime = 2;

//print
string Util::getTimeStr(time_t t)
{
	struct tm timeinfo;
	char buffer[80];
	localtime_s(&timeinfo, &t);
	strftime(buffer, 80, "%Y/%m/%d %R", &timeinfo);
	//strftime(buffer, 80, "%Y/%m/%d %T", &timeinfo);
	return buffer;
}

void Util::printCurTime()
{
	time_t t = time(NULL);
	cout << ">>> TIME: " << Util::getTimeStr(t) << endl;
}