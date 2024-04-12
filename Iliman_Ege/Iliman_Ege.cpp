/*********
   CTIS164 - Template Source Program
----------
STUDENT :Ege ILIMAN
SECTION :1
HOMEWORK:1
----------
PROBLEMS:program stops when user creates new object(after all object leaves the screen).It sometimes stop but sometimes doesn't.
----------
ADDITIONAL FEATURES:road lines move, every car has a randomly assigned color.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  32 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

#define SIZE 10 //size of array

/* Global Variables for Template File */
bool f1 = false, up = false, down = false, right = false, left = false;
int  winWidth, winHeight;// current Window width and height
int clicks = 0; // count of clicks
int linex1 = -5, linex2 = 5, line1y1 = 240, line1y2 = 80, line2y1 = line1y1 - 320, line2y2 = line1y2 - 320, line3y1 = line1y1 - 640, line3y2 = line1y2 - 640;//coordinates of the road lines
float r[SIZE], g[SIZE], b[SIZE];//color code of the car
int xB[SIZE], yB[SIZE];//coordinates of the car in openGL form
int lowestY = 0;//to find lowset y value of the created car
bool draw = false;
bool inside[10] = { 1,1,1,1,1,1,1,1,1,1 };

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char* string, void* font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

//
//display car
//
void displayNewCar(int r, int g, int b, int xB, int yB) {

	//
	//main body of the car
	//
	glColor3ub(r, g, b);
	circle(xB, yB, 28);
	glBegin(GL_POLYGON);
	glVertex2f(xB - 28, yB - 90);
	glVertex2f(xB + 28, yB - 90);
	glVertex2f(xB + 28, yB);
	glVertex2f(xB - 28, yB);
	glEnd();


	//
	//roof
	//
	glLineWidth(4);
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	glVertex2f(xB - 8, yB - 18);
	glVertex2f(xB - 8, yB - 90);
	glVertex2f(xB + 8, yB - 18);
	glVertex2f(xB + 8, yB - 90);
	glEnd();


	//
	//lights
	//
	glColor3f(255 / 255., 255 / 255, 0);
	circle(xB + 21, yB + 18, 3);
	circle(xB - 21, yB + 18, 3);

	//
	//hood
	//
	glLineWidth(1);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(xB - 18, yB + 18);
	glVertex2f(xB + 18, yB + 18);

	glVertex2f(xB - 15, yB + 21);
	glVertex2f(xB + 15, yB + 21);

	glVertex2f(xB - 12, yB + 24);
	glVertex2f(xB + 12, yB + 24);


	glEnd();




	//
	//front window
	//
	glBegin(GL_QUADS);
	glColor3f(100 / 255., 100 / 255., 100 / 255.);
	glVertex2f(xB - 10, yB);
	glVertex2f(xB + 10, yB);
	glVertex2f(xB + 17, yB - 18);
	glVertex2f(xB - 17, yB - 18);
	glEnd();


	//
	//rear window
	//
	glBegin(GL_QUADS);
	glColor3f(100 / 255., 100 / 255., 100 / 255.);
	glVertex2f(xB - 17, yB - 50);
	glVertex2f(xB + 17, yB - 50);
	glVertex2f(xB + 10, yB - 68);
	glVertex2f(xB - 10, yB - 68);
	glEnd();

	//
	//left and right window
	//
	glLineWidth(2);
	glBegin(GL_QUADS);
	glColor3f(100 / 255., 100 / 255., 100 / 255.);
	glVertex2f(xB - 17, yB - 20);
	glVertex2f(xB - 17, yB - 50);
	glVertex2f(xB - 25, yB - 52);
	glVertex2f(xB - 25, yB - 18);

	glVertex2f(xB + 17, yB - 20);
	glVertex2f(xB + 17, yB - 50);
	glVertex2f(xB + 25, yB - 52);
	glVertex2f(xB + 25, yB - 18);
	glEnd();


	glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(xB - 17, yB - 35);
	glVertex2f(xB - 25, yB - 34);

	glVertex2f(xB + 17, yB - 37);
	glVertex2f(xB + 25, yB - 36);

	glEnd();
	//
	//exhaust
	//
	glColor3f(100 / 255., 100 / 255., 100 / 255.);
	glRectf(xB - 25, yB - 90, xB - 21, yB - 95);
	glRectf(xB + 25, yB - 90, xB + 21, yB - 95);

	//
	//fire from exhaust
	//
	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 0);
	glVertex2f(xB - 25, yB - 95);
	glVertex2f(xB - 21, yB - 95);
	glColor3f(1, 0, 0);
	glVertex2f(xB - 23, yB - 105);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 0);
	glVertex2f(xB + 25, yB - 95);
	glVertex2f(xB + 21, yB - 95);
	glColor3f(1, 0, 0);
	glVertex2f(xB + 23, yB - 105);
	glEnd();
}

