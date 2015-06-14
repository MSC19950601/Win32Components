#pragma once

#include "Common.h"
#include "WindowBase.h"
#include "IndependentResource.h"

namespace Yupei
{
	class Application 
	{
	public:
		
		WPARAM Run()
		{
			mainWindow->Show();
			return mainWindow->Run();
		}
		static Application& GetInstance()
		{
			static Application mainApp;
			return mainApp;
		}
		void SetMainWindow(WindowBase* window) noexcept
		{
			mainWindow = window;
			mainWindow->SetResources(&resources);
		}
		~Application();
	private:
		Application();
		WindowBase* mainWindow = nullptr;
		DeviceIndependentResource resources;

		void InitializeCommonControls();
		void InitializeCom();
		void InitializeResources();
	};
}
