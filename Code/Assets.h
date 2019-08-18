#pragma once
#include <string>
#include <list> 
#include <iterator> 
#include "Points.h"
#include <vector>

using namespace std;


//class SpriteSheet 
//{
//private:
//
//public:
//
//	SDL_Surface spriteSheet;
//	int frameCount;
//
//	//height x width
//	Point2Int frameDimensions;
//
//	//Constructors
//	SpriteSheet(SDL_Surface sheet, int count, Point2Int dim) : spriteSheet(sheet), frameCount(count), frameDimensions(dim) {};
//
//	vector<SDL_Surface> ReturnSprites() 
//	{
//		vector<SDL_Surface> sprites;
//		//TODO: add in sprite sheet support.
//	};
//};
//
//class AnimFrame2D
//{
//private:
//
//public:
//	SDL_Surface frame;
//	int duration;
//	//TODO: add possible animation tie-ins like scripts etc. & figure out method to remove duration
//
//	AnimFrame2D(SDL_Surface image, int dur = 1)
//	{
//		frame = image;
//		duration = dur;
//	}
//};
//
//class Animation2D 
//{
//private:
//
//public:
//	vector <AnimFrame2D> animData;
//
//	Animation2D(vector<SDL_Surface> imageSet) 
//	{
//		animData = vector <AnimFrame2D>();
//
//		for (int i=0; i < imageSet.size(); i++) 
//		{
//			animData.insert(animData.end(),AnimFrame2D(imageSet.at(i), 1));
//		}
//	};
//};