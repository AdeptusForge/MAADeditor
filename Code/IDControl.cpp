#include "framework.h"
#include <fstream>
#include <String>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "debug.h"
#include <vector>
#include "IDControl.h"

IDController controllerTest1 = IDController(RENDER_ID);
IDController controllerTest2 = IDController(INVENTORY_ID);


void IDStartup()
{
	IDController::MAAD_ID* testID1 = controllerTest1.CreateNewID();
	WriteDebug(testID1->ID());
	IDController::MAAD_ID* testID2 = controllerTest1.CreateNewID();
	WriteDebug(testID2->ID());
	IDController::MAAD_ID* testID3 = controllerTest1.CreateNewID();
	WriteDebug(testID3->ID());
	controllerTest1.RecycleID(testID3);
	IDController::MAAD_ID* testID4 = controllerTest1.CreateNewID();
	WriteDebug(testID4->ID());

	WriteDebug("ID Control Startup -- Successful");
}