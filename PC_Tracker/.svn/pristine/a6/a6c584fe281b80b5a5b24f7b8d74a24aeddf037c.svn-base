#include "TestHelperFunctions.h"
#include "TestDefinitions.h"
#include "TestStream.h"

using namespace std;

void OK()
{
	PrintOK();
}
void PrintOK()
{
	test << "OK" <<endl;
}
int PrintTestFailed()
{
	return PrintFailed();
	//return -1;
}
int PrintFailed()
{
	test << " Test Failed " << endl;
	return TEST_FAILED;
}

//void PrintTestNumber(int &testnum)
//{
//	test << "Test" << testnum++ << ": ";
//}

void Stub()
{
	test << "Stub Function!! " <<endl ;
}