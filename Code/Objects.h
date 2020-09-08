#pragma once
#include "Physics.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "assets.h"
#include "IDControl.h"
#include <string>
#include <queue>
#include <map>
#include <algorithm>
class EventListener;

#pragma region Fixed Point Arith Reqs
const unsigned int MAX_DECIMAL_PRECISION = 10000;
const int INT360 = 3600000;
glm::ivec3 ConvertFloatVec(glm::vec3 floatVec);
#pragma endregion

#pragma region Phyics and Transform Reqs
#define PI 3.14159265

struct RotationVector
{
private:
	glm::ivec3 eulerRotation;
public:
	RotationVector() {};

	RotationVector(glm::vec3 rot, bool isFloat)
	{
		if (isFloat)
			eulerRotation = ConvertFloatVec(rot);
		else
			eulerRotation = rot;

		if (eulerRotation.x > INT360 || eulerRotation.x < -INT360)
		{
			//WriteDebug("RotationVector x angle outside euler angle bounds of -360 <-> 360. X Value set to 0.");
			eulerRotation.x = 0;
		}
		if (eulerRotation.y > INT360 || eulerRotation.y < -INT360)
		{
			//WriteDebug("RotationVector y angle outside euler angle bounds of -360 <-> 360. Y Value set to 0.");
			eulerRotation.y = 0;
		}
		if (eulerRotation.z > INT360 || eulerRotation.z < -INT360)
		{
			//WriteDebug("RotationVector z angle outside euler angle bounds of -360 <-> 360. Z Value set to 0.");
			eulerRotation.z = 0;
		}
	}

	glm::ivec3 GetTrueRotation() { return eulerRotation; }
	glm::vec3 GetEulerRotation()
	{
		glm::vec3 rot;
		//WriteDebug(vecToStr(position));
		rot.x = (float)eulerRotation.x / MAX_DECIMAL_PRECISION;
		rot.y = (float)eulerRotation.y / MAX_DECIMAL_PRECISION;
		rot.z = (float)eulerRotation.z / MAX_DECIMAL_PRECISION;
		//WriteDebug(vecToStr(worldPosition));
		return  rot;
	}
	RotationVector operator + (RotationVector& obj) {
		RotationVector res;
		glm::ivec3 newVec = eulerRotation + obj.GetTrueRotation();
		if (newVec.x > INT360) { newVec.x - INT360; }
		if (newVec.x < -INT360) { newVec.x + INT360; }
		if (newVec.y > INT360) { newVec.y - INT360; }
		if (newVec.y < -INT360) { newVec.y + INT360; }
		if (newVec.z > INT360) { newVec.z - INT360; }
		if (newVec.z < -INT360) { newVec.z + INT360; }
		res = RotationVector(newVec, false);
		return res;
	}
	RotationVector operator + (glm::vec3& obj) {
		RotationVector res;
		glm::ivec3 newVec =
			eulerRotation + glm::ivec3(obj.x * MAX_DECIMAL_PRECISION, obj.y * MAX_DECIMAL_PRECISION, obj.z * MAX_DECIMAL_PRECISION);
		if (newVec.x > INT360) { newVec.x - INT360; }
		if (newVec.x < -INT360) { newVec.x + INT360; }
		if (newVec.y > INT360) { newVec.y - INT360; }
		if (newVec.y < -INT360) { newVec.y + INT360; }
		if (newVec.z > INT360) { newVec.z - INT360; }
		if (newVec.z < -INT360) { newVec.z + INT360; }
		res = RotationVector(newVec, false);
		return res;
	}


	RotationVector operator - (RotationVector& obj) {
		RotationVector res;
		glm::ivec3 newVec = eulerRotation - obj.GetTrueRotation();
		if (newVec.x > INT360) { newVec.x - INT360; }
		if (newVec.x < -INT360) { newVec.x + INT360; }
		if (newVec.y > INT360) { newVec.y - INT360; }
		if (newVec.y < -INT360) { newVec.y + INT360; }
		if (newVec.z > INT360) { newVec.z - INT360; }
		if (newVec.z < -INT360) { newVec.z + INT360; }
		WriteDebug(vecToStr(newVec));
		res = RotationVector(newVec, false);
		return res;
	}
};

struct PhysicsTransform
{
private:
	glm::ivec3 position;
	RotationVector rotation;
public:

