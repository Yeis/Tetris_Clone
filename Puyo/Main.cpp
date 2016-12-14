#include <SDL.h>
#undef main
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>       
#include <vector>
#include <SDL_ttf.h>
#include "Defines.h"
#include "Block.h"


using namespace std;

//Variables 
SDL_Window* gameWindow ; 
SDL_Renderer* gameRenderer ; 
SDL_Event* gameEventHandler ;
SDL_Texture* spriteSheet = NULL;

vector<Block*> idleBlocks;
Block* mainBlock; 

//value variables used trough the game 
int timer; 
bool quit = false;
//Function that Initialize all Components 
bool Init() 
{
	bool Init = true;
	//Initialize all Modules 
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		Init = false;
	}
	else 
	{
		gameWindow = SDL_CreateWindow("Puyo Puyo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gameWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			Init = false;
		}
		//initialize SDL_Image and renderer

		gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);
		if (gameRenderer == NULL)
		{
			printf("Game Renderer could not initialize! SDL_image Error: \n");
			Init = false;
		}
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			Init = false;
		}
		//load SpriteSheet
		SDL_Surface* image = IMG_Load("C://Users/Admin/Documents/Tetris_Clone/Puyo/SpriteSheet.png");
		spriteSheet = SDL_CreateTextureFromSurface(gameRenderer, image);
		if (spriteSheet == NULL)
		{
			printf("Error loading the spritesheet!");
			Init = false;
		}
		//get time at the beginning of the game
		timer = SDL_GetTicks();
		//Initialize or first Block
		mainBlock = new Block(BLOCK_START_X, BLOCK_START_Y);
	}
	return Init; 
}
void ShutDown() 
{
	SDL_DestroyWindow(gameWindow);
	SDL_DestroyTexture(spriteSheet);
	SDL_DestroyRenderer(gameRenderer);
}
void Game() 
{
	//we check if it is time to render a new frame 
	if (SDL_GetTicks() - timer >= FRAME_RATE)
	{

		mainBlock->Move(DOWN);
		//first we clear all things in the screen before rendering
		SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gameRenderer);

		mainBlock->Draw(gameRenderer, spriteSheet);

		//at last we render all 
		SDL_RenderPresent(gameRenderer);

		//finally we get the current ticks of the game 
		timer = SDL_GetTicks();
	}
}


//Check Collisions with game are
bool CheckGameAreaCollision(Piece* piece , direction dir) 
{
	int posx
}


int main(int argc, char argv[]) 
{
	if (Init())
	{
		while (!quit)
		{
			//STart the game 
			Game();
		}
		
	}
	else 
	{
		printf("Could not Initialize the Game");
	}


	return 0;
};