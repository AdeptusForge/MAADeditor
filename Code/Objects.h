#pragma once
#include "Physics.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "assets.h"
#include "Components.h"


class ViewModel
{
public:
	PhysicsTransform modelOffset;
	Model viewModel;


};


class GameObject
{
public:
	PhysicsTransform transform;
	std::string name;
	int ID;
	std::vector<ViewModel> models;

	class ObjectComponent
	{
	private:
		GameObject* parent;
	protected:
		virtual void ComponentUpdate()
		{
		}
	public:
		ObjectComponent() {};

	};
	class Animator : ObjectComponent
	{
	private:
		AnimData currentAnim;
		ViewModel* animatee;
		unsigned int currentFrame = 0;
		bool paused = true;
		bool looping = false;
	public:
		Animator() {};
		void ChangeAnimatee(ViewModel* newModel)
		{
			currentFrame = 0;
			animatee = newModel;
		}

		void ComponentUpdate()
		{
			if (!paused) { currentFrame++; }


			//AnimEvent Check & Activation.
			//if(currentAnim.GetCurrFrame(currentFrame))








			if (currentFrame < currentAnim.GetLength())
			{
				if (!looping)
				{
					paused = true;
				}
				currentFrame = 0;
			}

		}

		//Plays the current animation where it last left off.
		void Play()
		{
			paused = false;
		}
		//Overload that changes the animation in question.
		void Play(AnimData newAnim)
		{
			currentFrame = 0;
		}

		void Pause()
		{
			paused = true;
		}
	};

	void AddComponent()
	{

	}
private:
	std::vector<ObjectComponent> components;
};





