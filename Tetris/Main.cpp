#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>       // std::cout
#include <SDL_ttf.h>
#include <stack>
#include "Defines.h"
#include "Enums.h"
#include "cBlock.h"
#include "time.h"
#include "math.h"
#include <vector>

using namespace std;
//the stl stack can't take a fuction pointer as a type 
//se we encapsulate	a function pointer with a struct 
struct StateSturct
{
	void(*StatePointer)();
};
stack<StateSturct> g_stateStack ;

SDL_Surface* g_Bitmap =  NULL;
SDL_Texture* g_Texture = NULL; 
SDL_Window* g_Window = NULL;
SDL_Surface* g_Window_Surface = NULL;
SDL_Event g_Event;
SDL_Renderer* gRenderer;
cBlock* g_FocusBlock = NULL;
cBlock* g_NextBlock = NULL;

vector<cSquare*> g_OldSquares; //The squares that are no longer form the focus block 
int g_Score = 0;
int g_Level = 1; 
int g_FocusBlockSpeed = INITIAL_SPEED;
int g_Timer;


//Game State Methods
void Menu();
void Game();
void Exit();

void HandleWinLoseInput();
//win conditions 
void CheckWin();
void CheckLoss();

//CHANGE SQUARE HANDLING 
void HandleBottomCollision();
void ChangeFocusBlock();
int CheckCompletedLines();

//Collisions Functions
bool CheckEntityCollisions(cSquare* square, Direction dir);
bool CheckWallCollisions(cSquare* square, Direction dir);
bool CheckEntityCollisions(cBlock* block, Direction dir);
bool CheckWallCollisions(cBlock* block, Direction dir);
bool CheckRotationCollisions(cBlock* block);



//Init and Shutdown functions
void Init();
void ShutDown();



//Helper functions LIVE   | FINISHED 
#pragma region Helper Functions Handling 


