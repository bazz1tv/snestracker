#include "CollisionTest.h"
#include "TestDefinitions.h"
#include "TestSuite.h"

struct TwoRectIntersect
{
	SDL_Rect r1,r2;
	SDL_bool intersect_expected;
};

void cCollisionTest::PrintOK()
{
	cout << " | OK | " <<endl;
}

TwoRectIntersect RectTestList[] = {
	{ {0,0,20,20},{30,0,20,20},SDL_FALSE },
	{ {0,0,20,20},{0,20,20,20},SDL_FALSE },
	{ {0,0,20,20},{0,19,20,20},SDL_TRUE  },
	{ {0,0,20,20},{18,18,20,20},SDL_TRUE },
	{ {10,10,10,10},{8,0,20,30}, SDL_TRUE },
	{ {0,20,20,20},{20,0,20,20}, SDL_FALSE},
	{ {0,0,20,20},{0,18,20,20}, SDL_TRUE },
	{ {0,0,0,0},{0,0,0,0},SDL_FALSE },
};

void cCollisionTest::PrintVerboseInfo()
{
	if (TS::verbose)
	{
		cout << endl;
		cout << "r1: " << "X: " << r1->x << "\tY: " << r1->y << endl << "\t" << "W: " << r1->w << "\tH: " << r1->h << endl;
		cout << "r2: " << "X: " << r2->x << "\tY: " << r2->y << endl << "\t" << "W: " << r2->w << "\tH: " << r2->h << endl;
	}
}
int cCollisionTest::Test()
{	
	while (1)
	{
		PrintTestNumber();
		
		r1 = &RectTestList[testnum-1].r1;
		r2 = &RectTestList[testnum-1].r2;
		
		PrintVerboseInfo();
		
		if (RectIntersect(r1,r2))
		{
			if (RectTestList[testnum-1].intersect_expected)
			{
				PrintOK();
				cout << endl;
			}
			else
			{
				PrintTestFailed();
				cout << endl;
				SomethingFailed = SDL_TRUE;
			}
		}
		else
		{
			if (RectTestList[testnum-1].intersect_expected)
			{
				PrintTestFailed();
				cout << endl;
				SomethingFailed = SDL_TRUE;
			}
			else
			{
				PrintOK();
				cout << endl;
			}
		}
		
		// If r1,r2 are both set to 0 everything, then we are done. That's our terminator
		if ((RectTestList[testnum-1].r1.x == 0 && RectTestList[testnum-1].r1.y == 0 && RectTestList[testnum-1].r1.w == 0 && RectTestList[testnum-1].r1.h == 0) && (RectTestList[testnum-1].r2.x == 0 && RectTestList[testnum-1].r2.y == 0 && RectTestList[testnum-1].r2.w == 0 && RectTestList[testnum-1].r2.h == 0) )
			break;
	}
	
	
	if (SomethingFailed)
		return TEST_FAILED;
	
	return TEST_SUCCESSFUL;
}