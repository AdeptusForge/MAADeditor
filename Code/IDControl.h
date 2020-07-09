#pragma once

#include "framework.h"
#include <fstream>
#include <String>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "debug.h"
#include <vector>
#include <algorithm>

enum IDTYPE
{
	RENDER_ID = 16384,
	INVENTORY_ID = 512,
	CAMERA_ID = 32,
	MAPENTITY_ID = 32,
};

class IDController
{
private:

	IDTYPE IDPurpose;
	unsigned int IDCheck;
	std::vector <unsigned int> recycledIDs;
	//DO NOT EDIT DURING RUNTIME
	unsigned int totalIDsAvailable;
	unsigned int GetFirstAvailableID() {
		if (recycledIDs.size() >= 1)
		{
			//WriteDebug("Using recycled ID");
			unsigned int result = *std::min_element(recycledIDs.begin(), recycledIDs.end());
			recycledIDs.erase(std::min_element(recycledIDs.begin(), recycledIDs.end()));
			return result;
		}
		else
		{
			unsigned int newID = IDCheck;
			IDCheck++;
			return newID;
		}
	};
public:
	unsigned int GetTotalAvailable() { return totalIDsAvailable; }
	IDTYPE CheckPurpose() { return IDPurpose; }
	std::vector<unsigned int>* GetRecycledIDs() { return &recycledIDs; }
	IDController(IDTYPE purpose) :
		IDPurpose(purpose), IDCheck(0), totalIDsAvailable((unsigned int)purpose) {};


	class MAAD_ID
	{
	private:
		IDTYPE type;
		unsigned int id;

	public:
		const unsigned int ID() { return id; }
		IDTYPE Type() { return type; }
		//DO NOT USE. USE CreateNewID() UNDER AN IDController INSTEAD.
		MAAD_ID(IDTYPE newtype, unsigned int ID) : type(newtype), id(ID) {};

	};
private:
	std::vector <MAAD_ID> allIDs;
	struct IDFinder {

		IDFinder(unsigned int const& id) : ID(id) { }
		bool operator () (MAAD_ID el) const { return  el.ID() == ID; }
	private:
		unsigned int ID;
	};
public:
	MAAD_ID* CreateNewID()
	{
		if (allIDs.size() >= totalIDsAvailable)
		{
			WriteDebug("ERROR -- TOO MANY IDS CREATED");
			return nullptr;
		}
		allIDs.push_back(MAAD_ID(CheckPurpose(), GetFirstAvailableID()));
		return &allIDs.back();
	};
	void RecycleID(MAAD_ID* idCheck)
	{
		if (idCheck->Type() != IDPurpose) { WriteDebug("ERROR -- Cannot Recycle ID for different controller."); }

		if (!std::none_of(recycledIDs.begin(), recycledIDs.end(), [&](unsigned int i) {return i == idCheck->ID(); }))
			WriteDebug("ERROR -- Cannot recycle already recycled ID.");
		else
		{
			recycledIDs.push_back(idCheck->ID());
		}
	}
};

void IDStartup();

