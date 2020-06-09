#pragma once
#include <string>
#include <queue>

class Event
{
public:
	virtual~Event();
	typedef const std::string DescriptorType;

};

class TestEvent : public Event
{
	DescriptorType descriptor = "TestEventlul";
};

class EventController 
{



};

