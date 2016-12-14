#pragma once
#include "enums.h"
#include <SDL.h>
#include "Defines.h"

class Piece
{
private:
	Color _Color;
	//Positions from the center of the piece  
	int PosX, PosY;
public:
	Piece() {}
	Piece(int posx, int posy, Color _color) : PosX(posx), PosY(posy), _Color(_color) {}
	void Move(Direction direction)
	{
		switch (direction)
		{
		case LEFT:
			PosX -= PIECE_WIDTH;
			break;
		case DOWN:
			PosY += PIECE_HEIGHT;
			break;
		case RIGHT:
			PosX += PIECE_WIDTH;

			break;
		default:
			break;
		}
	}
	void Draw(SDL_Renderer* Renderer , SDL_Texture* texture  )
	{
		SDL_Rect temp ;
		switch (_Color)
		{
		case RED:
			temp = { RED_POSITION_X ,RED_POSITION_Y , PIECE_WIDTH ,PIECE_HEIGHT };
			break;
		case GREEN:
			temp = { GREEN_POSITION_X ,GREEN_POSITION_Y  , PIECE_WIDTH , PIECE_HEIGHT };
			break;
		case YELLOW:
			temp = {YELLOW_POSITION_X , YELLOW_POSITION_Y , PIECE_WIDTH , PIECE_HEIGHT};
			break;
		case PURPLE:
			temp = { PURPLE_POSITION_X , PURPLE_POSITION_Y , PIECE_WIDTH , PIECE_HEIGHT };
			break;
		default:
			break;
		}
		SDL_Rect destination = { PosX - (PIECE_WIDTH / 2), PosY - (PIECE_HEIGHT / 2) , PIECE_WIDTH , PIECE_HEIGHT };
		SDL_RenderCopy(Renderer, texture, &temp	, &destination);
	}	
	//Get and Set positions
	int getPosX() { return PosX; }
	int getPosY() { return PosY; }
	void setPosX(int value) { PosX = value; }
	void setPosY(int value) { PosY = value; }


};