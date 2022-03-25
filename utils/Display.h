#pragma once

#include <GL/freeglut.h>
#include "zBuffer.h"

struct Display{
    static float T[4][4];
    static zBuffer* zbuffer;
    Display(zBuffer* buffer);
    static void run();
    static void loop();
    static void dragMouse(int button, int state, int x, int y);
    static void keyboard(int key, int x, int y);
};