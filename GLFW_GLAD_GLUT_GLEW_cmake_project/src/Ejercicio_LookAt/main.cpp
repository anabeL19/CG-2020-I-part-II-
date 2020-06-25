#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <glfw/glfw3.h>

// Usage of degrees is deprecated. Use radians for glm functions.
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shaders.h"


int mouseoldx, mouseoldy ; // For mouse motion
float eyeloc = 2.0 ; // Where to look from; initially 0 -2, 2
float up = 1.0 ; // Where to look from; initially 0 -2, 2
GLuint vertexshader, fragmentshader, shaderprogram ; // shaders
GLuint vertexshader1, fragmentshader1, shaderprogram1 ; // shaders
GLuint vertexshader2, fragmentshader2, shaderprogram2 ; // shaders
GLuint projectionPos, modelviewPos; // Locations of uniform variables
glm::mat4 projection, modelview; // The mvp matrices themselves

// camera
glm::vec3 cameraPos   = glm::vec3(0, -eyeloc, eyeloc);
// glm::vec3 cameraPos   = glm::vec3(0, -2.0, 2.0);
glm::vec3 cameraFront = glm::vec3(0.0f, eyeloc, -eyeloc);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  1.0f);
int i = 0;
float red_value , green_value, blue_value;

// timing
float deltaTime = 0.0f;	
float lastFrame = 0.0f;
/***************   BASIC SET UP FOR DRAWING OBJECTS ***********************/

// const int numobjects = 2 ; // number of objects for buffer 
const int numobjects = 20 ; // number of objects for buffer 
const int numperobj  = 3 ; // Vertices, colors, indices
GLuint VAOs[numobjects]; // A VAO for each object

GLuint buffers[numperobj*numobjects] ; // List of buffers for geometric data

GLuint objects[numobjects] ; // For each object
GLenum PrimType[numobjects] ; // Primitive Type (quads, polygons etc.)
GLsizei NumElems[numobjects] ; // Number of geometric elements

// Floor Geometry is specified with a vertex array

enum {Vertices, Colors, Elements} ; // For arrays for object 
enum {FLOOR, FLOOR2, FRONT_CUBE, BACK_CUBE, RIGHT_CUBE, LEFT_CUBE, 
      BASE_PYRAMID, FRONT_PYRAMID, BACK_PYRAMID, RIGHT_PYRAMID, LEFT_PYRAMID,
      BASE_DIAMOND, FRONT_DIAMOND, BACK_DIAMOND, RIGHT_DIAMOND, LEFT_DIAMOND, FRONT_DIAMOND_DOWN, BACK_DIAMOND_DOWN, RIGHT_DIAMOND_DOWN, LEFT_DIAMOND_DOWN} ;
// enum {BASE_PYRAMID, FRONT_PYRAMID, BACK_PYRAMID, RIGHT_PYRAMID, LEFT_PYRAMID} ; 
// enum {BASE_DIAMOND, FRONT_DIAMOND, BACK_DIAMOND, RIGHT_DIAMOND, LEFT_DIAMOND, FRONT_DIAMOND_DOWN, BACK_DIAMOND_DOWN, RIGHT_DIAMOND_DOWN, LEFT_DIAMOND_DOWN} ; 

//--------------------------------    CUBE    -----------------------------------
// FIRST
const GLfloat floorverts[4][3] = {
  {0.4, 0.13, 0.0}, {0.1, 0.13, 0.0}, {0.1, -0.13, 0.0}, {0.4,-0.13, 0.0}
} ; 
const GLfloat floorcol[4][3] = {
  {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 1.0}
} ; 
const GLubyte floorinds[1][6] = { {0, 1, 2, 0, 2, 3} } ; 

// SECOND
const GLfloat floorverts2[4][3] = {
  {0.4, 0.13, 0.3}, {0.1, 0.13, 0.3}, {0.1, -0.13, 0.3}, {0.4, -0.13, 0.3}
} ; 
const GLfloat floorcol2[4][3] = {
  {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}
} ; 
const GLubyte floorinds2[1][6] = { { 0, 1, 2, 0, 2, 3 } } ;

// FRONT
const GLfloat front_cube_verts[4][3] = {
  {0.1, 0.13, 0.0}, {0.1, 0.13, 0.3}, {0.1, -0.13, 0.3}, {0.1, -0.13, 0.0}
} ; 
const GLfloat front_cube_col[4][3] = {
  {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}
} ; 
const GLubyte front_cube_inds[1][6] = { { 0, 1, 2, 0, 2, 3 } } ;

// BACK
const GLfloat back_cube_verts[4][3] = {
  {0.4, 0.13, 0.0}, {0.4, 0.13, 0.3}, {0.4, -0.13, 0.3}, {0.4, -0.13, 0.0}
} ; 
const GLfloat back_cube_col[4][3] = {
  {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}
} ; 
const GLubyte back_cube_inds[1][6] = { { 0, 1, 2, 0, 2, 3 } } ;

