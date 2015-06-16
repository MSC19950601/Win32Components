#include "MainWindow.h"

int WINAPI WinMain(
	HINSTANCE  hInstance ,
	HINSTANCE /* hPrevInstance */,
	LPSTR /* lpCmdLine */,
	int /* nCmdShow */
	)
{
	auto app = Yupei::Application::GetInstance();
	auto mainWindow = std::make_unique<MainWindow>();
	app.SetMainWindow(mainWindow.get());
	return app.Run();
}