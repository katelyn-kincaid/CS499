//============================================================================
// Name        : Final.cpp
// Author      : Katelyn Kincaid
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

// header inclusions
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

//glm math header inclusions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//SOIL image loader inclusion
#include "SOIL2/SOIL2.h"

using namespace std; //standard namespace

#define WINDOW_TITLE "Final Project" //window title macro

//shader program macro
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif

//variable declaration for shader, window size initialization, buffer and array objects
GLint tableShaderProgram, lampShaderProgram, WindowWidth = 800, WindowHeight = 600;
GLuint VBO, TableVAO, LightVAO, texture;

//subject (table) position and scale
glm::vec3 tablePosition(0.0f, 0.0f, 0.0f); //makes the table the center
glm::vec3 tableScale(2.0f);

//table and light color
glm::vec3 objectColor(0.0f, 0.0f, 0.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

//light position and scale
glm::vec3 lightPosition(0.5f, 2.0f, -2.0f);
glm::vec3 lightScale(0.5f);

GLfloat cameraSpeed = 0.000f; //movement speed per frame

GLfloat lastMouseX = 400, lastMouseY = 300; //locks mouse cursor at the center of the screen
GLfloat mouseXOffset, mouseYOffset, yaw = 0.0f, pitch = 0.0f; //mouse offset, yaw, and pitch variables
GLfloat sensitivity = 0.005f; //used for mouse/camera rotation sensitivity
bool mouseDetected = true; //initially true when mouse movement is detected

//global vector declarations
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f); //initial camera position
glm::vec3 CameraUpY = glm::vec3(0.0f, 1.0f, 0.0f); //temporary y unit vector
glm::vec3 CameraForwardZ = glm::vec3(0.0f, 0.0f, -1.0f); //temporary z unit vector
glm::vec3 front; //temporary z unit vector for mouse

//function prototypes
void UResizeWindow(int, int);
void URenderGraphics(void);
void UCreateShader(void);
void UCreateBuffers(void);
void UMouseMove(int x, int y);
void UGenerateTexture(void);

//table vertex shader source code
const GLchar * tableVertexShaderSource = GLSL(330,
	layout (location = 0) in vec3 position; //vertex data from vertex attrib pointer 0
	layout (location = 1) in vec3 normal; //VAP position 1 for normals
	layout (location = 2) in vec2 textureCoordinate;

	out vec3 FragmentPos; //for outgoing colors/pixels
	out vec3 Normal; //for outgoing normals to fragment shader
	out vec2 mobileTextureCoordinate;

//global variables for the transform matrices
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

void main(){
		gl_Position = projection * view * model * vec4(position, 1.0f); //transforms vertices to clip coordinates
		FragmentPos = vec3(model * vec4(position, 1.0f)); //gets fragment/pixel position in world space only (exclude view and projection)
		Normal = mat3(transpose(inverse(model))) * normal; //get normal vectors in world space only and exclude normal translation properties
		mobileTextureCoordinate = vec2(textureCoordinate.x, 1 - textureCoordinate.y); //flips the texture horizontally
	}
);

//table fragment shader source code
const GLchar * tableFragmentShaderSource = GLSL(330,

		in vec3 FragmentPos; //for incoming fragment position
		in vec3 Normal; //for incoming normals
		in vec2 mobileTextureCoordinate;

		out vec4 tableColor; //for outgoing table color

		//uniform/global variables
		uniform vec3 lightColor;
		uniform vec3 lightPos;
		uniform vec3 viewPosition;

		uniform sampler2D uTexture; //useful when working with multiple textures

		void main(){

			//phong lighting model calculations to generate ambient, diffuse, and specular components

			//calculate ambient lighting
			float ambientStrength = 0.5; //set ambient or global lighting strength
			vec3 ambient = ambientStrength * lightColor; //generate ambient light color

			//calculate diffuse lighting
			vec3 norm = normalize(Normal); //normalize vectors to 1 unit
			vec3 lightDirection = normalize(lightPos - FragmentPos); //calculate distance (light direction) between light source and fragments/pixels on
			float impact = max(dot(norm, lightDirection), 0.0); //calculate diffuse impact by generating dot product of normal and light
			vec3 diffuse = impact * lightColor; //generate diffuse light color

			//calculate specular lighting
			float specularIntensity = 0.8f; //set specular light strength
			float highlightSize = 128.0f; //set specular highlight size
			vec3 viewDir = normalize(viewPosition - FragmentPos); //calculate view direction
			vec3 reflectDir = reflect(-lightDirection, norm); //calculate reflection vector

			//calculate specular component
			float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
			vec3 specular = specularIntensity * specularComponent * lightColor;

			//calculate phong result
			vec3 objectColor = texture(uTexture, mobileTextureCoordinate).xyz;
			vec3 phong = (ambient + diffuse) * objectColor + specular;
			tableColor = vec4(phong, 1.0f); //send lighting results to gpu

		}
);

