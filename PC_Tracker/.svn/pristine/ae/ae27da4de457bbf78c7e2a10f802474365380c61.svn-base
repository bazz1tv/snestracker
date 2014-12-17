#include "TestHelperFunctions.h"
#include "TestDefinitions.h"

using namespace std;

void OK()
{
	PrintOK();
}
void PrintOK()
{
	cout << "OK" <<endl;
}
int PrintTestFailed()
{
	return PrintFailed();
	//return -1;
}
int PrintFailed()
{
	cout << " Test Failed " << endl;
	return TEST_FAILED;
}

void PrintTestNumber(int &testnum)
{
	cout << "Test" << testnum++ << ": ";
}

void Stub()
{
	cout << "Stub Function!! " <<endl ;
}