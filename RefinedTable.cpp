//============================================================================
// Name        : table.cpp
// Author      : Katelyn Kincaid
// Version     : 2.1
// Copyright   : Your copyright notice
// Description : Enhancement One: Software Design & Engineering
//
// This is only the first stopping point. It was more difficult than I
// originally thought to have 2 objects, texture, and mouse movements.
//
// Step 2: getting a texture implemented
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

//image loader inclusion for texture
#include "SOIL2/SOIL2.h"

using namespace std; //standard namespace

#define WINDOW_TITLE "Enhancement One" //window title macro

//shader program macro
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif

//variable declaration for shader and buffers
GLint shaderProgram, cubeShaderProgram, WindowWidth = 800, WindowHeight = 600;
GLuint VBO, VAO, CubeVAO, texture;
GLfloat degrees = glm::radians(15.0f); //converts float to degrees

//function prototypes
void UCreateShader(void);
void UCreateBuffers(void);
void UGenerateTexture(void);
void UResizeWindow(int, int);
void URenderGraphics(void);

//shader program source code for table
const GLchar * vertexShaderSource = GLSL(330,
		layout (location = 0) in vec3 position; //vertex data from vertex attrib pointer 0
		layout (location = 1) in vec3 color; //color data from vertex attrib pointer 1
		layout (location = 2) in vec2 textureCoordinate;

		out vec3 mobileColor; //variable to transfer color data to the fragment shader
		out vec2 mobileTextureCoordinate;

		//global variables for the transform matrices
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(position, 1.0f); //transforms vertices to clip coordinates
		mobileColor = color; //references incoming color data
		mobileTextureCoordinate = vec2(textureCoordinate.x, 1 - textureCoordinate.y); //flips the texture horizontally
		}
);

//fragment shader source code for table
const GLchar * fragmentShaderSource = GLSL(330,
		in vec3 mobileColor; //variable to hold incoming color data from vertex shader
		in vec2 mobileTextureCoordinate;

		out vec4 gpuColor; //variable to pass color data to the GPU
		out vec4 gpuTexture;

		uniform sampler2D uTexture;

	void main() {
		gpuColor = vec4(mobileColor, 1.0); //sends color data to the GPU for rendering
		gpuTexture = texture(uTexture, mobileTextureCoordinate);
	}
);

//cube vertex shader source code
const GLchar * cubeVertexShaderSource = GLSL(330,
		layout (location = 0) in vec3 position; //vertex data from vertex attrib pointer 0
		layout (location = 1) in vec3 color; //color data from vertex attrib pointer 1

		out vec3 mobileColor; //variable to transfer color data to the fragment shader

		//global variables for the transform matrices
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(position, 1.0f); //transforms vertices to clip coordinates
		mobileColor = color; //references incoming color data
	}
);

//cube fragment shader source code
const GLchar * cubeFragmentShaderSource = GLSL(330,
		in vec3 mobileColor; //variable to hold incoming color data from vertex shader

		out vec4 gpuColor; //variable to pass color data to the GPU

	void main()
	{
		gpuColor = vec4(mobileColor, 1.0); //sends color data to the GPU for rendering
	}
);

//main function
int main(int argc, char** argv)
{
	glutInit(&argc, argv);            // Initialize GLUT
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); // Enable double buffered mode
	glutInitWindowSize(WindowWidth, WindowHeight); // Set the window's initial width & height
	glutCreateWindow(WINDOW_TITLE);          // Create window with the given title
	glutReshapeFunc(UResizeWindow);

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		std::cout<<"Failed to initialize GLEW"<<std::endl;
		return -1;
	}

	UCreateShader();
	UCreateBuffers();
	UGenerateTexture();
	glUseProgram(shaderProgram); //uses the table shader program
	glUseProgram(cubeShaderProgram);

	glClearColor(0.0f, 0.5f, 1.0f, 1.0f); //sets bg color to sky blue and opaque
	glutDisplayFunc(URenderGraphics);
	glutMainLoop();                 // Enter the infinite event-processing loop

	//destroys buffer objects once used
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &CubeVAO);
	glDeleteBuffers(1, &VBO);

	return 0;
}

//resize the window
void UResizeWindow(int w, int h)
{
	WindowWidth = w;
	WindowHeight = h;
	glViewport(0, 0, WindowWidth, WindowHeight);
}

