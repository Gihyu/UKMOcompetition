#include"Util.h"
#include"Schedule.h"
#include"IO.h"

int main()
{
	Schedule* sche = new Schedule();
	IO* io = new IO();
	bool isTraining = true;
	io->input(sche, isTraining);
	
	return 0;
}