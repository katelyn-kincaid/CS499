//============================================================================
// Name        : table.cpp
// Author      : Katelyn Kincaid
// Version     : 2.0
// Copyright   : Your copyright notice
// Description : Enhancement One: Software Design & Engineering
//
// This is only the first stopping point. It was more difficult than I
// originally thought to have 2 objects, texture, and mouse movements.
//============================================================================

//header inclusions
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>

//glm math header inclusions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* Global variables */
char title[] = "Enhancement One";
GLfloat angleTable = 0.0f;  // Rotational angle for table
GLfloat angleCube = 0.0f;     // Rotational angle for cube
int refreshMills = 15;        // refresh interval in milliseconds

//Initialize OpenGL Graphics
void initGL() {
   glClearColor(0.0f, 0.5f, 1.0f, 1.0f); // Set background color to sky blue and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

}


void display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

   glLoadIdentity();                 // Reset the model-view matrix
   glTranslatef(0.0f, 1.5f, -6.0f);  // Centered and then moved up and back
   glRotatef(angleCube, 1.0f, 1.0f, 1.0f);  // Rotate about (1,1,1)-axis

   glBegin(GL_QUADS);                // Begin drawing the cube
      // Top face
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f( 0.25f, 0.25f, -0.25f);
      glVertex3f(-0.25f, 0.25f, -0.25f);
      glVertex3f(-0.25f, 0.25f,  0.25f);
      glVertex3f( 0.25f, 0.25f,  0.25f);

      // Bottom face
      glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      glVertex3f( 0.25f, -0.25f,  0.25f);
      glVertex3f(-0.25f, -0.25f,  0.25f);
      glVertex3f(-0.25f, -0.25f, -0.25f);
      glVertex3f( 0.25f, -0.25f, -0.25f);

      // Front face
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f( 0.25f,  0.25f, 0.25f);
      glVertex3f(-0.25f,  0.25f, 0.25f);
      glVertex3f(-0.25f, -0.25f, 0.25f);
      glVertex3f( 0.25f, -0.25f, 0.25f);

      // Back face
      glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
      glVertex3f( 0.25f, -0.25f, -0.25f);
      glVertex3f(-0.25f, -0.25f, -0.25f);
      glVertex3f(-0.25f,  0.25f, -0.25f);
      glVertex3f( 0.25f,  0.25f, -0.25f);

      // Left face
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(-0.25f,  0.25f,  0.25f);
      glVertex3f(-0.25f,  0.25f, -0.25f);
      glVertex3f(-0.25f, -0.25f, -0.25f);
      glVertex3f(-0.25f, -0.25f,  0.25f);

      // Right face
      glColor3f(1.0f, 0.0f, 1.0f);     // pink
      glVertex3f(0.25f,  0.25f, -0.25f);
      glVertex3f(0.25f,  0.25f,  0.25f);
      glVertex3f(0.25f, -0.25f,  0.25f);
      glVertex3f(0.25f, -0.25f, -0.25f);
   glEnd();  // End of drawing color-cube


   glLoadIdentity();                  // Reset the model-view matrix
   glTranslatef(0.0f, -1.0f, -6.0f);  // centered and pushed back and down
   glRotatef(angleTable, 1.0f, 1.0f, 1.0f);  // Rotate about the (1,1,1)-axis

   glBegin(GL_TRIANGLES);
	//table top
	glColor3f(0.5f, 0.5f, 0.5f); //grey
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);

	//bottom of table top
	glColor3f(0.5f, 0.5f, 0.5f); //grey
	glVertex3f(-1.0f, 0.9f, -1.0f);
	glVertex3f(1.0f, 0.9f, -1.0f);
	glVertex3f(1.0f, 0.9f, 1.0f);
	glVertex3f(1.0f, 0.9f, 1.0f);
	glVertex3f(-1.0f, 0.9f, 1.0f);
	glVertex3f(-1.0f, 0.9f, -1.0f);


	//back facing edge
	glColor3f(0.5f, 0.5f, 0.5f); //grey
	glVertex3f(-1.0f, 0.8f, -1.0f);
	glVertex3f(1.0f, 0.8f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 0.8f, -1.0f);

	//front facing edge
	glColor3f(0.5f, 0.5f, 0.5f); //grey
	glVertex3f(-1.0f, 0.8f, 1.0f);
	glVertex3f(1.0f, 0.8f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 0.8f, 1.0f);

	//left facing edge
	glColor3f(0.5f, 0.5f, 0.5f); //grey
	glVertex3f(-1.0f, 0.8f, -1.0f);
	glVertex3f(-1.0f, 0.8f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 0.8f, -1.0f);

	//right facing edge
	glColor3f(0.5f, 0.5f, 0.5f); //grey
	glVertex3f(1.0f, 0.8f, -1.0f);
	glVertex3f(1.0f, 0.8f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 0.8f, -1.0f);

	//front left table leg
	//parallel version A
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(-1.0f, 0.8f, 1.0f);
	glVertex3f(-0.9f, 0.8f, 1.0f);
	glVertex3f(-0.9f, -0.9f, 1.0f);
	glVertex3f(-0.9f, -0.9f, 1.0f);
	glVertex3f(-1.0f, -0.9f, 1.0f);
	glVertex3f(-1.0f, 0.8f, 1.0f);

	//parallel version A
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(-1.0f, 0.8f, 1.0f);
	glVertex3f(-0.9f, 0.8f, 1.0f);
	glVertex3f(-0.9f, -0.9f, 0.9f);
	glVertex3f(-0.9f, -0.9f, 0.9f);
	glVertex3f(-1.0f, -0.9f, 0.9f);
	glVertex3f(-1.0f, 0.8f, 0.9f);


	//parallel version B
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(-1.0f, 0.8f, 1.0f);
	glVertex3f(-1.0f, 0.8f, 0.9f);
	glVertex3f(-1.0f, -0.9f, 0.9f);
	glVertex3f(-1.0f, -0.9f, 0.9f);
	glVertex3f(-1.0f, -0.9f, 1.0f);
	glVertex3f(-1.0f, 0.8f, 1.0f);

	//parallel version B
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(-0.9f, 0.8f, 1.0f);
	glVertex3f(-0.9f, 0.8f, 0.9f);
	glVertex3f(-0.9f, -0.9f, 0.9f);
	glVertex3f(-0.9f, -0.9f, 0.9f);
	glVertex3f(-0.9f, -0.9f, 1.0f);
	glVertex3f(-0.9f, 0.8f, 1.0f);

	//bottom front left leg
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(-1.0f, -0.9f, 1.0f);
	glVertex3f(-0.9f, -0.9f, 1.0f);
	glVertex3f(-0.9f, -0.9f, 0.9f);
	glVertex3f(-0.9f, -0.9f, 0.9f);
	glVertex3f(-1.0f, -0.9f, 0.9f);
	glVertex3f(-1.0f, -0.9f, 1.0f);

	//front right table leg
	//parallel version A
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(1.0f, 0.8f, 1.0f);
	glVertex3f(0.9f, 0.8f, 1.0f);
	glVertex3f(0.9f, -0.9f, 1.0f);
	glVertex3f(0.9f, -0.9f, 1.0f);
	glVertex3f(1.0f, -0.9f, 1.0f);
	glVertex3f(1.0f, 0.8f, 1.0f);

	//parallel version A
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(1.0f, 0.8f, 0.9f);
	glVertex3f(0.9f, 0.8f, 0.9f);
	glVertex3f(0.9f, -0.9f, 0.9f);
	glVertex3f(0.9f, -0.9f, 0.9f);
	glVertex3f(1.0f, -0.9f, 0.9f);
	glVertex3f(1.0f, 0.8f, 0.9f);

	//parallel version B
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(1.0f, 0.8f, 1.0f);
	glVertex3f(1.0f, 0.8f, 0.9f);
	glVertex3f(1.0f, -0.9f, 0.9f);
	glVertex3f(1.0f, -0.9f, 0.9f);
	glVertex3f(1.0f, -0.9f, 1.0f);
	glVertex3f(1.0f, 0.8f, 1.0f);

	//parallel version B
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(0.9f, 0.8f, 1.0f);
	glVertex3f(0.9f, 0.8f, 0.9f);
	glVertex3f(0.9f, -0.9f, 0.9f);
	glVertex3f(0.9f, -0.9f, 0.9f);
	glVertex3f(0.9f, -0.9f, 1.0f);
	glVertex3f(0.9f, 0.8f, 1.0f);

	//bottom front right leg
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(1.0f, -0.9f, 1.0f);
	glVertex3f(0.9f, -0.9f, 1.0f);
	glVertex3f(0.9f, -0.9f, 0.9f);
	glVertex3f(0.9f, -0.9f, 0.9f);
	glVertex3f(1.0f, -0.9f, 0.9f);
	glVertex3f(1.0f, -0.9f, 1.0f);

	//right back leg
	//parallel version A
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(1.0f, 0.8f, -1.0f);
	glVertex3f(1.0f, 0.8f, -0.9f);
	glVertex3f(1.0f, -0.9f, -0.9f);
	glVertex3f(1.0f, -0.9f, -0.9f);
	glVertex3f(1.0f, -0.9f, -1.0f);
	glVertex3f(1.0f, 0.8f, -1.0f);

	//parallel version A
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(0.9f, 0.8f, -1.0f);
	glVertex3f(0.9f, 0.8f, -0.9f);
	glVertex3f(0.9f, -0.9f, -0.9f);
	glVertex3f(0.9f, -0.9f, -0.9f);
	glVertex3f(0.9f, -0.9f, -1.0f);
	glVertex3f(0.9f, 0.8f, -1.0f);

	//parallel version B
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(1.0f, 0.8f, -0.9f);
	glVertex3f(0.9f, 0.8f, -0.9f);
	glVertex3f(0.9f, -0.9f, -0.9f);
	glVertex3f(0.9f, -0.9f, -0.9f);
	glVertex3f(1.0f, -0.9f, -0.9f);
	glVertex3f(1.0f, 0.8f, -0.9f);

	//parallel version B
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(1.0f, 0.8f, -1.0f);
	glVertex3f(0.9f, 0.8f, -1.0f);
	glVertex3f(0.9f, -0.9f, -1.0f);
	glVertex3f(0.9f, -0.9f, -1.0f);
	glVertex3f(1.0f, -0.9f, -1.0f);
	glVertex3f(1.0f, 0.8f, -1.0f);

	//bottom right back leg
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(1.0f, -0.9f, -1.0f);
	glVertex3f(0.9f, -0.9f, -1.0f);
	glVertex3f(0.9f, -0.9f, -0.9f);
	glVertex3f(0.9f, -0.9f, -0.9f);
	glVertex3f(1.0f, -0.9f, -0.9f);
	glVertex3f(1.0f, -0.9f, -1.0f);

	//back left leg
	//parallel version A
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(-1.0f, 0.8f, -1.0f);
	glVertex3f(-1.0f, 0.8f, -0.9f);
	glVertex3f(-1.0f, -0.9f, -0.9f);
	glVertex3f(-1.0f, -0.9f, -0.9f);
	glVertex3f(-1.0f, -0.9f, -1.0f);
	glVertex3f(-1.0f, 0.8f, -1.0f);

	//parallel version A
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(-0.9f, 0.8f, -1.0f);
	glVertex3f(-0.9f, 0.8f, -0.9f);
	glVertex3f(-0.9f, -0.9f, -0.9f);
	glVertex3f(-0.9f, -0.9f, -0.9f);
	glVertex3f(-0.9f, -0.9f, -1.0f);
	glVertex3f(-0.9f, 0.8f, -1.0f);

	//parallel version B
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(-1.0f, 0.8f, -0.9f);
	glVertex3f(-0.9f, 0.8f, -0.9f);
	glVertex3f(-0.9f, -0.9f, -0.9f);
	glVertex3f(-0.9f, -0.9f, -0.9f);
	glVertex3f(-1.0f, -0.9f, -0.9f);
	glVertex3f(-1.0f, 0.8f, -0.9f);

	//parallel version B
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(-1.0f, 0.8f, -1.0f);
	glVertex3f(-0.9f, 0.8f, -1.0f);
	glVertex3f(-0.9f, -0.9f, -1.0f);
	glVertex3f(-0.9f, -0.9f, -1.0f);
	glVertex3f(-1.0f, 0.8f, -1.0f);

	//bottom back left leg
	glColor3f(0.0f, 0.0f, 0.0f); //black
	glVertex3f(-1.0f, -0.9f, -1.0f);
	glVertex3f(-0.9f, -0.9f, -1.0f);
	glVertex3f(-0.9f, -0.9f, -0.9f);
	glVertex3f(-0.9f, -0.9f, -0.9f);
	glVertex3f(-1.0f, -0.9f, -0.9f);
	glVertex3f(-1.0f, -0.9f, -1.0f);


   glEnd();   // Done drawing the table

   glutSwapBuffers();  // Swap the front and back frame buffers

   // Update the rotational angle after each refresh
   angleTable -= 0.15f;
   angleCube += 0.10f;
}


void timer(int value) {



   glutPostRedisplay();
   glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}


void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;

   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);

   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
   glutInitWindowSize(640, 480);   // Set the window's initial width & height
   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   glutCreateWindow(title);          // Create window with the given title
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   initGL();                       // Our own OpenGL initialization
   glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
   glutMainLoop();                 // Enter the infinite event-processing loop
   return 0;
}