void URenderGraphics(void)
{
	glEnable(GL_DEPTH_TEST); //enables z-depbth
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clears the screen

	GLint modelLoc, viewLoc, projLoc, uTextureLoc;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	//activates the VAO before drawing
	glBindVertexArray(VAO);

	//transforms the table
	model = glm::translate(model, glm::vec3(0.0, 0.0f, 0.0f)); //places object at center
	model = glm::rotate(model, degrees, glm::vec3(2.0, 1.0f, 0.0f)); //rotates object
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)); //increases object size

	//transforms the camera
	view = glm::translate(view, glm::vec3(0.5f, 0.0f, -5.0f)); //moves the world 0.5 units on x and -5 on z

	//creates a perspective projection
	projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);

	//retrieves and passes transform matrices to the shader program
	modelLoc = glGetUniformLocation(shaderProgram, "model");
	viewLoc = glGetUniformLocation(shaderProgram, "view");
	projLoc = glGetUniformLocation(shaderProgram, "projection");
	uTextureLoc = glGetUniformLocation(shaderProgram, "uTexture");

	//pass data to the table shader program
	glUniform1i(uTextureLoc, 0); //texture unit 0
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture); //activates the texture

	glutPostRedisplay();

	//draw the table
	glDrawArrays(GL_TRIANGLES, 0, 156);

	glBindVertexArray(0); //deactivates the VAO
	glutSwapBuffers();

	glBindVertexArray(CubeVAO);

	//transforms the cube
	model = glm::translate(model, glm::vec3(0.0, 0.0f, 0.0f)); //places object at center
	model = glm::rotate(model, degrees, glm::vec3(1.0, 1.0f, 0.0f)); //rotates object
	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); //increases object size

	//transforms the camera
	view = glm::translate(view, glm::vec3(0.5f, 0.5f, 0.5f)); //moves the world 0.5 units on x and -5 on z

	//creates a perspective projection
	projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);

	//retrieves and passes transform matrices to the shader program
	modelLoc = glGetUniformLocation(cubeShaderProgram, "model");
	viewLoc = glGetUniformLocation(cubeShaderProgram, "view");
	projLoc = glGetUniformLocation(cubeShaderProgram, "projection");

	//pass data to the table shader program
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glutPostRedisplay();

	//draw the cube
	glDrawArrays(GL_QUADS, 0, 36);

	glBindVertexArray(0); //deactivates the VAO
	glutSwapBuffers();

}

void UCreateShader()
{
	//table vertex shader
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER); //creates the vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //attaches the vertex shader to the source code
	glCompileShader(vertexShader); //compiles the vertex shader

	//table fragment shader
	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //creates the fragment shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); //attaches the fragment shader to the source code
	glCompileShader(fragmentShader); //compiles the fragment shader

	//table shader program
	shaderProgram = glCreateProgram(); //creates the shader program and returns an ID
	glAttachShader(shaderProgram, vertexShader); //attach vertex shader to the shader program
	glAttachShader(shaderProgram, fragmentShader);; //attach fragment shader to the shader program
	glLinkProgram(shaderProgram); //links vertex and fragment shaders to the shader program

	//delete vertex and fragment shaders once linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//cube vertex shader
	GLint cubeVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(cubeVertexShader, 1, &cubeVertexShaderSource, NULL);
	glCompileShader(cubeVertexShader);

	//cube fragment shader
	GLint cubeFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(cubeFragmentShader, 1, &cubeFragmentShaderSource, NULL);
	glCompileShader(cubeFragmentShader);

	//cube shader program
	cubeShaderProgram = glCreateProgram();
	glAttachShader(cubeShaderProgram, cubeVertexShader);
	glAttachShader(cubeShaderProgram, cubeFragmentShader);
	glLinkProgram(cubeShaderProgram);

	//delete the vertex and fragment shaders once linked
	glDeleteShader(cubeVertexShader);
	glDeleteShader(cubeFragmentShader);
}

