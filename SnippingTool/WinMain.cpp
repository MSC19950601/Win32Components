#include "MainWindow.h"
#include "..\BasicLibrary\Application.h"

int WINAPI WinMain(
	HINSTANCE  hInstance ,
	HINSTANCE /* hPrevInstance */,
	LPSTR /* lpCmdLine */,
	int /* nCmdShow */
	)
{
	auto mainWindow = MainWindow::GetInstance();
	mainWindow->Initialize();
	return Yupei::Application::Run(mainWindow.get());
}