// RIGHT
const GLfloat right_cube_verts[4][3] = {
  {0.4, -0.13, 0.3}, {0.1, -0.13, 0.3}, {0.1, -0.13, 0.0}, {0.4, -0.13, 0.0}
} ; 
const GLfloat right_cube_col[4][3] = {
  {0.0, 0.0, 0.3}, {0.0, 0.0, 0.3}, {0.0, 0.0, 0.2}, {0.0, 0.0, 0.2}
} ; 
const GLubyte right_cube_inds[1][6] = { { 0, 1, 2, 0, 2, 3 } } ;

// LEFT
const GLfloat left_cube_verts[4][3] = {
  {0.4, 0.13, 0.3}, {0.1, 0.13, 0.3}, {0.1, 0.13, 0.0}, {0.4, 0.13, 0.0}
} ; 
const GLfloat left_cube_col[4][3] = {
  {0.0, 0.3, 0.0}, {0.0, 0.3, 0.0}, {0.0, 0.2, 0.0}, {0.0, 0.2, 0.0}
} ; 
const GLubyte left_cube_inds[1][6] = { { 0, 1, 2, 0, 2, 3 } } ;

//--------------------------------    PYRAMID   -----------------------------
// BASE
const GLfloat base_pyramid_verts[4][3] = {
  {0.0, 0.13, 0.0}, {-0.3, -0.13, 0.0}, {-0.3, -0.1, 0.0}, {0.0,-0.13, 0.0}
} ; 
const GLfloat base_pyramid_col[4][3] = {
  {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 1.0}
} ; 
const GLubyte base_pyramid_inds[1][6] = { {0, 1, 2, 0, 2, 3} } ; 

// FRONT
const GLfloat front_pyramid_verts[3][3] = {
  {0.0, -0.13, 0.0}, {-0.3, -0.13, 0.0}, {-0.13, 0.0, 0.5}
} ; 
const GLfloat front_pyramid_col[4][3] = {
  {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}
} ; 
const GLubyte front_pyramid_inds[1][3] = { {0, 1, 2} } ; 

// BACK
const GLfloat back_pyramid_verts[3][3] = {
  {0.0, 0.13, 0.0}, {-0.3, 0.13, 0.0}, {-0.13, 0.0, 0.5}
} ; 
const GLfloat back_pyramid_col[4][3] = {
  {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}
} ; 
const GLubyte back_pyramid_inds[1][3] = { {0, 1, 2} } ; 

// RIGHT
const GLfloat right_pyramid_verts[3][3] = {
  {0.0, -0.13, 0.0}, {0.0, 0.13, 0.0}, {-0.13, 0.0, 0.5}
} ; 
const GLfloat right_pyramid_col[4][3] = {
  {0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}
} ; 
const GLubyte right_pyramid_inds[1][3] = { {0, 1, 2} } ; 

// LEFT
const GLfloat left_pyramid_verts[3][3] = {
  {-0.3, -0.13, 0.0}, {-0.3, 0.13, 0.0}, {-0.13, 0.0, 0.5}
} ; 
const GLfloat left_pyramid_col[4][3] = {
  {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}
} ; 
const GLubyte left_pyramid_inds[1][3] = { {0, 1, 2} } ; 

// //--------------------------------    DIAMOND   -----------------------------
// BASE
const GLfloat base_diamond_verts[4][3] = {
  {-0.4, 0.13, 0.3}, {-0.7, 0.13, 0.3}, {-0.7, -0.13, 0.3}, {-0.4,-0.13, 0.3}
} ; 
const GLfloat base_diamond_col[4][3] = {
  {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 1.0}
} ; 
const GLubyte base_diamond_inds[1][6] = { {0, 1, 2, 0, 2, 3} } ; 

// FRONT
const GLfloat front_diamond_verts[3][3] = {
  {-0.7, 0.13, 0.3}, {-0.7, -0.13, 0.3}, {-0.53, 0.0, 0.5}
} ; 
const GLfloat front_diamond_col[4][3] = {
  {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}
} ; 
const GLubyte front_diamond_inds[1][3] = { {0, 1, 2} } ; 

// BACK
const GLfloat back_diamond_verts[3][3] = {
  {-0.4, 0.13, 0.3}, {-0.4, -0.13, 0.3}, {-0.53, 0.0, 0.5}
} ; 
const GLfloat back_diamond_col[4][3] = {
  {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}
} ; 
const GLubyte back_diamond_inds[1][3] = { {0, 1, 2} } ; 

// RIGHT
const GLfloat right_diamond_verts[3][3] = {
  {-0.4, -0.13, 0.3}, {-0.7, -0.13, 0.3}, {-0.53, 0.0, 0.5}
} ; 
const GLfloat right_diamond_col[4][3] = {
  {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}
} ; 
const GLubyte right_diamond_inds[1][3] = { {0, 1, 2} } ; 

