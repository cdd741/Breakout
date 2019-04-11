# Breakout

Breakout is a arcade game, originally published by Atari in 1976. "In the game, a layer of bricks lines the top third of the screen. A ball travels across the screen, bouncing off the top and side walls of the screen. When a brick is hit, the ball bounces away and the brick is destroyed. The player loses a turn when the ball touches the bottom of the screen. To prevent this from happening, the player has a movable paddle to bounce the ball upward, keeping it in play." (see [Wikipedia](https://en.wikipedia.org/wiki/Breakout_(video_game)).)

### Prerequisite

You need to run under Linux with the support of Xlib.

### Make and Run

Type

```
make
```

and

```
./breakout
```

to run the program

### Rules:

1. Using paddle to eliminate all bricks to get a high score.

2. 3 additional lives in total in a single game.

3. After all lives you recieve a score and score reset to zero.

4. After all bricks are eliminated, a new layout will appear.

### Control:

1. left arrow or 'a' to move the paddle left.

2. right arror or 'd' to move the paddle right.

3. space to start or pause.

4. 'q' to quit.

### Features:

Bricks' color is generated randomly from a series of selected colors.

There are 3 different layouts, and the layout will be randomly chosen.

The ball and paddle's color change as the color that the ball hit the brick.
