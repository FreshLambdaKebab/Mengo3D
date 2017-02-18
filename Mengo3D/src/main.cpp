#include "Framework.h"

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nShowCmd)
{
	Framework* framework;
	bool result;
	
	//create the framework object pointer
	framework = new Framework();
	if (!framework)
	{
		return 0;
	}

	//initialize and run the framework object
	result = framework->Init();
	if (result)
	{
		framework->Run();
	}
	
	//shut down and get rid of the framework object
	framework->Shutdown();
	delete framework;
	framework = 0;

	return 0;
}