// LEFT
const GLfloat left_diamond_verts[3][3] = {
  {-0.4, 0.13, 0.3}, {-0.7, 0.13, 0.3}, {-0.53, 0.0, 0.5}
} ; 
const GLfloat left_diamond_col[4][3] = {
  {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}
} ; 
const GLubyte left_diamond_inds[1][3] = { {0, 1, 2} } ; 

// FRONT DOWN 
const GLfloat front_diamond_down_verts[3][3] = {
  {-0.7, 0.13, 0.3}, {-0.7, -0.13, 0.3}, {-0.53, 0.0, 0.0}
} ; 
const GLfloat front_diamond_down_col[4][3] = {
  {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}
} ; 
const GLubyte front_diamond_down_inds[1][3] = { {0, 1, 2} } ; 

// BACK DOWN
const GLfloat back_diamond_down_verts[3][3] = {
  {-0.4, 0.13, 0.3}, {-0.4, -0.13, 0.3}, {-0.53, 0.0, 0.0}
} ; 
const GLfloat back_diamond_down_col[4][3] = {
  {0.7, 0.0, 0.0}, {0.7, 0.0, 0.0}, {0.3, 0.0, 0.0}, {0.3, 0.0, 0.0}
} ; 
const GLubyte back_diamond_down_inds[1][3] = { {0, 1, 2} } ; 

// RIGHT DOWN
const GLfloat right_diamond_down_verts[3][3] = {
  {-0.4, -0.13, 0.3}, {-0.7, -0.13, 0.3}, {-0.53, 0.0, 0.0}
} ; 
const GLfloat right_diamond_down_col[4][3] = {
  {0.0, 0.1, 0.0}, {0.0, 0.1, 0.0}, {0.0, 1.0, 1.0}, {0.0, 0.9, 1.0}
} ; 
const GLubyte right_diamond_down_inds[1][3] = { {0, 1, 2} } ; 

// LEFT DOWN
const GLfloat left_diamond_down_verts[3][3] = {
  {-0.4, 0.13, 0.3}, {-0.7, 0.13, 0.3}, {-0.53, 0.0, 0.0}
} ; 
const GLfloat left_diamond_down_col[4][3] = {
  {1.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.2, 0.0, 0.0}, {1.0, 0.0, 0.0}
} ; 
const GLubyte left_diamond_down_inds[1][3] = { {0, 1, 2} } ; 

// Treat this as a destructor function. Delete any dynamically allocated memory here
void deleteBuffers() 
{
	glDeleteVertexArrays(numobjects, VAOs);
	glDeleteBuffers(numperobj*numobjects, buffers);
}

// This function takes in a vertex, color, index and type array 
// And does the initialization for an object.  
// The partner function below it draws the object 
void initobject(GLuint object, GLfloat * vert, GLint sizevert, GLfloat * col, GLint sizecol, GLubyte * inds, GLint sizeind, GLenum type)
{
  int offset = object * numperobj;

  glUseProgram(shaderprogram);
  glBindVertexArray(VAOs[object]);

  int vertexColorLocation = glGetUniformLocation(shaderprogram, "ourColorT");
  glUniform4f(vertexColorLocation, red_value, green_value, blue_value, 1.0f);

  glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices+offset]) ; 
  glBufferData(GL_ARRAY_BUFFER, sizevert, vert,GL_STATIC_DRAW);
  // Use layout location 0 for the vertices
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[Colors+offset]) ; 
  glBufferData(GL_ARRAY_BUFFER, sizecol, col,GL_STATIC_DRAW);
  // Use layout location 1 for the colors
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Elements+offset]) ; 
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeind, inds,GL_STATIC_DRAW);
  PrimType[object] = type;
  NumElems[object] = sizeind;
  // Prevent further modification of this VAO by unbinding it
  glBindVertexArray(0);
}


void drawobject(GLuint object) 
{
  glBindVertexArray(VAOs[object]);
  glDrawElements(PrimType[object], NumElems[object], GL_UNSIGNED_BYTE, 0); 
  glBindVertexArray(0);
}

