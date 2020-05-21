#pragma once
#include <ctime>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <String>
#include "Debug.h"

void TimeStartup();
int GetCurrentTick();
void UpdateTime(GLFWwindow*);

unsigned int SecondsToFrames(float seconds);

//
////Widely used frame-locked timer. Timer starts counting from 1 instead of 0, like all clocks.
//class MAADTimer 
//{
//public:
//	MAADTimer() {};
//	MAADTimer(unsigned int duration, std::string ID) : timerDuration(duration), timerID(ID) {};
//
//	void PauseTimer()
//	{
//		if (timerCount < timerDuration)
//		{
//			paused = true;
//		}
//		else 
//			WriteDebug("Cannot pause timer [" + timerID + "] -- Duration over.");
//	};
//	void RunTimer() 
//	{
//		paused = false;
//	};
//	void TimerEnd()
//	{
//
//	}
//	void TimerCompletion()
//	{
//		//TODO:: SEND TIMER COMPLETED EVENT
//		TimerEnd();
//	}
//	void TimerUpdate()
//	{
//		WriteDebug(timerID + " Updated");
//		if (!paused) 
//		{
//			if (timerCount >= timerDuration)
//			{
//				TimerCompletion();
//			}
//			timerCount++;
//		}
//	}
//private:
//	//Timer's duration in frames;
//	unsigned int timerDuration;
//	unsigned int timerCount;
//	bool paused = false;
//	std::string timerID;
//};