	//PhysicsTransform(glm::ivec3 pos, glm::ivec3 rot) : position(pos), rotation(rot) {};
	PhysicsTransform(glm::vec3 pos, glm::vec3 rot, bool isFloat)
	{
		if (!isFloat)
		{
			position = pos;
			rotation = RotationVector(rot, false);
		}
		else
		{
			position = ConvertFloatVec(pos);
			rotation = RotationVector(ConvertFloatVec(rot), false);
		}
	}
	PhysicsTransform() {};


	glm::vec3 GetWorldPosition()
	{
		glm::vec3 worldPosition;
		//WriteDebug(vecToStr(position));
		worldPosition.x = (float)position.x / MAX_DECIMAL_PRECISION;
		worldPosition.y = (float)position.y / MAX_DECIMAL_PRECISION;
		worldPosition.z = (float)position.z / MAX_DECIMAL_PRECISION;
		//WriteDebug(vecToStr(worldPosition));
		return  worldPosition;
	}
	glm::vec3 GetWorldRotation()
	{
		glm::vec3 worldRotation;
		//WriteDebug(vecToStr(position));
		worldRotation.x = (float)rotation.GetEulerRotation().x / MAX_DECIMAL_PRECISION;
		worldRotation.y = (float)rotation.GetEulerRotation().y / MAX_DECIMAL_PRECISION;
		worldRotation.z = (float)rotation.GetEulerRotation().z / MAX_DECIMAL_PRECISION;
		//WriteDebug(vecToStr(worldPosition));
		return  worldRotation;
	}

	void ModifyTransformPos(glm::vec3 newPos)
	{
		position = ConvertFloatVec(newPos);
	}
	void ModifyTransformRot(glm::vec3 newRot)
	{
		rotation = RotationVector(ConvertFloatVec(newRot), false);
	}

	//Original Physics Lock conserved. NOT modified.
	PhysicsTransform operator + (PhysicsTransform const& obj) {
		PhysicsTransform res;
		res.position = position + obj.position;
		return res;
	}
};
#pragma endregion

const glm::vec3 baseCameraUp = glm::vec3(0.0, 1.0, 0.0);
const glm::vec3 baseCameraFront = glm::vec3(0.0, 0.0, 1.0);

#pragma region Event Reqs
enum EventType
{
	EngineEvent,
	InputEvent,
	InventoryEvent,
	SoundEvent,
	MapEvent,
	NarrativeEvent,
	UIEvent,
};

class EventData
{
private:
	std::map<std::string, int > intData;
	std::map<std::string, int >::iterator intIter;
	std::map<std::string, std::string > stringData;
	std::map<std::string, std::string >::iterator stringIter;
	std::map<std::string, float > floatData;
	std::map<std::string, float >::iterator floatIter;
	std::map<std::string, glm::vec2 > vec2Data;
	std::map<std::string, glm::vec2 >::iterator vec2Iter;
	std::map<std::string, glm::vec3 > vec3Data;
	std::map<std::string, glm::vec3 >::iterator vec3Iter;
	//std::map<std::string, MAAD_GameObject> objectData;
	//std::map<std::string, MAAD_GameObject>::iterator objectIter;
	//std::map<std::string, MAAD_GameObject*> objectPTRData;
	//std::map<std::string, MAAD_GameObject*>::iterator objectPTRIter;


public:
#pragma region Setters
	void SetInt(std::string tag, int value)
	{
		intIter = intData.begin();
		intData.insert({ tag, value });
	}

	void SetString(std::string tag, std::string value)
	{
		stringIter = stringData.begin();
		stringData.insert({ tag, value });
	}

	void SetFloat(std::string tag, float value)
	{
		floatIter = floatData.begin();
		floatData.insert({ tag, value });
	}

	void SetVector2(std::string tag, glm::vec2 value) 
	{
		vec2Iter = vec2Data.begin();
		vec2Data.insert({ tag, value });
	}
	void SetVector3(std::string tag, glm::vec3 value)
	{
		vec3Iter = vec3Data.begin();
		vec3Data.insert({ tag, value });
	}
	//void SetObject(std::string tag, MAAD_GameObject value)
	//{
	//	objectIter = objectData.begin();
	//	objectData.insert({ tag, value });
	//}
	//void SetObjectPTR(std::string tag, MAAD_GameObject* value)
	//{
	//	objectPTRIter = objectPTRData.begin();
	//	objectPTRData.insert({ tag, value });
	//}
#pragma endregion

#pragma region Getters
	const int GetInt(std::string tag) { return intData.find(tag)->second; }
	const std::string GetString(std::string tag) { return stringData.find(tag)->second; }
	const float GetFloat(std::string tag) { return floatData.find(tag)->second; }
	const glm::vec2 GetVec2(std::string tag) { if (&vec2Data.find(tag) != nullptr) return vec2Data.find(tag)->second; }
	const glm::vec3 GetVec3(std::string tag) { if(&vec3Data.find(tag) != nullptr) return vec3Data.find(tag)->second; }


