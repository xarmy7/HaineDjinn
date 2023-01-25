
#include "Core/EventSubscriber.h"
#include "Core/Event.h"

//EventSubscriber::~EventSubscriber()
//{
//	if (subEvent == nullptr)
//		return;
//
//	subEvent->Unsubscribe(this);
//}

void EventSubscriber::Subscribe(Event* event)
{
	if (subEvent != nullptr)
		subEvent->Unsubscribe(this);

	subEvent = event;

	if (subEvent == nullptr)
		return;

	subEvent->AddSubscriber(this);
}
