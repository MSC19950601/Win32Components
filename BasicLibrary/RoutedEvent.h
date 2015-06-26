#pragma once

#include "Event.h"

namespace Yupei
{
	struct RoutedEventArgs : EventArgs
	{
		bool wasHandled = false;
		void Reset()
		{
			wasHandled = false;
		}
	};

	template<typename Type>
	void RoutedInvoke(Event<Type>& _event, void* sender, Type* args)
	{
		auto& callbacks = _event.GetHandlers();
		for (auto&& callback : callbacks)
		{
			std::invoke(callback, sender, args);
			if (args->wasHandled)
			{
				break;
			}
		}
		args->wasHandled = false;
	}

	// Event Arguments

	struct CreateArgs : RoutedEventArgs
	{

	};

	struct MouseArgs : RoutedEventArgs
	{
		int X, Y;
		bool IsCtrlDown;
		enum class MouseButtons
		{
			LeftButton = 0x0001,
			RightButton = 0x0002,
			MiddleButton = 0x0010
		};
		MouseButtons ButtonClicked;
		bool IsShiftDown;
		// so on
	};

}
