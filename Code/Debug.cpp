#include <string>
#include <fstream>
#include "Time.h"
#include <iostream>


void WriteDebug(std::string phrase)
{
	std::string frameCounter = "Frame: " + std::to_string(GetCurrentTick()) + " - ";
	std::cout << frameCounter + phrase << std::endl;
}
