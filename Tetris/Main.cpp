#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>       // std::cout
#include <SDL_ttf.h>
#include <stack>


//the stl stack can't take a fuction pointer as a type 
//se we encapsulate	a function pointer with a struct 
struct StateSturct
{
	void(*StatePointer);
};

std::stack<StateSturct> g_stateStack;

SDL_Surface* g_Bitamp;
SDL_Surface* g_Window;
SDL_Event g_Event;
int g_Timer;