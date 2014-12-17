#include "CollisionTest.h"

int cCollisionTest::Test()
{
	// All that really has to be done here to Checking for different rects in RectIntersect
	SDL_Rect r1,r2;
	int testnum=1;
	
	cout << "Collision Test\n---------------" << endl;
	
	cout << "Test" << testnum++ << ": ";
	r1 = SetRect(0,0,20,20);
	r2 = SetRect(30,0,20,20);
	if (RectIntersect(&r1, &r2))
	{
		cout << " ERROR: Rect Intersect!" << endl;
		return -1;
	}
	cout << "OK" << endl;
	
	cout << "Test" << testnum++ << ": ";
	r1 = SetRect(0,0,20,20);
	r2 = SetRect(0,20,20,20);
	if (RectIntersect(&r1, &r2))
	{
		cout << " ERROR: Rect Intersect!" << endl;
		return -1;
	}
	cout << "OK" << endl;
	
	cout << "Test" << testnum++ << ": ";
	r1 = SetRect(0,0,20,20);
	r2 = SetRect(0,20,20,20);
	if (RectIntersect(&r1, &r2))
	{
		cout << " ERROR: Rect Intersect!" << endl;
		return -1;
	}
	cout << "OK" << endl;
	
	cout << "Test" << testnum++ << ": ";
	r1 = SetRect(0,0,20,20);
	r2 = SetRect(0,19,20,20);
	if (!RectIntersect(&r1, &r2))
	{
		cout << " ERROR: NO Rect Intersect!" << endl;
		return -1;
	}
	cout << "OK" << endl;
	
	cout << "Test" << testnum++ << ": ";
	r1 = SetRect(0,0,20,20);
	r2 = SetRect(18,18,20,20);
	if (!RectIntersect(&r1, &r2))
	{
		cout << " ERROR: NO Rect Intersect!" << endl;
		return -1;
	}
	cout << "OK" << endl;
	
	cout << "Test" << testnum++ << ": ";
	r1 = SetRect(10,10,10,10);
	r2 = SetRect(8,0,20,30);
	if (!RectIntersect(&r1, &r2))
	{
		cout << " ERROR: NO Rect Intersect!" << endl;
		return -1;
	}
	cout << "OK" << endl;
	
	cout << "Test" << testnum++ << ": ";
	r1 = SetRect(0,20,20,20);
	r2 = SetRect(20,0,20,20);
	if (RectIntersect(&r1, &r2))
	{
		cout << " ERROR: Rect Intersect!" << endl;
		return -1;
	}
	cout << "OK" << endl;
	
	cout << "Test" << testnum++ << ": ";
	r1 = SetRect(0,0,20,20);
	r2 = SetRect(0,18,20,20);
	if (!RectIntersect(&r1, &r2))
	{
		cout << " ERROR: NO Rect Intersect!" << endl;
		return -1;
	}
	cout << "OK" << endl;
	
	return 0;
}