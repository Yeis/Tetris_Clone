#pragma once
//window related defines
#define WINDOW_WIDTH = 800
#define WINDOW_HEIGHT = 600
#define WINDOW_CAPTION = "Tetris Game"
#define GAME_AREA_LEFT 53
#define GAME_AREA_RIGHT 251
#define GAME_AREA_BOTTOM 298 

//Game related Defines
#define FRAMES_PER_SECOND = 30
#define FRAME_RATE = 1000/FRAMES_PER_SECOND


#define NUM_LEVELS 5 // number of levels in the game
#define POINTS_PER_LINE 525 // points player receives for completing a line
#define POINTS_PER_LEVEL 6300 // points player needs to advance a level
#define INITIAL_SPEED 60 // initial interval at which focus block moves down
#define SPEED_CHANGE 10 // the above interval is reduced by this much each level