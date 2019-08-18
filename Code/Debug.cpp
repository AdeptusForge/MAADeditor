#include <string>
#include <fstream>
#include "Time.h"
#include <iostream>

using namespace std;


void WriteDebug(string phrase)
{
	string frameCounter = "Frame: " + to_string(GetCurrentTick()) + " - ";
	cout << frameCounter + phrase << endl;
}