/****************   BASIC SETUP FOR DRAWING OBJECTS ***********************/
void display(GLFWwindow* window)
{
  /* Clear all pixels  */
  glClear (GL_COLOR_BUFFER_BIT);

  // draw polygon (square) of unit length centered at the origin
  // Note that vertices must generally go counterclockwise
  // This code draws each vertex in a different color.  
  // The hardware will blend between them.  
  // This is a useful debugging trick.  I can make sure that each vertex 
  // appears exactly where I expect it to appear.

  // The old OpenGL code of using glBegin... glEnd no longer appears. 
  // The new version uses vertex array and vertex buffer objects from init.  
  // CUBE 
  drawobject(FLOOR);
  drawobject(FLOOR2) ; 
  drawobject(FRONT_CUBE) ; 
  drawobject(BACK_CUBE) ; 
  drawobject(RIGHT_CUBE) ; 
  drawobject(LEFT_CUBE) ; 

  // PYRAMID
  drawobject(BASE_PYRAMID);
  drawobject(FRONT_PYRAMID) ; 
  drawobject(BACK_PYRAMID) ; 
  drawobject(RIGHT_PYRAMID) ; 
  drawobject(LEFT_PYRAMID) ; 

  // DIAMOND 
  drawobject(BASE_DIAMOND);
  drawobject(FRONT_DIAMOND) ; 
  drawobject(BACK_DIAMOND) ; 
  drawobject(RIGHT_DIAMOND) ; 
  drawobject(LEFT_DIAMOND) ; 
  drawobject(FRONT_DIAMOND_DOWN) ; 
  drawobject(BACK_DIAMOND_DOWN) ; 
  drawobject(RIGHT_DIAMOND_DOWN) ; 
  drawobject(LEFT_DIAMOND_DOWN) ; 
}

/* Defines a Mouse callback to zoom in and out */
/* This is done by modifying gluLookAt         */
/* The actual motion is in mousedrag           */
/* mouse simply sets state for mousedrag       */

/* Defines what to do when various mouse */
void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_LEFT)
  {
    if (action == GLFW_RELEASE) {
      // Do Nothing ;
    }
    else if (action == GLFW_PRESS) {
      double x, y;
      glfwGetCursorPos(window, &x, &y);

      mouseoldx = (int)x ; 
	    mouseoldy = (int)y ; // so we can move wrt x , y 
    }
  }
  else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS )
  { 
    
	  // // Reset gluLookAt
    eyeloc = 2.0 ;
    cameraPos   = glm::vec3(0, -eyeloc, eyeloc);
	  modelview = glm::lookAt(
      cameraPos, // eye
      // glm::vec3(0, eyeloc, eyeloc), // eye
      cameraFront + cameraPos, // center/target
      cameraUp); // up
      // glm::vec3(0, 1, -1)); // up
	  // Send the updated matrix to the shader
	  glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
  }
}

/* Defines what to do when various mousedrag */
void mousedrag_callback(GLFWwindow* window, double x, double y)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		int yloc = (int)y - mouseoldy;	// We will use the y coord to zoom in/out

		eyeloc += 0.007f*yloc;		// Where do we look from

		if (eyeloc < 0)
		  eyeloc = 0.0;

		mouseoldy = (int)y;
    cameraPos   = glm::vec3(0, -eyeloc, eyeloc);
		// Set the eye location 
		modelview = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
    // std::cout<<"cameraPos: "<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<"\n";
    // std::cout<<"cameraFront: "<<(cameraFront + cameraPos).x<<" "<<(cameraFront + cameraPos).y<<" "<<(cameraFront + cameraPos).z<<"\n";
    // std::cout<<"cameraUp: "<<cameraUp.x<<" "<<cameraUp.y<<" "<<cameraUp.z<<"\n";
		
    // modelview = glm::lookAt(glm::vec3(0, eyeloc, eyeloc), glm::vec3(0, 0, 0), glm::vec3(0, 1, -1));

		// Send the updated matrix over to the shader
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
	}
}

