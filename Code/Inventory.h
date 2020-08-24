#pragma once

#include "framework.h"
#include <fstream>
#include <String>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "debug.h"
#include <vector>
#include "UIControl.h"

enum ItemType 
{
	TestItem,

};

class InventoryItem
{
private:
	glm::ivec2 shape;
	bool horizontal;
	std::string itemID;
	//Location is based upon the bottom-left corner of the object within a given inventory grid.
	glm::ivec2 location;
	ItemType iType;


public:
	InventoryItem() {}
	InventoryItem(glm::ivec2 size, std::string ID, ItemType newType) : shape(size), itemID(ID), iType(newType) {};

	std::string GetItemID() { return itemID; };
	glm::ivec2 GetShape() { return shape; };
	ItemType GetType() { return iType; };
	void HeldAction()
	{
		switch(iType) 
		{
			case TestItem: 
			{
				WriteDebug("Held Action of TestItem");
				break;
			}
			Default:
			{
				WriteDebug("No Item Type on this item.");
				break;
			}
		}
	}
	void Interact() {}
	void Combine() {}


};


class InventoryNode 
{
private:
	InventoryItem* occupiedBy;

public:

	bool Filled() {
		if (occupiedBy != nullptr) return true;
		return false;
	}
	InventoryItem* GetItem() 
	{ 
		if (occupiedBy != nullptr)
			return occupiedBy;
		WriteDebug("No Item in this node");
		return nullptr;
	}
	//Fills the node with an item reference. If no reference is given, will empty the node instead.
	void OccupyNode(InventoryItem* item = nullptr) 
	{
		//if (item == nullptr) { WriteDebug("Emptying Node"); }
		//if (item != nullptr) { WriteDebug("Filling Node"); }
		WriteDebug(item->GetItemID());
		if (occupiedBy == nullptr)
			occupiedBy = item;
		else
			WriteDebug("Already has occupying item");
	}
	InventoryNode() { occupiedBy = nullptr; };
	InventoryNode(InventoryItem* item) { occupiedBy = item; };

};

class InventorySpace
{
private:
	glm::ivec2 size;
	std::vector<std::vector<InventoryNode>> nodes;
	std::vector<InventoryItem> allItems;

public:
	struct IDFinder {
		
		IDFinder(std::string const& id) : ID(id) { }

		bool operator () (InventoryItem el) const { return  el.GetItemID() == ID; }

	private:
		std::string ID;
	};
	InventoryItem* GetItem(std::string ID)
	{
		if (!allItems.empty())
		{
			InventoryItem* p;
			std::vector<InventoryItem>::iterator it = std::find_if(allItems.begin(), allItems.end(), IDFinder(ID));
			if (!std::none_of(allItems.begin(), allItems.end(), IDFinder(ID)))
				p = &(*it);
			else
			{
				WriteDebug("No InventoryItem with matching ID");
				return nullptr;
			}
			return p;

		}
		WriteDebug("No AllItems");
		return nullptr;
	};
	std::vector<InventoryItem> *GetAllItems() { return &allItems; }
	glm::ivec2 GetNodeSize() { return size; };
	InventoryNode* GetNode(glm::ivec2 location) { return &nodes[1][1]; }
	
	void TransferItemTo(InventoryItem* transfer, InventorySpace* to, glm::vec2 newLocation) 
	{
		if (to != this)
		{
			
		}
		else
			WriteDebug("Inventory cannot transfer to itself");
	}


