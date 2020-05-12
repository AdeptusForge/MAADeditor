#pragma once

#include "Entity.h"
#include "Skills.h"

struct EntitySlot 
{
	Entity& occupiedBy;
};

struct EntityTeam 
{
	EntitySlot slot1;
	EntitySlot slot2;
	EntitySlot slot3;
};

EntityTeam playerTeam;
EntityTeam enemyTeam;