#pragma once
#include "Defines.h"
#include "Piece.h"


class Block 
{
private:
	int CenterPosX, CenterPosY;
	Piece* m_Pieces[2];

public:
	Block(int posx , int posy  ):CenterPosX(posx), CenterPosY(posy)
	{
		for (int i = 0; i < 2; i++)
		{
			m_Pieces[i] = NULL;
		}
		//We create the pieces from random colors 
		m_Pieces[0] = new  Piece(posx, posy, static_cast<Color>(rand() % 4));		
		m_Pieces[1] = new  Piece(posx + PIECE_WIDTH, posy, static_cast<Color>(rand() % 4));		
	}
	void Draw(SDL_Renderer* Renderer , SDL_Texture* texture)
	{
		for (int i = 0; i < 2; i++)
		{
			m_Pieces[i]->Draw(Renderer, texture);
		}
	}
	void Move(Direction dir) 
	{
		switch (dir)
		{
		case LEFT:
			CenterPosX -= PIECE_WIDTH;
			break;
		case DOWN:
			CenterPosY += PIECE_HEIGHT;
			break;
		case RIGHT:
			CenterPosX -= PIECE_WIDTH;
			break;
		default:
			break;
		}
		for (int i = 0; i < 2; i++)
		{
			m_Pieces[i]->Move(dir);
		}
	}
	void Rotate() 
	{
		//2d rotation matrix
		//(cos0 , -sin0)
		//(sin0 , cos0)
		//as rotation is only done in 90 degrees we just get cos(90) = 0  and sin(90) = 1 (in Radians)

		//helper variables 
		int X1, X2, Y1, Y2;
		//we rotate each one of the block 
		for (int i = 0; i < 2; i++)
		{
			X1 = m_Pieces[i]->getPosX();
			Y1 = m_Pieces[i]->getPosY();
			//change position to its local origin
			X1 -= CenterPosX;
			Y1 -= CenterPosY; 
			//The actual rotation
			X2 = -Y1;
			Y2 = X1; 
			//we return to its position
			X2 += CenterPosX; 
			Y2 += CenterPosY;
			//and we set the new position
			m_Pieces[i]->setPosX(X2);
			m_Pieces[i]->setPosY(Y2);
		}


	}
	Piece** GetPiece() { return m_Pieces; }
};