//lamp shader source code
const GLchar * lampVertexShaderSource = GLSL(330,

		layout (location = 0) in vec3 position; //VAP position 0 for vertex position data

		//uniform/global variables
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		void main()
		{
			gl_Position = projection * view * model * vec4(position, 1.0f); //transforms vertices into clip coordinates
		}
);

//lamp fragment shader source code
const GLchar * lampFragmentShaderSource = GLSL(330,

		out vec4 color; //for outgoing lamp color

		void main()
		{
			color = vec4(1.0f); //alpha white
		}
);

//main program
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow(WINDOW_TITLE);

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

	glClearColor(0.0f, 0.5f, 1.0f, 1.0f); //sets the background color to sky blue

	glutDisplayFunc(URenderGraphics);

	glutPassiveMotionFunc(UMouseMove); //detects mouse movement

	glutMainLoop();

	//destroys buffer objects once used
	glDeleteVertexArrays(1, &TableVAO);
	glDeleteVertexArrays(1, &LightVAO);
	glDeleteBuffers(1, &VBO);

	return 0;

}

//resizes the window
void UResizeWindow(int w, int h)
{
	WindowWidth = w;
	WindowHeight = h;
	glViewport(0, 0, WindowWidth, WindowHeight);
}

//renders graphics
void URenderGraphics(void)
{

	glEnable(GL_DEPTH_TEST); //enable z-depth

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clears the screen

	CameraForwardZ = front; //replaces camera forward vector with radians normalized as a unit vector

	GLint modelLoc, viewLoc, projLoc, uTextureLoc, lightColorLoc, lightPositionLoc, viewPositionLoc;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	//use the table shader to activate the shader vertex array object for rendering and transforming
	glUseProgram(tableShaderProgram);
	glBindVertexArray(TableVAO);

	//transforms the table
	model = glm::translate(model, tablePosition);
	model = glm::scale(model, tableScale);

	//transforms the camera
	view = glm::lookAt(CameraForwardZ, cameraPosition, CameraUpY);

	//creates a perspective projection
	projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);

	//reference matrix uniforms from the table shader program
	modelLoc = glGetUniformLocation(tableShaderProgram, "model");
	viewLoc = glGetUniformLocation(tableShaderProgram, "view");
	projLoc = glGetUniformLocation(tableShaderProgram, "projection");

	//pass matrix data to the table shader program's matrix uniform
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//reference matrix uniforms from the table shader program for the additional uniforms
	uTextureLoc = glGetUniformLocation(tableShaderProgram, "uTexture");
	lightColorLoc = glGetUniformLocation(tableShaderProgram, "lightColor");
	lightPositionLoc = glGetUniformLocation(tableShaderProgram, "lightPos");
	viewPositionLoc = glGetUniformLocation(tableShaderProgram, "viewPosition");

	//pass color, light, and camera data to the table shader program
	glUniform1i(uTextureLoc, 0); //texture unit 0
	glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
	glUniform3f(lightPositionLoc, lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, 156); //draw the table

	glBindVertexArray(0); //deactivates the pyramid vertex array object

	//use the lamp shader and activate the lamp vertex array object for rendering and transforming
	glUseProgram(lampShaderProgram);
	glBindVertexArray(LightVAO);

	//transform the smaller object used as a visual cue for the light source
	model = glm::translate(model, lightPosition);
	model = glm::scale(model, lightScale);

	//reference matrix uniforms from the lamp shader program
	modelLoc = glGetUniformLocation(lampShaderProgram, "model");
	viewLoc = glGetUniformLocation(lampShaderProgram, "view");
	projLoc = glGetUniformLocation(lampShaderProgram, "projection");

	//pass matrix uniforms from the lamp shader program
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//draws the triangles
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glutPostRedisplay();
	glDrawArrays(GL_TRIANGLES, 0, 156);

	glBindVertexArray(0); //deactivates the lamp vertex array object

	glutSwapBuffers(); //flips the back buffer with the front buffer every frame

}

