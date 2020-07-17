#include "framework.h"
#include <fstream>
#include <String>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "debug.h"
#include <vector>
#include "IDControl.h"
#include <algorithm>


std::vector<MAAD_IDController> allIDControllers;

MAAD_IDController* GetIDController(std::string purpose) 
{
	struct IDFinder {

		IDFinder(std::string const& id) : ID(id) { }

		bool operator () (MAAD_IDController el) const { return  el.CheckPurpose() == ID; }
	private:
		std::string ID;
	};
	MAAD_IDController* p;
	std::vector<MAAD_IDController>::iterator it = std::find_if(allIDControllers.begin(), allIDControllers.end(), IDFinder(purpose));

	if (!std::none_of(allIDControllers.begin(), allIDControllers.end(), IDFinder(purpose)))
		p = &(*it);
	else
	{
		WriteDebug("No IDController with matching ID");
		return nullptr;
	}
	return p;
}

void IDStartup()
{
	allIDControllers.push_back(MAAD_IDController("GameObjects", 262144));

	allIDControllers.push_back(MAAD_IDController("RenderObjects", 16384));
	allIDControllers.push_back(MAAD_IDController("InventoryItems", 128));
	allIDControllers.push_back(MAAD_IDController("Cameras", 32));


	WriteDebug("ID Control Startup -- Successful");
}