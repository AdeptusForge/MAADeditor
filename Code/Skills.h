#pragma once

#include "framework.h"
#include "Debug.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "Vector"
#include "iterator"
#include "Entity.h"

enum HitLevel 
{
	NoLevel,
	Clean,
	Hard,
};

enum TargetMode 
{
	Self,
	Single,
	SingleAlly,
	SingleEnemy,
	Multiple,
	MultipleAlly,
	MultipleEnemy,
	All,
	AllAllies,
	AllEnemies,
	RandomSingle,
	RandomSingleAlly,
	RandomSingleEnemy,
	RandomMultiple,
	RandomMultipleAlly,
	RandomMultipleEnemy,
};

bool isAutoTargeted(TargetMode mode) 
{
	switch (mode) 
	{
	case(Self): return true;
	case(All): return true;
	case(AllEnemies): return true;
	case(AllAllies): return true;
	case(RandomSingle): return true;
	case(RandomSingleAlly): return true;
	case(RandomSingleEnemy): return true;
	case(RandomMultiple): return true;
	case(RandomMultipleAlly): return true;
	case(RandomMultipleEnemy): return true;
	default: return false;
	}
	WriteDebug("Somehow got out of isAutoTargeted switch");
	return false;
}

enum EffectType 
{
	Damage,
	Buff,
	Debuff,
	Heal,
};

struct Effect 
{
	EffectType type;
	
};

//The real meat behind the Skill class. Links into each other for multihit attacks or for affecting costs
class SkillActionSegment
{
protected:
	int segmentID;
	TargetMode targetMode;
	//std::vector<Entity> targets;
	HitLevel skillHitLevel;
	Effect effect;

	void TargetEntities()
	{
		if (!isAutoTargeted(targetMode)) 
		{

		}
		else
		{
			switch (targetMode)
			{
			case(Self): break;
			case(All): break;
			case(AllEnemies): break;
			case(AllAllies): break;
			case(RandomSingle): break;
			case(RandomSingleAlly): break;
			case(RandomSingleEnemy): break;
			case(RandomMultiple): break;
			case(RandomMultipleAlly): break;
			case(RandomMultipleEnemy): break;
			}
		}
	};


public:
	void FailureCheck()
	{

	};
	void CriticalCheck()
	{

	};
	//void SuccessEffects(std::vector<Entity> targets) 
	//{
	//	WriteDebug("SkillActionSegment: " + std::to_string(segmentID) + " succeeded");
	//};
	//void FailureEffects(std::vector<Entity> targets) 
	//{
	//	WriteDebug("SkillActionSegment: " + std::to_string(segmentID) + " failed");
	//};
};

class TestSkill: public SkillActionSegment
{


	//void SuccessEffects(std::vector<Entity> targets)
	//{
	//	SkillActionSegment::SuccessEffects(targets);
	//	WriteDebug("TestSkill succeeded");
	//}
	//void FailureEffects(std::vector<Entity> targets)
	//{
	//	SkillActionSegment::FailureEffects(targets);
	//	WriteDebug("TestSkill succeeded");
	//}
};

//specifically for UI and storage purposes
struct Skill 
{
	unsigned int skillID;
	std::string name;
	std::string description;
	unsigned int failChance;
	SkillActionSegment effect;
};

struct SkillSet
{
private:
	Skill skill1;
	Skill skill2;
	Skill skill3;
	Skill skill4;
public:
};

void ActivateSkill(Skill skill) 
{
};