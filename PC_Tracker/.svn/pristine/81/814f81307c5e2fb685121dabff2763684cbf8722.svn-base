#include "CollisionTest.h"
#include "TestDefinitions.h"
#include "TestSuite.h"

struct TwoRectIntersect
{
	SDL_Rect r1,r2;
	SDL_bool intersect_expected;
};

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
		cout << "r1: " << "X: " << r1->x << "\tY: " << r1->y << endl << "\t" << "w: " << r1->w << "\th: " << r1->h << endl;
		cout << "r2: " << "X: " << r2->x << "\tY: " << r2->y << endl << "\t" << "w: " << r2->w << "\th: " << r2->h << endl;
	}
}
int cCollisionTest::Test()
{
	int i=0;
	
	while (1)
	{
		PrintTestNumber();
		
		r1 = &RectTestList[i].r1;
		r2 = &RectTestList[i].r2;
		
		PrintVerboseInfo();
		
		if (RectIntersect(r1,r2))
		{
			if (RectTestList[i].intersect_expected)
			{
				PrintOK();
			}
			else
			{
				PrintTestFailed();
				SomethingFailed = SDL_TRUE;
			}
		}
		else
		{
			if (RectTestList[i].intersect_expected)
			{
				PrintTestFailed();
				SomethingFailed = SDL_TRUE;
			}
			else
			{
				PrintOK();
			}
		}
		
		// increase TestListCounter
		i++;
		
		
		// If r1,r2 are both set to 0 everything, then we are done. That's our terminator
		if ((RectTestList[i].r1.x == 0 && RectTestList[i].r1.y == 0 && RectTestList[i].r1.w == 0 && RectTestList[i].r1.h == 0) && (RectTestList[i].r2.x == 0 && RectTestList[i].r2.y == 0 && RectTestList[i].r2.w == 0 && RectTestList[i].r2.h == 0) )
			break;
	}
	
	
	if (SomethingFailed)
		return TEST_FAILED;
	
	return TEST_SUCCESS;
}