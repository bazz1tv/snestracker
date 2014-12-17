#include "TC_Template.h"
#include "TestDefinitions.h"
//#include "TestSuite.h"

/*void cCollisionTest::PrintVerboseInfo()
{
	if (TS::verbose)
	{
		
	}
}*/

int cSampleComponent::Test()
{	
	if (SomethingFailed)
		return TEST_FAILED;
	
	return TEST_SUCCESSFUL;
}