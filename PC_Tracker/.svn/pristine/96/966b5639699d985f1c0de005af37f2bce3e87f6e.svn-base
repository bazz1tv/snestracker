#include "TestSuite.h"
#include "TestDefinitions.h"

using namespace std;


/*TestSuite :: TestSuite()
{
	numtabs = 0;
}*/
namespace TS {

	cConsoleTest TC_console;
	cCollisionTest TC_collision;
	cSettingsPreferencesTest TC_settingspreferences;
	
	SDL_bool verbose=SDL_FALSE;
	int numtabs=0;
	//Component *ComponentList = NULL;

// ALL COMPONENTS HERE
SingleComponent CL[] =
{
	{"Collision",CollisionTest},
	{"Console",ConsoleTest},
	{"Settings&Preferences",SettingsPreferencesTest},
	
	// The Terminator
	{"", NULL}
};
	
void init()
{
	//sc[1].handler();
}

void Begin()
{
	int input;
	
	cout << "Test Mode!\n----------" << endl;
	
	
	init();
	
	cout << "Make a Selection" << endl;
	cout << "1) Test All" << endl;
	cout << "2) Test a specific Component" << endl;
	cout << " : ";
	
	cin >> input;
	
	cout << "" << endl;
	
	if (input == 1)
		TestAll();
	else if (input == 2)
	{
		ComponentView();
	}
	
	//Quit
	
}
	

	
void ComponentView()
{
	for (int i=0; CL[i].component_name != ""; i++ )
	{
		cout << i+1 << ") " << CL[i].component_name << endl;
	}
	
	cout << ": ";
	
	// Select Component
	int selected_component_number;
	cin >> selected_component_number;
	
	//Test that Component
	CL[selected_component_number-1].TestComponent();
}

void tabprint()
{
	for (register Uint8 i=0; i < numtabs; i++)
		cout << "\t";
}



void TestAll()
{	
	for (int i=0; CL[i].component_name != ""; i++ )
	{
		cout << i+1 << ") " << CL[i].component_name << " Test" << endl << "---------------" << endl;
		
		if (CL[i].TestComponent() == TEST_SUCCESSFUL)
		{
			cout << CL[i].component_name << " Test Successful!\n" << endl;
		}
		else if (CL[i].TestComponent() == TEST_FAILED)
		{
			cout << CL[i].component_name << " Test Failed!!\n" << endl;
		}
	}
}

int CollisionTest()
{
	return TC_collision.Test();
}




int ConsoleTest()
{
	return TC_console.Test();

}
	
int SettingsPreferencesTest()
{
	return TC_settingspreferences.Test();
}
	
// END NAMESPACE
}
