#ifndef PADDLE_H
#define PADDLE_H

#include <vector>

class Paddle
{
    // position
    double x, y;
    // size
    double width, height;
    bool left, right;

  public:
    Paddle() : x(640), y(720), width(150), height(25)
    {
    }
    double getX() { return x; }
    double getY() { return y; }
    double getWidth() { return width; }
    double getHeight() { return height; }
    double &setX() { return x; }
    double &setY() { return y; }

    std::pair<double, double> pos() { return std::make_pair(x, y); }
    std::pair<double, double> ulPos() { return std::make_pair(x - getWidth() / 2, y - getHeight() / 2); }
    std::pair<double, double> urPos() { return std::make_pair(x + getWidth() / 2, y - getHeight() / 2); }

    void moveLeft()
    {
        if ((width - x) > 0 && left)
            x -= 25 / 1000000;
    }

    void moveRight()
    {
        if ((width + x) < 1280 && left)
            x += 25 / 1000000;
    }
};
#endif
