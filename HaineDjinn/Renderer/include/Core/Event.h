#pragma once

#include <deque>

class EventSubscriber;

class Event
{
public:
	~Event();

	void Broadcast(void* parameters);

	void AddSubscriber(EventSubscriber* subscriber);
	void Unsubscribe(EventSubscriber* subscriber);
	
private:
	std::deque<EventSubscriber*> subscribers;

};
