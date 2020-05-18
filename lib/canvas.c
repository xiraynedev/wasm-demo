#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h>

#define NUM_CIRCLES 500

// circle struct
struct Circle
{
  int x;  // x coord
  int y;  // y coord
  int r;  // radius
  int cr; // color red
  int cg; // color green
  int cb; // color blue
};

// Circle animation data struct
struct CircleAnimationData
{
  int x;  // x coord
  int y;  // y coord
  int r;  // radius
  int xv; // x velocity
  int yv; // y velocity
  int xd; // x direction
  int yd; // y direction
};

// Circles variables
struct Circle circles[NUM_CIRCLES];
struct CircleAnimationData animationData[NUM_CIRCLES];

// random number generator
int getRand(max)
{
  return (rand() % max);
}

// init circle data and start render - JS
int main()
{

  // seed random number generator
  srand(time(NULL));

  // create circles
  for (int i = 0; i < NUM_CIRCLES; i++)
  {

    // radius
    int radius = getRand(50);

    // coords
    int x = getRand(1000) + radius;
    int y = getRand(1000) + radius;

    // fill animation data struct
    animationData[i].x = x;
    animationData[i].y = y;
    animationData[i].r = radius;
    animationData[i].xv = getRand(15);
    animationData[i].yv = getRand(15);
    animationData[i].xd = 1;
    animationData[i].yd = 1;

    // fill circle struct
    circles[i].x = x;
    circles[i].y = y;
    circles[i].r = radius;
    circles[i].cr = getRand(255);
    circles[i].cg = getRand(255);
    circles[i].cb = getRand(255);
  }

  // start JS rendering
  EM_ASM({ render($0, $1); }, NUM_CIRCLES * 6, 6);
}

// return circles to JS
struct Circle *getCircles(int canvasWidth, int canvasHeight)
{
  // update circle data
  for (int i = 0; i < NUM_CIRCLES; i++)
  {

    // collision on right - set x direction backward
    if (animationData[i].x + animationData[i].r >= canvasWidth)
    {
      animationData[i].xd = 0;
    }

    // collision on left - set x direction forward
    if (animationData[i].x - animationData[i].r <= 0)
    {
      animationData[i].xd = 1;
    }

    // collision on top - set y direction forward
    if (animationData[i].y - animationData[i].r <= 0)
    {
      animationData[i].yd = 1;
    }

    // collision on bottom - set y direction backward
    if (animationData[i].y + animationData[i].r >= canvasHeight)
    {
      animationData[i].yd = 0;
    }

    // Move circle in specified direction
    if (animationData[i].xd == 1)
    {
      animationData[i].x += animationData[i].xv;
    }
    else
    {
      animationData[i].x -= animationData[i].xv;
    }

    if (animationData[i].yd == 1)
    {
      animationData[i].y += animationData[i].yv;
    }
    else
    {
      animationData[i].y -= animationData[i].yv;
    }

    // update matching circle
    circles[i].x = animationData[i].x;
    circles[i].y = animationData[i].y;
  }

  return circles;
}