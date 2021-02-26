#include "util.h"
#ifndef DRAW_H_
#define DRAAW_H_
//
enum Direction { UP, RIGHT, LEFT, DOWN, STATIONARY };

void DrawBomberman(float sx/*center x*/, float sy/*center y*/,
	float radius/*Radius*/,
	const ColorNames &colorname/*Bomberman Colour*/, Direction dir = RIGHT);

void DrawEnemy(int x/*starting x*/, int y/*starting y*/,
	ColorNames color/*color*/, float gw = 6/*Enemy Width in Units*/,
	float gh = 7/*Enemy Height in Units*/);

int Distance(pair<int, int>, pair<int, int>);
#endif // !DRAW_H_

