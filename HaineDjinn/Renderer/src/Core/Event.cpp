
#include "Core/EventSubscriber.h"
#include "Core/Event.h"
#include "Core/Debug.h"

Event::~Event()
{
	//for (EventSubscriber* sub : subscribers)
	//	sub->Subscribe(nullptr);
	//
	//subscribers.clear();
}

void Event::AddSubscriber(EventSubscriber* subscriber) 
{
	if (subscriber != nullptr)
		subscribers.push_back(subscriber);
}

void Event::Broadcast(void* parameters)
{
	for (int i = 0; i < subscribers.size(); i++)
		subscribers[i]->function(parameters);
}

void Event::Unsubscribe(EventSubscriber* subscriber)
{
	for (int i = 0; i < subscribers.size(); i++)
	{
		if (subscribers[i] == subscriber)
		{
			subscribers.erase(subscribers.begin() + i);
			break;
		}
	}
}
