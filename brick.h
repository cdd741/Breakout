#ifndef BRICK_H
#define BRICK_H

#include <vector>

using namespace std;

class brick
{
    pair<double, double> pos;

  public:
    bool activ;
    // constructor
    brick(double x, double y) : activ(true), pos(make_pair(x, y)){};
    // get brick position
    pair<double, double> getPos() { return pos; }
    /*
// get brick Size
pair <int, int> getSize() { return Size; }
*/
    // check if the brick has not been hit
    bool isActiv() { return activ; }
    // when ball hits the brick
    void hit() { activ = false; }
};

#endif