	//const MAAD_GameObject* GetObjPTR(std::string tag) { return objectPTRData.find(tag)->second; }
	//const MAAD_GameObject GetObjData(std::string tag) { return objectData.find(tag)->second; }

	//const int GetInt(std::string tag) { return intData.find(tag)->second; }
	//const int GetInt(std::string tag) { return intData.find(tag)->second; }
#pragma endregion
public:
	EventData() {};
};

class MAAD_EVENT
{
public:
	EventType Etype;
	EventData data = EventData();
	MAAD_EVENT(EventType etype, EventData edata) : Etype(etype), data(edata) {};
	MAAD_EVENT(EventType etype) : Etype(etype) { data = EventData(); };
	MAAD_EVENT() {};
};



void EventManagerUpdate();
void AddEventListener(EventListener* newListener, EventType criterion);
void AddEventListener(EventListener* newListener);
void RemoveEventListener(EventListener* removee);

class EventListener
{
public:
	std::vector <EventType> typeCriterion;
protected:
	virtual void EventResponse(MAAD_EVENT eventNotice)
	{
		//WriteDebug("Recieved Event:" + eventNotice.data.GetString("TestEvent"));
	};

public:
	void BASEEventResponse(MAAD_EVENT e) { EventResponse(e); }

	EventListener(std::vector<EventType> crit) : typeCriterion(crit) {};
	EventListener() {};
	virtual ~EventListener() { RemoveEventListener(this); }
};

class EventSender
{

	std::vector<EventListener*> listeners;
	std::vector<MAAD_EVENT> eventQueue;
	MAAD_EVENT currEvent;
public:

	EventSender() {};
	void QueueEvent(MAAD_EVENT e)
	{
		eventQueue.push_back(e);
	};
	void SendQueuedEvents() 
	{
		if (eventQueue.size() > 0) 
		{
			while (eventQueue.size() > 0)
			{
				SendEventImmediately(*eventQueue.begin());
				eventQueue.erase(eventQueue.begin());
			}
		}
		else 
		{
			//WriteDebug("No Events to send.");
		}
	}
	void SendEventImmediately(MAAD_EVENT eventNotice)
	{
		currEvent = eventNotice;
		for (int i = 0; i < listeners.size(); i++)
			if (!none_of(listeners[i]->typeCriterion.begin(), listeners[i]->typeCriterion.end(), [=](EventType eT) {return eT == eventNotice.Etype; }))
			{
				EventListener* p = &(*listeners[i]);
				p->BASEEventResponse(currEvent);
			}
	}
	void AddListener(EventListener* newLis) 
	{
		//WriteDebug("Adding listener " + std::to_string(listeners.size()));
		if (none_of(listeners.begin(), listeners.end(), [=](EventListener* e) {return e == newLis;}))
			listeners.push_back(newLis); 
		else { WriteDebug("Already contains that listener."); }
	}
	void RemoveListener(EventListener* removee) 
	{
		//WriteDebug("Removing listener " + std::to_string(listeners.size()));

		for (int i = 0; i < listeners.size(); i++)
		{
			if (listeners[i] == removee)
			{
				listeners.erase(listeners.begin() + i);
				//WriteDebug("Removed listener " + std::to_string(listeners.size()));
			}
		}
	}
	int GetTotalListeners() { return listeners.size(); };
};
#pragma endregion

class ViewModel
{
public:
	unsigned int ID;
	PhysicsTransform modelOffset;
	Model viewModel;
private:

};

class MAAD_GameObject : public EventListener
{
private:
	PhysicsTransform transform;
	std::string name;
	MAAD_IDController::MAAD_ID* objectID;
	std::vector<ViewModel> models;
public:
	void EventResponse(MAAD_EVENT e) 
	{
		EventListener:EventResponse(e);
	}
	MAAD_GameObject(std::string newName, int newID = -1, PhysicsTransform trans = PhysicsTransform()) :
		name(newName), transform(trans), objectID(GetIDController("GameObjects")->CreateNewID(newID)) {};
	MAAD_GameObject() {};
	
