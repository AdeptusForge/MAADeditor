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

class MAAD_IDController
{
private:

	std::string IDPurpose;
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
	std::string CheckPurpose() { return IDPurpose; }
	std::vector<unsigned int>* GetRecycledIDs() { return &recycledIDs; }

	MAAD_IDController(std::string purpose, unsigned int maxIDs) :
		IDPurpose(purpose), IDCheck(0), totalIDsAvailable(maxIDs) {};


	class MAAD_ID
	{
	private:
		std::string type;
		unsigned int id;

	public:
		const unsigned int ID() { return id; }
		std::string Type() { return type; }
		//DO NOT USE. USE CreateNewID() UNDER AN IDController INSTEAD.
		MAAD_ID(std::string newtype, unsigned int ID) : type(newtype), id(ID) {};

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
	std::vector<MAAD_ID>* GetAllIDs() { return &allIDs; }
	//Creates a pointer to a new ID. Use a positive number to create an ID with a specific number. 
	//Negative numbers are used for standard generation
	MAAD_ID* CreateNewID(int setID = -1)
	{
		if (allIDs.size() >= totalIDsAvailable)
		{
			WriteDebug("ERROR -- TOO MANY IDS CREATED");
			return nullptr;
		}

		if (setID < 0) 
		{
			allIDs.push_back(MAAD_ID(CheckPurpose(), GetFirstAvailableID()));
		}
		else 
		{
			if (std::none_of(allIDs.begin(), allIDs.end(), [=](MAAD_ID elem) {return setID == elem.ID(); })) 
			{
				allIDs.push_back(MAAD_ID(CheckPurpose(), setID));
			}
			else 
			{
				WriteDebug("Cannot Create Identical ID");
			}
		}
		return &allIDs.back();

	};

	void DeleteID(MAAD_ID* idCheck) 
	{
		std::vector<MAAD_ID>::iterator eraser = std::find_if(allIDs.begin(), allIDs.end(), [&](MAAD_ID i) {return i.ID() == idCheck->ID(); });
		allIDs.erase(eraser);
	}
	void DeleteID(unsigned int idCheck)
	{
		std::vector<MAAD_ID>::iterator eraser = std::find_if(allIDs.begin(), allIDs.end(), [&](MAAD_ID i) {return i.ID() == idCheck; });
		allIDs.erase(eraser);
	}

	void RecycleID(MAAD_ID* idCheck)
	{
		if (idCheck->Type() != IDPurpose) { WriteDebug("ERROR -- Cannot Recycle ID for different controller."); }

		if (!std::none_of(recycledIDs.begin(), recycledIDs.end(), [&](unsigned int i) {return i == idCheck->ID(); }))
			WriteDebug("ERROR -- Cannot recycle already recycled ID.");
		else
		{
			DeleteID(idCheck);
			recycledIDs.push_back(idCheck->ID());
		}
	}
	void RecycleID(unsigned int idCheck)
	{
		if (!std::none_of(recycledIDs.begin(), recycledIDs.end(), [&](unsigned int i) {return i == idCheck; }))
			WriteDebug("ERROR -- Cannot recycle already recycled ID.");
		else
		{
			DeleteID(idCheck);
			recycledIDs.push_back(idCheck);
		}
	}
};

void IDStartup();
MAAD_IDController* GetIDController(std::string purpose);