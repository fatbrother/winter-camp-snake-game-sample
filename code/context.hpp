#pragma once

#include <bits/stdc++.h>

#define FPS 60
#define WIDTH 20    // width of the grid
#define HEIGHT 20   // height of the grid
#define GRIDSIZE 40 // size of the single grid
#define SPEED 5.0   // speed of the snake

#define UP 0
#define DOWN 2
#define LEFT 1
#define RIGHT 3

static int DIR[5] = {0, -1, 0, 1, 0};

int getClosestInt(double x)
{
    return (int)(x + 0.5);
}