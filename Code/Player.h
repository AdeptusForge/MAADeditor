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
#include "objects.h"
#include "rngroll.h"

const unsigned int maxHandHealth = 15;
const unsigned int maxTorsoHealth = 15;
const unsigned int bandageTempHealth = 5;

enum FingerStatus
{
	NormalFinger,
	BrokenFinger,
	RemovedFinger,
	UnusableFinger
};

const unsigned int totalBreakPatterns = 25;

//All possible break patterns for the hands.
const unsigned int breakPatterns[25][5] =
{ {1,2,3,4,5}
};

class FingerBreakPattern 
{
private:
	unsigned int fingerOrder[5];
public:
	unsigned int GetNextBreak(unsigned int breakNumber) { 
		if (breakNumber <= 5 && breakNumber >= 1) { return fingerOrder[breakNumber-1]; }
		else { WriteDebug("Incorrect break number input for break pattern."); }
	}
	void ResetBreakPattern() 
	{
		int newPatternIndex = RollInt(totalBreakPatterns, 0);
		for(int i = 0; i < 4; i++)
			fingerOrder[i] = breakPatterns[newPatternIndex][i];
	};
};

class Affliction : public EventListener 
{
public:
	enum AfflictionType 
	{

	};

	void EventResponse() {}

};

//Records both the status of the player and the equipment they currently possess.
class PlayerStatus : public EventListener
{
public:
	class Hands
	{
	private:
		//true = right, false = left. 
		//2-handed objects are always "wielded" from the dominant hand, and supported by the non-dominant hand
		bool dominantHand;
		InventoryItem multiHandItem;

		class Hand
		{
		private:
			InventoryItem heldItem;
			unsigned int health = maxHandHealth;
			unsigned int tempHealth;
			unsigned int armor;
			FingerBreakPattern breakPattern;

		public:
			struct Finger
			{
			public:
				Finger() {};
			private:
				FingerStatus fStatus;
			public:
				Finger(FingerStatus status) : fStatus(status) {};
				FingerStatus GetStatus() { return fStatus; }
				void ChangeFingerStatus(FingerStatus status) { fStatus = status; };
			};
			//0 = Thumb, 1 = Index, 2 = Middle, 3 = Ring, 4 = Pinky
			Finger fingers[5] = { Finger(NormalFinger),Finger(NormalFinger) ,Finger(NormalFinger) ,Finger(NormalFinger) ,Finger(NormalFinger) };
			Hand() {};
			Hand(InventoryItem held, unsigned int health, unsigned int tempHealth, FingerBreakPattern bP) {};
			bool CanHoldItem(InventoryItem newItem)
			{

			};
			bool CanPerformAction(bool actionNumber)
			{
			};
			void ChangeHeldItem(InventoryItem newItem)
			{
				if (CanHoldItem(newItem))
				{
				}
			}
			//Positive numbers deal damage, negative numbers restore health.
			void AdjustHealth(int amount)
			{
				unsigned int amountRemaining = amount;

				if (amountRemaining >= 0)
				{
					amountRemaining -= armor;
					if (amountRemaining >= tempHealth)
					{
						amountRemaining -= tempHealth;
						tempHealth = 0;
					}
					else
						tempHealth -= amountRemaining;
				}

				health -= amountRemaining;
				if (health <= 0)
				{
					//Destroy Hand.
				}
				else if (health > maxHandHealth) { health = maxHandHealth; }
			}
			void GiveTempHealth(unsigned int amount) { tempHealth += amount; };
			void ChangeFinger(FingerStatus newStatus, int finger)
			{
				if (newStatus == NormalFinger) { breakPattern.ResetBreakPattern(); }
				fingers[finger].ChangeFingerStatus(newStatus);
			};
			int GetFingerStatus(int finger) { return (int)fingers[finger].GetStatus(); }
			int GetHealth() { return health; }
			int GetTempHealth() { return tempHealth; }
			int GetArmor() { return armor; }
		};
		Hand rightHand;
		Hand leftHand;
		bool bandageWrapped;

	public:
		Hands() {};
		Hands(bool domHand, Hand l, Hand r, bool bandaged) : dominantHand(domHand), rightHand(r), leftHand(l), bandageWrapped(bandaged) {};
		void LeftHandAction1() {};
		void LeftHandAction2() {};
		void RightHandAction1() {};
		void RightHandAction2() {};
		bool BandageHands() { if (!bandageWrapped) bandageWrapped = true; rightHand.GiveTempHealth(bandageTempHealth); }
		std::string GetHandsStatus() 
		{
			std::string newString = "Right Hand: \n"+ std::to_string(rightHand.GetHealth()) + " " + std::to_string(rightHand.GetArmor()) + " " + std::to_string(rightHand.GetTempHealth());
			for(int i =0; i < 4; i++)
				newString += " " + std::to_string(rightHand.GetFingerStatus(i));
			return newString;
		};
	};

	class Arm
	{
	private:
		InventoryItem clothLayer1;
		InventoryItem clothLayer2;

	public:
		void ApplySplint() {};
		Arm() {};
	};
	class Torso
	{
	private:
		InventoryItem clothLayer1;
		InventoryItem clothLayer2;

		//If it goes to 0, you die.
		unsigned int health = maxTorsoHealth;
		unsigned int tempHealth = 0;
		//Flat damage reduction. Occurs before temp damage is taken.
		unsigned int armor = 0;
	public:
		//Positive numbers deal damage, negative numbers restore health.
		void AdjustHealth(int amount)
		{
			unsigned int amountRemaining = amount;

			if (amountRemaining >= 0)
			{
				amountRemaining -= armor;
				if (amountRemaining >= tempHealth)
				{
					amountRemaining -= tempHealth;
					tempHealth = 0;
				}
				else
					tempHealth -= amountRemaining;
			}

			health -= amountRemaining;
			if (health <= 0)
			{
				//Kill Player.
			}
			else if (health > maxTorsoHealth) { health = maxHandHealth; }
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
	std::vector<Affliction> afflictions;


public:
	PlayerStatus() {};
	void DamagePlayer() 
	{
	}
	void GetHandsStatus() 
	{
		WriteDebug(hands.GetHandsStatus());
	}
	void EventResponse(MAAD_EVENT e)
	{
		WriteDebug("PlayerStatus Recieving events " + e.data.GetString("TestEvent"));
	};

	void PlayerStatusStartup() { 
		GetHandsStatus(); 
		AddEventListener(this, EngineEvent);
		for (int i = 0; i < afflictions.size(); i++) 
		{
			AddEventListener(&afflictions[i], EngineEvent);
		}

	};
	void ApplyAffliction(Affliction newAfflic) 
	{

	}
};

void LoadPlayerStatus();