void ClearScreen()
{

	//Earlier version for clean the screen if a blitsurface has been displayed 

	//function that clear all the screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
}
void DrawBackground()
{
	SDL_Surface* windowSurface = SDL_GetWindowSurface(g_Window);
	SDL_Rect source;
	switch (g_Level)
	{
	case 1:
		source = { LEVEL_ONE_X , LEVEL_ONE_Y , WINDOW_WIDTH , WINDOW_HEIGHT };
		break;
	case 2:
		source = { LEVEL_TWO_X , LEVEL_TWO_Y , WINDOW_WIDTH , WINDOW_HEIGHT };
		break;
	case 3:
		source = { LEVEL_THREE_X , LEVEL_THREE_Y , WINDOW_WIDTH , WINDOW_HEIGHT };
		break;
	case 4:
		source = { LEVEL_FOUR_X , LEVEL_FOUR_Y , WINDOW_WIDTH , WINDOW_HEIGHT };
		break;
	case 5:
		source = { LEVEL_FIVE_X , LEVEL_FIVE_Y , WINDOW_WIDTH , WINDOW_HEIGHT };
		break;
	default:
		break;
	}
		SDL_Rect destination = {0,0,WINDOW_WIDTH , WINDOW_HEIGHT};
		
		SDL_RenderCopy(gRenderer, g_Texture, &source, &destination);

}
void HandleMenuInput() 
{
	if (SDL_PollEvent(&g_Event))
	{
		if (g_Event.type == SDL_QUIT)
		{
			while (!g_stateStack.empty())
			{
				g_stateStack.pop();
			}
			return;
		}
		if (g_Event.type == SDL_KEYDOWN)
		{
			//Quit Game

			if (g_Event.key.keysym.sym == SDLK_ESCAPE)
			{
				g_stateStack.pop();
				return; //state is done
			}
			//Quit Game

			if (g_Event.key.keysym.sym == SDLK_q)
			{
				g_stateStack.pop();
				return; //state is done
			}
			//start game
			if (g_Event.key.keysym.sym == SDLK_g)
			{
				StateSturct temp; 
				temp.StatePointer = Game;
				g_stateStack.push(temp);
				return;//state done
			}
		}
	}
}
void HandleGameInput() 
{
	//These Variables allow the user to hold the arrow keys 
	static bool down_pressed = false;
	static bool left_pressed = false;
	static bool right_pressed = false;

	if (SDL_PollEvent(&g_Event) )
	{
		if (g_Event.type == SDL_QUIT)
		{
			while (!g_stateStack.empty())
			{
				g_stateStack.pop();
			}
			return; //game is over exit the functoins 
		}
		
		if (g_Event.type == SDL_KEYDOWN)
		{
			
			if (g_Event.key.keysym.sym == SDLK_ESCAPE)
			{
				g_stateStack.pop();
				return;
			}
			if (g_Event.key.keysym.sym == SDLK_UP)
			{
				if (!CheckRotationCollisions(g_FocusBlock))
				{
					g_FocusBlock->Rotate();
				}
			}
			if (g_Event.key.keysym.sym == SDLK_LEFT) 
			{
				left_pressed = true;
			}
			if (g_Event.key.keysym.sym == SDLK_RIGHT)
			{
				right_pressed = true; 
			}
			if (g_Event.key.keysym.sym == SDLK_DOWN)
			{
				down_pressed = true; 
			}
		}
		if (g_Event.type == SDL_KEYUP)
		{
			if (g_Event.key.keysym.sym == SDLK_LEFT)
			{
				left_pressed = false;
			}
			if (g_Event.key.keysym.sym == SDLK_RIGHT)
			{
				right_pressed = false;
			}
			if (g_Event.key.keysym.sym == SDLK_DOWN)
			{
				down_pressed = false;
			}
		}
	}
	//Now we handle the arrow keys, making sure to handle collisions 
	//MOVING DOWN 
	if (down_pressed)
	{
		if (!CheckWallCollisions(g_FocusBlock,DOWN) && !CheckEntityCollisions(g_FocusBlock, DOWN))
		{
			g_FocusBlock->Move(DOWN);

		}
	}
	//MOVING LEFT 
	if (left_pressed)
	{
		if (!CheckWallCollisions(g_FocusBlock, LEFT) && !CheckEntityCollisions(g_FocusBlock, LEFT))
		{
			g_FocusBlock->Move(LEFT);

		}
	}
	//MOVING RIGHT	 
	if (right_pressed)
	{
		if (!CheckWallCollisions(g_FocusBlock, RIGHT) && !CheckEntityCollisions(g_FocusBlock, RIGHT))
		{
			g_FocusBlock->Move(RIGHT);

		}
	}
}
void HandleExitInput() 
{
	if (SDL_PollEvent(&g_Event))
	{
		if (g_Event.type == SDL_QUIT)
		{
			while (!g_stateStack.empty())
			{
				g_stateStack.pop();
			}
			return; //the game is over 
		}
		//Handle keyboard input 
		if (g_Event.type == SDL_KEYDOWN)
		{
			if (g_Event.key.keysym.sym == SDLK_ESCAPE)
			{
				g_stateStack.pop();
				return;  //this state is done, exit the function
			}
			 if (g_Event.key.keysym.sym == SDLK_y) 
			{
				g_stateStack.pop();
				return; //this state is done , exit the fucntion
			}
			 if (g_Event.key.keysym.sym == SDLK_n)
			 {
				 StateSturct temp;
				 temp.StatePointer = Menu;
				 g_stateStack.push(temp);
				 return; //this state is done, exit the fucntion
			 }
		}
	}
}
//FINISHED
void DisplayText(string text, int x, int y , int w, int h, int size, int fR, int fG, int fB, int bR, int bG, int bB)
{
	TTF_Font* font = TTF_OpenFont("C://Users/Admin/Documents/Tetris_Clone/Tetris/arial.ttf", size);

	if (font == NULL)
	{
		printf("Could not display the Text ");
	}
	else 
	{
		//we try to render the text surface 
		//SDL_Color foreground = { fR , fG ,fB };
		//Override Foregrounf to make it Black 
		SDL_Color foreground = {0,0,0};
		SDL_Color background = { bR,bG , bB };
		SDL_Rect destination = { x, y,w, h };
		SDL_Surface* temp = TTF_RenderText_Solid(font, text.c_str(), foreground);
	//	SDL_Surface* temp = TTF_RenderText_Shaded(font, text.c_str(), foreground, background);
		if (temp == NULL)
		{
			printf("Failed to render text shaded ");
		}
		else 
		{
			SDL_Texture* texttexture = SDL_CreateTextureFromSurface(gRenderer, temp);

			if (texttexture == NULL)
			{
				printf("Could not create the text texture");

			}
			SDL_RenderCopy(gRenderer, texttexture, NULL, &destination);
			SDL_RenderPresent(gRenderer);
			TTF_CloseFont(font);
			SDL_FreeSurface(temp);
			SDL_DestroyTexture(texttexture);
		}
	


	
	}
	





}