	void InitFillNodes()
	{
		for (int y = 0; y < size.y; y++)
		{
			std::vector<InventoryNode> temp;

			for (int x = 0; x < size.x; x++)
			{
				//WriteDebug(std::to_string(x) + ", " + std::to_string(y));
				temp.push_back(InventoryNode());
			}
			nodes.push_back(temp);
		}
	};
	bool NodesOpen(glm::ivec2 location, glm::ivec2 region = glm::ivec2(1))
	{
		for (int y = location.y; y < region.y + location.y; y++)
		{
			for (int x = location.x; x < region.x + location.x; x++)
			{
				if (nodes[x][y].Filled()) 
				{
					WriteDebug("Nodes occupied.");
					return false;

				}
			}
		}
		WriteDebug("Nodes open.");
		return true;
	}
	void EditNodesInArea(glm::ivec2 location, glm::ivec2 region = glm::ivec2(1), InventoryItem* item = nullptr)
	{
		std::vector<InventoryNode*> nodesToEdit;
		for (int y = location.y; y < region.y + location.y; y++)
		{
			for (int x = location.x; x < region.x + location.x; x++)
			{
				WriteDebug(vecToStr(glm::vec2(x,y)));
				nodesToEdit.push_back(&nodes[x][y]);
			}
		}
		for (int i = 0; i < nodesToEdit.size(); i++) 
		{
			nodesToEdit[i]->OccupyNode(item);
		}
		WriteDebug("Got here");

	}

	void AddItem(InventoryItem newItem, glm::vec2 newLocation)
	{
		InventoryItem freshItem = newItem;
		if (NodesOpen(newLocation, freshItem.GetShape()))
		{
			allItems.push_back(freshItem);
			EditNodesInArea(newLocation, freshItem.GetShape(), &allItems[allItems.size() - 1]);
		}
	}
	void RemoveItem(InventoryItem* item)
	{

	}
	InventorySpace() { InitFillNodes(); };
	InventorySpace(glm::ivec2 newSize) : size(newSize) { InitFillNodes(); }
};

class InventorySelector
{
private:
	//The Location of the selector within a given inventory.
	glm::ivec2 selectorLocation;
	//The InventorySpace the selector is currently acting within.
	InventorySpace* inventoryCurrentlyIn;
	InventoryItem* selectedItem;
public:

	void MoveSelector(MapDirection move) 
	{
		glm::ivec2 moveVec;
		switch (move) 
		{
			case North: { moveVec = glm::vec2(0, 1);  break; }
			case South: { moveVec = glm::vec2(0, -1);  break; }
			case East: { moveVec = glm::vec2(1, 0);  break; }
			case West: { moveVec = glm::vec2(-1, 0);  break; }
		}

		glm::vec2 testVec = selectorLocation + moveVec;
		if (testVec.x > inventoryCurrentlyIn->GetNodeSize().x -1 || testVec.x < 0) 
		{
			WriteDebug("cannot move to testVec location:" + vecToStr(testVec));
		}
		else if (testVec.y > inventoryCurrentlyIn->GetNodeSize().y -1 || testVec.y < 0)
		{
			WriteDebug("cannot move to testVec location:" + vecToStr(testVec));
		}
		else 
			selectorLocation = testVec;
		WriteDebug(vecToStr(selectorLocation));
	}
	void InteractWithItem() 
	{
		if (selectedItem != nullptr) 
		{
			selectedItem->HeldAction(); 
		}
	}

	void SelectNode() 
	{
		if (inventoryCurrentlyIn == nullptr) 
		{
			WriteDebug("cannot use selector if it isn't be inside an inventory.");
		}
		else
		{
			if (selectedItem == nullptr)
			{
				InventoryNode* node = inventoryCurrentlyIn->GetNode(selectorLocation);
				if (node->Filled())
				{
					InventoryItem* iPtr = inventoryCurrentlyIn->GetNode(selectorLocation)->GetItem();
					selectedItem = (iPtr != nullptr) ? iPtr : nullptr;
					WriteDebug("ItemSelected");
				}
				else
					WriteDebug(vecToStr(selectorLocation) + "empty");
			}
			else 
			{
				//TODO: ADD SOMETHING HERE
			}
		}
	}
	InventorySelector() {};
	InventorySelector(InventorySpace* space) : inventoryCurrentlyIn(space) {};
};