#include "Input.h"
#include <GLFW/glfw3.h>
#include "Points.h"
#include <iterator>
#include <vector>

using namespace std;

vector<InputFrame> priorFrames;
vector<InputFrame>::iterator ptr;


void InputControlStartup() 
{
	for (int i = 0; i < 20; i++) 
	{
		ptr = priorFrames.begin();
		//ptr = priorFrames.insert(ptr, InputFrame());
	}
}

