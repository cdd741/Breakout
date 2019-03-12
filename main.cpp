// CS 349 Fall 2018
// A1: Breakout code sample
// You may use any or all of this code in your assignment!
// See makefile for compiling instructions

#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <sys/time.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <string>
#include <cmath>
#include "layout.h"
#include "ball.h"
#include "paddle.h"

using namespace std;

// X11 structures
Display *display;
Window window;

// Calculate Real Speed
int getRS(int FPS, int level)
{
	return level * 4000 / FPS;
}

// Calculate Distance
double calDis(pair<double, double> p1, pair<double, double> p2)
{
	return sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
}

// FPS, Level, Real Speed & ballSize
int FPS = 60;
int level = 5;
int RS = getRS(FPS, level);
int ballSize = 15;

vector<const char *> LOGO;
const char *LOGO1 = R"( ______  ______  ______  ______  __  __  ______  __  __  ______ )";
const char *LOGO2 = R"(/\  == \/\  == \/\  ___\/\  __ \/\ \/ / /\  __ \/\ \/\ \/\__  _\)";
const char *LOGO3 = R"(\ \  __<\ \  __<\ \  __\\ \  __ \ \  _"-\ \ \/\ \ \ \_\ \/_/\ \/)";
const char *LOGO4 = R"( \ \_____\ \_\ \_\ \_____\ \_\ \_\ \_\ \_\ \_____\ \_____\ \ \_\)";
const char *LOGO5 = R"(  \/_____/\/_/ /_/\/_____/\/_/\/_/\/_/\/_/\/_____/\/_____/  \/_/)";

void renderLOGO(Display *display, Window &window, GC gc)
{
	XDrawString(display, window, gc, 347, 300, LOGO1, 64);
	XDrawString(display, window, gc, 347, 315, LOGO2, 64);
	XDrawString(display, window, gc, 347, 330, LOGO3, 64);
	XDrawString(display, window, gc, 347, 345, LOGO4, 64);
	XDrawString(display, window, gc, 347, 360, LOGO5, 64);
}
// get current time
unsigned long
now()
{
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}
unsigned long long fps_passed = 0;

