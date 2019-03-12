#include <sstream>
#include <cmath>
#include "layout.h"

using namespace std;

Layout::Layout(int n, Display *display)
{
    unsigned int x;
    for (auto it : colorHex)
    {
        std::stringstream ss;
        ss << std::hex << it;
        ss >> x;
        color.emplace_back(x);
    }

    XColor xcolor, colorcell;
    // pattern #1
    if (n == 1)
    {
        int idx = rand() % (color.size() - 10);
        for (int i = 2; i < 11; ++i)
            for (int j = 3; j < 19; ++j)
            {

                bricks.emplace_back(brick(j * Size.first, i * Size.second, color[idx + i - 2]));
            }
    }
    // pattern #2
    else if (n == 2)
    {
        int idx = rand() % (color.size() - 10);
        int count = 1;
        for (int i = 2; i < 11; ++i)
        {
            for (int j = 0; j < count; ++j)
            {
                bricks.emplace_back(brick(640 + j * Size.first, 32 + i * Size.second, color[idx + i - 2]));
                bricks.emplace_back(brick(640 - (j + 1) * Size.first, 32 + i * Size.second, color[idx + i - 2]));
            }
            if (i - 1 < 5)
                count += 2;
            else
                count -= 2;
        }
    }
    // pattern #3
    else
    {
        int idx = rand() % (color.size() - 10);
        int count = 9;
        for (int i = 2; i < 11; ++i)
        {
            for (int j = 0; j < count; ++j)
            {
                bricks.emplace_back(brick(128 + j * Size.first, 32 + i * Size.second, color[idx + i - 2]));
                if (j == 9)
                    continue;
                bricks.emplace_back(brick(1152 - (j + 1) * Size.first, 32 + i * Size.second, color[idx + i - 2]));
            }
            if (i - 1 < 5)
                count -= 2;
            else
                count += 2;
        }
    }
}

pair<double, double> Layout::checkHit(pair<double, double> ballPos, int ballSize, pair<double, double> ballDir, string &s, int &score, Display *display, GC &gc)
{
    unsigned int pix;
    int skr = 0;
    s = "";
    pair<double, double> point = make_pair(0, 0);
    for (auto &it : bricks)
    {
        pair<int, int> pos = it.getPos();
        if (it.isActiv() == false)
            continue;

        double xNext = ballPos.first + ballDir.first;
        double yNext = ballPos.second + ballDir.second;

        if (((ballPos.second + ballSize / 2 + ballDir.second > pos.second) &&
             (ballPos.second + ballSize / 2 + ballDir.second < pos.second + Size.second)) &&
            !((ballPos.second - ballSize / 2 + ballDir.second > pos.second) &&
              (ballPos.second - ballSize / 2 + ballDir.second < pos.second + Size.second)) &&
            (pos.first < ballPos.first && ballPos.first < pos.first + Size.first))
        {
            ++skr;
            pix = it.hit();
            s = s + "uHorizontal ";
        }
        else if (((ballPos.second - ballSize / 2 + ballDir.second > pos.second) &&
                  (ballPos.second - ballSize / 2 + ballDir.second < pos.second + Size.second)) &&
                 !((ballPos.second + ballSize / 2 + ballDir.second > pos.second) &&
                   (ballPos.second + ballSize / 2 + ballDir.second < pos.second + Size.second)) &&
                 (pos.first < ballPos.first && ballPos.first < pos.first + Size.first))
        {
            ++skr;
            pix = it.hit();
            s = s + "lHorizontal ";
        }
        else if (((ballPos.first + ballSize / 2 + ballDir.first > pos.first) &&
                  (ballPos.first + ballSize / 2 + ballDir.first < pos.first + Size.first)) &&
                 !((ballPos.first - ballSize / 2 + ballDir.first > pos.first) &&
                   (ballPos.first - ballSize / 2 + ballDir.first < pos.first + Size.first)) &&
                 (pos.second < ballPos.second && ballPos.second < pos.second + Size.second))
        {
            ++skr;
            pix = it.hit();
            s = s + "lVertical ";
        }
        else if (((ballPos.first - ballSize / 2 + ballDir.first > pos.first) &&
                  (ballPos.first - ballSize / 2 + ballDir.first < pos.first + Size.first)) &&
                 !((ballPos.first + ballSize / 2 + ballDir.first > pos.first) &&
                   (ballPos.first + ballSize / 2 + ballDir.first < pos.first + Size.first)) &&
                 (pos.second < ballPos.second && ballPos.second < pos.second + Size.second))
        {
            ++skr;
            pix = it.hit();
            s = s + "rVertical ";
        }

        else if (sqrt(pow(ballPos.first - pos.first, 2) + pow(ballPos.second - pos.second, 2)) < ballSize / 2)
        {
            ++skr;
            pix = it.hit();
            s = s + "ul ";
            point.first = pos.first;
            point.second = pos.second;
        }
        else if (sqrt(pow(ballPos.first - (pos.first + Size.first), 2) + pow(ballPos.second - pos.second, 2)) < ballSize / 2)
        {
            ++skr;
            pix = it.hit();
            s = s + "ur ";
            point.first = pos.first + Size.first;
            point.second = pos.second;
        }
        else if (sqrt(pow(ballPos.first - pos.first, 2) + pow(ballPos.second - (pos.second + Size.second), 2)) < ballSize / 2)
        {
            ++skr;
            pix = it.hit();
            s = s + "ll ";
            point.first = pos.first;
            point.second = pos.second + Size.second;
        }
        else if (sqrt(pow(ballPos.first - (pos.first + Size.first), 2) + pow(ballPos.second - (pos.second + Size.second), 2)) < ballSize / 2)
        {
            ++skr;
            pix = it.hit();
            s = s + "lr ";
            point.first = pos.first + Size.first;
            point.second = pos.second + Size.second;
        }
    }
    score *= skr;
    if (skr != 0)
        XSetForeground(display, gc, pix);
    return point;
}

void Layout::render(Display *display, Window &window, GC &gc)
{
    for (auto it : bricks)
    {
        pair<int, int> pos = it.getPos();
        if (it.isActiv())
        {
            XSetForeground(display, gc, it.pix);
            XFillRectangle(display, window, gc, pos.first, pos.second, Size.first - 4,
                           Size.second - 4);
        }
    }
    //XSetForeground(display, gc, 0);
}

bool Layout::finished()
{
    for (auto it : bricks)
    {
        if (it.isActiv())
            return false;
    }
    return true;
}
