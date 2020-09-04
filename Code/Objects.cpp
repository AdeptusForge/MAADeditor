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
	globalEventManager.SendEventFromQueue();
};

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

void QueueEvent(MAAD_EVENT e)
{
	globalEventManager.QueueEvent(e);
}
void SendEventImmediately(MAAD_EVENT e)
{
	globalEventManager.SendEventImmediately(e);
}
