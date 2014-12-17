//Template for Test Component, 
//This would be useful since I am making several TestComponents.

#pragma once
#include "Os.h"
#include "TestComponent.h"
#include "TestStream.h"
#include "TestHelperFunctions.h"
#include "TestDefinitions.h"

using namespace TS;

class cSampleComponent : TestComponent
{
public:
	cSampleComponent() : TestComponent() {}
	
	//void PrintVerboseInfo();
	int Test();
	
	// Data Here
	
};