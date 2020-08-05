#pragma once

#include "filecontrol.h"
#include "bounding.h"
#include "assets.h"
#include "vector"
#include "camera.h"
#include "assets.h"
#include "map"
#include "Debug.h"

#define DEFAULT_2D_UIELEMENT 

class MAAD_UIElement : public MAAD_GameObject
{
protected:
	unsigned int elementID;
	Model* modelPTR;
	glm::ivec2 pixelSize;
	//Element scale. Either in pixels, or in 0 to 1 percentages.
	glm::vec2 scale;
	glm::vec2 screenlocation;
	bool active;

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
		WriteDebug(vecToStr(newScale));

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
			newScale.x = scale.x * pixelSize.x / windowSize.x;
		if (scale.y > windowSize.y)
			WriteDebug("Y Pixel size is larger than the screen");
		else
			newScale.y = scale.y * pixelSize.y / windowSize.y;
		WriteDebug(vecToStr(newScale));

		return newScale;
	};
	//Places the object based on percentage sizes of the screen. Rounds to the nearest pixel to prevent half-pixels.
	//Returns center of the UIElement in OpenGL screen coordinates.
	glm::vec3 UILocationAbsolute(glm::vec2 windowSize)
	{
		glm::vec3 newLoc;
		return newLoc;
	};

	void BindModel(Model* newModel) 
	{
		WriteDebug("BoundModel to UI element");
		modelPTR = newModel;
	};
	Model GetModel() 
	{
		return *modelPTR;
	}

	unsigned int GetID() { return elementID; };

	bool Active() { return active; };
	//Activates/Deactivates given UIElement.
	void SetState(bool state) {
		active = state; 
	};
	virtual glm::vec3 CalculateElementOffset(Camera* target)
	{
		return glm::vec3(0,0,0);
	};
	virtual void UpdateElement(Shader shader, Camera* target)
	{
		//WriteDebug("Updated element: " + std::to_string(elementID));
		if (modelPTR != nullptr) 
		{
			//WriteDebug("Camera Position: " + vecToStr(CalculateElementOffset(target)));
			shader.setMat4("model", 
				modelPTR->ModelRefresh(shader, CalculateElementOffset(target), UNIVERSAL_RENDERSCALE, glm::vec3(0)));
		}
	};

	MAAD_UIElement() : active(true) {};
	MAAD_UIElement(unsigned int id) : elementID(id), active(true) {};
};

class TestUIElement : public MAAD_UIElement 
{
protected:


public:
	TestUIElement(unsigned int id)
	{
		elementID = id;
		active = true;
		//model = Model("UI");
	}
	void UpdateElement(Shader shader, Camera* target)
	{
		MAAD_UIElement::UpdateElement(shader, target);
		//WriteDebug("TestUIElement Update");
	}
};

class MAAD_UIContext 
{
private:
	std::map<unsigned int, Model> uiModels;
	std::map<unsigned int, Model>::iterator uiModelIter;

	Camera* targetCamera;
public:
	MAAD_UIContext() {};
	std::vector<MAAD_UIElement*> elementPTRs;
	TestUIElement newTest = TestUIElement(14);

	void UIStartup() 
	{
		targetCamera = FindCamera(17);
		AddElement(&newTest, Model("TestCube"));
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