	std::vector<ViewModel> GetModels() { return models; }
	PhysicsTransform GetTransform() { return transform; }
	void MoveObjectToPosition(glm::vec3 vec) 
	{
		transform.ModifyTransformPos(vec);
	}
	void MoveObjectRelative(glm::vec3 vec)
	{
		transform.ModifyTransformPos(transform.GetWorldPosition() + vec);
	}

	class ObjectComponent
	{
	private:
	protected:
		MAAD_GameObject* parent;
		virtual void ComponentUpdate()
		{
		}
	public:
		ObjectComponent() {};
		glm::vec3 GetParentPosition() { return parent->GetTransform().GetWorldPosition(); }

	};
	class AnimatorComponent : public ObjectComponent
	{
	private:
		AnimData currentAnim;
		ViewModel* animatee;
		unsigned int currentFrame = 0;
		bool paused = true;
		bool looping = false;
	public:
		AnimatorComponent() {};
		void ChangeAnimatee(ViewModel* newModel)
		{
			currentFrame = 0;
			if (animatee != newModel)
				animatee = newModel;
		}

		void ComponentUpdate()
		{
			if (!paused) { currentFrame++; }


			//AnimEvent Check & Activation.
			//if(currentAnim.GetCurrFrame(currentFrame))








			if (currentFrame < currentAnim.GetLength())
			{
				if (!looping)
				{
					paused = true;
				}
				currentFrame = 0;
			}

		}

		//Plays the current animation where it last left off.
		void Play()
		{
			paused = false;
		}
		//Overload that changes the animation in question.
		void Play(AnimData newAnim)
		{
			currentFrame = 0;
		}

		void Pause()
		{
			paused = true;
		}
	};

	class PhysicsComponent: public ObjectComponent 
	{
	private: 
		PhysicsTransform* objectTransform;
		void ComponentUpdate()
		{

		}
	};

	class CameraComponent : public ObjectComponent
	{
		enum CameraType
		{
			Perspective,
			Orthographic,
		};
		enum CameraMode
		{
			LockedOnView,
			FreeView
		};
	private:
		glm::vec3 cameraOffset;
		RotationVector rotVec = RotationVector(glm::ivec3(0), false);
		glm::vec3 rotation = glm::vec3(0, 0, 0);
		glm::mat4 cameraView = glm::mat4(1.0f);
		unsigned int currActionFrame;
		//CameraAction currAction = noAction;
	public:

		MAAD_IDController::MAAD_ID* cameraID;
		CameraMode mode = FreeView;
		CameraType cameraType;
		glm::vec3 GetOffset() { return cameraOffset; }
		glm::vec3 GetTruePosition() { return GetParentPosition() + cameraOffset; }
		glm::vec3 GetCurrentRotation() { return rotation; };
		float cameraFov = 45.0f;

		CameraComponent(glm::vec3 offset = glm::vec3(), glm::vec3 rotation = glm::vec3(), float fov = 90, CameraType type = Perspective, int ID = -1) :
			cameraID(GetIDController("Cameras")->CreateNewID(ID)), cameraFov(fov), cameraType(type)
		{
			cameraOffset = offset;
			UpdateCameraView();
		};
		CameraComponent() {};

		glm::mat4 GetCameraView() { return cameraView; }

		void RotateCamera(glm::vec3 newRot)
		{
			rotVec = rotVec + newRot;
			UpdateCameraView();
		}

		glm::vec3 CalculateCamFront()
		{
			glm::vec3 finalFront = baseCameraFront;
			glm::vec3 rotation = rotVec.GetEulerRotation();
			//X Axis Operation
			finalFront.x = glm::sin(glm::radians(rotation.x));
			finalFront.z = glm::cos(glm::radians(rotation.x));

			if (rotation.y != 0)
			{
				//Y Axis Operation
				finalFront.x *= glm::cos(glm::radians(rotation.y));
				finalFront.z *= glm::cos(glm::radians(rotation.y));
				finalFront.y = glm::sin(glm::radians(rotation.y));
			}

			//WriteDebug("CameraFront: " + vecToStr(finalFront) + ". Rotation: " + vecToStr(rotation));
			//finalFront = baseCameraFront;
			//WriteDebug("BaseFront: " + vecToStr(finalFront));

			return finalFront;
		};
		glm::vec3 CalculateCamUp()
		{
			glm::vec3 camFront = CalculateCamFront();
			glm::vec3 finalUp = baseCameraUp;
			glm::vec3 rotation = rotVec.GetEulerRotation();

			//Y Axis Operation
			//finalUp.x *= glm::cos(glm::radians(rotation.y +90));
			finalUp.z = glm::cos(glm::radians(rotation.y + 90));
			finalUp.y = glm::sin(glm::radians(rotation.y + 90));

			//finalUp.x = ;
			//finalUp.z = ;

			//finalUp.y = glm::sin(glm::radians(rotation.y + 90));


			//WriteDebug("CameraUp: " + vecToStr(finalUp) + ". Rotation: " + vecToStr(rotation));
			finalUp = baseCameraUp;
			return finalUp;

		};


