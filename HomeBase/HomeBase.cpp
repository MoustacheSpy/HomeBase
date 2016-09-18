// HomeBase.cpp : Defines the entry point for the console application.
//

#pragma once
#include "stdafx.h"
#include <SDL.h>
#include <SDL_image.h>
#include "ik.h"
#define SCREENWIDTH 600
#define SCREENHEIGHT 600
#include <Windows.h>

int initSDL(SDL_Window * window, SDL_Surface* screenSurface,SDL_Renderer *renderer) {
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	//The window we'll be rendering to
	window = NULL;

	//The surface contained by the window
	screenSurface = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREENWIDTH, SCREENHEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return 0;
		}
		else
		{
			renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL)
				printf("renderer could not be created. ERROR :%s \n", SDL_GetError());
			return 1;
		}

	}
	
}

SDL_Point rotate_point(float cx, float cy, float angle, SDL_Point p)
{
	
	p.x = cx + (p.x - cx)*cos(angle) - (p.y - cy)*sin(angle);

	p.y = cy + (p.x - cx)*sin(angle) + (p.y - cy)*cos(angle);
	return p;
}
double get_degrees(double input)
{
	
	return (input * 180.0 / M_PI);
}

int main(int argc, char* args[]){

	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	//The window we'll be rendering to
	window = NULL;

	//The surface contained by the window
	screenSurface = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREENWIDTH, SCREENHEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			
		}
		else
		{
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL)
				printf("renderer could not be created. ERROR :%s \n", SDL_GetError());
			
		}

	}
	
	bool quit = false;
	SDL_Event event;
	if (1) {
		printf("init successful");
		SDL_Surface* roboBone1S = NULL;
		SDL_Texture* roboBone1 = NULL;
		SDL_Surface* roboBone2S = NULL;
		SDL_Texture* roboBone2 = NULL;
		roboBone1S = IMG_Load("robot arm.png");
		if (roboBone1S == NULL)
			printf("couldnt load robo arm. ERROR %s \n", IMG_GetError());
		roboBone2S = IMG_Load("robot arm.png");
		if (roboBone2S == NULL)
			printf("couldnt load robo arm. ERROR %s \n", IMG_GetError());
		roboBone1 = SDL_CreateTextureFromSurface(renderer, roboBone1S);
		if (roboBone1 == NULL)
			printf("Couldnt create Robo bone texture! Error : %s\n", IMG_GetError());
		roboBone2 = SDL_CreateTextureFromSurface(renderer, roboBone2S);
		if (roboBone2 == NULL)
			printf("Couldnt create Robo bone texture! Error : %s\n", IMG_GetError());
		double angle1 = 0;
		double angle2 = 0;
		double lenght1 = 200;
		double lenght2 = 200;
		SDL_Rect dsrect1 = { 50,10,100,50 };
		SDL_Rect dsrect2 = { 0,0,100,50 };
		SDL_Point p = { 0,0 };
		SDL_Point childCor = { 100,0 };
		int mousex = 0, mousey = 0;
		double oldAngle = angle2;
		while (1)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderClear(renderer);
			SDL_GetMouseState(&mousex, &mousey);
			solveIK(&angle1, &angle2, false, dsrect1.w, dsrect2.w, mousex, mousey);
			angle1 = get_degrees(angle1);
			angle2 = get_degrees(angle2);
			if (angle1 != oldAngle) {
				childCor = rotate_point(dsrect1.x, dsrect1.y, (oldAngle - angle1), childCor);
				oldAngle = angle1;
			}
			SDL_RenderCopyEx(renderer, roboBone1, NULL, &dsrect1, angle1, &p, SDL_FLIP_NONE);
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
			SDL_RenderDrawLine(renderer, childCor.x - 10, childCor.y - 10, childCor.x + 10, childCor.y + 10);
			SDL_RenderDrawLine(renderer, childCor.x + 10, childCor.y - 10, childCor.x - 10, childCor.y + 10);
			
			dsrect2.x = childCor.x;
			dsrect2.y = childCor.y;

			//SDL_RenderCopyEx(renderer, roboBone2, NULL, &dsrect2, angle2, &p, SDL_FLIP_NONE);
			
			SDL_RenderPresent(renderer);
			SDL_PollEvent(&event);
			SDL_Delay(16);


			// SDL_QUIT event has come in, quit. 
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}
	}

	
    return 0;
}