// entry point
int main(int argc, char *argv[])
{
	// command line
	if (argc > 1)
		FPS = atoi(argv[1]);
	if (argc > 2)
		level = atoi(argv[2]);
	// create window
	display = XOpenDisplay("");
	if (display == NULL)
		exit(-1);
	int screennum = DefaultScreen(display);
	long background = BlackPixel(display, screennum);
	long foreground = WhitePixel(display, screennum);
	window = XCreateSimpleWindow(display, DefaultRootWindow(display),
								 10, 10, 1280, 800, 2, foreground, background);

	XSizeHints xsh;
	//xsh.flags = (width | height | min_width | min_height | max_width | max_height);
	xsh.min_width = xsh.max_width = xsh.base_width = 1280;
	xsh.min_height = xsh.max_height = xsh.base_height = 800;

	XSetWMNormalHints(display, window, &xsh);

	// set events to monitor and display window
	XSelectInput(display, window, ButtonPressMask | KeyPressMask | KeyReleaseMask);
	XMapRaised(display, window);
	XFlush(display);

	// random seed
	srand(now());

	// create paddle
	Paddle paddle = Paddle();
	// create ball
	Ball ball(paddle.getX(), paddle.getY() - ballSize, ballSize);
	// page flag
	string flag = "page1";

	// create gc for drawing
	unsigned int pix;
	std::stringstream ss;

	GC gc = XCreateGC(display, window, 0, 0);
	GC gcBall = XCreateGC(display, window, 0, 0);
	ss << std::hex << "FFF8DC";
	ss >> pix;
	XSetForeground(display, gcBall, pix);
	XSetForeground(display, gc, pix);

	XWindowAttributes w;
	XGetWindowAttributes(display, window, &w);

	// font
	XSetFont(display, gc, XLoadQueryFont(display, "9x15")->fid);

	// save time of last window paint
	unsigned long lastRepaint = 0;

	// event handle for current event
	XEvent event;
	// bricks layout
	Layout layout(rand() % 3 + 1, display);

	int test = rand() % 140 + 10;

	int live = 3;
	int score = 0;

	ball.setX() = paddle.getX();
	ball.setY() = paddle.getY() - paddle.getHeight() / 2 - ballSize / 2;
	bool start = false;
	bool moveRight = false;
	bool moveLeft = false;
	bool finished = false;
	auto pixmap = XCreatePixmap(display, window, w.width, w.height, w.depth);

	// Starter loop
	while (true)
	{
		// process if we have any events
		if (XPending(display) > 0)
		{
			XNextEvent(display, &event);

			KeySym key;
			char text[10];
			int i = XLookupString((XKeyEvent *)&event, text, 10, &key, 0);

			switch (event.type)
			{

			// mouse button press
			case ButtonPress:
				cout << "CLICK" << endl;
				break;

			case KeyRelease:
				if (key == XK_Right || (i == 1 && (text[0] == 'd' || text[0] == 'D')))
					moveRight = false;

				if (key == XK_Left || (i == 1 && (text[0] == 'a' || text[0] == 'A')))
					moveLeft = false;
				break;

			case KeyPress: // any keypress

				// move right
				if ((key == XK_Right || (i == 1 && (text[0] == 'd' || text[0] == 'D'))) && !moveRight)
					moveRight = true;
				// move left
				if ((key == XK_Left || (i == 1 && (text[0] == 'a' || text[0] == 'A'))) && !moveLeft)
					moveLeft = true;

				// start moving
				if (key == XK_space)
				{
					if (flag == "page1" || flag == "page2")
					{
						flag = "begin";
						layout = Layout(rand() % 3 + 1, display);
						score = 0;
						live = 3;
					}
					else if (flag == "begin")
					{
						ball.setAngle(rand() % 140 + 10);
						flag = "start";
					}
					else if (flag == "start")
					{
						flag = "pause";
					}
					else if (flag == "pause")
					{
						flag = "start";
					}
				}

				// quit game
				if (i == 1 && (text[0] == 'q' || text[0] == 'Q'))
				{
					XCloseDisplay(display);
					exit(0);
				}
				break;
			}
		}

		unsigned long end = now(); // get current time in microsecond

		if (end - lastRepaint > 1000000 / FPS)
		{
			for (int i = 0; i < (100 * level + FPS) / FPS; ++i)
			{
				if (moveRight && (flag == "start" || flag == "begin"))
					if ((paddle.getX() + paddle.getWidth() / 2) < w.width)
						for (int i = 0; i < 1; ++i)
							paddle.setX() += 1;

				if (moveLeft && (flag == "start" || flag == "begin"))
					if ((paddle.getX() - paddle.getWidth() / 2) > 0)
						for (int i = 0; i < 1; ++i)
							paddle.setX() -= 1;

				// update ball position
				if (flag == "begin")
				{
					ball.setX() = paddle.getX();
					ball.setY() = paddle.getY() - paddle.getHeight() / 2 - ballSize / 2;
				}
				else if (flag == "start")
				{
					for (int i = 0; i < 1; ++i)
					{
						ball.setX() += ball.getDX() * 0.5;
						ball.setY() -= ball.getDY() * 0.5;
					}
				}

				// bounce ball
				// Wall
				if (ball.getX() + ballSize / 2 > w.width ||
					ball.getX() - ballSize / 2 < 0)
					ball.vBounce();
				if (ball.getY() - ballSize / 2 < 0)
					ball.hBounce();
				/////testing purpose///////////////////////

				///////////////////////////////////////////
				// Paddle//
				// longSide
				if ((ball.getY() + ballSize / 2 > paddle.getY() - paddle.getHeight() / 2) &&
					((paddle.getX() - paddle.getWidth() / 2 <= ball.getX()) &&
					 (ball.getX() <= paddle.getX() + paddle.getWidth() / 2)))
				{
					ball.paddleBounce(((paddle.getX() + paddle.getWidth() / 2) - ball.getX()) / paddle.getWidth());
					if (finished)
						layout = Layout(rand() % 3 + 1, display);
				}

				//side
				/*
					else if (((paddle.getY() - paddle.getHeight() / 2 < ball.nextPos().second) &&
							  (ball.nextPos().second < paddle.getY() + paddle.getHeight() / 2)) &&
							 (((ball.nextPos().first > paddle.getX() - paddle.getWidth()) &&
							   (ball.pos().first <= paddle.getX() - paddle.getWidth())) ||
							  ((ball.nextPos().first < paddle.getX() + paddle.getWidth()) &&
							   (ball.pos().first >= paddle.getX() + paddle.getWidth()))))
						ball.vBounce();
						*/
				// point

				else if ((calDis(ball.nextPos(), paddle.ulPos()) <= ballSize / 2) ||
						 (calDis(ball.nextPos(), paddle.urPos()) <= ballSize / 2))
					ball.paddleBounce(((paddle.getX() + paddle.getWidth() / 2) - ball.getX()) / paddle.getWidth());

				// bricks
				string s;
				int skr = level;
				pair<double, double> point = layout.checkHit(ball.pos(), ballSize, ball.dir(), s, skr, display, gcBall);
				size_t foundUH = s.find("uHorizontal");
				size_t foundLH = s.find("lHorizontal");
				size_t foundLV = s.find("lVertical");
				size_t foundRV = s.find("rVertical");
				size_t foundUL = s.find("ul");
				size_t foundUR = s.find("ur");
				size_t foundLL = s.find("ll");
				size_t foundLR = s.find("lr");

				if (foundUH != std::string::npos)
					ball.hBounce();
				else if (foundLH != std::string::npos)
					ball.hBounce();
				if (foundLV != std::string::npos)
					ball.vBounce();
				else if (foundRV != std::string::npos)
					ball.vBounce();
				else
				{
					if (foundUL != std::string::npos)
						ball.ulBounce(point);
					else if (foundUR != std::string::npos)
						ball.urBounce(point);
					else if (foundLL != std::string::npos)
						ball.llBounce(point);
					else if (foundLR != std::string::npos)
						ball.lrBounce(point);
				}

				// update Score
				score += (skr * level);

				if ((ball.getY() + ballSize / 2 > paddle.getHeight() + paddle.getY() || ball.getX() < 0 || ball.getX() > w.width) && flag != "page2")
				{
					flag = "begin";
					if (live == 0)
						flag = "page2";
					--live;
				}

				finished = layout.finished();
			}

			/////////////////////
			///////DRAW//////////
			/////////////////////

			// clear background
			XClearWindow(display, window);

			// create buffer
			// create off screen buffer

			// size and *depth* of pixmap
			// draw into the buffer
			// note that a window and a pixmap are “drawables”
			XSetForeground(display, gc, background);
			XFillRectangle(display, pixmap, gc, 0, 0, w.width, w.height);
			XSetForeground(display, gc, foreground);

			// draw rectangle
			XDrawRectangle(display, pixmap, gcBall,
						   paddle.getX() - paddle.getWidth() / 2,
						   paddle.getY() - paddle.getHeight() / 2,
						   paddle.getWidth(), paddle.getHeight());

			// draw ball from centre
			XFillArc(display, pixmap, gcBall,
					 ball.getX() - ballSize / 2,
					 ball.getY() - ballSize / 2,
					 ballSize, ballSize,
					 0, 360 * 64);

			// draw layout
			if (flag == "page1")
			{
				if (fps_passed % FPS < FPS / 2)
				{
					XDrawString(display, pixmap, gc, 14, 15, "PRESS 'SPACE' TO START", 22);
					XDrawString(display, pixmap, gc, 541, 600, "PRESS 'SPACE' TO START", 22);
				}
				renderLOGO(display, pixmap, gc);

				XDrawString(display, pixmap, gc, 5, 32, "'LEFT'  / 'A' TO MOVE THE PADDLE LEFT", 36);
				XDrawString(display, pixmap, gc, 5, 50, "'RIGHT' / 'D' TO MOVE THE PADDLE RIGHT", 37);
				XDrawString(display, pixmap, gc, 5, 68, "'SPACE' TO PAUSE", 16);
				XDrawString(display, pixmap, gc, 5, 86, "'Q' TO QUIT", 11);
				XDrawString(display, pixmap, gc, 550, 500, "CS 349 ASSIGNMENT ONE", 21);
				XDrawString(display, pixmap, gc, 586, 518, "BAIZHOU CHEN", 12);
				XDrawString(display, pixmap, gc, 604, 536, "20622312", 8);
			}
			else if (flag == "begin")
			{
				layout.render(display, pixmap, gc);
				if (fps_passed % FPS < FPS / 2)
				{
					XDrawString(display, pixmap, gc, 541, 600, "PRESS 'SPACE' TO START", 22);
				}
				XDrawString(display, pixmap, gc, 1180, 15, "SCORE: ", 7);
				XDrawString(display, pixmap, gc, 1262, 15, to_string(score).c_str(), to_string(score).length());
				for (int i = 0; i < live; ++i)
				{
					XFillArc(display, pixmap, gc,
							 1240 - i * 28,
							 760,
							 15, 15,
							 0, 360 * 64);
				}
			}

			else if (flag == "start")
			{
				layout.render(display, pixmap, gc);
				XDrawString(display, pixmap, gc, 1180, 15, "SCORE: ", 7);
				XDrawString(display, pixmap, gc, 1243, 15, to_string(score).c_str(), to_string(score).length());
				for (int i = 0; i < live; ++i)
				{
					XFillArc(display, pixmap, gc,
							 1240 - i * 28,
							 760,
							 15, 15,
							 0, 360 * 64);
				}
			}
			else if (flag == "pause")
			{
				layout.render(display, pixmap, gc);
				XDrawString(display, pixmap, gc, 1180, 15, "SCORE: ", 7);
				XDrawString(display, pixmap, gc, 1243, 15, to_string(score).c_str(), to_string(score).length());
				layout.render(display, pixmap, gc);
				if (fps_passed % FPS < FPS / 2)
				{
					XDrawString(display, pixmap, gc, 541, 600, "PRESS 'SPACE' TO CONTINUE", 25);
				}
				for (int i = 0; i < live; ++i)
				{
					XFillArc(display, pixmap, gc,
							 1240 - i * 28,
							 760,
							 15, 15,
							 0, 360 * 64);
				}
			}

			// GAME OVER
			else if (flag == "page2")
			{
				XDrawString(display, pixmap, gc, 600, 500, "GAME OVER", 9);
				XDrawString(display, pixmap, gc, 568, 530, "YOUR SCORE: ", 12);
				XDrawString(display, pixmap, gc, 691, 530, to_string(score).c_str(), to_string(score).length());
				if (fps_passed % FPS < FPS / 2)
				{
					XDrawString(display, pixmap, gc, 541, 600, "PRESS 'SPACE' TO RESTART", 24);
				}
				renderLOGO(display, pixmap, gc);
			}
			// copy buffer to window
			XCopyArea(display, pixmap, window, gc,
					  0, 0, w.width, w.height, // pixmap region to copy
					  0, 0);				   // top left corner of pixmap in window

			XFlush(display);
			fps_passed++;
			lastRepaint = now(); // remember when the paint happened
		}

		// IMPORTANT: sleep for a bit to let other processes work
		if (XPending(display) == 0)
		{
			usleep(1000000 / FPS - (now() - lastRepaint));
		}
	}

	XCloseDisplay(display);
}
