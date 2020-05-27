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

class InventoryItem
{
private:

	
	

protected: 
	glm::ivec2 shape;
	bool horizontal;
	std::string itemID;
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

	std::string ItemID() { return itemID; };
	
	void BASEHeldAction() { HeldAction(); }
	void BASEInteract() { Interact(); }
	void BASECombine() { Combine(); }

};

class TestItem : InventoryItem
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
		else if (occupiedBy->ItemID() != EMPTY_ITEM_ID) return false;
		else return true;
	}
	InventoryNode() { occupiedBy = nullptr; };


};

class InventorySpace
{
private:
	glm::ivec2 size;
	std::vector<std::vector<InventoryNode>> nodes;
	std::vector<InventoryItem> heldItems;

public:
	InventoryItem* GetItem(std::string ID) 
	{
	};
	glm::ivec2 GetNodeSize() { return size; };
	InventoryNode* GetNode(glm::ivec2 location) {}
	glm::ivec2 VecToNodeLoc(glm::vec2 location) 
	{
		return glm::ivec2(0);
	}
	void FillNodes(InventoryItem* item, glm::vec2 location) {};
	void TransferItemTo(InventoryItem* transfer, InventorySpace* to, glm::vec2 spot) 
	{

	}

	InventorySpace() {};
	InventorySpace(glm::ivec2 newSize) : size(newSize)
	{

	}
};