#pragma endregion

//Collider Functions Live  | FINISHED 
#pragma region Collider Functions





bool CheckEntityCollisions(cSquare* square, Direction dir)
{
	//Check collisions between a given square and the square in g_oldsquares 
	//Distance between two squares if they have collided
	int distance = SQUARE_MEDIAN * 2;
	//center of given square 
	int centerX = square->GetCenterX(), centerY = square->GetCenterY();

	switch (dir)
	{
	case LEFT:
		centerX -= distance; 
		break;
	case RIGHT:
		centerX += distance;
		break;
	case DOWN:
		centerY += distance;
		break;
	default:
		break;
	}
	//iterate trough the old square vector, checking for collisions
	for (int i = 0; i < g_OldSquares.size(); i++)
	{
		if ( (abs(centerX - g_OldSquares[i]->GetCenterX()) < distance )&& (abs(centerY - g_OldSquares[i]->GetCenterY()) < distance))
		{
			return true;
		}
	
	}


	return false;
}
bool CheckWallCollisions(cSquare* square, Direction dir)
{
	//check Collisions between a given square  and the sides of the game area 
	int x = square->GetCenterX() , y = square->GetCenterY();

	//get the location of the square after moving and see if its out of bonds 
	switch (dir)
	{
	case LEFT:
		if ((x - (SQUARE_MEDIAN * 2)) < GAME_AREA_LEFT)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case RIGHT:
		if ((x + (SQUARE_MEDIAN * 2)) > GAME_AREA_RIGHT)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case DOWN:
		if ((y + (SQUARE_MEDIAN *2)) > GAME_AREA_BOTTOM)
		{
			return true;
		}
		else 
		{
			return false; 
		}
		break;
	default:
		break;
	}

	return false;
}
bool CheckEntityCollisions(cBlock* block, Direction dir) 
{
	cSquare** temp_array = block->Getsquares();

	//check collision for each one of the squares of the focus piece 
	for (int i = 0; i < 4; i++)
	{
		if (CheckEntityCollisions(temp_array[i] , dir))
		{
			return true;
		}
	}

	return false; 
}
bool CheckWallCollisions(cBlock* block, Direction dir) 
{
	//cSquare** temp_array  = block->
	cSquare** temp_array = block->Getsquares();
	for (int i = 0; i < 4; i++)
	{
		if (CheckWallCollisions(temp_array[i],dir))
		{
			return true;
		}
	}
	return false; 
}
bool CheckRotationCollisions(cBlock* block) 
{
	//Get an array of values for the location of the rotated block's squares 
	int* temp_array = block->GetRotatedSquares();

	//distance the two touching sqaures 
	int distance = SQUARE_MEDIAN * 2;
	for (int i = 0; i < 4; i++)
	{
		//check if the block will go out of bounds 
		if ((temp_array[i*2] < GAME_AREA_LEFT) || (temp_array[i*2] >  GAME_AREA_RIGHT))
		{
			delete temp_array;
			return true;
		}
		if (temp_array[i*2 + 1 ] > GAME_AREA_BOTTOM)
		{
			delete temp_array; 
			return true;
		}
		//we also check if the block does not collide wiht any other squares 
		for (int j = 0; j < g_OldSquares.size(); j++)
		{
			if (abs(temp_array[i*2] - g_OldSquares[j]->GetCenterX() < distance ) && abs(temp_array[i*2 + 1 ] - g_OldSquares[j]->GetCenterY() < distance))
			{
				delete temp_array;
				return true;
			}
		}
	}
	delete temp_array;
	return false; 
}
#pragma endregion


