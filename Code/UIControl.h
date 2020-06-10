#pragma once

#include "filecontrol.h"
#include "bounding.h"
#include "assets.h"
#include "vector"
#include "camera.h"
#include "assets.h"

#define DEFAULT_2D_UIELEMENT 


class UISelectable : MAAD_BoundingBox 
{
private:
	typedef void (*BB_UIActivateCallbackFunc)();
	//Used when a UIBoundingBox is selected using UI code.
	BB_UIActivateCallbackFunc uiCallbackFunc;

public:
	//Sets size in pixels. Does not edit the Z dimension.
	void SetPixelDimensions(glm::ivec2 pixelDims) { dim = glm::vec3(pixelDims,dim.z); };
	//Sets size based on screen size percentage. Does not edit the Z dimension.
	void SetScreenDimensions(glm::vec2 screenDims) 
	{

	};
	
	//Function that is rarely if ever used. Kept just in case.
	//Only really used in making UI bounding boxes 3D, but there's never a need for that.
	void EditZDimension(float newZ) { dim = glm::vec3(dim.x, dim.y, newZ); };

	//Sets UI bounding box position. 
	//UIContext edits this using a camera reference to keep the UI aligned to the target camera.
	void SetPosition(glm::vec3 position) {};

};

class MAAD_UIElement
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

	void bindModel(Model* newModel) 
	{
		modelPTR = newModel;
	};
	unsigned int GetID() { return elementID; };

	bool Active() { return active; };
	//Activates/Deactivates given UIElement.
	void SetState(bool state) {
		active = state; 
	};

	virtual void UpdateElement(Shader shader)
	{
		//WriteDebug("Updated element: " + std::to_string(elementID));
		if (modelPTR != nullptr) 
		{
			WriteDebug("RefreshedModel");
			modelPTR->ModelRefresh(shader);
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
	void UpdateElement(Shader shader)
	{
		MAAD_UIElement::UpdateElement(shader);
		//WriteDebug("TestUIElement Update");
	}
};



class MAAD_UIContext 
{
private:
	std::vector<Model> uiModels;
	Camera* targetCamera;
public:
	MAAD_UIContext() {};
	std::vector<MAAD_UIElement*> elementPTRs;
	TestUIElement newTest = TestUIElement(14);

	void UIStartup() 
	{
		AddElement(&newTest);
	}


	MAAD_UIElement* GetElementByID(const unsigned int id) 
	{

	}
	void UIMouseSelect(glm::vec2 position) {};
	void AddElement(MAAD_UIElement* newElement)
	{
		elementPTRs.push_back(newElement);
	}
};