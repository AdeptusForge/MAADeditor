#include "Objects.h"

std::vector<MAAD_GameObject> allGameObjects;
EventSender globalEventManager;


glm::ivec3 ConvertFloatVec(glm::vec3 floatVec)
{
	glm::ivec3 newVec;
	newVec.x = floatVec.x * MAX_DECIMAL_PRECISION;
	newVec.y = floatVec.y * MAX_DECIMAL_PRECISION;
	newVec.z = floatVec.z * MAX_DECIMAL_PRECISION;
	return newVec;
}

void EventManagerUpdate()
{
	globalEventManager.SendQueuedEvents();
};

void QueueEvent(MAAD_EVENT e)
{
	globalEventManager.QueueEvent(e);
	//WriteDebug(std::to_string(globalEventManager.GetTotalListeners()));
}
void SendEventImmediately(MAAD_EVENT e)
{
	globalEventManager.SendEventImmediately(e);
}
void AddEventListener(EventListener* newListener, EventType criterion)
{
	globalEventManager.AddListener(newListener);
	newListener->typeCriterion.push_back(criterion);
	WriteDebug(std::to_string(globalEventManager.GetTotalListeners()));
}
void AddEventListener(EventListener* newListener)
{
	globalEventManager.AddListener(newListener);
	WriteDebug(std::to_string(globalEventManager.GetTotalListeners()));
}
void RemoveEventListener(EventListener* removee)
{
	globalEventManager.RemoveListener(removee);
}
void EventStartup()
{
	globalEventManager = EventSender();
	EventManagerUpdate();
}