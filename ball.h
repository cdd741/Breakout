#ifndef BALL_H
#define BALL_H

#include <iostream>
#include <string>

#define PI 3.14159265

class Ball
{
  // size
  int size;
  // position
  double x, y;
  // angle to horizon
  int angle;
  // ball direction, always a unit vector
  double dx, dy;

public:
  Ball(double x, double y, int size) : size(size), x(x), y(y), angle(0), dx(0), dy(0) {}
  // getter
  double getX() { return x; }
  double getY() { return y; }
  double getDX() { return dx; }
  double getDY() { return dy; }
  int getAngle() { return angle; }
  // setter
  double &setX() { return x; }
  double &setY() { return y; }
  double &setDX() { return dx; }
  double &setDY() { return dy; }
  void setAngle(int ag);
  void nextMove();
  void vBounce();
  void hBounce();
  void ulBounce(std::pair<double, double> paddlePos);
  void urBounce(std::pair<double, double> paddlePos);
  void llBounce(std::pair<double, double> paddlePos);
  void lrBounce(std::pair<double, double> paddlePos);
  void paddleBounce(double pct);
  std::pair<double, double> pos() { return std::make_pair(x, y); }
  std::pair<double, double> dir() { return std::make_pair(dx, dy); }
  std::pair<double, double> nextPos() { return std::make_pair(x + dx, y + dy); }
};

#endif
