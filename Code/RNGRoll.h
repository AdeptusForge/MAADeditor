#pragma once

#include "framework.h"
#include "Debug.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>


enum RollType 
{
	FlatPercentRoll,
	CumulativePercentRoll,
	AdjustedPercentRoll
};


struct RollParameters 
{
	std::pair<int, int> rollOutOf;

	unsigned int failureChance;
	RollType typeOfRoll;
	int attemptNumber;

	//RNG roll between any numbers specified;
	RollParameters(int min=1, int max = 100, int failPercent = 0, RollType type =FlatPercentRoll, int attempts =0) :
		rollOutOf(std::pair<int, int>(min, max)), failureChance(failPercent), typeOfRoll(type), attemptNumber(attempts) {};
	//Generalized RNG roll between 1 and 100;
	RollParameters(int failPercent, RollType type, int attempts = 0)
		: rollOutOf(std::pair<int, int>(1, 100)), failureChance(failPercent), typeOfRoll(type), attemptNumber(attempts) {};

};

//Rolls and integer based on the size & location of the roll.
//location is the lowest number possible.
//Range: location <= RollInt <= location+size
int RollInt(int size=100, int location=1) {return std::rand() % size + location;}

//Rolls a success or failure based on the roll parameters. One roll occurs at every location RollBool is executed.
bool RollBool(RollParameters params) 
{
	int rollLocation = std::get<0>(params.rollOutOf);
	int rollSize = std::get<1>(params.rollOutOf) - rollLocation + 1;
	float failurePercent =  (float)params.failureChance/ 100;
	int failureNumber = (rollSize * failurePercent) + rollLocation -1;
	int rolledNumber = RollInt(rollSize, rollLocation);

	//WriteDebug("Size: " + std::to_string(rollSize) + ". Location: " +
	//	std::to_string(rollLocation) + ". Failure Number: " + std::to_string(failureNumber) + ".");


	switch(params.typeOfRoll) 
	{
		case(FlatPercentRoll): 
		{
			break;
		}
		case(CumulativePercentRoll): 
		{
			break;
		}
		case(AdjustedPercentRoll): 
		{
			break;
		}
	}

	if (rolledNumber < failureNumber || rolledNumber == rollLocation) { WriteDebug("Failed" + std::to_string(rolledNumber)); return false; }
	else { WriteDebug("Succeeded" + std::to_string(rolledNumber)); return true; }
};