void UCreateBuffers()
{
	//draw the table
	GLfloat verticesTable[] = {
	   		   //positions				//colors			//texture coords
	   		   //top of table top
	   		   -1.0f,  1.0f, -1.0f, 	1.0f, 1.0f, 1.0f,	0.0f, 1.0f,
	   		    1.0f,  1.0f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f,  1.0f,  1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
	   		    1.0f,  1.0f,  1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
	   		   -1.0f,  1.0f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f,  1.0f, -1.0f, 	1.0f, 1.0f, 1.0f, 	1.0f, 0.0f,

	   		   //bottom of table top
	   		   -1.0f,  0.9f, -1.0f,		1.0f, 1.0f, 1.0f, 	0.0f, 1.0f,
	   		    1.0f,  0.9f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f,  0.9f,  1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
	   		    1.0f,  0.9f,  1.0f, 	1.0f, 1.0f, 1.0f, 	1.0f, 1.0f,
	   		   -1.0f,  0.9f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f,  0.9f, -1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f,

	   		   //back facing edge of table top
	   		   -1.0f,  0.8f, 1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 1.0f,
	   		    1.0f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f,  1.0f, -1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
	   		    1.0f,  1.0f, -1.0f,		1.0f, 1.0f, 1.0f, 	1.0f, 1.0f,
	   		   -1.0f,  1.0f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f, 	1.0f, 0.0f,

	   		   //front facing edge of table
	   		   -1.0f,  0.9f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 1.0f,
	   		    1.0f,  0.9f, -1.0f, 	1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f,  0.9f,  1.0f,		1.0f, 1.0f, 1.0f, 	1.0f, 1.0f,
	   		    1.0f,  0.9f,  1.0f, 	1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
	   		   -1.0f,  0.9f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f,  0.9f, -1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f,

	   		   //left facing edge of table
	   		   -1.0f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 1.0f,
	   		   -1.0f,  0.8f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f,  1.0f,  1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
	   		   -1.0f,  1.0f,  1.0f,		1.0f, 1.0f, 1.0f, 	1.0f, 1.0f,
	   		   -1.0f,  1.0f, -1.0f, 	1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		   -1.0f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f, 	1.0f, 0.0f,

	   		   //right facing edge of table
	   		    1.0f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f, 	0.0f, 1.0f,
	   		    1.0f,  0.8f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f,  1.0f,  1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
	   		    1.0f,  1.0f,  1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
	   		    1.0f,  1.0f, -1.0f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		    1.0f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f,  	1.0f, 0.0f,

	   		   //parallel version A1 of front left leg
	   		   -1.0f,  0.8f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f,  0.8f,  1.0f, 	1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		   -0.9f, -0.9f,  1.0f, 	1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		   -0.9f, -0.9f,  1.0f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		   -1.0f, -0.9f,  1.0f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		   -1.0f,  0.8f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

	   		   //parallel version A2 of front left leg
	   		   -1.0f, -0.9f,  0.9f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		   -0.9f, -0.9f,  0.9f, 	1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f,  0.8f,  0.9f, 	1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f,  0.8f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f,  0.8f,  0.9f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		   -1.0f, -0.9f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

	   		   //parallel version B1 of front left leg
	   		   -1.0f,  0.8f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f,  0.8f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f, -0.9f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f, -0.9f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f, -0.9f,  1.0f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		   -1.0f,  0.8f,  1.0f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,

	   		   //parallel version B2 of front left leg
	   		   -0.9f,  0.8f,  1.0f, 	1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f,  0.8f,  0.9f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		   -0.9f, -0.9f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f, -0.9f,  0.9f, 	1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		   -0.9f, -0.9f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f,  0.8f,  1.0f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,

	   		   //bottom of front left leg
	   		   -1.0f, -0.9f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f, -0.9f,  1.0f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		   -0.9f, -0.9f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f, -0.9f,  0.9f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		   -1.0f, -0.9f,  0.9f, 	1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f, -0.9f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

	   		   //front right table leg
	   		   //parallel version A1 of front fright leg
	   		    1.0f,  0.8f,  1.0f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		    0.9f,  0.8f,  1.0f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		    0.9f, -0.9f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f, -0.9f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f, -0.9f,  1.0f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		    1.0f,  0.8f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

	   		   //parallel version A2 of front right leg
	   		    1.0f,  0.8f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f,  0.8f,  0.9f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		    0.9f, -0.9f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f, -0.9f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f, -0.9f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f,  0.8f,  0.9f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,

	   		   //parallel version B1 of front right leg
	   		    1.0f,  0.8f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f,  0.8f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f, -0.9f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f, -0.9f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f, -0.9f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f,  0.8f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

	   		   //parallel version B2 of front right leg
	   		    0.9f,  0.8f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f,  0.8f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f, -0.9f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f, -0.9f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f, -0.9f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f,  0.8f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

	   		   //bottom of front right leg
	   		    1.0f, -0.9f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f, -0.9f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f, -0.9f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f, -0.9f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f, -0.9f,  0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f, -0.9f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

	   		   //parallel version A1 of back right leg
	   		    1.0f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		    1.0f,  0.8f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f, -0.9f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

	   		   //parallel version A2 of back right leg
	   		    0.9f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		    0.9f,  0.8f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f, -0.9f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

	   		   //parallel version B1 of back right leg
	   		    1.0f,  0.8f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f,  0.8f, -0.9f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		    0.9f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		    0.9f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f,  0.8f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

	   		   //parallel version B2 of back right leg
	   		    1.0f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		    0.9f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f, -0.9f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f, -0.9f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f, -0.9f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

	   		   //bottom of back right table leg
	   		    1.0f, -0.9f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f, -0.9f, -1.0f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		    0.9f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    0.9f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		    1.0f, -0.9f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

	   		   //parallel version A1 of back left leg
	   		   -1.0f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f,  0.8f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		   -1.0f, -0.9f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

	   		   //parallel version A2 of back left leg
	   		   -0.9f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f,  0.8f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f, -0.9f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,

	   		   //parallel version B1 of back left leg
	   		   -1.0f,  0.8f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f,  0.8f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f,  0.8f, -0.9f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,

	   		   //parallel version B2 of back left leg
	   		   -1.0f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f, -0.9f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f, -0.9f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f, -0.9f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f,  0.8f, -1.0f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,

	   		   //bottom of back left leg
	   		   -1.0f, -0.9f, -1.0f,		1.0f, 1.0f, 1.0f, 	0.0f, 0.0f,
	   		   -0.9f, -0.9f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -0.9f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f, -0.9f, -0.9f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	   		   -1.0f, -0.9f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f
	   };

	   //draw the cube
	   GLfloat verticesCube[] = {
			   //position					//color
			   //top face
			    0.25f,  0.25f, -0.25f,		0.0f, 1.0f, 0.0f, //green
			   -0.25f,  0.25f, -0.25f,		0.0f, 1.0f, 0.0f,
			   -0.25f,  0.25f,  0.25f,		0.0f, 1.0f, 0.0f,
			    0.25f,  0.25f,  0.25f,		0.0f, 1.0f, 0.0f,

			   //bottom face
			    0.25f, -0.25f,  0.25f,		1.0f, 0.5f, 0.0f, //orange
			   -0.25f, -0.25f,  0.25f,		1.0f, 0.5f, 0.0f,
			   -0.25f, -0.25f, -0.25f,		1.0f, 0.5f, 0.0f,
			    0.25f, -0.25f, -0.25f,		1.0f, 0.5f, 0.0f,

			   //front face
			    0.25f,  0.25f,  0.25f,		1.0f, 0.0f, 0.0f, //red
			   -0.25f,  0.25f,  0.25f,		1.0f, 0.0f, 0.0f,
			   -0.25f, -0.25f,  0.25f,		1.0f, 0.0f, 0.0f,
			    0.25f, -0.25f,  0.25f,		1.0f, 0.0f, 0.0f,

			   //back face
			    0.25f, -0.25f, -0.25f,		1.0f, 1.0f, 0.0f, //yellow
			   -0.25f, -0.25f, -0.25f,		1.0f, 1.0f, 0.0f,
			   -0.25f,  0.25f, -0.25f,		1.0f, 1.0f, 0.0f,
			   0.25f,  0.25f, -0.25f,		1.0f, 1.0f, 0.0f,

			   //left face
			   -0.25f,  0.25f,  0.25f,		0.0f, 0.0f, 1.0f, //blue
			   -0.25f,  0.25f, -0.25f,		0.0f, 0.0f, 1.0f,
			   -0.25f, -0.25f, -0.25f,		0.0f, 0.0f, 1.0f,
			   -0.25f, -0.25f,  0.25f,		0.0f, 0.0f, 1.0f,

			   //right face
			   0.25f,  0.25f, -0.25f,		1.0f, 0.0f, 1.0f, //pink
			   0.25f,  0.25f,  0.25f,		1.0f, 0.0f, 1.0f,
			   0.25f, -0.25f,  0.25f,		1.0f, 0.0f, 1.0f,
			   0.25f, -0.25f, -0.25f,		1.0f, 0.0f, 1.0f,
	   };

	//generate buffer ids for table
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//activate the VAO before binding and setting any VBOs
	glBindVertexArray(VAO);

	//activate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTable), verticesTable, GL_STATIC_DRAW); //copy vertices to VBO

	//set attribute pointer 0 to hold position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); //enables vertex attribute

	//set attribute pointer 1 to hold color data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	//set attribute pointer 2 to hold texture coordinate data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); //deactivates the VAO

	//activate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW); //copy vertices to VBO

	//set attribute pointer 0 to hold position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); //enables vertex attribute

	//set attribute pointer 1 to hold color data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1); //enables vertex attribute

	glBindVertexArray(0); //deactivates the VAO
}

void UGenerateTexture()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height;

	unsigned char* image = SOIL_load_image("texture.JPG", &width, &height, 0, SOIL_LOAD_RGB); //loads texture file
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); //unbind the texture
}