//creates the shader program
void UCreateShader()
{

	//table vertex shader
	GLint tableVertexShader = glCreateShader(GL_VERTEX_SHADER); //creates the vertex shader
	glShaderSource(tableVertexShader, 1, &tableVertexShaderSource, NULL); //attaches the vertex shader to the source code
	glCompileShader(tableVertexShader); //compiles the vertex shader

	//table fragment shader
	GLint tableFragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //creates the fragment shader
	glShaderSource(tableFragmentShader, 1, &tableFragmentShaderSource, NULL); //attaches the fragment shader to source code
	glCompileShader(tableFragmentShader); //compiles the fragment shader

	// table shader program
	tableShaderProgram = glCreateProgram(); //creates the shader program and returns an id
	glAttachShader(tableShaderProgram, tableVertexShader); //attach vertex shader to the shader program
	glAttachShader(tableShaderProgram, tableFragmentShader);; //attach fragment shader to the shader program
	glLinkProgram(tableShaderProgram); //link vertex and fragment shaders to shader program

	//delete the vertex and fragment shaders once linked
	glDeleteShader(tableVertexShader);
	glDeleteShader(tableFragmentShader);

	//lamp vertex shader
	GLint lampVertexShader = glCreateShader(GL_VERTEX_SHADER); //creates the lamp vertex shader
	glShaderSource(lampVertexShader, 1, &lampVertexShaderSource, NULL); //attaches the lamp vertex shader to the source code
	glCompileShader(lampVertexShader); //compiles the lamp vertex shader

	//lamp fragment shader
	GLint lampFragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //creates the fragment shader
	glShaderSource(lampFragmentShader, 1, &lampFragmentShaderSource, NULL); //attaches the fragment shader to source code
	glCompileShader(lampFragmentShader); //compiles the lamp fragment shader

	//lamp shader program
	lampShaderProgram = glCreateProgram(); //creates the lamp shader program and returns an id
	glAttachShader(lampShaderProgram, lampVertexShader); //attaches lamp vertex shader to the shader program
	glAttachShader(lampShaderProgram, lampFragmentShader);; //attaches lamp fragment shader to the shader program
	glLinkProgram(lampShaderProgram); //links vertex and fragment shaders to shader program

	//delete the lamp shaders once linked
	glDeleteShader(lampVertexShader);
	glDeleteShader(lampFragmentShader);

}
//creates the buffer and array objects
void UCreateBuffers()
{
	GLfloat vertices[] = {
								//positions          	normals					texture coordinates

								//table top				positive y normals
								-1.0f,  1.0f, -1.0f, 	 0.0f,  1.0f,  0.0f, 	0.0f, 1.0f,
								 1.0f,  1.0f, -1.0f, 	 0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
								 1.0f,  1.0f,  1.0f, 	 0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
								 1.0f,  1.0f,  1.0f, 	 0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
								-1.0f,  1.0f,  1.0f, 	 0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
								-1.0f,  1.0f, -1.0f, 	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f,

								//bottom of table top	negative y normals
								-1.0f,  0.9f, -1.0f, 	 0.0f, -1.0f,  0.0f,	0.0f, 1.0f,
								 1.0f,  0.9f, -1.0f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								 1.0f,  0.9f,  1.0f, 	 0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
								 1.0f,  0.9f,  1.0f, 	 0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
								-1.0f,  0.9f,  1.0f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								-1.0f,  0.9f, -1.0f, 	 0.0f, -1.0f,  0.0f,	1.0f, 0.0f,

								//back facing edge		negative z normals
								-1.0f,  0.8f, -1.0f, 	 0.0f,  0.0f, -1.0f,	0.0f, 1.0f,
								 1.0f,  0.8f, -1.0f, 	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
								 1.0f,  1.0f, -1.0f, 	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
								 1.0f,  1.0f, -1.0f, 	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
								-1.0f,  1.0f, -1.0f, 	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
								-1.0f,  0.8f, -1.0f, 	 0.0f,  0.0f, -1.0f,	1.0f, 0.0f,

								//front facing edge		positive z normals
								-1.0f,  0.8f,  1.0f, 	 0.0f,  0.0f,  1.0f,	0.0f, 1.0f,
								 1.0f,  0.8f,  1.0f, 	 0.0f,  0.0f,  1.0f,	0.0f, 0.0f,
								 1.0f,  1.0f,  1.0f, 	 0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
								 1.0f,  1.0f,  1.0f, 	 0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
								-1.0f,  1.0f,  1.0f, 	 0.0f,  0.0f,  1.0f,	0.0f, 0.0f,
								-1.0f,  0.8f,  1.0f, 	 0.0f,  0.0f,  1.0f,	1.0f, 0.0f,

								//left facing edge		negative x normals
								-1.0f,  0.8f, -1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
								-1.0f,  0.8f,  1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-1.0f,  1.0f,  1.0f, 	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
								-1.0f,  1.0f,  1.0f, 	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
								-1.0f,  1.0f, -1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-1.0f,  0.8f, -1.0f, 	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

								//right facing edge		positive x normals
								 1.0f,  0.8f, -1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
								 1.0f,  0.8f,  1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 1.0f,  1.0f,  1.0f, 	 1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
								 1.0f,  1.0f,  1.0f, 	 1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
								 1.0f,  1.0f, -1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 1.0f,  0.8f, -1.0f, 	 1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

								//front left table leg
								//parallel pair A.1		negative x normals
								-1.0f,  0.8f,  1.0f, 	-1.0f,  0.0f,  0.0f, 	0.0f, 0.0f,
								-0.9f,  0.8f,  1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-0.9f, -0.9f,  1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-0.9f, -0.9f,  1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-1.0f, -0.9f,  1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-1.0f,  0.8f,  1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

								//parallel pair A.2		negative x normals
								-1.0f, -0.9f,  0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-0.9f, -0.9f,  0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-0.9f,  0.8f,  0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-0.9f,  0.8f,  0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-1.0f,  0.8f,  0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-1.0f, -0.9f,  0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

								//parallel pair B.1		negative x normals
								-1.0f,  0.8f,  1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-1.0f,  0.8f,  0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-1.0f, -0.9f,  0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-1.0f, -0.9f,  0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-1.0f, -0.9f,  1.0f, 	-1.0f,  0.0f,  0.0f, 	0.0f, 0.0f,
								-1.0f,  0.8f,  1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

								//parallel pair B.2		negative x normals
								-0.9f,  0.8f,  1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-0.9f,  0.8f,  0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-0.9f, -0.9f,  0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-0.9f, -0.9f,  0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-0.9f, -0.9f,  1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-0.9f,  0.8f,  1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

								//bottom front left leg	negative y normals
								-1.0f, -0.9f,  1.0f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								-0.9f, -0.9f,  1.0f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								-0.9f, -0.9f,  0.9f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								-0.9f, -0.9f,  0.9f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								-1.0f, -0.9f,  0.9f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								-1.0f, -0.9f,  1.0f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,

								//front right table leg
								//parallel pair A.1		positive x normals
								 1.0f,  0.8f,  1.0f, 	 1.0f,  0.0f,  0.0f, 	0.0f, 0.0f,
								 0.9f,  0.8f,  1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f,  1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f,  1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 1.0f, -0.9f,  1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 1.0f,  0.8f,  1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

								//parallel pair A.2		positive x normals
								 1.0f,  0.8f,  0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 0.9f,  0.8f,  0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f,  0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f,  0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 1.0f, -0.9f,  0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 1.0f,  0.8f,  0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

								//parallel pair B.1		positive x normals
								 1.0f,  0.8f,  1.0f, 	 1.0f,  0.0f,  0.0f, 	0.0f, 0.0f,
								 1.0f,  0.8f,  0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 1.0f, -0.9f,  0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 1.0f, -0.9f,  0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 1.0f, -0.9f,  1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 1.0f,  0.8f,  1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

								//parallel pair B.2		positive x normals
								 0.9f,  0.8f,  1.0f, 	 1.0f,  0.0f,  0.0f, 	0.0f, 0.0f,
								 0.9f,  0.8f,  0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f,  0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f,  0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f,  1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 0.9f,  0.8f,  1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

								//bottom front right leg negative y normals
								 1.0f, -0.9f,  1.0f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f,  1.0f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f,  0.9f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f,  0.9f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								 1.0f, -0.9f,  0.9f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								 1.0f, -0.9f,  1.0f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,

								//right back leg
								//parallel pair A.1		positive x normals
								 1.0f,  0.8f, -1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 1.0f,  0.8f, -0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 1.0f, -0.9f, -0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 1.0f, -0.9f, -0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 1.0f, -0.9f, -1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 1.0f,  0.8f, -1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

								//parallel pair A.2		positive x normals
								 0.9f,  0.8f, -1.0f, 	 1.0f,  0.0f,  0.0f, 	0.0f, 0.0f,
								 0.9f,  0.8f, -0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f, -0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f, -0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f, -1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 0.9f,  0.8f, -1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

								//parallel pair B.1		positive x normals
								 1.0f,  0.8f, -0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 0.9f,  0.8f, -0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f, -0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f, -0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 1.0f, -0.9f, -0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 1.0f,  0.8f, -0.9f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

								//parallel pair B.2		positive x normals
								 1.0f,  0.8f, -1.0f, 	 1.0f,  0.0f,  0.0f, 	0.0f, 0.0f,
								 0.9f,  0.8f, -1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f, -1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f, -1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 1.0f, -0.9f, -1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								 1.0f,  0.8f, -1.0f, 	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

								//bottom right back leg negative y normals
								 1.0f, -0.9f, -1.0f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f, -1.0f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f, -0.9f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								 0.9f, -0.9f, -0.9f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								 1.0f, -0.9f, -0.9f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								 1.0f, -0.9f, -1.0f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,

								//back left leg
								//parallel pair A.1		negative x normals
								-1.0f,  0.8f, -1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-1.0f,  0.8f, -0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-1.0f, -0.9f, -0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-1.0f, -0.9f, -0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-1.0f, -0.9f, -1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-1.0f,  0.8f, -1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

								//parallel pair A.2		negative x normals
								-0.9f,  0.8f, -1.0f, 	-1.0f,  0.0f,  0.0f, 	0.0f, 0.0f,
								-0.9f,  0.8f, -0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-0.9f, -0.9f, -0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-0.9f, -0.9f, -0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-0.9f, -0.9f, -1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-0.9f,  0.8f, -1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

								//parallel pair B.1		negative x normals
								-1.0f,  0.8f, -0.9f, 	-1.0f,  0.0f,  0.0f, 	0.0f, 0.0f,
								-0.9f,  0.8f, -0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-0.9f, -0.9f, -0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-0.9f, -0.9f, -0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-1.0f, -0.9f, -0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-1.0f,  0.8f, -0.9f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

								//parallel pair B.2		negative x normals
								-1.0f,  0.8f, -1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-0.9f,  0.8f, -1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-0.9f, -0.9f, -1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-0.9f, -0.9f, -1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-1.0f, -0.9f, -1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
								-1.0f,  0.8f, -1.0f, 	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

								//bottom back left leg	negative y normals
								-1.0f, -0.9f, -1.0f, 	 0.0f, -1.0f,  0.0f, 	0.0f, 0.0f,
								-0.9f, -0.9f, -1.0f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								-0.9f, -0.9f, -0.9f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								-0.9f, -0.9f, -0.9f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								-1.0f, -0.9f, -0.9f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
								-1.0f, -0.9f, -1.0f, 	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f

							};

	//generate buffer ids for table
	glGenVertexArrays(1, &TableVAO);
	glGenBuffers(1, &VBO);

	//activate the vertex array object before binding and setting any VBOs and vertex attribute pointers
	glBindVertexArray(TableVAO);

	//activate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //copy vertices to VBO

	//set attribute pointer 0 to hold position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); //enables vertex attribute

	//set attribute pointer 1 to hold normal data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1); //enables vertex attribute

	//set attribute pointer 2 to hold texture coordinate data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); //deactivates the table VAO which is good practice

	//generate buffer ids for the lamp
	glGenVertexArrays(1, &LightVAO); //vertex array for table vertex copies to serve as light source

	//activate the vertex array object before binding and setting any VBOs and vertex attribute pointers
	glBindVertexArray(LightVAO);

	//referencing the same VBO for its vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//set attribute pointer to 0 to hold position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

//implements the UMouseMove function
void UMouseMove(int x, int y)
{

	//immediately replaces center locked coordinates with new mouse coordinates
	if(mouseDetected)
	{
		lastMouseX = x;
		lastMouseY = y;
		mouseDetected = false;
	}

	//gets the direction the mouse was moved in x and y
	mouseXOffset = x - lastMouseX;
	mouseYOffset = lastMouseY - y; //inverted y

	//updates with the new mouse coordinates
	lastMouseX = x;
	lastMouseY = y;

	//applies sensitivity to mouse direction
	mouseXOffset *= sensitivity;
	mouseYOffset *= sensitivity;

	//accumulates the yaw and pitch variables
	yaw += mouseXOffset;
	pitch += mouseYOffset;

	//maintains a 90 degree pitch for global lock
	if(pitch > 89.0)
		pitch = 89.0f;

	if(pitch < -89.0f)
		pitch = -89.0f;

	//orbits around the center
	front.x = 10.0f * cos(yaw);
	front.y = 10.0f * sin(pitch);
	front.z = sin(yaw) * cos(pitch) * 10.f;
}

//generate the load and texture
void UGenerateTexture(){

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height;

	unsigned char* image = SOIL_load_image("texture.jpg", &width, &height, 0, SOIL_LOAD_RGB); //loads texture file

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); //unbind the texture
}
