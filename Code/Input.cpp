#include <glad/glad.h> 
#include "Input.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iterator>
#include <vector>
#include "Debug.h"
#include "AudioControl.h"
#include "Render.h"
#include "Camera.h"
#include "Objects.h"
#include "RNGRoll.h"
#include "map.h"
#include "UIcontrol.h"
#include "Inventory.h"
#include "Player.h"

ModelDataChunk TestChunk;

std::vector<InputFrame> priorFrames;
std::vector<InputFrame>::iterator ptr;
MouseMode currMouseMode;

glm::bvec4 dirs;
glm::bvec4 buttons;
glm::bvec4 buttons2;

InputFrame newFrame;
InputFrame currFrame;
MAAD_GameObject testGameObject;
bool isSelecting;

InventorySpace testInventory = InventorySpace(glm::vec2(7,7));
InventorySelector testSelector = InventorySelector(&testInventory);


PlayerStatus testStatus = PlayerStatus();
MAAD_EVENT inputEvent;


//Defines when the player can input.
bool canInput;
void BlockAllInputs() { canInput = false; };
void UnblockAllInputs() { canInput = true; };


static void cursorPoitionCallback(GLFWwindow* window, double xPos, double yPos) 
{
	WriteDebug(vecToStr(glm::vec2(xPos, yPos)));
}
static void cursorClickCallback(GLFWwindow* window, int button, int action, int mods) 
{
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);

	if (action == GLFW_PRESS) 
	{
		WriteDebug("Clicked at location: " + vecToStr(glm::vec2(xPos, yPos)));
		inputEvent = MAAD_EVENT(EngineEvent);
		inputEvent.data.SetVector2("MousePos", glm::vec2(xPos, yPos));
		if(button == GLFW_MOUSE_BUTTON_LEFT)
			inputEvent.data.SetInt("LeftClick", 1);
		else if(button == GLFW_MOUSE_BUTTON_RIGHT)
			inputEvent.data.SetInt("LeftClick", 0);

		QueueEvent(inputEvent);
		//UIMouseSelect(glm::vec2(xPos, yPos));
	}
}

//sets the key event callback to CollectInputs() and fills the inputframe list with empty data so no input errors can occur.
void InputControlStartup(GLFWwindow* window)
{
	glfwSetKeyCallback(window, CollectInputs);
	//glfwSetCursorPosCallback(window, cursorPoitionCallback);
	glfwSetMouseButtonCallback(window, cursorClickCallback);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	for (int i = 1; i <= MAX_SAVED_INPUTFRAMES; i++) 
	{
		ptr = priorFrames.begin();
		ptr = priorFrames.insert(ptr, InputFrame());
	}

	
	UnblockAllInputs();
}

//Records inputs on a given frame and saves them as an InputFrame.
////BE CAREFUL--
//glfwSetKeyCallback is very quirky in how it collects inputs and shows behaviour that is strange.
//Example: it will pull in a single input per key-press, then ignore/forget all others currently occurring.
void CollectInputs(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (canInput) 
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			if (key == GLFW_KEY_W)
			{
				dirs.w = true;
			}
			if (key == GLFW_KEY_D)
			{
				dirs.x = true;
			}
			if (key == GLFW_KEY_S)
			{
				dirs.y = true;
			}
			if (key == GLFW_KEY_A)
			{
				dirs.z = true;
			}


			if (key == GLFW_KEY_U)
			{
				buttons.w = true;
			}
			if (key == GLFW_KEY_I)
			{
				buttons.x = true;
			}
			if (key == GLFW_KEY_O)
			{
				buttons.y = true;
			}
			if (key == GLFW_KEY_P)
			{
				buttons.z = true;
			}
			if (key == GLFW_KEY_ESCAPE)
			{
				buttons2.x = true;
			}
		}

		if (action == GLFW_RELEASE)
		{
			if (key == GLFW_KEY_W)
			{
				dirs.w = false;
			}
			if (key == GLFW_KEY_D)
			{
				dirs.x = false;
			}
			if (key == GLFW_KEY_S)
			{
				dirs.y = false;
			}
			if (key == GLFW_KEY_A)
			{
				dirs.z = false;
			}

			if (key == GLFW_KEY_U)
			{
				buttons.w = false;
			}
			if (key == GLFW_KEY_I)
			{
				buttons.x = false;
			}
			if (key == GLFW_KEY_O)
			{
				buttons.y = false;
			}
			if (key == GLFW_KEY_P)
			{
				buttons.z = false;
			}
			if (key == GLFW_KEY_ESCAPE)
			{
				buttons2.x = false;
			}
		}
		newFrame = InputFrame(dirs, buttons, buttons2);
	}
}

