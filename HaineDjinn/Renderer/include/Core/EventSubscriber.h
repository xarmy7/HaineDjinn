#pragma once

#include "Core/Event.h"
#include <functional>

class EventSubscriber
{
public:
	~EventSubscriber() {
		if (subEvent == nullptr)
			return;

		subEvent->Unsubscribe(this);
	}

	void Subscribe(Event* event);
	void SetFunction(std::function<void(void*)> inFunction) { function = inFunction; }
private:
	std::function<void(void*)> function;

	Event* subEvent = nullptr;
	friend Event;
};
