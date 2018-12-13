//
//  snow.cpp
//  graphics1.3
//
//  Created by Heidi He on 12/12/18.
//  Copyright © 2018 Heidi He. All rights reserved.
//

#include "snow.hpp"


float slowdown = 2.0;
float velocity = 0.0;
float zoom = -40.0;
float pan = 0.0;
float tilt = 0.0;
float hailsize = 0.1;

int loop;
int fall;

//floor colors
float r = 0.0;
float g = 1.0;
float b = 0.0;
float ground_points[21][21][3];
float ground_colors[21][21][4];
float accum = -10.0;

// Paticle System
particles par_sys[MAX_PARTICLES];


// Initialize/Reset Particles - give them their attributes
void initParticles(int i) {
    par_sys[i].alive = true;
    par_sys[i].life = 1.0;
    par_sys[i].fade = float(rand()%100)/1000.0f+0.003f;
    
//    par_sys[i].xpos = (float) ((rand() % 21) - 10);
//    par_sys[i].ypos = 1.0;
//    par_sys[i].zpos = (float) (rand() % 21) - 10;
    par_sys[i].xpos = (float) ((rand() % 21) - 10)/10;
    par_sys[i].ypos = -0.5;
    par_sys[i].zpos = -(float) ((rand() % 21) - 10)/10;
    
    
    
    par_sys[i].red = 0.5;
    par_sys[i].green = 0.5;
    par_sys[i].blue = 1.0;
    
    par_sys[i].vel = velocity;
    par_sys[i].gravity = -0.8;//-0.8;
    
}

void initSnow( ) {
    printf("initialized snow scene\n");
    int x, z;
    
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    
    // Ground Verticies
    // Ground Colors
    for (z = 0; z < 21; z++) {
        for (x = 0; x < 21; x++) {
            ground_points[x][z][0] = x - 10.0;
            ground_points[x][z][1] = accum;
            ground_points[x][z][2] = z - 10.0;
            
            ground_colors[z][x][0] = r; // red value
            ground_colors[z][x][1] = g; // green value
            ground_colors[z][x][2] = b; // blue value
            ground_colors[z][x][3] = 0.0; // acummulation factor
        }
    }
    
    // Initialize particles
    for (loop = 0; loop < MAX_PARTICLES; loop++) {
        initParticles(loop);
    }
}

// For Snow
void drawSnow() {
    
    printf("draw snow \n");
    
    float x, y, z;
    for (loop = 0; loop < MAX_PARTICLES; loop=loop+2) {
        if (par_sys[loop].alive == true) {
            x = par_sys[loop].xpos;
            y = par_sys[loop].ypos;
            z = par_sys[loop].zpos + zoom;
            
            // Draw particles
            glColor3f(1.0, 1.0, 1.0);
            glPushMatrix();
            glTranslatef(x, y, z);
//            glutSolidSphere(0.2, 16, 16);
            glutSolidSphere(5, 16, 16);
            glPopMatrix();
            
            // Update values
            //Move
            par_sys[loop].ypos += par_sys[loop].vel / (slowdown*1000);
            par_sys[loop].vel += par_sys[loop].gravity;
            // Decay
            par_sys[loop].life -= par_sys[loop].fade;
            
            if (par_sys[loop].ypos <= -10) {
                int zi = z - zoom + 10;
                int xi = x + 10;
                ground_colors[zi][xi][0] = 1.0;
                ground_colors[zi][xi][2] = 1.0;
                ground_colors[zi][xi][3] += 1.0;
                if (ground_colors[zi][xi][3] > 1.0) {
                    ground_points[xi][zi][1] += 0.1;
                }
                par_sys[loop].life = -1.0;
            }
            
            printf("particle at %f %f %f \n", par_sys[loop].xpos, par_sys[loop].ypos, par_sys[loop].zpos );
            
            //Revive
            if (par_sys[loop].life < 0.0) {
                initParticles(loop);
            }
        }
    }
}

// Draw Particles
void drawScene( ) {
//    initSnow();
    
    int i, j;
    float x, y, z;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    
    
    glRotatef(pan, 0.0, 1.0, 0.0);
    glRotatef(tilt, 1.0, 0.0, 0.0);
    
    // GROUND?!
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    // along z - y const
    for (i = -10; i+1 < 11; i++) {
        // along x - y const
        for (j = -10; j+1 < 11; j++) {
            glColor3fv(ground_colors[i+10][j+10]);
            glVertex3f(ground_points[j+10][i+10][0],
                       ground_points[j+10][i+10][1],
                       ground_points[j+10][i+10][2] + zoom);
            glColor3fv(ground_colors[i+10][j+1+10]);
            glVertex3f(ground_points[j+1+10][i+10][0],
                       ground_points[j+1+10][i+10][1],
                       ground_points[j+1+10][i+10][2] + zoom);
            glColor3fv(ground_colors[i+1+10][j+1+10]);
            glVertex3f(ground_points[j+1+10][i+1+10][0],
                       ground_points[j+1+10][i+1+10][1],
                       ground_points[j+1+10][i+1+10][2] + zoom);
            glColor3fv(ground_colors[i+1+10][j+10]);
            glVertex3f(ground_points[j+10][i+1+10][0],
                       ground_points[j+10][i+1+10][1],
                       ground_points[j+10][i+1+10][2] + zoom);
        }
        
    }
    glEnd();

    
    drawSnow();
    
//    glutSwapBuffers();
}



