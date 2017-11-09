#include"Util.h"
#include"Schedule.h"
#include"IO.h"

int main()
{
	Schedule* schedule = new Schedule();
	IO* io = new IO();
	io->preProcess();
	//io->input(schedule);
	
	return 0;
}