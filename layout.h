#ifndef LAYOUT_H
#define LAYOUT_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <sys/time.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

static std::pair<int, int> Size = std::make_pair(60, 30);

class brick
{
    std::pair<int, int> pos;

  public:
    bool activ;
    unsigned int pix;

    // constructor
    brick(int x, int y, unsigned int pix) : activ(true), pos(std::make_pair(x, y)), pix(pix){};
    // get brick position
    std::pair<int, int> getPos() { return pos; }
    /*
// get brick Size
pair <int, int> getSize() { return Size; }
*/
    // check if the brick has not been hit
    bool isActiv() { return activ; }
    // when ball hits the brick
    unsigned int hit()
    {
        activ = false;
        return pix;
    }
};

class Layout
{
    std::vector<brick> bricks;
    std::vector<std::string> colorHex = {"FF6EB4", "FF6A6A", "FFF68F", "FFFACD", "BFEFFF", "E0FFFF", "FFEC8B", "FFB6C1", "FFA07A", "87CEFA", "B0C4DE", "FF00FF", "FF34B3", "E066FF", "AB82FF", "FFE4E1", "FFDEAD", "FF82AB", "FFDAB9", "FFC0CB", "FFBBFF"};
    std::vector<unsigned int> color;

  public:
    Layout(int n, Display *display);
    std::pair<double, double> checkHit(std::pair<double, double> ballPos, int ballSize,
                                       std::pair<double, double> ballDir,
                                       std::string &s, int &score, Display *display, GC &gc);

    void render(Display *display, Window &window, GC &gc);
    bool finished();
};

#endif
