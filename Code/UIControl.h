#pragma once

#include "filecontrol.h"
#include "bounding.h"
#include "assets.h"
#include "vector"
#include "camera.h"

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
private:
	const unsigned int elementID;
public:
	bool active;
	std::vector<UISelectable> selectionBoxes;
	std::vector<MAAD_UIElement> children;

	void ActivationFunction() {};
};

class MAAD_UIContext 
{
	std::vector<MAAD_UIElement> elements;
	Camera* targetCamera;
	void UpdateUI() {};
	void UIMouseSelect(glm::vec2 position) {};
};

