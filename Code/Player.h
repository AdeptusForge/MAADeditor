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
#include "map.h"
#include "Inventory.h"

//Records both the status of the player and the equipment they currently possess.
class PlayerStatus
{
public:
	class Hands
	{
	private:
		//true = right, false = left. 
		//2-handed weapons are always "wielded" from the dominant hand, and supported by the non-dominant hand
		bool dominantHand;
		InventoryItem leftHand;
		InventoryItem rightHand;
		bool bandageWrapped;
		unsigned int health;
		unsigned int tempHealth;
		unsigned int armor;

		//TODO:: Finger Damage

	public:
		Hands() {};

		void LeftHandAction1() {};
		void LeftHandAction2() {};
		void RightHandAction1() {};
		void RightHandAction2() {};
	};

	class Arm 
	{
	private:
			InventoryItem clothLayer1;
			InventoryItem clothLayer2;

	public:
		Arm() {};
	};

	class Torso 
	{
	private:
		//If it goes to 0, you die.
		unsigned int health;
		//tempHealth is unaffected by armor.
		unsigned int tempHealth = 0;
		//Flat damage reduction
		unsigned int armor = 0;
	public:
		void InjureHealth(int amount)
		{
			unsigned int amountRemaining = amount;
			if (tempHealth > 0) 
			{
				if (amountRemaining >= tempHealth) 
				{
					tempHealth = 0;
					amountRemaining = amount - tempHealth;
				}
				else 
					tempHealth -= amountRemaining;
			}
			else if (tempHealth = 0 || amountRemaining > 0) 
			{
				health -= (amountRemaining - armor);
			}
		}
	};
	class Head 
	{
		bool bandageWrapped;
	public:
		Head() {};
	};
private:
	Hands hands;
	Arm rightArm;
	Arm leftArm;
	Torso torso;
	Head head;

public:
	PlayerStatus() {};

	void PlayerStatusStartup() {};

};
