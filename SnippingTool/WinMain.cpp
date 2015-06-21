#include "MainWindow.h"
#include "..\BasicLibrary\Application.h"

int WINAPI WinMain(
	HINSTANCE  hInstance ,
	HINSTANCE /* hPrevInstance */,
	LPSTR /* lpCmdLine */,
	int /* nCmdShow */
	)
{
	auto mainWindow = std::make_shared<MainWindow>();
	mainWindow->Initialize();
	return Yupei::Application::Run(mainWindow.get());
}