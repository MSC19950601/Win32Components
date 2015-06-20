#pragma once

#include <Windows.h>
#include "RoutedEvent.h"

namespace Yupei
{
	class WindowBase;
	class Application
	{
	public:
		static void Suspend()
		{
			::SuspendThread(::GetCurrentThread());
		}
		static void Resume()
		{
			::ResumeThread(::GetCurrentThread());
		}
		static void Restart()
		{

		}
		static void ExitApplication()
		{
			::PostQuitMessage(0);
		}
		static Application& GetInstance()
		{
			static Application mainApp;
			return mainApp;
		}
		Event<RoutedEventArgs> ApplicationExit;
		Event<RoutedEventArgs> ApplicationLoaded;
		static WPARAM Run(WindowBase* window);
	private:
		Application() = default;
		Application(const Application&) = delete;
	};


}
