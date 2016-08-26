#pragma once

#include "cSquare.h"

class cBlock 
{
private:
	//location of the center of the block 
	int m_CenterX, m_CenterY;
	//Type of Block 
	BlockType  m_Type; 

	//Array of pointers to the squares  that make up the block 
	cSquare* m_Squares[4];
public:
	//The consturctor
	cBlock(int x, int y, SDL_Surface* bitmap, BlockType type) : m_CenterX(x), m_CenterY(y),
		m_Type(type) 
	{
		for (int i = 0; i < 4; i++)
		{
			m_Squares[i] = 0;
		}
		SetupSquares(x, y, bitmap);
	};
	//Setup or block  according to its location and its type 
	void SetupSquares(int x, int y, SDL_Surface* bitmap) 
	{
		m_CenterX = x; 
		m_CenterY = y;
		for (int i = 0; i < 4; i++)
		{
			if (m_Squares[i])
				delete m_Squares[i];
		}
		switch (m_Type)
		{
		case SQUARE_BLOCK:
			//[0][2]
			//[1][3]
			m_Squares[0] = new cSquare(x - SQUARE_MEDIAN, y - SQUARE_MEDIAN, bitmap, m_Type);
			m_Squares[1] = new cSquare(x - SQUARE_MEDIAN, y + SQUARE_MEDIAN, bitmap, m_Type);
			m_Squares[2] = new cSquare(x + SQUARE_MEDIAN, y - SQUARE_MEDIAN, bitmap, m_Type);
			m_Squares[3] = new cSquare(x + SQUARE_MEDIAN, y + SQUARE_MEDIAN, bitmap, m_Type);

			break;
		case T_BLOCK:
			//   [0]
			//[2][1][3]
			m_Squares[0] = new cSquare(x + SQUARE_MEDIAN, y - SQUARE_MEDIAN, bitmap, m_Type);
			m_Squares[1] = new cSquare(x + SQUARE_MEDIAN, y + SQUARE_MEDIAN, bitmap, m_Type);
			m_Squares[2] = new cSquare(x - SQUARE_MEDIAN, y + SQUARE_MEDIAN, bitmap, m_Type);
			m_Squares[3] = new cSquare(x + (SQUARE_MEDIAN * 3) , y + SQUARE_MEDIAN, bitmap, m_Type);
			break;
		case L_BLOCK:
			//[0]
			//[1]
			//[2][3]
			m_Squares[0] = new cSquare(x - SQUARE_MEDIAN, y - SQUARE_MEDIAN, bitmap, m_Type);
			m_Squares[1] = new cSquare(x - SQUARE_MEDIAN, y + SQUARE_MEDIAN, bitmap, m_Type);
			m_Squares[2] = new cSquare(x - SQUARE_MEDIAN, y + (SQUARE_MEDIAN * 3), bitmap, m_Type);
			m_Squares[3] = new cSquare(x + SQUARE_MEDIAN, y + (SQUARE_MEDIAN * 3), bitmap, m_Type);
			break;
			//   [0]
			//   [1]
			//[3][2]
		case BACKWARDS_L_BLOCK:
			m_Squares[0] = new cSquare(x + SQUARE_MEDIAN, y - SQUARE_MEDIAN, bitmap, m_Type);
			m_Squares[1] = new cSquare(x + SQUARE_MEDIAN, y + SQUARE_MEDIAN, bitmap, m_Type);
			m_Squares[2] = new cSquare(x + SQUARE_MEDIAN, y + (SQUARE_MEDIAN * 3), bitmap, m_Type);
			m_Squares[3] = new cSquare(x - SQUARE_MEDIAN, y + (SQUARE_MEDIAN * 3), bitmap, m_Type);
			break;
			//[0]
			//[1]
			//[2]
			//[3]
		case STRAIGHT_BLOCK:
			m_Squares[0] = new cSquare(x + SQUARE_MEDIAN, y - (SQUARE_MEDIAN * 3), bitmap, m_Type);
			m_Squares[1] = new cSquare(x + SQUARE_MEDIAN, y - SQUARE_MEDIAN, bitmap, m_Type);
			m_Squares[2] = new cSquare(x + SQUARE_MEDIAN, y + SQUARE_MEDIAN, bitmap, m_Type);
			m_Squares[3] = new cSquare(x + SQUARE_MEDIAN, y + (SQUARE_MEDIAN * 3), bitmap, m_Type);
			break;
			//    [1][0]
			// [3][2] 
		case S_BLOCK:
			m_Squares[0] = new cSquare(x + (SQUARE_MEDIAN * 3), y - SQUARE_MEDIAN, bitmap, m_Type);
			m_Squares[1] = new cSquare(x + SQUARE_MEDIAN, y - SQUARE_MEDIAN, bitmap, m_Type);
			m_Squares[2] = new cSquare(x + SQUARE_MEDIAN, y + SQUARE_MEDIAN, bitmap, m_Type);
			m_Squares[3] = new cSquare(x - SQUARE_MEDIAN, y + SQUARE_MEDIAN, bitmap, m_Type);
			break;

			// [0][1]
			//    [2][3]
		case BACKWARDS_S_BLOCK:
			m_Squares[0] = new cSquare(x - SQUARE_MEDIAN, y - SQUARE_MEDIAN, bitmap, m_Type);
			m_Squares[1] = new cSquare(x + SQUARE_MEDIAN, y - SQUARE_MEDIAN, bitmap, m_Type);
			m_Squares[2] = new cSquare(x + SQUARE_MEDIAN, y + SQUARE_MEDIAN, bitmap, m_Type);
			m_Squares[3] = new cSquare(x + (SQUARE_MEDIAN * 3), y + SQUARE_MEDIAN, bitmap, m_Type);
			break;
		default:
			break;
		}
	};
	 void Draw(SDL_Texture* texture  , SDL_Renderer* renderer)
	 {
		 for (int i = 0; i < 4; i++)
		 {
			 m_Squares[i]->Draw(texture , renderer);
		 }
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

		 for (int i = 0; i < 4; i++)
		 {
			 m_Squares[i]->Move(dir);
		 }
	 };
	 //rotating wujuu xD
	 void Rotate() 
	 {
		 int x1, x2, y1, y2;
		 for (int i = 0; i < 4; i++)
		 {
			 //get the center of the current sqaure 
			 x1 = m_Squares[i]->GetCenterX();
			 y1 = m_Squares[i]->GetCenterY();

			 //Move the square so it's positioned at the origin
			 x1 -= m_CenterX;
			 y1 -= m_CenterY;

			 //do the actual rotation 
			 x2 = -y1;
			 y2 = x1;

			 x2 += m_CenterX;
			 y2 += m_CenterY;

			 //set the square location to out temporary variables 
			 m_Squares[i]->SetCenterX(x2);
			 m_Squares[i]->SetCenterY(y2);
		 }
	 };
	 int* GetRotatedSquares() 
	 {
		 int* temp_array = new int[8];
		 int x1, y1, y2, x2;
		 for (int i = 0; i < 4; i++)
		 {
			 x1 = m_Squares[i]->GetCenterX();
			 y1 = m_Squares[i]->GetCenterY();
			 x1 -= m_CenterX;
			 y1 -= m_CenterX;
			 x2 = -y1;
			 y2 = x1;
			 x2 += m_CenterX;
			 y2 += m_CenterY;
			 // Instead of setting the squares, we just store the values
			 temp_array[i * 2] = x2;
			 temp_array[i * 2 + 1] = y2;
		 }
		 return temp_array;
	 }
	 cSquare** Getsquares() 
	 {
		 return m_Squares; 
	 }

};