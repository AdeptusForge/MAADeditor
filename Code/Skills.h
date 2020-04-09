#pragma once

#include "framework.h"
#include "Debug.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "Vector"
#include "iterator"

enum HitLevel 
{
	None,
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

enum EffectType 
{
	Attack,
	Buff,
	Debuff,

};

struct Effect 
{
	EffectType type;
	TargetMode target;
	HitLevel skillHitLevel;
	int EffectID;
	int duration;
	bool canMiss;
	bool canCrit;
};

struct Skill 
{
	std::string name;
	std::string description;
	std::vector<Effect> effects;


	Skill() {};
};


struct SkillSet 
{
	Skill skill1;
	Skill skill2;
	Skill skill3;
	Skill skill4;
};

void ApplyEffects(Effect) {};