/* Defines what to do when various keys are pressed */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
  {
	  deleteBuffers();
	  glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		eyeloc += 0.005f;		// Where do we look from

		if (eyeloc < 0)
		  eyeloc = 0.0;
    cameraPos   = glm::vec3(0, -eyeloc, eyeloc);
		// Set the eye location 
		modelview = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
    // std::cout<<"cameraPos: "<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<"\n";
    // std::cout<<"cameraFront: "<<(cameraFront + cameraPos).x<<" "<<(cameraFront + cameraPos).y<<" "<<(cameraFront + cameraPos).z<<"\n";
    // std::cout<<"cameraUp: "<<cameraUp.x<<" "<<cameraUp.y<<" "<<cameraUp.z<<"\n";
    // modelview = glm::lookAt(glm::vec3(0, eyeloc, eyeloc), glm::vec3(0, 0, 0), glm::vec3(0, 1, -1));

		// Send the updated matrix over to the shader
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
  }
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		// // Reset gluLookAt
    eyeloc = 2.0 ;
    cameraPos   = glm::vec3(0, -eyeloc, eyeloc);
	  modelview = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp); 
      // glm::vec3(0, 1, -1)); // up
	  // Send the updated matrix to the shader
	  glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
  }
  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		eyeloc -= 0.005f;		// Where do we look from

		if (eyeloc < 0)
		  eyeloc = 0.0;
    cameraPos   = glm::vec3(0, -eyeloc, eyeloc);
		// Set the eye location 
		modelview = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);

		// Send the updated matrix over to the shader
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
  }
  float cameraSpeed = 2.5 * deltaTime; 
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp));
    modelview = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
    
    glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
    // std::cout<<"cameraPos: "<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<"\n";
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
    cameraUp    = glm::vec3(0.0f, up,  up);
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp));
    modelview = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);

    glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
    // std::cout<<"cameraPos: "<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<"\n";
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    modelview = glm::translate(modelview, glm::vec3(0.0f, 0.0f, 0.5f));
    // modelview = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
		// // Send the updated matrix over to the shader
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    modelview = glm::translate(modelview, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
  }
  if (i == 0 && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
    eyeloc -= 0.010f;		// Where do we look from
		if (eyeloc < 0)
		  eyeloc = 0.0;
    float radius = 10.0f;
    up = 1.0f;
    // float eyelocX   = sin(glfwGetTime()) * radius;
    // float eyelocZ   = cos(glfwGetTime()) * radius;
    cameraPos   = glm::vec3(eyeloc, 0, 0);
    cameraFront   = glm::vec3(-eyeloc, 0, 0);
    cameraUp   = glm::vec3(0, up, up);

		// Set the eye location 
		modelview = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
    // std::cout<<"cameraPos: "<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<"\n";
    // std::cout<<"cameraFront: "<<(cameraFront + cameraPos).x<<" "<<(cameraFront + cameraPos).y<<" "<<(cameraFront + cameraPos).z<<"\n";
    // std::cout<<"cameraUp: "<<cameraUp.x<<" "<<cameraUp.y<<" "<<cameraUp.z<<"\n";

		// // Send the updated matrix over to the shader
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
    i=1;
  }
  if (i == 1 && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
    eyeloc -= 0.010f;		// Where do we look from
		if (eyeloc < 0)
		  eyeloc = 0.0;
    float radius = 10.0f;
    up = -1.0f;
    // float eyelocX   = sin(glfwGetTime()) * radius;
    // float eyelocZ   = cos(glfwGetTime()) * radius;
    cameraPos   = glm::vec3(eyeloc, 0, 0);
    cameraFront   = glm::vec3(-eyeloc, 0, 0);
    cameraUp   = glm::vec3(0, up, up);

		// Set the eye location 
		modelview = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
    // std::cout<<"cameraPos2: "<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<"\n";
    // std::cout<<"cameraFront2: "<<(cameraFront + cameraPos).x<<" "<<(cameraFront + cameraPos).y<<" "<<(cameraFront + cameraPos).z<<"\n";
    // std::cout<<"cameraUp2: "<<cameraUp.x<<" "<<cameraUp.y<<" "<<cameraUp.z<<"\n";

		// // Send the updated matrix over to the shader
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
    i=0;
  }
  if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS){
    eyeloc -= 0.010f;		// Where do we look from
		if (eyeloc < 0)
		  eyeloc = 0.0;
    
    cameraPos   = glm::vec3(0, eyeloc, 0);
    cameraFront   = glm::vec3(0, -eyeloc, 0);
    // cameraUp   = glm::vec3(0, up, up);

		// Set the eye location 
		modelview = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
    // std::cout<<"cameraPos: "<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<"\n";
    // std::cout<<"cameraFront: "<<(cameraFront + cameraPos).x<<" "<<(cameraFront + cameraPos).y<<" "<<(cameraFront + cameraPos).z<<"\n";
    // std::cout<<"cameraUp: "<<cameraUp.x<<" "<<cameraUp.y<<" "<<cameraUp.z<<"\n";

		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
  }
  if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
    eyeloc -= 0.010f;		// Where do we look from
		if (eyeloc < 0)
		  eyeloc = 0.0;

    cameraPos   = glm::vec3(0, 0, eyeloc);
    cameraFront   = glm::vec3(0, 0, -eyeloc);
    cameraUp   = glm::vec3(0, up, up);

		// Set the eye location 
		modelview = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
    // std::cout<<"cameraPos: "<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<"\n";
    // std::cout<<"cameraFront: "<<(cameraFront + cameraPos).x<<" "<<(cameraFront + cameraPos).y<<" "<<(cameraFront + cameraPos).z<<"\n";
    // std::cout<<"cameraUp: "<<cameraUp.x<<" "<<cameraUp.y<<" "<<cameraUp.z<<"\n";

		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
    float angle = 10.0f;
    modelview = glm::rotate(modelview, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    modelview = glm::translate(modelview, glm::vec3(0.0f, 0.0f, 0.05f));
		// Send the updated matrix over to the shader
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
  }
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
    float angle = 0.1f;
    modelview = glm::rotate(modelview, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
  }
  if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
    up = -1.0f;
    cameraUp    = glm::vec3(0.0f, up,  up);
    modelview = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);

    glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
    // std::cout<<"cameraPos: "<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<"\n";
  }
  if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
    float angle_x = 10.0f;
    modelview = glm::rotate(modelview, glm::radians(-angle_x), glm::vec3(1.0f, 0.0f, 0.0f));
    // // modelview = glm::translate(modelview, glm::vec3(0.0f, 0.0f, 0.0f));
		// // Send the updated matrix over to the shader
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
    // std::cout<<"cameraPos: "<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<"\n";
  }
  if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS){
    float angle_y = 10.0f;
    modelview = glm::rotate(modelview, glm::radians(-angle_y), glm::vec3(0.0f, 1.0f, 0.0f));
		// Send the updated matrix over to the shader
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
    // std::cout<<"cameraPos: "<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<"\n";
  }
  if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS){
    float angle_z = 10.0f;
    modelview = glm::rotate(modelview, glm::radians(-angle_z), glm::vec3(0.0f, 0.0f, 1.0f));
		// Send the updated matrix over to the shader
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
    // std::cout<<"cameraPos: "<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<"\n";
  }
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
    eyeloc = 2.0f;		// Where do we look from
    up = 1.0f;	
		if (eyeloc < 0)
		  eyeloc = 0.0;
    
    cameraPos   = glm::vec3(0, -eyeloc, eyeloc);
    cameraFront   = glm::vec3(0, eyeloc, -eyeloc);
    cameraUp   = glm::vec3(0, up, up);

		// Set the eye location 
		modelview = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
    // std::cout<<"cameraPos: "<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<"\n";
    // std::cout<<"cameraFront: "<<(cameraFront + cameraPos).x<<" "<<(cameraFront + cameraPos).y<<" "<<(cameraFront + cameraPos).z<<"\n";
    // std::cout<<"cameraUp: "<<cameraUp.x<<" "<<cameraUp.y<<" "<<cameraUp.z<<"\n";

		// // Send the updated matrix over to the shader
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
  }
  // color
  if (key == GLFW_KEY_C && action == GLFW_PRESS) {
    red_value = 1.0; 
    green_value = 1.0; 
    blue_value = 1.0; 
  }
}

