#pragma once

#include "filecontrol.h"
#include "bounding.h"
#include "assets.h"
#include "vector"
#include "camera.h"
#include "assets.h"
#include "map"
#include "Debug.h"
#include "Objects.h"

#define DEFAULT_2D_UIELEMENT 

const unsigned int SCR_H = 360;
const unsigned int SCR_W = 640;

class MAAD_UIElement : public EventListener
{
protected:
	unsigned int elementID;
	Model* modelPTR;
	glm::ivec2 pixelSize;
	//Element scale. Either in pixels, or in 0 to 1 percentages.
	//Until further notice, set scale as the size of the image you want to use in your UI for best results
	glm::vec2 scale;
	//always defined in pixels, but processed into pixel coordinates or absolute coordinates before rendering
	glm::vec2 screenLocation;
	bool active = true;
	bool is2D = true;

public:
	//Scales the object based on percentage sizes of the screen.
	glm::vec3 UIScaleAbsolute(glm::ivec2 windowSize) 
	{
		glm::vec3 newScale;
		if (scale.x > 1)
			WriteDebug("X Percentage is larger than 100% of the screen");
		else
			newScale.x = scale.x * pixelSize.x * windowSize.x;
		if (scale.y > 1)
			WriteDebug("Y Percentage is larger than 100% of the screen");
		else
			newScale.y = scale.y * pixelSize.y * windowSize.y;
		//WriteDebug("scale?:" +vecToStr(newScale));

		return newScale;
	};
	//Scales the object based on exact pixel size of the screen. 
	//Essentially this means that things will scale to preserve the size of the pixels of the element rather than the state of the UI.
	glm::vec3 UIScalePixels(glm::ivec2 windowSize) 
	{
		glm::vec3 newScale;
		if (scale.x > windowSize.x)
			WriteDebug("X Pixel size is larger than the screen");
		else
			newScale.x = scale.x * (pixelSize.x * 10) / windowSize.x;
		if (scale.y > windowSize.y)
			WriteDebug("Y Pixel size is larger than the screen");
		else
			newScale.y = scale.y * (pixelSize.y*10) / windowSize.y;
		//WriteDebug(vecToStr(newScale));

		return newScale;
	};
	//Places the object based on percentage sizes of the screen. Rounds to the nearest pixel to prevent half-pixels.
	//Returns center of the UIElement in OpenGL screen coordinates.
	glm::vec3 UILocationAbsolute(glm::vec2 screenDims)
	{
		glm::vec3 newLoc = glm::vec3(screenLocation, 0);
		//float measureX = screenLocation.x / screenDims.x;
		//float measureY = screenLocation.y / screenDims.y;
		//newLoc.x = (measureX * 20) - 10;
		//newLoc.y = (measureY * 20) - 10;
		return newLoc;
	};

	void BindModel(Model* newModel) 
	{
		WriteDebug("BoundModel to UI element");
		modelPTR = newModel;
	};
	Model* GetModelPTR() 
	{
		return modelPTR;
	}

	unsigned int GetID() { return elementID; };

	bool Active() { return active; };
	//Activates/Deactivates given UIElement.
	void SetState(bool state) {
		active = state; 
	};
	//Used for 3D objects
	virtual glm::vec3 CalculateElementOffset(Camera* target)
	{
		return glm::vec3(0);
	};
	virtual void UpdateElement(Shader shader, Camera* target)
	{
		//WriteDebug("Updated element: " + std::to_string(elementID));
		if (modelPTR != nullptr) 
		{
			scale = UNIVERSAL_RENDERSCALE;
			pixelSize = glm::vec2(8,8);
			scale = UIScalePixels(glm::vec2(SCR_W,SCR_H));
			//WriteDebug("Scale" + vecToStr(scale));
			if (is2D) 
			{
				shader.setMat4("model",
					modelPTR->ModelRefresh(shader, UILocationAbsolute(glm::vec2(SCR_W,SCR_H)), UNIVERSAL_RENDERSCALE * glm::vec3(scale, 1), glm::vec3(0)));

			}
			else 
			{
				shader.setMat4("model",
					modelPTR->ModelRefresh(shader, CalculateElementOffset(target), UNIVERSAL_RENDERSCALE * glm::vec3(scale, 1), glm::vec3(0)));
			}
		}
	};

	protected:
	MAAD_UIElement() : active(true) 
	{
		typeCriterion.push_back(UIEvent);
		typeCriterion.push_back(EngineEvent);
		AddEventListener(this);
	};
	public:
	MAAD_UIElement(unsigned int id) : elementID(id), active(true) 
	{ 
		typeCriterion.push_back(UIEvent); 
		typeCriterion.push_back(EngineEvent);
		AddEventListener(this);
	};
};

class InventoryGrid : public MAAD_UIElement 
{
private: 
	//InventorySpace* inventory;
protected:


public:
	InventoryGrid(unsigned int id, glm::vec2 scrLoc = glm::vec2(0), glm::vec2 pixSize = glm::vec2(8))
	{
		elementID = id;
		active = true;
		is2D = true;
		screenLocation = scrLoc;
		//model = Model("UI");
	}
	void UpdateElement(Shader shader, Camera* target)
	{
		MAAD_UIElement::UpdateElement(shader, target);
	}
};

class MAAD_UIContext : public MAAD_GameObject
{
private:
	std::map<unsigned int, Model> uiModels;
	std::map<unsigned int, Model>::iterator uiModelIter;

	Camera* targetCamera;
public:
	MAAD_UIContext() {};
	std::vector<MAAD_UIElement*> elementPTRs;
	InventoryGrid newTest1 = InventoryGrid(14);
	InventoryGrid newTest2 = InventoryGrid(15, glm::vec2(0,5));
	InventoryGrid newTest3 = InventoryGrid(16, glm::vec2(5, 0));



	void UIStartup() 
	{
		targetCamera = FindCamera(17);
		AddElement(&newTest1, Model("UIPlaneTest"));
		AddElement(&newTest2, Model("UIPlaneTest"));
		//AddElement(&newTest3, Model("UIPlaneTest"));


	}
	
	Camera* GetTargetCamera() { return targetCamera; }
	MAAD_UIElement* GetElementByID(const unsigned int id) 
	{

	}
	Model* GetUIModel() 
	{

	}
	void UIMouseSelect(glm::vec2 position) {};
	void AddElement(MAAD_UIElement* newElement, Model elementModel)
	{
		uiModelIter = uiModels.begin();
		uiModels.insert({ newElement->GetID(), elementModel });
		newElement->BindModel(&uiModels.find(newElement->GetID())->second);
		elementPTRs.push_back(newElement);
	}
};