		//Updates camera view based on its offset and its current position.
		//cameraView is based on cameraFront and cameraUp, not on the rotation of the camera.
		void UpdateCameraView()
		{
			cameraView = glm::lookAt(GetTruePosition(), GetTruePosition() + CalculateCamFront(), CalculateCamUp());;
		}
		//Teleports the camera to a new location by changing the camera's offset from its base object.
		//--Overloads--
		//CameraCoords()
		//glm::vec3
		void MoveCamera(glm::vec3 newOffset)
		{
			cameraOffset = newOffset;
			UpdateCameraView();
		}
		//Teleports the camera to a new location. Does not reset the camera offset.
		//--Overloads--
		//CameraCoords()
		//glm::vec3
		//void MoveCamera(glm::vec3 moveTo)
		//{
		//	//cameraCoords.cameraPos += moveTo;
		//	UpdateCameraView();
		//}



		//Sets the current CameraAction and sets current active frame to 0
		//void PlayCameraAction(CameraAction action) 
		//{

		//	currAction = action;
		//	currActionFrame = 0;

		//}

		//Rotates camera action coordinates based upon the current rotation of the camera
		//CameraCoords RotateCoords(CameraCoords check) 
		//{
		//	CameraCoords newCoords = check;
		//	//WriteDebug("CurrRotation: " + vecToStr(currRotationAngle) + " Frame Rotation: " + vecToStr(newCoords.rotation));
		//	glm::quat checkQuat = glm::radians((currRotationAngle + newCoords.rotation) * 0.5f);
		//	//WriteDebug("Quat: " + quatToStr(checkQuat));
		//	//WriteDebug("Inverse: " + quatToStr(glm::inverse(checkQuat)));
		//	glm::vec3 rotatedPoint = checkQuat * newCoords.cameraPos * glm::inverse(checkQuat);
		//	glm::vec3 rotatedFront = checkQuat * newCoords.cameraFront * glm::inverse(checkQuat);
		//	glm::vec3 rotatedUp = checkQuat * newCoords.cameraUp * glm::inverse(checkQuat);

		//	//WriteDebug("Rotated: " + vecToStr(rotatedFront));
		//	CameraCoords rotatedCoords = CameraCoords(rotatedPoint, newCoords.rotation, rotatedFront, rotatedUp);

		//	return rotatedCoords;
		//}

		//Updates the current frame of the camera and the camera view. Runs once every RenderUpdate().
		void UpdateCamera()
		{
			UpdateCameraView();

			//if (currAction.ID != noAction.ID) 
			//{
			//	if (currActionFrame < currAction.actionFrames.size())
			//	{
			//		offsetCoords = RotateCoords(currAction.actionFrames[currActionFrame]);
			//		currActionFrame++;
			//	}
			//	else 
			//	{
			//		if (currAction.ID != noAction.ID) 
			//		{
			//			cameraCoords = cameraCoords + offsetCoords;
			//			currRotationAngle += currAction.actionFrames[currAction.actionFrames.size() - 1].rotation;
			//			//WriteDebug(vecToStr(cameraCoords.cameraFront));
			//			offsetCoords = CameraCoords();
			//			currAction = noAction;
			//		}
			//	}
			//}
		}

	};


private:
	std::vector<ObjectComponent> components;


public:
	void AddComponent()
	{
		
	}
};

class TestGameObject : public MAAD_GameObject 
{
public:
	void EventResponse(MAAD_EVENT e) 
	{
		WriteDebug("Will this work? " + e.data.GetString("TestEvent")); 
	};
	TestGameObject() { MAAD_GameObject(); };
};
void EventStartup();
void QueueEvent(MAAD_EVENT);
void SendEventImmediately(MAAD_EVENT);