/* Reshapes the window appropriately */
void reshape_callback(GLFWwindow* window, int w, int h)
{
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  
  // Think about the rationale for this choice
  // What would happen if you changed near and far planes? 
  // Note that the field of view takes in a radian angle!
  
  if (h > 0)
  {
	  projection = glm::perspective(30.0f / 180.0f * glm::pi<float>(), (GLfloat)w / (GLfloat)h, 1.0f, 100.0f);
	  glUniformMatrix4fv(projectionPos, 1, GL_FALSE, &projection[0][0]);
  }
 
}

void init (void) 
{
  /* select clearing color 	*/
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  /* initialize viewing values  */
  projection = glm::mat4(1.0f); // The identity matrix

  // Think about this.  Why is the up vector not normalized?
  modelview = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
  // modelview = glm::lookAt(glm::vec3(0,eyeloc,eyeloc), glm::vec3(0,0,0), glm::vec3(0,1,-1));

  // Now create the buffer objects to be used in the scene later
  // Remember to delete all the VAOs and VBOs that you create when the program terminates!
  glGenVertexArrays(numobjects, VAOs);
  glGenBuffers(numperobj*numobjects, buffers);

  // Initialize the CUBE
  initobject(FLOOR, (GLfloat *) floorverts, sizeof(floorverts), (GLfloat *) floorcol, sizeof (floorcol), (GLubyte *) floorinds, sizeof (floorinds), GL_TRIANGLES) ; 
  initobject(FLOOR2, (GLfloat *) floorverts2, sizeof(floorverts2), (GLfloat *) floorcol2, sizeof (floorcol2), (GLubyte *) floorinds2, sizeof (floorinds2), GL_TRIANGLES) ;
  initobject(FRONT_CUBE, (GLfloat *) front_cube_verts, sizeof(front_cube_verts), (GLfloat *) front_cube_col, sizeof (front_cube_col), (GLubyte *) front_cube_inds, sizeof (front_cube_inds), GL_TRIANGLES) ;
  initobject(BACK_CUBE, (GLfloat *) back_cube_verts, sizeof(back_cube_verts), (GLfloat *) back_cube_col, sizeof (back_cube_col), (GLubyte *) back_cube_inds, sizeof (back_cube_inds), GL_TRIANGLES) ;
  initobject(RIGHT_CUBE, (GLfloat *) right_cube_verts, sizeof(right_cube_verts), (GLfloat *) right_cube_col, sizeof (right_cube_col), (GLubyte *) right_cube_inds, sizeof (right_cube_inds), GL_TRIANGLES) ;
  initobject(LEFT_CUBE, (GLfloat *) left_cube_verts, sizeof(left_cube_verts), (GLfloat *) left_cube_col, sizeof (left_cube_col), (GLubyte *) left_cube_inds, sizeof (left_cube_inds), GL_TRIANGLES) ;

  // Initialize the PYRAMID
  initobject(BASE_PYRAMID, (GLfloat *) base_pyramid_verts, sizeof(base_pyramid_verts), (GLfloat *) base_pyramid_col, sizeof (base_pyramid_col), (GLubyte *) base_pyramid_inds, sizeof (base_pyramid_inds), GL_TRIANGLES) ; 
  initobject(FRONT_PYRAMID, (GLfloat *) front_pyramid_verts, sizeof(front_pyramid_verts), (GLfloat *) front_pyramid_col, sizeof (front_pyramid_col), (GLubyte *) front_pyramid_inds, sizeof (front_pyramid_inds), GL_TRIANGLES) ;
  initobject(BACK_PYRAMID, (GLfloat *) back_pyramid_verts, sizeof(back_pyramid_verts), (GLfloat *) back_pyramid_col, sizeof (back_pyramid_col), (GLubyte *) back_pyramid_inds, sizeof (back_pyramid_inds), GL_TRIANGLES) ;
  initobject(RIGHT_PYRAMID, (GLfloat *) right_pyramid_verts, sizeof(right_pyramid_verts), (GLfloat *) right_pyramid_col, sizeof (right_pyramid_col), (GLubyte *) right_pyramid_inds, sizeof (right_pyramid_inds), GL_TRIANGLES) ;
  initobject(LEFT_PYRAMID, (GLfloat *) left_pyramid_verts, sizeof(left_pyramid_verts), (GLfloat *) left_pyramid_col, sizeof (left_pyramid_col), (GLubyte *) left_pyramid_inds, sizeof (left_pyramid_inds), GL_TRIANGLES) ;


  // Initialize the DIAMOND
  initobject(BASE_DIAMOND, (GLfloat *) base_diamond_verts, sizeof(base_diamond_verts), (GLfloat *) base_diamond_col, sizeof (base_diamond_col), (GLubyte *) base_diamond_inds, sizeof (base_diamond_inds), GL_TRIANGLES) ; 
  initobject(FRONT_DIAMOND, (GLfloat *) front_diamond_verts, sizeof(front_diamond_verts), (GLfloat *) front_diamond_col, sizeof (front_diamond_col), (GLubyte *) front_diamond_inds, sizeof (front_diamond_inds), GL_TRIANGLES) ;
  initobject(BACK_DIAMOND, (GLfloat *) back_diamond_verts, sizeof(back_diamond_verts), (GLfloat *) back_diamond_col, sizeof (back_diamond_col), (GLubyte *) back_diamond_inds, sizeof (back_diamond_inds), GL_TRIANGLES) ;
  initobject(RIGHT_DIAMOND, (GLfloat *) right_diamond_verts, sizeof(right_diamond_verts), (GLfloat *) right_diamond_col, sizeof (right_diamond_col), (GLubyte *) right_diamond_inds, sizeof (right_diamond_inds), GL_TRIANGLES) ;
  initobject(LEFT_DIAMOND, (GLfloat *) left_diamond_verts, sizeof(left_diamond_verts), (GLfloat *) left_diamond_col, sizeof (left_diamond_col), (GLubyte *) left_diamond_inds, sizeof (left_diamond_inds), GL_TRIANGLES) ;
  initobject(FRONT_DIAMOND_DOWN, (GLfloat *) front_diamond_down_verts, sizeof(front_diamond_down_verts), (GLfloat *) front_diamond_down_col, sizeof (front_diamond_down_col), (GLubyte *) front_diamond_down_inds, sizeof (front_diamond_down_inds), GL_TRIANGLES) ;
  initobject(BACK_DIAMOND_DOWN, (GLfloat *) back_diamond_down_verts, sizeof(back_diamond_down_verts), (GLfloat *) back_diamond_down_col, sizeof (back_diamond_down_col), (GLubyte *) back_diamond_down_inds, sizeof (back_diamond_down_inds), GL_TRIANGLES) ;
  initobject(RIGHT_DIAMOND_DOWN, (GLfloat *) right_diamond_down_verts, sizeof(right_diamond_down_verts), (GLfloat *) right_diamond_down_col, sizeof (right_diamond_down_col), (GLubyte *) right_diamond_down_inds, sizeof (right_diamond_down_inds), GL_TRIANGLES) ;
  initobject(LEFT_DIAMOND_DOWN, (GLfloat *) left_diamond_down_verts, sizeof(left_diamond_down_verts), (GLfloat *) left_diamond_down_col, sizeof (left_diamond_down_col), (GLubyte *) left_diamond_down_inds, sizeof (left_diamond_down_inds), GL_TRIANGLES) ;

  std::string vertex_shader_path;
  std::string fragment_shader_path;

#ifdef __unix__         
  vertex_shader_path = "//home//lebana//Documents//CompGraf//CG-2020-I-part-II-//GLFW_GLAD_GLUT_GLEW_cmake_project//src//mytest1_glut//shaders//nop.vert";
  fragment_shader_path = "//home//lebana//Documents//CompGraf//CG-2020-I-part-II-//GLFW_GLAD_GLUT_GLEW_cmake_project//src//mytest1_glut//shaders//nop.frag";

#elif defined(_WIN32) || defined(WIN32) 
  vertex_shader_path = "D://Documents//Manuel//UCSP//Docencia//2020//CG//Laboratorio//GLFW_GLAD_GLUT_GLEW_cmake_project//src//mytest1_glfw//shaders//nop.vert";
  fragment_shader_path = "D://Documents//Manuel//UCSP//Docencia//2020//CG//Laboratorio//GLFW_GLAD_GLUT_GLEW_cmake_project//src//mytest1_glfw//shaders//nop.frag";
#endif


  // Initialize the shader program
  vertexshader = initshaders(GL_VERTEX_SHADER, vertex_shader_path.c_str());
  fragmentshader = initshaders(GL_FRAGMENT_SHADER, fragment_shader_path.c_str());

  shaderprogram = initprogram(vertexshader, fragmentshader) ;

  // Get the positions of the uniform variables
  projectionPos = glGetUniformLocation(shaderprogram, "projection");
  modelviewPos = glGetUniformLocation(shaderprogram, "modelview");
  // Pass the projection and modelview matrices to the shader
  glUniformMatrix4fv(projectionPos, 1, GL_FALSE, &projection[0][0]);
  glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);

}

