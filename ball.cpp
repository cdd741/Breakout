#include <cmath>
#include "ball.h"

using namespace std;

void Ball::setAngle(int ag)
{
    angle = ag;
    while (angle > 360)
        angle = angle - 360;
    while (angle < 0)
        angle = angle + 360;
    dx = cos(angle * PI / 180.0);
    dy = sin(angle * PI / 180.0);
    cout << "angle: " << ag << endl
         << "dx: " << dx << endl
         << "dy: " << dy << endl;
}

void Ball::nextMove()
{
    x += dx;
    y -= dy;
}
void Ball::vBounce()
{
    if (angle <= 180)
    {
        angle = 180 - angle;
    }
    else
        angle = 540 - angle;
    setAngle(angle);
    cout << "vBounce" << endl;
}

void Ball::hBounce()
{
    angle = -angle;
    setAngle(angle);
    cout << "hBounce" << endl;
}

void Ball::ulBounce(pair<double, double> paddlePos)
{
    double r = atan2(y - paddlePos.second, paddlePos.first - x);

    int a = r * 180 / PI;
    cout << "angle bounce UL!" << endl;
    cout << "bounce angle in  = " << angle << endl;
    cout << "bounce angle out  = " << 2 * a - angle << endl;
    if (angle < 135)
        r -= 45;
    setAngle(2 * a - angle - 180);
    //setAngle(90 - angle);
    nextMove();
}

void Ball::urBounce(pair<double, double> paddlePos)
{
    double r = atan2(y - paddlePos.second, paddlePos.first - x);

    int a = r * 180 / PI;
    cout << "angle bounce UR!" << endl;
    cout << "bounce angle in  = " << angle << endl;
    cout << "bounce angle out  = " << 2 * a - angle + 180 << endl;
    // setAngle(angle - 270);
    setAngle(2 * a - angle + 180);
    nextMove();
}

void Ball::llBounce(pair<double, double> paddlePos)
{
    double r = atan2(y - paddlePos.second, paddlePos.first - x);
    int a = r * 180 / PI;
    cout << "angle bounce LL!" << endl;
    cout << "bounce angle in  = " << angle << endl;
    cout << "bounce angle out  = " << 2 * a - angle - 180 << endl;

    setAngle(2 * a - angle - 180);
    nextMove();
}

void Ball::lrBounce(pair<double, double> paddlePos)
{
    double r = atan2(y - paddlePos.second, paddlePos.first - x);
    int a = r * 180 / PI;
    cout << "angle bounce LR!" << endl;
    cout << "bounce angle in  = " << angle << endl;
    cout << "bounce angle out  = " << 3 * a - 2 * angle - 540 << endl;
    setAngle(2 * a - angle - 540);
    //  if (angle < 45)
    //    r += 45;
    //   if (angle > 315)
    //    r -= 90;
    nextMove();
}

void Ball::paddleBounce(double pct)
{
    angle = 150 * pct + 15;
    setAngle(angle);
}
