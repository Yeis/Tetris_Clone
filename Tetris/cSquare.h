#pragma once
#include <SDL.h>
#include "Defines.h"
#include "Enums.h"

class cSquare
{
private:
	int m_CenterX, m_CenterY;
	BlockType m_BlockType;
	//pointer to bitmap surface 
	SDL_Surface* m_Bitmap;
public:
	//Default constructor 
	cSquare() {}
	//Main Constructor 
	cSquare(int x, int y, SDL_Surface* bitmap, BlockType blocktype) :
		m_CenterX(x), m_CenterY(y), m_Bitmap(bitmap), m_BlockType(blocktype) 
	{}

	//Draw() takes a pointer to the surface to draw to (our window)
	void Draw(SDL_Texture* texture ,  SDL_Renderer* renderer) 
	{
		SDL_Rect source;
		//Determine the location of the square within our bitmap
		switch (m_BlockType)
		{
		case SQUARE_BLOCK:
			source = { RED_SQUARE_X ,RED_SQUARE_Y, SQUARE_MEDIAN * 2 , SQUARE_MEDIAN * 2 };
			break;
		case T_BLOCK:
			source = { PURPLE_SQUARE_X , PURPLE_SQUARE_Y , SQUARE_MEDIAN * 2 , SQUARE_MEDIAN * 2 };
			break;
		case L_BLOCK:
			source = {GREY_SQUARE_X , GREY_SQUARE_Y, SQUARE_MEDIAN * 2 , SQUARE_MEDIAN * 2 };
			break;
		case BACKWARDS_L_BLOCK:
			source = {BLUE_SQUARE_X , BLUE_SQUARE_Y, SQUARE_MEDIAN * 2 , SQUARE_MEDIAN * 2 };
			break;
		case STRAIGHT_BLOCK:
			source = { GREEN_SQUARE_X , GREEN_SQUARE_Y , SQUARE_MEDIAN * 2 , SQUARE_MEDIAN * 2 };
			break;
		case S_BLOCK:
			source = { BLACK_SQUARE_X , BLACK_SQUARE_Y , SQUARE_MEDIAN * 2 , SQUARE_MEDIAN * 2 };
			break;
		case BACKWARDS_S_BLOCK:
			source = {YELLOW_SQUARE_X , YELLOW_SQUARE_Y , SQUARE_MEDIAN * 2 , SQUARE_MEDIAN * 2 };
			break;
		default:
			break;
		}
		// Draw at square's current location. Remember that m_X and m_Y store the center of the square. 
		SDL_Rect Destination = { m_CenterX - SQUARE_MEDIAN , m_CenterY - SQUARE_MEDIAN , SQUARE_MEDIAN * 2 , SQUARE_MEDIAN * 2  };
		SDL_RenderCopy(renderer , texture , &source , &Destination);
	};

	void Move(Direction dir) 
	{
		switch (dir)
		{
		case LEFT:
			m_CenterX -= SQUARE_MEDIAN * 2;
			break;
		case RIGHT:
			m_CenterX += SQUARE_MEDIAN * 2;
			break;
		case DOWN:
			m_CenterY += SQUARE_MEDIAN * 2;
			break;
		default:
			break;
		}
	};

	//Accessors 
	int GetCenterX() { return m_CenterX; }
	int GetCenterY() { return m_CenterY;  }

	//Mutators 
	void SetCenterX(int x) { m_CenterX = x; }
	void SetCenterY(int y) { m_CenterY = y; }


};
	