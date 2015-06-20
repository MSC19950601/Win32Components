#include "Application.h"
#include "WindowBase.h"
#include <memory>

namespace Yupei
{
	WPARAM Application::Run(WindowBase * window)
	{
		auto& app = Application::GetInstance();
		if (window != nullptr) window->Show();
		auto tempArgs = std::make_unique<RoutedEventArgs>();
		RoutedInvoke(app.ApplicationLoaded, nullptr, tempArgs.get());
		MSG messages;
		while (true)
		{
			if (::PeekMessage(&messages, nullptr, 0, 0, PM_REMOVE))
			{
				if (messages.message == WM_QUIT) break;
				::TranslateMessage(&messages);
				::DispatchMessage(&messages);
			}
			else
			{
				if(window != nullptr)
					window->OnRender();
			}
		}
		tempArgs->Reset();
		RoutedInvoke(app.ApplicationExit, nullptr, tempArgs.get());
		return messages.wParam;
	}
}