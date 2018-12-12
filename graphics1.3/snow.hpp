//
//  snow.hpp
//  graphics1.3
//
//  Created by Heidi He on 12/12/18.
//  Copyright © 2018 Heidi He. All rights reserved.
//

#ifndef snow_hpp
#define snow_hpp

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

#define MAX_PARTICLES 1000
#define WCX		640
#define WCY		480
#define RAIN	0
#define SNOW	1
#define	HAIL	2



#endif /* snow_hpp */




typedef struct {
    // Life
    bool alive;	// is the particle alive?
    float life;	// particle lifespan
    float fade; // decay
    // color
    float red;
    float green;
    float blue;
    // Position/direction
    float xpos;
    float ypos;
    float zpos;
    // Velocity/Direction, only goes down in y dir
    float vel;
    // Gravity
    float gravity;
}particles;


void initParticles(int i);

void initSnow( );

void drawSnow() ;

void drawScene( ) ;