//Functions Init and Shutdown
#pragma region Init-ShutDown
void Init() 
{
	//Initialize SDL_Timer and Video
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) <  0 )
	{
		printf("Failed to load SDL");
	}
	else 
	{
		//create window 
		g_Window = SDL_CreateWindow(WINDOW_CAPTION, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
		g_Timer = SDL_GetTicks();
		if (g_Window == NULL)
		{
			printf("Window could not be loaded ");
		}
		else 
		{
			gRenderer = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created ");
			}
			else 
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
				}
				else
				{
					//initialize TTF
					if (TTF_Init() == -1)
					{
						printf("SDL_TTF could no be Initialized");
					}
				}
			}
		}
	}


	

	//path C:\Users\Admin\Documents\Tetris_Clone\Tetris\Data

	g_Bitmap = IMG_Load("C://Users/Admin/Documents/Tetris_Clone/Tetris/Data/FallingBlocks.bmp");
	if (g_Bitmap == NULL)
	{
		printf("Failed to load FallingBlocks Image ");
	}
	g_Texture = SDL_CreateTextureFromSurface(gRenderer, g_Bitmap);
	if (g_Texture == NULL)
	{
		printf("Failed to create the Texture");
	}
	//seed our  random generator
	srand(time(0));

	StateSturct state;
	state.StatePointer = Exit;
	g_stateStack.push(state);
	// Then we add a pointer to our menu state, this will 
	// be the first thing the player sees of our game. 
	state.StatePointer = Game; // for debug pruporses Menu;
	g_stateStack.push(state);


	//INITIALIZES FIRST BLOCKS 
	g_FocusBlock = new cBlock(BLOCK_START_X, BLOCK_START_Y, g_Bitmap, (BlockType)(rand() % 7));
	g_NextBlock = new cBlock(NEXT_BLOCK_CIRCLE_X, NEXT_BLOCK_CIRCLE_Y, g_Bitmap, (BlockType)(rand() % 7));

}
void ShutDown() 
{
	//reference the sqaures of the focus and nect block so we dont lose their reference
	cSquare** temp_array1 = g_FocusBlock->Getsquares();
	cSquare** temp_array2 = g_NextBlock->Getsquares();

	//delete the blocks
	delete g_FocusBlock;
	delete g_NextBlock;

	//delete the temporary arrays of squares 
	for (int i = 0; i < 4; i++)
	{
		delete temp_array1[i];
		delete temp_array2[i];

	}
	//delete each one of the squares that are in our game saved in our vector 

	for (int i = 0; i < g_OldSquares.size(); i++)
	{
		delete g_OldSquares[i];
	}

	TTF_Quit();
	SDL_DestroyTexture(g_Texture);
	SDL_FreeSurface(g_Bitmap);
	SDL_DestroyWindow(g_Window);

	SDL_Quit();
}

#pragma endregion

//Game State Functions

//Function that handles the Main Menu basically the player can decide if they want to play or quit 
void Menu() 
{
	// Here we compare the difference between the current time and the last time we 
	// handled a frame. If FRAME_RATE amount of time has, it's time for a new frame.
	if ((SDL_GetTicks() - g_Timer) >= FRAME_RATE)
	{
		HandleMenuInput();

		//Clear Screen
		//ClearScreen();
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		DisplayText("Start Game press G", 120, 120,400, 30, 12, 255, 255, 255, 0, 0, 0);
		DisplayText("Quit Game Press Q", 120, 150,400,30 ,12, 255, 255, 255, 0, 0, 0);

		//tell SDL to update Buffer 
		
		SDL_RenderPresent(gRenderer);

			g_Timer = SDL_GetTicks();

	}
}
void Game() 
{
	static int force_down_counter = 0;
	static int slide_counter = SLIDE_TIME;
	//we compare the difference between time and th elast time a frame was handled 
	if ((SDL_GetTicks() - g_Timer) >= FRAME_RATE)
	{
		HandleGameInput();

		force_down_counter++;
		if (force_down_counter >= g_FocusBlockSpeed)
		{
			//always check for collisions before movign anything 
			if (!CheckWallCollisions(g_FocusBlock,DOWN) && !CheckEntityCollisions(g_FocusBlock , DOWN))
			{
				g_FocusBlock->Move(DOWN);
				force_down_counter = 0;
			}
		}
		//check to see if focus block's bottom has hit something, if it has, we decrement our counter 
		if (CheckWallCollisions(g_FocusBlock,DOWN) || CheckEntityCollisions(g_FocusBlock,DOWN))
		{
			slide_counter--;
		}
		//if there isnt a collision, we reset our counter. this is in case the player moves out of colission
		else 
		{
			slide_counter = SLIDE_TIME;
		}
		//if the counter reaches zero, we reset it and call our function that handles  changing the focus block 
		if (slide_counter == 0 )
		{
			slide_counter = SLIDE_TIME; 
			HandleBottomCollision();
		}


		//Clear Screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		DrawBackground();
		g_FocusBlock->Draw(g_Texture, gRenderer);
		g_NextBlock->Draw(g_Texture, gRenderer);

		//Draw the old squares
		for (int i = 0; i < g_OldSquares.size(); i++)
		{
			g_OldSquares[i]->Draw(g_Texture, gRenderer);
		}
		//rendr texts  itoa game the error Severity	Code	Description	Project	File	Line	Suppression State
        //Error	C4996	'itoa': The POSIX name for this item is deprecated.Instead, use the ISO C and C++ conformant name 

		//DisplayText 
		DisplayText(("Score: " + to_string(g_Score)), SCORE_RECT_X, SCORE_RECT_Y, 80, 20, 12, 0, 0, 0, 255, 255, 255);
		DisplayText(("Needed Score: " + to_string(g_Level * POINTS_PER_LEVEL)), NEEDED_SCORE_RECT_X, NEEDED_SCORE_RECT_Y, 100, 18, 10, 0, 0, 0, 255, 255, 255);
		DisplayText(("Level: " + to_string(g_Level)), LEVEL_RECT_X, LEVEL_RECT_Y, 80, 20, 12, 0, 0, 0, 255, 255, 255);






		//SDL_updaterect window render blakcbuffer 

		SDL_RenderPresent(gRenderer);


		g_Timer = SDL_GetTicks();

	}
}
//this function handles the exit screen it will display a message 
void Exit() 
{
	if ((SDL_GetTicks() - g_Timer) >= FRAME_RATE)
	{
		HandleExitInput();
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		DisplayText("Quit Game (Y or N)?", 100, 150,300,30, 12, 255, 255, 255, 0, 0,0);

			//sdl updatte
		g_Timer = SDL_GetTicks();
	}
}

