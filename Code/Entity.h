#pragma once

#include "Skills.h"




enum EntityType
{
	NoType = 0,
	Human = 1,
	Beast = 2,
	Spirit = 3,
	Night = 4,
	Flood = 5,
	Plague = 6,
	Guardian = 7,
	Puppet = 8,
	Avian = 9,
	Lady = 10,
	Wicked = 11,
	Serpent = 12,
	Beacon = 13,
	Goddess = 14,
	Fiend = 15,
	Star = 16,
	Ancient = 17,
};

struct Entity 
{
private: 
	unsigned int maxHealth;
	int currHealth;
	bool canSwapSkills;
public:
	int getCurrHealth() { return currHealth; };
	void setCurrHealth(int value){ currHealth = value; }
	unsigned int getMaxHealth() { return maxHealth; };
	void setMaxHealth(unsigned int value) { maxHealth = value; }


	std::string name;

	SkillSet equippedSkills;
	SkillSet aiUseableSkills;
};