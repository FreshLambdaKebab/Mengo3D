#include "Framework.h"

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR pScmdLine,int iCmdShow)
{
	Framework* pFramework;
	bool result;

	//create the framework object
	pFramework = new Framework();
	if (!pFramework)
		return 0;

	//init and run the framework 
	result = pFramework->Initialize();
	if (result)
		pFramework->Run();

	//shutdown and release the framework object
	pFramework->Shutdown();
	delete pFramework;
	pFramework = 0;

	return 0;
}