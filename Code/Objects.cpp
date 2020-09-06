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
	EventData testData = EventData();
	testData.SetString("TestEvent", "What Whut");
	globalEventManager.SendEventImmediately(MAAD_EVENT(EngineEvent, testData));
};


void QueueEvent(MAAD_EVENT e)
{
	globalEventManager.QueueEvent(e);
}
void SendEventImmediately(MAAD_EVENT e)
{
	globalEventManager.SendEventImmediately(e);
}
void AddEventListener(EventListener* newListener, EventType criterion)
{
	globalEventManager.AddListener(newListener);
	newListener->typeCriterion.push_back(criterion);
}

void EventStartup()
{
	EventData testData = EventData();
	TestGameObject doubleTest = TestGameObject();
	testData.SetString("TestEvent", "What Whut");
	globalEventManager = EventSender();
	doubleTest.typeCriterion.push_back(EngineEvent);
	globalEventManager.AddListener(&doubleTest);
	globalEventManager.QueueEvent(MAAD_EVENT(EngineEvent, testData));
	EventManagerUpdate();
}