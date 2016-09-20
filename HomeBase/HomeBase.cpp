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
#include <time.h>
#include <ctime>
int initSDL(SDL_Window * window, SDL_Surface* screenSurface, SDL_Renderer *renderer) {
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
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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
float looptime = 0;

typedef struct {
	float xVelocity = 0;
	float yVelocity = 0;
	SDL_Rect body = { 0,0,40,40 };
	float oldXVel = 0;
	float oldYVel = 0;
}physicsbox;
void simpleBoxPhysics(physicsbox *box, SDL_Point ground1, SDL_Point ground2, bool enablePhysics = true, bool enableMover = false) {
	if (enablePhysics) {
		if ((box->body.y + box->body.h) > ground1.y - 20 || (box->body.y + box->body.h) > ground2.y - 20) {
			//box->body.y -= (box->body.y - ground1.y);
			box->yVelocity = 0;
			box->oldYVel = 0;
			box->body.y -= (box->body.y + box->body.h) - ground1.y;
			if (enableMover) {
				if (box->xVelocity > 2)
					box->xVelocity -= 0.1;
				else if (box->xVelocity < 2)
					box->xVelocity += 2.1;
			}
			else if (box->xVelocity > 0.1)
				box->xVelocity -= 0.1;
			else if (box->xVelocity < -0.1)
				box->xVelocity += 0.1;


		}
		else {

			box->yVelocity += box->oldYVel = 9.8*0.0016 + box->oldYVel;
		}
		box->body.x += box->xVelocity;
		box->body.y += box->yVelocity;
	}
	if (box->body.x > SCREENWIDTH)
		box->body.x = 0;
	if (box->body.x < 0)
		box->body.x = SCREENWIDTH;


}
int main(int argc, char* args[]) {

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
	double angle1 = 0.1, angle2 = 0, oldangle1 = 0, oldangle2 = 0;
	SDL_Point line11 = { 10,0 }, line12 = { 110,0 }, line21 = { 110,0 }, line22 = { 210,0 };
	SDL_Event event;
	bool dir = false;
	int mousex = 0, mousey = 0;
	SDL_Point ground11 = { 0,190 }, ground12 = { SCREENWIDTH,190 }, ground21 = { 0,230 }, ground22 = { SCREENWIDTH,230 };
	physicsbox testPhysics;
	bool physics = true;
	bool grabbed = false;
	while (SDL_PollEvent(NULL)) {
		// Get the start time here
		clock_t start = clock();

		// Do stuff

		// While clock didn't elapse, do nothing, because the lowest sleeping time is 10 ms
		// You want higher resolution than that.
		// Sleep time is 16 ms because 1 / 60fps = ~16.66ms


		SDL_GetMouseState(&mousex, &mousey);
		if(GetAsyncKeyState('D'))
			
		if (GetAsyncKeyState('W'))
			if (dir == false)
				dir = true;
			else dir = false;
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderClear(renderer);
			if (angle1 < oldangle1 - 0.01 || angle1 > oldangle1 + 0.01) {
				line12 = { 110,0 }, line21 = { 110,0 };
				line12 = rotate_point(0, 0, angle1, line12);
				line21 = rotate_point(0, 0, angle1, line21);
				oldangle1 = angle1;
				if (angle2 == 0) {
					line22.x = line12.x + line21.x;
					line22.y = line12.y + line21.y;

				}
			}
			if (angle2 < oldangle2 - 0.1 || angle2 > oldangle2 + 0.1) {
				line22.x = line12.x + line21.x;
				line22.y = line12.y + line21.y;
				line22 = rotate_point(line21.x, line21.y, (angle2), line22);
				oldangle2 = angle2;

			}
			if (((line22.x > testPhysics.body.x && line22.x < (testPhysics.body.x + testPhysics.body.w) && line22.y > testPhysics.body.y && line22.y < (testPhysics.body.y + testPhysics.body.h)) && GetAsyncKeyState(VK_SPACE)) || grabbed) {
				physics = false;
				testPhysics.body.x = line22.x - 10;
				testPhysics.body.y = line22.y - 10;
				testPhysics.xVelocity = mousex/2 - testPhysics.oldXVel;
				testPhysics.oldXVel = mousex/2;
				testPhysics.yVelocity = mousey/2 - testPhysics.oldYVel;
				testPhysics.oldYVel = mousey/2;
				if (GetAsyncKeyState(VK_SPACE)) {
					grabbed = true;
				}
				else {
					grabbed = false;
					testPhysics.oldYVel = 0;
					physics = true;
				}


			}
			printf("xvel:%f yvel:%f oldvy:%f\n", testPhysics.xVelocity, testPhysics.yVelocity, testPhysics.oldYVel);
			//wprintf("%f ", angle2);
			
			dsrect1.x = line11.x;
			dsrect1.y = line11.y - 25;
			dsrect2.x = line21.x;
			dsrect2.y = line21.y - 25;
			simpleBoxPhysics(&testPhysics, ground21, ground22, physics, true);

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
			SDL_RenderDrawLine(renderer, ground11.x, ground11.y, ground12.x, ground12.y);
			SDL_RenderDrawLine(renderer, ground21.x, ground21.y, ground22.x, ground22.y);
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
			//SDL_RenderCopyEx(renderer, roboArm1T, NULL, &dsrect1, angle1, &line11, SDL_FLIP_NONE);
			//if(angle2==0)
			//SDL_RenderCopyEx(renderer, roboArm1T, NULL, &dsrect2, angle1, &line21, SDL_FLIP_NONE);
			//else //SDL_RenderCopyEx(renderer, roboArm1T, NULL, &dsrect2, angle2, &line21, SDL_FLIP_NONE);
			SDL_RenderDrawLine(renderer, line11.x, line11.y, line12.x, line12.y);
			SDL_RenderDrawLine(renderer, line21.x, line21.y, line22.x, line22.y);
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
			SDL_RenderDrawRect(renderer, &testPhysics.body);
			// SDL_RenderDrawRect(renderer, &dsrect2);
			solveIK(&angle1, &angle2, dir, 100, 100, mousex, mousey);
			angle1 = get_degrees(angle1);
			angle2 = get_degrees(angle2);
			SDL_RenderPresent(renderer);
			SDL_Delay(16);


			while (((clock() - start) * 1000 / CLOCKS_PER_SEC) < 16)
				;

			// Time in milliseconds which took the loop to complete
			float MS = (float)((clock() - start) * 1000 / CLOCKS_PER_SEC);
			// Time in seconds
			looptime = MS / 1000.0f;
			// Frames per seconds
			float FPS = 1000.0f / MS;
	}



	return 0;
}

