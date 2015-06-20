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
	}

	// Event Arguments

	struct CreateArgs : RoutedEventArgs
	{

	};

	struct MouseArgs : RoutedEventArgs
	{
		int X, Y;
		// so on
	};

}