//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to dark grey
	//
	glClearColor(20 / 255., 20 / 355., 20 / 255., 0);
	glClear(GL_COLOR_BUFFER_BIT);


	//
	//create road borders
	//
	glColor3f(192 / 255., 192 / 255., 192 / 255.);
	glRectf(-449, 400, -445, -400);
	glRectf(449, 400, 445, -400);

	//
	//create green areas with drawing 2 rectangles
	//
	glColor3f(34 / 255., 139 / 255., 34 / 255.);
	glRectf(-450, 400, -700, -400);
	glRectf(450, 400, 700, -400);


	//
	//create trees for decoration(using 2 triangles and 1 rectangle)
	// 
	glColor3f(139 / 255., 69 / 255., 19 / 255.);
	glRectf(-650, 250, -640, 230);

	glRectf(-490, 0, -500, 20);

	glRectf(-550, -250, -540, -230);

	glRectf(650, 250, 640, 230);

	glRectf(490, 0, 500, 20);

	glRectf(550, -250, 540, -230);

	glColor3f(0, 80 / 255., 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(-660, 250);
	glVertex2f(-630, 250);
	glVertex2f(-645, 265);
	glVertex2f(-660, 260);
	glVertex2f(-630, 260);
	glVertex2f(-645, 275);

	glVertex2f(-510, 20);
	glVertex2f(-480, 20);
	glVertex2f(-495, 35);
	glVertex2f(-510, 30);
	glVertex2f(-480, 30);
	glVertex2f(-495, 45);

	glVertex2f(-560, -230);
	glVertex2f(-530, -230);
	glVertex2f(-545, -215);
	glVertex2f(-560, -220);
	glVertex2f(-530, -220);
	glVertex2f(-545, -205);

	glVertex2f(660, 250);
	glVertex2f(630, 250);
	glVertex2f(645, 265);
	glVertex2f(660, 260);
	glVertex2f(630, 260);
	glVertex2f(645, 275);

	glVertex2f(510, 20);
	glVertex2f(480, 20);
	glVertex2f(495, 35);
	glVertex2f(510, 30);
	glVertex2f(480, 30);
	glVertex2f(495, 45);

	glVertex2f(560, -230);
	glVertex2f(530, -230);
	glVertex2f(545, -215);
	glVertex2f(560, -220);
	glVertex2f(530, -220);
	glVertex2f(545, -205);

	glEnd();



	//
	//draw the road lines
	//
	glColor3f(1, 1, 1);
	glRectf(linex1, line1y1, linex2, line1y2);
	glRectf(linex1, line2y1, linex2, line2y2);
	glRectf(linex1, line3y1, linex2, line3y2);

	//
	//display cars
	//
	for (int i = 0; i < clicks; i++) {

		if (inside[i])
			displayNewCar(r[i], g[i], b[i], xB[i], yB[i]);
		if (yB[i] > 400) {
			inside[i] = 0;
			draw = false;

		}
		if (yB[i] < yB[lowestY])
			lowestY = i;
	}
	if (yB[lowestY] > 400) {
		clicks = 0;
		f1 = !f1;
		for (int i = 0; i < 10; i++)
			inside[i] = 1;
	}

	//
	//create an area to print name, surname and id
	//
	glColor3f(0, 0, 1);
	glRectf(-60, 350, 60, 300);
	glColor3f(0, 128 / 255., 128 / 255.);
	glRectf(-55, 345, 55, 305);
	glColor3f(0, 0, 0);
	vprint(-45, 330, GLUT_BITMAP_8_BY_13, "EGE ILIMAN");
	vprint(-45, 310, GLUT_BITMAP_8_BY_13, "22102429");

	//
	//print the info
	//
	glColor3f(1, 0, 1);
	if (clicks <= 10) {
		vprint(-685, -350, GLUT_BITMAP_8_BY_13, "Press mouse left button to create a car. You created %d cars. You can create maximum 10 cars.", clicks);
	}

	vprint(455, -350, GLUT_BITMAP_8_BY_13, "Press F1 key to move the cars.");


	glutSwapBuffers();
}


//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	case GLUT_KEY_F1: f1 = true; break;
	}
	int yB = winHeight / 2 - y;
	if (f1 == true) {
		draw = true;
	}


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.


	//
	//when user presses left click program gets mouse location, creates random color codes and stores them in an array
	//
	int xGL = x - winWidth / 2;
	int yGL = winHeight / 2 - y;
	if (xGL < 450 && xGL > -450) {
		if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && !draw == true) {
			if (clicks < 10) {
				r[clicks] = rand() % 256;
				g[clicks] = rand() % 256;
				b[clicks] = rand() % 256;
				xB[clicks] = xGL;
				yB[clicks] = yGL;
				clicks++;
			}
		}
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.


	//
	//if user presses F1 key cars go upwards with 10 fx/32ms and lines go up 5px/32ms
	// 
	if (f1 == true) {
		//
		//move the road lines
		//
		if (line1y2 < 400) {
			line1y1 += 5;
			line1y2 += 5;
		}
		else if (line1y2 == 400) {
			line1y1 = -400;
			line1y2 = -560;
		}
		if (line2y2 < 400) {
			line2y1 += 5;
			line2y2 += 5;
		}
		else if (line2y2 == 400) {
			line2y1 = -400;
			line2y2 = -560;
		}
		if (line3y2 < 400) {
			line3y1 += 5;
			line3y2 += 5;
		}
		else if (line3y2 == 400) {
			line3y1 = -400;
			line3y2 = -560;
		}

		for (int i = 0; i < clicks; i++) {

		}
		//
		//move the cars
		//
		for (int i = 0; i < clicks; i++) {
			yB[i] += 10;
		}
	}


	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("Ege ILIMAN Homework-1");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}