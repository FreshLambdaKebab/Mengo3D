#include "Framework.h"
#include <memory>

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR pScmdLine,int iCmdShow)
{
	std::unique_ptr<Framework>framework;
	bool result;

	//create the framework object
	framework = std::make_unique<Framework>();
	if (!framework)
		return 0;

	//init and run the framework 
	result = framework->Initialize();
	if (result)
		framework->Run();

	//shutdown and release the framework object
	framework->Shutdown();
	framework = 0;

	return 0;
}