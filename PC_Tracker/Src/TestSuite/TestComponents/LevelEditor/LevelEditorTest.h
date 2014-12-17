//Template for Test Component, 
//This would be useful since I am making several TestComponents.

#pragma once
#include "Os.h"
#include "TestComponent.h"
#include "TestStream.h"
#include "TestHelperFunctions.h"
#include "TestDefinitions.h"

using namespace TS;

class cLevelEditorTest : TestComponent
{
public:
	cLevelEditorTest() : TestComponent() {}
	
	//void PrintVerboseInfo();
	int Test();
	
	int TestMoveSingleTile();
	int TestCopyPaste();
	
	int TestMultiSelect_Select();
	int TestMultiSelect_Move();
	int TestMultiSelect_CopyPaste();
	
	// Data Here
	
};