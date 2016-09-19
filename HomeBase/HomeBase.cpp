// HomeBase.cpp : Defines the entry point for the console application.
//

#pragma once
#include "stdafx.h"
#include "SDL.h"
#include "SDL_image.h"
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

SDL_Point rotate_point(double cx, double cy, double angle, SDL_Point p)
{
	double pi = acos(-1);
	double rotation_angle = (double)angle / 180.0 * pi;
	double s = sin(rotation_angle);
	double c = cos(rotation_angle);

	// translate point back to origin:
	p.x -= cx;
	p.y -= cy;

	// rotate point
	double xnew = p.x * c - p.y * s;
	double ynew = p.x * s + p.y * c;

	// translate point back:
	p.x = xnew + cx;
	p.y = ynew + cy;
	
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
	SDL_Surface *roboArm1 = NULL;
	SDL_Texture *roboArm1T = NULL;
	roboArm1 = IMG_Load("robot arm.png");
	roboArm1T = SDL_CreateTextureFromSurface(renderer, roboArm1);
	SDL_Rect dsrect1 = { 0,25,100,50 };
	SDL_Rect dsrect2 = { 100,25,100,50 };
	double angle1=0.1, angle2=0,oldangle1=0,oldangle2=0;
	SDL_Point line11 = { 10,0 }, line12 = { 110,0 }, line21 = { 110,0 }, line22 = { 210,0 };
	SDL_Event event;
	int mousex = 0, mousey = 0;
	while (SDL_PollEvent(NULL)) {
		 
		SDL_GetMouseState(&mousex, &mousey);
		if (GetAsyncKeyState('W'))
			angle1 += 0.1;
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		if (angle1 < oldangle1-0.01||angle1 > oldangle1 + 0.01) {
			line12 = { 110,0 }, line21 = { 110,0 };
			line12 = rotate_point(0, 0, angle1, line12);
			line21 = rotate_point(0, 0, angle1 , line21);
			oldangle1 = angle1;
			if (angle2 == 0) {
				line22.x = line12.x + line21.x;
				line22.y = line12.y + line21.y;
				
			}
		}
		if (angle2 < oldangle2 - 0.1 || angle2 > oldangle2 + 0.1) {
			line22.x = line12.x + line21.x;
			line22.y = line12.y + line21.y;
			line22 = rotate_point(line21.x,line21.y, (angle2), line22);
			oldangle2 = angle2;
			 
		}
		
		printf("%f ", angle2);
		dsrect1.x = line11.x;
		dsrect1.y = line11.y-25;
		dsrect2.x = line21.x;
		dsrect2.y = line21.y-25;
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
		SDL_RenderCopyEx(renderer, roboArm1T, NULL, &dsrect1, angle1, &line11, SDL_FLIP_NONE);
		if(angle2==0)
			SDL_RenderCopyEx(renderer, roboArm1T, NULL, &dsrect2, angle1, &line21, SDL_FLIP_NONE);
		else SDL_RenderCopyEx(renderer, roboArm1T, NULL, &dsrect2, angle2, &line21, SDL_FLIP_NONE);
		SDL_RenderDrawLine(renderer, line11.x, line11.y, line12.x, line12.y);
		SDL_RenderDrawLine(renderer, line21.x, line21.y, line22.x, line22.y);
		SDL_RenderDrawRect(renderer, &dsrect2);
		solveIK(&angle1, &angle2, false, 100, 100, mousex, mousey);
		angle1 = get_degrees(angle1);
		angle2 = get_degrees(angle2);
		SDL_RenderPresent(renderer);
		SDL_Delay(16);

	}
		

	
    return 0;
}

