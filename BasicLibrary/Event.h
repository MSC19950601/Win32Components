#pragma once

#include <functional>
#include <vector>


namespace Yupei
{
	template<typename ArgsType>
	using EventHandler = std::function<void(void* sender, ArgsType* args)>;

	template<typename ArgsType>
	struct Event
	{
	public:
		using handler_type = EventHandler<ArgsType>;
		void AddHandler(const handler_type& _handler)// for lvalue
		{
			handlers.push_back(_handler);
		}
		void AddHandler(handler_type&& _handler) // for rvalue
		{
			handlers.emplace_back(_handler);
		}
		std::vector<EventHandler<ArgsType>>& GetHandlers() 
		{
			return handlers;
		}
		const std::vector<EventHandler<ArgsType>>& GetHandlers() const 
		{
			return handlers;
		}
	private:
		std::vector<EventHandler<ArgsType>> handlers;
	};

	struct EventArgs
	{
		
	};
}
