#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <numeric>
#include <string>
#include <iostream>
#include <cstring>

#include <GLUT/GLUT.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>

const unsigned int windowWidth = 800, windowHeight = 600;
int majorVersion = 3, minorVersion = 0;
bool keyboardState[256];

char FILE_ROOT_PATH[100] = "/Users/liweijiang/Desktop/CS451Project1/graphics1.3/graphics1.3/resource/";
