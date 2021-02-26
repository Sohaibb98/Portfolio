#include "Draw.h"




const int npmvertices = 1220;
GLfloat pmvertices[npmvertices][2];
void InitPMVertices(float radius) {

	float hdegree = (M_PI - M_PI / 2.0) / 360.0;
	float angle = M_PI + M_PI / 6.0;
	for (int i = 0; i < npmvertices; ++i) {
		pmvertices[i][0] = radius * cos(angle);
		pmvertices[i][1] = radius * sin(angle);
		angle += hdegree;
	}
}

void DrawBomberman(float sx/*center x*/, float sy/*center y*/,
	float radius/*Radius*/,
	const ColorNames &colorname/*Bomberman Colour*/, Direction dir) {

	glColor3fv(colors[colorname]); // set the circle color
	InitPMVertices(radius);
	glBegin(GL_TRIANGLE_FAN);
	glVertex4f(sx, sy, 0, 1);
	if (dir == UP || dir == STATIONARY)
	{
		for (int i = 0; i < npmvertices; ++i)
			glVertex4f(sx - pmvertices[i][1], sy - pmvertices[i][0], 0, 1);
	}
	else if (dir == DOWN)
	{
		for (int i = 0; i < npmvertices; ++i)
			glVertex4f(sx + pmvertices[i][1], sy + pmvertices[i][0], 0, 1);
	}
	else if (dir == LEFT)
	{
		for (int i = 0; i < npmvertices; ++i)
			glVertex4f(sx + pmvertices[i][0], sy + pmvertices[i][1], 0, 1);
	}
	else if (dir == RIGHT)
	{
		for (int i = 0; i < npmvertices; ++i)
			glVertex4f(sx - pmvertices[i][0], sy - pmvertices[i][1], 0, 1);
	}
	glEnd();
	DrawCircle(sx - radius + radius / 2, sy + (radius - radius / 2),
		radius / 10, colors[BLACK]);
}

void DrawEnemy(int x/*starting x*/, int y/*starting y*/,
	ColorNames color/*color*/, float gw/*Enemy Width in Units*/,
	float gh /*Enemy Height in Units*/) {
	int ogw = 6, ogh = 7;

	glPushMatrix();
	float sx = (float)gw / ogw, sy = (float)gh / ogh;
	glTranslatef(x, y, 1);
	glScalef(sx, sy, 1);

	// Draw Enemy
	DrawRoundRect(0, 1, 6, 3, colors[color]);
	DrawCircle(3, 4, 3.01, colors[color]);
	glPushMatrix();
	glScalef(0.9, 1.1, 1);
	//  legs
	DrawCircle(0.75, 1, 0.75, colors[color]);
	DrawCircle(3.25, 1, 0.75, colors[color]);
	DrawCircle(5.85, 1, 0.75, colors[color]);

	glPopMatrix();
	//	glPopMatrix();

	// eyes

	glPushMatrix();
	glScalef(0.9, 1.1, 1);
	DrawCircle(1.85, 3.95, 0.75, colors[WHITE]);
	DrawCircle(4.95, 3.95, 0.75, colors[WHITE]);
	glPopMatrix();

	// eyes
	DrawCircle(1.65, 4.25, 0.45, colors[BLUE]);
	DrawCircle(4.45, 4.25, 0.45, colors[BLUE]);
	glPopMatrix();
}

int Distance(pair<int, int>p1, pair<int, int>p2)
{
	int x = pow(p1.second - p2.second, 2);
	int y = pow(p1.first - p2.first, 2);
	return (int)sqrt(x + y);
}