void GameWon() 
{
	if ((SDL_GetTicks() - g_Timer) >= FRAME_RATE)
	{
		HandleWinLoseInput();
		ClearScreen();
		DisplayText("You Win!!!", 100, 120, 100, 30, 12, 255, 255, 255, 0, 0, 0);
		DisplayText("Quit Game (Y or N)?", 100, 140, 100, 30, 12, 255, 255, 255, 0, 0, 0);

		 SDL_RenderPresent(gRenderer);
		 g_Timer = SDL_GetTicks();

	}
}
void GameLost()
{
	if ((SDL_GetTicks() - g_Timer) >= FRAME_RATE)
	{
		HandleWinLoseInput();
		ClearScreen();
		DisplayText("You Lose.", 100, 120,100,30, 12, 255, 255, 255, 0, 0,
			0);
		DisplayText("Quit Game (Y or N)?", 100, 140,100,30, 12, 255, 255,
			255, 0, 0, 0);
		SDL_RenderPresent(gRenderer);
		g_Timer = SDL_GetTicks();
	}
}
void HandleWinLoseInput() 
{
	if (SDL_PollEvent(&g_Event))
	{
		if (g_Event.type == SDL_QUIT)
		{
			while (!g_stateStack.empty())
			{
				g_stateStack.pop();
				return;
			}
		}
		//handle keyboard input 
		if (g_Event.type == SDL_KEYDOWN)
		{
			if (g_Event.key.keysym.sym == SDLK_ESCAPE)
			{
				g_stateStack.pop();
				return;
			}
			if (g_Event.key.keysym.sym == SDLK_y)
			{
				g_stateStack.pop();
				return;
			}
			// if player chooses to continue playing, we pop off current state and push exit and menu states back on
			if (g_Event.key.keysym.sym == SDLK_n)
			{
				g_stateStack.pop();
				StateSturct temp; 
				temp.StatePointer = Exit;
				g_stateStack.push(temp);
				temp.StatePointer = Menu;
				g_stateStack.push(temp);
				return;
			}
		}
	}
}
//win conditions 
void CheckWin() 
{
	//if current level is greater than number of levels, player has won
	if (g_Level > NUM_LEVELS)
	{
		while (!g_stateStack.empty())
		{
			g_stateStack.pop();
		}
		//Push teh victory state onto the stack 
		StateSturct win;
		win.StatePointer = GameWon;
		g_stateStack.push(win);
		
	}
}
void CheckLoss()
{
	//we call this function when the focus block is at the top of that, if the focus block is stuck now, is game over 
	if (CheckEntityCollisions(g_FocusBlock ,DOWN))
	{
		//we clear all the square vector 
		for (int i = 0; i < g_OldSquares.size(); i++)
		{
			delete g_OldSquares[i];
		}
		g_OldSquares.clear();
		//pop all states 
		while (!g_stateStack.empty())
		{
			g_stateStack.pop();
		}
		//Push the losing state onto the stack 
		StateSturct lose;
		lose.StatePointer = GameLost;
		g_stateStack.push(lose);
	}
}


