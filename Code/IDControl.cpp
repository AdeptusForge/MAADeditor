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

unsigned int renderIDCheck;
std::vector <unsigned int> recycledRenderIDs;
//DO NOT EDIT DURING RUNTIME
unsigned int TOTAL_RENDERIDS_AVAILABLE = 16384;
unsigned int inventoryIDCheck;
std::vector <unsigned int> recycledInventoryIDs;
//DO NOT EDIT DURING RUNTIME
unsigned int TOTAL_INVENTORYIDS_AVAIALBLE = 512;
unsigned int cameraIDCheck;
std::vector <unsigned int> recycledCameraIDs;
//DO NOT EDIT DURING RUNTIME
unsigned int TOTAL_CAMERAIDS_AVAIALBLE = 32;

void RecycleID(MAAD_ID idCheck)
{
	switch (idCheck.Type())
	{
		case(RENDER_ID):
		{
			if (!std::none_of(recycledRenderIDs.begin(), recycledRenderIDs.end(), [&](unsigned int i) {return i == idCheck.ID();}))
			{
				WriteDebug("ERROR -- Recycling already recycled ID for type: Render");
				break;
			}
			recycledRenderIDs.push_back(idCheck.ID());
			break;
		}
		case(INVENTORY_ID):
		{
			if (!std::none_of(recycledInventoryIDs.begin(), recycledInventoryIDs.end(), [&](unsigned int i) {return i == idCheck.ID(); }))
			{
				WriteDebug("ERROR -- Recycling already recycled ID for type: Inventory");
				break;
			}
			recycledInventoryIDs.push_back(idCheck.ID());
			break;
		}
		case(CAMERA_ID):
		{
			if (!std::none_of(recycledCameraIDs.begin(), recycledCameraIDs.end(), [&](unsigned int i) {return i == idCheck.ID(); }))
			{
				WriteDebug("ERROR -- Recycling already recycled ID for type: Inventory");
				break;
			}
			recycledCameraIDs.push_back(idCheck.ID());
			break;
		}
		default:
		{
			WriteDebug("ERROR -- No IDTYPE determined during recycling.");
			break;
		}
	}
}

unsigned int GetFirstAvailableID(IDTYPE idtype) 
{
	unsigned int* idCheck_PTR = nullptr;
	std::vector<unsigned int>* recycledIDs = nullptr;
	unsigned int maxIDs;
	switch (idtype) 
	{
		case(RENDER_ID): 
		{
			idCheck_PTR = &renderIDCheck;
			recycledIDs = &recycledRenderIDs;
			maxIDs = TOTAL_RENDERIDS_AVAILABLE;
			break;
		}
		case(INVENTORY_ID):
		{
			idCheck_PTR = &inventoryIDCheck;
			recycledIDs = &recycledInventoryIDs;
			maxIDs = TOTAL_INVENTORYIDS_AVAIALBLE;
			break;
		}
		case(CAMERA_ID):
		{
			idCheck_PTR = &cameraIDCheck;
			recycledIDs = &recycledCameraIDs;
			maxIDs = TOTAL_CAMERAIDS_AVAIALBLE;
			break;
		}
		default:
		{
			WriteDebug("ERROR -- No IDTYPE determined during checking availabiity.");
			break;
		}
	}
	unsigned int result = *idCheck_PTR;
	*idCheck_PTR += 1;
	//result = *idCheck_PTR;
	if (*idCheck_PTR >= maxIDs) 
	{
		WriteDebug("ERROR -- TOO MANY IDS");
	}
	if (recycledIDs->size() >= 1)
	{
		WriteDebug("using recycled id");
		result = *std::min_element(recycledIDs->begin(), recycledIDs->end());
		recycledIDs->erase(std::min_element(recycledIDs->begin(), recycledIDs->end()));
	}
	//WriteDebug("result " + std::to_string(result));
	//WriteDebug("render " + std::to_string(renderIDCheck));
	//WriteDebug("inventory " + std::to_string(inventoryIDCheck));

	return result;
}

void IDStartup()
{
	renderIDCheck, inventoryIDCheck = 0;
	WriteDebug("ID Control Startup -- Successful");

}