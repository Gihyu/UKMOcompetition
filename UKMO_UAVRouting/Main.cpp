#include"Util.h"
#include"Schedule.h"
#include"IO.h"

#include"BFS.h"

int main()
{
	//Schedule* schedule = new Schedule();
	//IO* io = new IO();
	//io->preProcess();
	//io->input(schedule);
	Block * testBlock = new Block(100,100,100);
	BFS * testBFS = new BFS(testBlock);
	testBFS->run_the_case();
	
	return 0;
}