//CHANGE SQUARE HANDLING 
void HandleBottomCollision()
{
	ChangeFocusBlock();
	//check for completed lines and store the number of lines completed 
	int num_lines = CheckCompletedLines();
	if (num_lines >  0 )
	{
		//Increase the player score according to number of lines completed 
		g_Score += POINTS_PER_LINE * num_lines;
		//check if it is time for a new level 
		if (g_Score >= g_Level * POINTS_PER_LEVEL)
		{
			g_Level++;
			CheckWin();
			//shorten teh focus blocks movement interval 
			g_FocusBlockSpeed -= SPEED_CHANGE;
		}
	}
	//now would be a good time to check to see if the player has lost
	CheckLoss();


}
void ChangeFocusBlock() 
{
	//Add the squares of the focus block to g_oldsquares   
	//and set the next block as the focus block 

	//get an array of pointers to the focus block of squares 
	cSquare**  square_array = g_FocusBlock->Getsquares();

	//add focus block to the vector 
	for (int i = 0; i < 4; i++)
	{
		g_OldSquares.push_back(square_array[i]);
	}
	delete g_FocusBlock;
	g_FocusBlock = g_NextBlock;

	g_FocusBlock->SetupSquares(BLOCK_START_X, BLOCK_START_Y, g_Bitmap);
	g_NextBlock = new cBlock(NEXT_BLOCK_CIRCLE_X, NEXT_BLOCK_CIRCLE_Y,  g_Bitmap, (BlockType)(rand() % 7));

}
int CheckCompletedLines()
{
	//Return the amount of lines cleared of zero if no lines were cleared 
	//store the amount of squares in each row in an array 
	int squares_per_row[13];

	//the compiller will fill the array with junk values if we dont do this 
	for (int i = 0; i  < 13; i++)
		squares_per_row[i] = 0;
	
	int row_size = SQUARE_MEDIAN * 2; //Pixel size of one row 
	int bottom = GAME_AREA_BOTTOM - SQUARE_MEDIAN; // Center of bottom row 
	int top = GAME_AREA_BOTTOM - (12 * row_size);
	int num_lines = 0; // number of lines cleared 
	int row; //multipurpose variable 

	//Check for full lines 
	for (int i = 0; i < g_OldSquares.size(); i++)
	{
		//get the row the current square is in 
		row = (g_OldSquares[i]->GetCenterY() - top) / row_size; 
		squares_per_row[row]++;
	}

	//Erase any full lines 
	for (int line= 0; line < 13; line++)
	{
		//check for completed lines
		if (squares_per_row[line] == SQUARES_PER_ROW)
		{
			//keep track of how many lines have been completed 
			num_lines++;
			//find any squares in current row and remove them 
			for (int j = 0; j < g_OldSquares.size(); j++)
			{
				if (((g_OldSquares[j]->GetCenterY()- top) / row_size) == line)
				{
					//delete the squares 
					delete g_OldSquares[j];
					//removes it from the vector
					g_OldSquares.erase(g_OldSquares.begin() + j);

					//when we delete a square, from the vector, the next square takes it place.
					//we have to be sure to stay at the current index, so we dont skip any squares
					//For example if we delete the first square, the second now becomes the first. we have to stay 
					//at the current first index so we cna check the second 
					//square now the first 
					j--;
				}
			}
		}
	}

	//Move squares above cleared line down 
	for (int i = 0; i < g_OldSquares.size(); i++)
	{
		for (int line = 0; line < 13; line++)
		{
			//determine if this row was filled 
			if (squares_per_row[line] == SQUARES_PER_ROW)
			{
				//if it was, get the location of it whitin the game area
				row = (g_OldSquares[i]->GetCenterY() - top) / row_size;

				//Now move any squares above that row down one 
				if (row < line)
				{
					g_OldSquares[i]->Move(DOWN);
				}
			}
		}
	}


	
	return num_lines; 
}





//Main method
int main(int argc , char**argv) 
{
	Init();
	while (!g_stateStack.empty())
	{
		g_stateStack.top().StatePointer(); 
	}
	ShutDown();
	return 0;
};