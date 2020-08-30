#include "time.h"
#include "SDL_timer.h"
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

std::vector<MAAD_Timer> allTimers;


//Gets the current ticks per frame. Used mostly in timestep testing.
int GetCurrentTick()
{
	return currTick / physicsFrameInTicks;
};

unsigned int SecondsToFrames(float seconds) {
	WriteDebug(seconds);
	WriteDebug((unsigned int)(seconds * 60));
	return (unsigned int)(seconds * 60);
}


//Begins time startup.
void TimeStartup() 
{

	priorTick = startup;
	WriteDebug("Time Startup -- Successful");
	//WriteDebug(to_string(physicsFrameInTicks));

};

//Updates game time based on the computer's clock and the current tick. 
//Updates Rendering, Physics, Inputs, and map interactions.
void UpdateTime(GLFWwindow* window)
{
	currTick = SDL_GetPerformanceCounter();
	deltaTime = currTick - priorTick;


	if (deltaTime >= physicsFrameInTicks)
	{
		priorTick = currTick;
		
		//PhysicsUpdate();
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