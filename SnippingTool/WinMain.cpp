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
	//mainWindow->SetFixedSize(500, 100);
	mainWindow->SetClientSize(500, 100);
	return app.Run();
}