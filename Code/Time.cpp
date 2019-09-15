#include "time.h"
#include "Debug.h"
#include "SDL_timer.h"
#include "Physics.h"
#include "Render.h"
#include "Input.h"

//Shows number of ticks since last update, useful for running time-based functions.
Uint64 deltaTime;
Uint64 startup = SDL_GetPerformanceCounter();
Uint64 totalTicksSinceStartup;
float physicsFrameInTicks = (SDL_GetPerformanceFrequency() / 120);
Uint64 currTick;
Uint64 priorTick;
bool lastFrameRender;

int totalRenders=0;

static bool cont = true;
int loops = 0;
int maxLoops = 2000000;
int i = 0;

bool TimeWorkingProperly() 
{
	if (cont)
		return true;
	return false;
}

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

void UpdateTime()
{
	loops++;
	currTick = SDL_GetPerformanceCounter();
	deltaTime = currTick - priorTick;


	if (deltaTime >= physicsFrameInTicks)
	{
		priorTick = currTick;
		
		PhysicsUpdate();
		SaveInputs();

			if (!lastFrameRender) 
			{
				RenderUpdate();
				lastFrameRender = true;
			}
			else
				lastFrameRender = false;
		loops = 0;
	}
	if (i++ > maxLoops) cont = false;

}

