#include "time.h"
#include "Debug.h"
#include "SDL_timer.h"
#include "Physics.h"
#include "Render.h"
#include "Input.h"
#include "map.h"

//Shows number of ticks since last update, useful for running time-based functions.
Uint64 deltaTime;
Uint64 startup = SDL_GetPerformanceCounter();
Uint64 totalTicksSinceStartup;
float physicsFrameInTicks = (SDL_GetPerformanceFrequency() / 120);
Uint64 currTick;
Uint64 priorTick;
bool lastFrameRender;

int totalRenders=0;

int GetCurrentTick()
{
	return currTick / physicsFrameInTicks;
};

void TimeStartup() 
{

	priorTick = startup;
	WriteDebug("Time Startup -- Successful");
	//WriteDebug(to_string(physicsFrameInTicks));

};

void UpdateTime(GLFWwindow* window)
{
	currTick = SDL_GetPerformanceCounter();
	deltaTime = currTick - priorTick;


	if (deltaTime >= physicsFrameInTicks)
	{
		priorTick = currTick;
		
		PhysicsUpdate();
		SaveInputs();

			if (!lastFrameRender) 
			{
				RenderUpdate(window);
				RunInputs();
				UpdateMap();
				lastFrameRender = true;
			}
			else
				lastFrameRender = false;
	}

}

