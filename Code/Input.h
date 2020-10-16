#pragma once
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const int MAX_SAVED_INPUTFRAMES = 20;

//An individual slice of inputs during every 1/120th of a second. Collected after processing.
class InputFrame
{
private:
	//W = up, X = right, Y = down, Z = left
	glm::bvec4 cardinalDirections;
	
	//Buttons correspond to alphabetical order. For example, W = Button 1 and Y = Button 3
	glm::bvec4 buttons;

	glm::bvec4 buttons2;


public:
	InputFrame(glm::bvec4 dirsB = glm::bvec4(false), glm::bvec4 buttonB = glm::bvec4(false), glm::bvec4(button2B) = glm::bvec4(false)) :
		cardinalDirections(dirsB), buttons(buttonB), buttons2(button2B) {};
	glm::bvec4 RAW_DIRECTIONS() { return cardinalDirections; }
	glm::bvec4 RAW_BUTTONS() { return buttons; }
	glm::bvec4 RAW_BUTTONS2() { return buttons2; }



	bool BUTTON_1() { return buttons.w; }
	bool BUTTON_2() { return buttons.x; }
	bool BUTTON_3() { return buttons.y; }
	bool BUTTON_4() { return buttons.z; }
	bool UP() { return cardinalDirections.w; }
	bool RIGHT() { return cardinalDirections.x; }
	bool DOWN() { return cardinalDirections.y; }
	bool LEFT() { return cardinalDirections.z; }
	bool VERT_NEUTRAL() { if (!UP() && !DOWN()) return true; else return false; }
	bool HORI_NEUTRAL() { if (!RIGHT() && !LEFT()) return true; else return false; }
	bool DIAG_UPRIGHT() { if (UP() && RIGHT()) return true; else return false; }
	bool DIAG_UPLEFT() { if (UP() && LEFT()) return true; else return false; }
	bool DIAG_DOWNRIGHT() { if (DOWN() && RIGHT()) return true; else return false; }
	bool DIAG_DOWNLEFT() { if (DOWN() && LEFT()) return true; else return false; }
	bool ESCAPE() { return buttons2.x; }
};

enum MouseMode
{
	MenuMode,
	ModelEditMode,
	ObjectEditMode
};

//Essentially designed to handle specific inputs in sequenced input frames (think QCF and DP motions)
//REFACTOR:: Input macro detection.
class CommandInputMacro
{
private:
public:

	CommandInputMacro(){};
};

void InputControlStartup(GLFWwindow* window);
void CollectInputs(GLFWwindow*, int, int, int, int);
void SaveInputs();
void RunInputs();
void ChangeMappedInput();
void BlockAllInputs();
void UnblockAllInputs();