//-----------------------------      SHOW OPERATORS  --------------------
//-----------------------------------------------------------------------
void menu() {
    std::cout<<"                   MENU                     \n";
    std::cout<<"    KEY 1 - MOUSE RIGHT: Acercar, aumenta tamaño \n";
    std::cout<<"    KEY 2 - tamaño inicial \n";
    std::cout<<"    KEY 3 - MOUSE LEFT: Alejar, disminuye tamaño \n";
    std::cout<<"    KEY W: hacia arriba \n";
    std::cout<<"    KEY S: hacia abajo \n";
    std::cout<<"    KEY A: Movimiento hacia la izquierda \n";
    std::cout<<"    KEY D: Movimiento hacia la derecha \n";
    std::cout<<"    KEY X: eje x \n";
    std::cout<<"    KEY Y: eje y \n";
    std::cout<<"    KEY Z: eje z \n";
    std::cout<<"    KEY E: Giros en el espacio \n";
    std::cout<<"    KEY L: Rotación lenta \n";
    std::cout<<"    KEY I: Invertir up \n";
    std::cout<<"    KEY R: Reset \n";
    std::cout<<"    KEY 9: Rotación eje x \n";
    std::cout<<"    KEY 8: Rotación eje y \n";
    std::cout<<"    KEY 7: Rotación eje z \n";
}
int main(int argc, char** argv)
{
  // Requests the type of buffers (Single, RGB).
  // Think about what buffers you would need...

  glfwInit(); //glutInit(&argc, argv);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glfw window creation
  // --------------------
  GLFWwindow* window = glfwCreateWindow(800, 600, "Simple Demo with Shaders", NULL, NULL);
  
  if (window == NULL)
  {
	  std::cout << "Failed to create GLFW window" << std::endl;
	  glfwTerminate();
	  return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetWindowPos(window, 100, 100);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
	  std::cout << "Failed to initialize GLAD" << std::endl;
	  return -1;
  }

  init (); // Always initialize first

  glfwSetFramebufferSizeCallback(window, reshape_callback);	//glutReshapeFunc(reshape) ;
  glfwSetKeyCallback(window, key_callback);					//glutKeyboardFunc(keyboard);
  glfwSetMouseButtonCallback(window, mouse_callback);		//glutMouseFunc(mouse) ;
  glfwSetCursorPosCallback(window, mousedrag_callback);		//glutMotionFunc(mousedrag) ;

  // First scene render
  reshape_callback(window, 800, 600);
  display(window); //glutDisplayFunc(display); 
  
  menu();
  // render loop
  // Start the main code
  while (!glfwWindowShouldClose(window)) //glutMainLoop(); 
  {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
	  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	  display(window); //glutDisplayFunc(display); 

	  // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	  // -------------------------------------------------------------------------------
	  glfwSwapBuffers(window);
	  glfwPollEvents();

	  //glFlush();
  }
  
  deleteBuffers(); // Program termination. Delete the buffers generated in init().
  
  glfwDestroyWindow(window);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  
  return 0;   /* ANSI C requires main to return int. */

}
