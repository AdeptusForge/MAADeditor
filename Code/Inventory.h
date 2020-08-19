#pragma once

#include "framework.h"
#include <fstream>
#include <String>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "debug.h"
#include <vector>

#define EMPTY_ITEM_ID "txtxtxtx"
#define EMPTY_ITEM InventoryItem(glm::vec2(1), EMPTY_ITEM_ID)

class InventoryItem
{
private:
protected: 
	glm::ivec2 shape;
	bool horizontal;
	std::string itemID;
	//Location is based upon the top-right corner of the object within a given inventory grid.
	glm::ivec2 location;

	virtual void HeldAction() 
	{
		WriteDebug("BaseItem: " + itemID + vecToStr(shape));
	}
	virtual void Interact() {}
	virtual void Combine() {}

public:
	InventoryItem() { itemID = EMPTY_ITEM_ID; }
	InventoryItem(glm::ivec2 size, std::string ID)
	{
		shape = size;
		itemID = ID;
	};

	const std::string GetItemID() { return itemID; };
	const glm::ivec2 GetShape() { return shape; };

	void BASEHeldAction() { HeldAction(); }
	void BASEInteract() { Interact(); }
	void BASECombine() { Combine(); }
};

class TestItem : public InventoryItem
{
private:

public:
	void HeldAction()
	{
		InventoryItem::HeldAction();
		WriteDebug("TestItem");
	}
	TestItem(glm::ivec2 size, std::string ID)
	{
		shape = size;
		itemID = ID;
	};
};

class InventoryNode 
{
private:
	InventoryItem* occupiedBy;
public:
	bool Filled() {
		if (occupiedBy == nullptr) return false;
		else if (occupiedBy->GetItemID() != EMPTY_ITEM_ID) return false;
		else return true;
	}
	InventoryNode() { occupiedBy = nullptr; };
	InventoryNode(InventoryItem* item) { occupiedBy = item; };

};

class InventorySpace
{
private:
	glm::ivec2 size;
	std::vector<std::vector<InventoryNode*>> nodes;
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
	InventoryNode* GetNode(glm::ivec2 location) {}
	glm::ivec2 VecToNodeLoc(glm::vec2 location) 
	{
		return glm::ivec2(0);
	}
	
	void TransferItemTo(InventoryItem* transfer, InventorySpace* to, glm::vec2 newLocation) 
	{
		if (to != this)
		{
			if (transfer->GetItemID() != EMPTY_ITEM_ID)
			{

			}
			else
				WriteDebug("Inventory cannot transfer Empty Node");
		}
		else
			WriteDebug("Inventory cannot transfer to itself");
	}
	void AddItem(InventoryItem newItem, glm::vec2 newLocation)
	{
		allItems.push_back(newItem);
		FillNodes(&newItem, newLocation);
	}
	void RemoveItem(InventoryItem* item)
	{

	}

	void FillNodes(InventoryItem* item, glm::vec2 location)
	{
		glm::ivec2 fill = item->GetShape();

		for (int y = 0; y < fill.y; y++)
		{
			for (int x = 0; x < fill.x; x++)
			{
				//nodes[x].push_back(InventoryNode(&InventoryItem()));
				WriteDebug(std::to_string(x) + ", " + std::to_string(y));
			}
		}
	};
	InventorySpace() 
	{
		FillNodes(&EMPTY_ITEM, glm::vec2(0));
	};
	InventorySpace(glm::ivec2 newSize) : size(newSize)
	{
		FillNodes(&EMPTY_ITEM, newSize);
	}
};