//Moves the inputFrame list forward to the current frame and erases the oldest one.
void SaveInputs()
{
	ptr = priorFrames.begin();
	ptr = priorFrames.insert(ptr, currFrame);
	currFrame = newFrame;
	priorFrames.pop_back();
	dirs = currFrame.RAW_DIRECTIONS();
	buttons = currFrame.RAW_BUTTONS();
	buttons2 = currFrame.RAW_BUTTONS2();
}

//Runs inputs. Called once every frame to prevent input errors in a singleplayer environment.
//If you wanted to handle multiplayer or an update-based framework, you'd need to change this to compensate.
void RunInputs() 
{
	Camera* cam = FindCamera(17);
	//WriteDebug(vecToStr(currFrame.RAW_DIRECTIONS()) + " " + vecToStr(currFrame.RAW_BUTTONS()));
	if (currFrame.UP()) 
	{
		//testSelector.MoveSelector(North);


		//GetMapEntity(0)->Walk(GetMapEntity(0)->GetCurrentFacing(1));
		//for (int i = 0; i < walkForward.size(); i++)
			//testGameObject.MoveObjectRelative(GetMapEntity(0)->RotateCoords(walkForward[i]));
		//WriteDebug(vecToStr(testGameObject.GetTransform().GetWorldPosition()));
		//testGameObject.MoveObjectRelative(glm::vec3(0,0,1));
	}
	if (currFrame.RIGHT())
	{
		//testSelector.MoveSelector(East);

		cam->RotateCamera(glm::vec3(-5, 0, 0));
		
		
		//GetMapEntity(0)->Rotate(0);

		
		//testGameObject.MoveObjectRelative(glm::vec3(-1, 0, 0));

	}
	if (currFrame.LEFT()) 
	{
		//testSelector.MoveSelector(West);

		cam->RotateCamera(glm::vec3(5, 0, 0));
		
		
		//GetMapEntity(0)->Rotate(1);
		
		//testGameObject.MoveObjectRelative(glm::vec3(1, 0, 0));
	}
	if (currFrame.DOWN())
	{
		//testSelector.MoveSelector(South);

		//cam->RotateCamera(glm::vec3(0, -5, 0));
		//testGameObject.MoveObjectRelative(glm::vec3(1,0,0));
		//WriteDebug(vecToStr(testGameObject.GetTransform().GetWorldPosition()));
		//GetMapEntity(0)->Flip();


		//testGameObject.MoveObjectRelative(glm::vec3(0, 0, -1));
	}

	if (currFrame.BUTTON_1()) 
	{
		//testSelector.SelectNode();

		//testGameObject.MoveObjectRelative(glm::vec3(0, 1, 0));
		//cam->RotateCamera(glm::vec3(0, 5, 0));

		//cam->RotateCamera(glm::vec3(0, 0, -5));
		//GetRenderObject(1)->objModel.StartAnim("AnimLoadTest");
	}
	if (currFrame.BUTTON_2())
	{
		//testSelector.InteractWithItem();

		testStatus.PlayerStatusStartup();

		//testGameObject.MoveObjectRelative(glm::vec3(0, -1, 0));
		//cam->RotateCamera(glm::vec3(0, -5, 0));
		//ResizeWindow(GetWindow(), glm::vec2(640, 360));

		//cam->RotateCamera(glm::vec3(0, 0, 5));
		//cam->PlayCameraAction(LookMiddleFromUp);
		//GetRenderObject(1)->objModel.StartAnim("AnimLoadTest");
	}
	if (currFrame.BUTTON_3()) 
	{
		//testStatus.TestRemoval();

		ResizeWindow(GetWindow(), glm::vec2(640, 360), false);

	}
	if (currFrame.BUTTON_4())
	{
		ResizeWindow(GetWindow(), glm::vec2(1920,1080), true);

		//cam->RotateCamera(glm::vec3(0, 0, 5));
		//cam->PlayCameraAction(LookMiddleFromUp);
		
		//GetRenderObject(1)->objModel.StartAnim("AnimLoadTest");

		//InventoryItem testItem(glm::vec2(2), "whatever", EmptyItemType);
		//testInventory.AddItem(testItem, glm::vec2(0));

	}
	cam->MoveCamera(testGameObject.GetTransform().GetWorldPosition());

	if (currFrame.ESCAPE()) 
	{
		ResizeWindow(GetWindow(), glm::vec2(640, 360), false);
	}
}