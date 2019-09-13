#pragma once
#include <GLFW/glfw3.h>
#include "Points.h"

using namespace std;



//An individual slice of inputs during every 1/120th of a second. Collected after processing.
class InputFrame
{
private:
public:
	bool Up = false;
	bool UpLeft = false;
	bool Left = false;
	bool DownLeft = false;
	bool Down = false;
	bool DownRight = false;
	bool Right = false;
	bool UpRight = false;
	bool NeutralVert = true;
	bool NeutralHori = true;


	Point2 mousePos;
	
	//NEEDS CONSTRUCTORS

//Essentially designed to handle specific inputs in sequenced input frames (think QCF and DP motions)
class CommandInputMacro
{
private:
public:


};

void InputControlStartup();



void CollectInputs(GLFWwindow* window);

InputFrame ProcessInputs();



