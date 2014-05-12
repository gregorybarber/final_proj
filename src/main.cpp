/* Simple opengl demo program. 
 *
 * Author: Papoj Thamjaroenporn
 *         Changxi Zheng
 * Spring 2013
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <map>

#include <GL/glew.h>
#if defined(_WIN32)
#   include <GL/wglew.h>
#endif

#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include "GLScreenCapturer.h"
#include "trackball.h"
#include "shader.h"
#include "GLSLProgram.h"
#include "SOIL.h"


using namespace std;

#define BUFFER_LENGTH 64
#define NUM_OBJ 5

GLfloat camRotX, camRotY, camPosX, camPosY, camPosZ;
GLint viewport[4];
GLdouble modelview[16];
GLdouble projection[16];

GLuint counter = 0;
class Node;

GLuint pickedObj = -1;
char titleString[150];

bool isTeapot1_selected = false;
bool isTeapot2_selected = false;

// Lights & Materials
GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
GLfloat position[] = {4, 4, 4, 1.0};
GLfloat mat_diffuse[] = {0.6, 0.6, 0.6, 1.0};
GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_shininess[] = {50.0};

GLfloat light1_amb[] = {0.0, 0.0, 0.0, 1.0};
GLfloat light1_pos[] = {0, 0, 4, 1.0};
GLfloat light1_dif[] = {0.1, 0.1, 0.1, 1.0};
GLfloat light1_spe[] = {1.0, 1.0, 1.0, 1.0};

static GLScreenCapturer screenshot("screenshot-%d.ppm");
static GLSLProgram*     shaderProg = NULL;

const char* normal_file = "textures/drops.png";
const char* color_file = "textures/smooth.png";

static GLuint normal_texture_id;
static GLuint color_texture_id;

static GLint gouraudToggle;
static GLint blinnPhongToggle;
static GLint colorTextureToggle;
static GLint normalTextureToggle;
static GLint checkerboardToggle;

void initLights(void)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_amb);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_dif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_spe);
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void setupRC()
{
	tbInit(GLUT_RIGHT_BUTTON);
	tbAnimate(GL_TRUE);
	
	// Place Camera
	camRotX = 750.0f;  //350, 800 was about vertical looking down
	camRotY = 680.0f; //680
	camPosX = 0.0f;
	camPosY = 0.0f;
	camPosZ = -30.5f;   //-10.5
	
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	initLights();
}

void setCamera( void )
{
	glTranslatef(0, 0, camPosZ);
	glRotatef(camRotX, 1, 0, 0);
	glRotatef(camRotY, 0, 1, 0);
}

void drawBox( GLfloat height, GLfloat width )
{
      /* draws the sides of a unit cube (0,0,0)-(1,1,1) */

    glBegin(GL_POLYGON);/* f1: front */
    { 
      	glNormal3f(-1.0f,0.0f,0.0f);
        glVertex3f(0.0f,0.0f,0.0f);
        glVertex3f(0.0f,0.0f,width);
        glVertex3f(width,0.0f,width);
        glVertex3f(width,0.0f,0.0f);
    }
    glEnd();
    glBegin(GL_POLYGON);/* f2: bottom */
    {
        glNormal3f(0.0f,0.0f,-1.0f);
        glVertex3f(0.0f,0.0f,0.0f);
        glVertex3f(width,0.0f,0.0f);
        glVertex3f(width,height,0.0f);
        glVertex3f(0.0f,height,0.0f);
    }
    glEnd();
    glBegin(GL_POLYGON);/* f3:back */
    { 
        glNormal3f(1.0f,0.0f,0.0f);
        glVertex3f(width,height,0.0f);
        glVertex3f(width,height,width);
        glVertex3f(0.0f,height,width);
        glVertex3f(0.0f,height,0.0f);
    }
    glEnd();
    glBegin(GL_POLYGON);/* f4: top */
    {
        glNormal3f(0.0f,0.0f,1.0f);
        glVertex3f(width,height,width);
        glVertex3f(width,0.0f,width);
        glVertex3f(0.0f,0.0f,width);
        glVertex3f(0.0f,height,width);
    }
    glEnd();
    glBegin(GL_POLYGON);/* f5: left */
    {
        glNormal3f(0.0f,1.0,0.0f);
        glVertex3f(0.0f,0.0f,0.0f);
        glVertex3f(0.0f,height,0.0f);
        glVertex3f(0.0f,height,width);
        glVertex3f(0.0f,0.0f,width);
    }
    glEnd();
    glBegin(GL_POLYGON);/* f6: right */
    {
		glNormal3f(0.0f,-1.0,0.0f); //neg?
        glVertex3f(width,0.0f,0.0f);
        glVertex3f(width,0.0f,width);
        glVertex3f(width,height,width);
        glVertex3f(width,height,0.0f);
    }
    glEnd();
}

/*typedef struct sceneObj sceneObj;

struct sceneObj
{
	//boolean for box?
	GLfloat color[4];		//color
	GLuint start,stop;            //delay the rising of the building
	GLfloat height,width;
	//material
	//shader
	sceneObj()
	{
		color = {0.0f,0.0f,0.0f,0.0f};
		start = 0;
		stop = 0;
		height = 0.0;
		width = 0.0;
	}
	sceneObj(GLfloat objColor, GLuint objStart, GLuint objStop, GLfloat objHeight, GLfloat objWidth)
	{
	}
} ;*/

GLuint nodeCount = 0;
map<GLuint, Node*> nodeMap;

class Node {
public:
    Node(GLfloat height=0.0, GLfloat width=0.0, GLuint start=0, 
         GLuint stop=0); //color?
    Node(const Node& other);
    ~Node();
    void virtual drawSelf();
    GLfloat height,width;
    GLuint start, stop, id;
};

Node::Node(GLfloat height, GLfloat width, GLuint start, GLuint stop) {
	this->height = height;
	this->width = width;
	this->start = start;
	this->stop = stop;
    id = nodeCount++;
    nodeMap[id] = this;
}

Node::Node(const Node& other) {
	//this->height = other->height;
	//this->
    id = nodeCount++;
    nodeMap[id] = this;
}

Node::~Node() {
    nodeMap.erase(id);
}

void Node::drawSelf() {
    GLfloat color1[] = {5.0, 0.0, 0.0, 1};

  //  if (currentNode == this) {
  //      glColor4fv(selectedColor);
  //      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, selectedColor);
  //  }
  //  else {
        glColor4fv(color1);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color1);        
 //   }

    glPushMatrix();
    {
       // glTranslatef(pos[0], pos[1], pos[2]);
        drawBox(this->height,this->width);
        //glutSolidSphere(0.1, 10, 10);
    }
    glPopMatrix();

}

Node *objList[NUM_OBJ]; // SET GLOBAL VARIABLE

void createObjects( void ) //fix for use with the node map
{
	int i;
	for (i=0; i<NUM_OBJ; i++) {
		Node *obj = new Node();
	}

}



void drawFloor( void )
{
	glBegin(GL_QUADS);
    	glVertex3f(10000,-2,-10000);
    	glVertex3f(-10000,-2,-10000);
    	glVertex3f(-10000,-2,10000);
    	glVertex3f(10000,-2,10000);
    glEnd();
}

void setShadeParam( void ) //give parameters as toggle and light direction
{
	shaderProg->enable();
	shaderProg->set_uniform_3f("lightDir", 2.0f, 1.0f, 3.0f);
	shaderProg->bind_texture("normalMap", normal_texture_id, GL_TEXTURE_2D, 0);
	shaderProg->bind_texture("colorMap", color_texture_id, GL_TEXTURE_2D, 1);
	shaderProg->set_uniform_1i("gouraudToggle", gouraudToggle);
	shaderProg->set_uniform_1i("blinnPhongToggle", blinnPhongToggle);
	shaderProg->set_uniform_1i("checkerboardToggle", checkerboardToggle);
	shaderProg->set_uniform_1i("colorTextureToggle", colorTextureToggle);
	shaderProg->set_uniform_1i("normalTextureToggle", normalTextureToggle);
}


float randFloat(const float& min, const float& max) {
    float range = max - min;
    float num = range * rand() / RAND_MAX;
    return (num + min);
}


void drawScene( void )
{
	float currentColor[4];
	glGetFloatv(GL_CURRENT_COLOR, currentColor);
	
	GLfloat selectedColor[] = {0, 1, 0, 1};
	GLfloat unselectedColor[] = {1, 0, 0, 1};

	// Initialize the name stack
	glInitNames();
	glPushName(0);
	
	setShadeParam();

	// Draw two teapots next to each other in z axis
	glPushMatrix();
	{
		drawFloor();
		if( isTeapot1_selected )
			glMaterialfv(GL_FRONT, GL_DIFFUSE, selectedColor);
		else
			glMaterialfv(GL_FRONT, GL_DIFFUSE, unselectedColor);
		glLoadName(0);
		Node *newNode = new Node(2.0,3.0,0,5);
		//newNode->drawSelf();
		//delete newNode;
		drawBox(newNode->height, newNode->width);
		//glutSolidTeapot(2.5);
        //drawBox();
		if( isTeapot2_selected )
			glMaterialfv(GL_FRONT, GL_DIFFUSE, selectedColor);
		else
			glMaterialfv(GL_FRONT, GL_DIFFUSE, unselectedColor);
		glLoadName(1);
		glTranslatef(0,0,5);
		glutSolidTeapot(1.5);
	}
	glPopMatrix();
	shaderProg->disable();
	
	glColor4fv(currentColor);

}

void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glPushMatrix();
	{
	
		setCamera();
		tbMatrix();
		
		drawScene();
		
		// Retrieve current matrice before they popped.
		glGetDoublev( GL_MODELVIEW_MATRIX, modelview );        // Retrieve The Modelview Matrix
		glGetDoublev( GL_PROJECTION_MATRIX, projection );    // Retrieve The Projection Matrix
		glGetIntegerv( GL_VIEWPORT, viewport );                // Retrieves The Viewport Values (X, Y, Width, Height)
	}
	glPopMatrix();

	glFlush();
	// End Drawing calls
	glutSwapBuffers();
}

void reshape( int w, int h )
{
	tbReshape(w, h);

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the clipping volume
	gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 1.0f, 100.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

static void setCurrentShader(GLSLProgram* s) {
	shaderProg = s;
	glutPostRedisplay();
}

static void setupShaders()
{
	printf("MSG: Initialize GLSL Shaders ...\n");

	glewInit();
	if ( !glewIsSupported("GL_VERSION_2_0 GL_ARB_multitexture GL_EXT_framebuffer_object") ) 
	{
		fprintf(stderr, "Required OpenGL extensions missing\n");
		exit(2);
	}
	
	shaderProg = new GLSLProgram(generalVS, generalFS);

	gouraudToggle = 1;
	blinnPhongToggle = 0;
	checkerboardToggle = 0;
	colorTextureToggle = 0;
	normalTextureToggle = 0;

	glActiveTexture(GL_TEXTURE0);
	normal_texture_id = SOIL_load_OGL_texture(normal_file, 
		                               SOIL_LOAD_AUTO,
		                               SOIL_CREATE_NEW_ID,
		                               SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);
	if (normal_texture_id == 0)
		cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << normal_file << ")" << endl;

	glActiveTexture(GL_TEXTURE1);
	color_texture_id = SOIL_load_OGL_texture(color_file, 
		                               SOIL_LOAD_AUTO,
		                               SOIL_CREATE_NEW_ID,
		                               SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);
	if (normal_texture_id == 0)
		cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << color_file << ")" << endl;
}

void keyboard( unsigned char key, int x, int y )
{
	switch(key)
	{
	case 27: // Escape key
		exit(0);
		break;
	case 'r':
		printf("save current screen\n");
		screenshot.capture();
		break;
	case '1': //switch gouraud
		blinnPhongToggle = 0;
		gouraudToggle = !gouraudToggle;
		normalTextureToggle = 0;
		glutPostRedisplay();
		break;
	case '2': //switch blinn-phong
		gouraudToggle = 0;
		blinnPhongToggle = !blinnPhongToggle;
		glutPostRedisplay();
		break;
	case '3'://switch checkerboard
		colorTextureToggle = 0;
		checkerboardToggle = !checkerboardToggle;
		glutPostRedisplay();
		break;
	case '4'://switch color texture
		checkerboardToggle = 0;
		colorTextureToggle = !colorTextureToggle;
		glutPostRedisplay();
		break;
	case '5'://toggle normals - requires blinn-phong
		normalTextureToggle = !normalTextureToggle;
		gouraudToggle = 0;
		blinnPhongToggle = 1;
		glutPostRedisplay();
		break;
	}
}

void processSelection(int xPos, int yPos)
{
	GLfloat fAspect;
	
	// Space for selection buffer
	static GLuint selectBuff[BUFFER_LENGTH];
	
	// Hit counter and viewport storage
	GLint hits, viewport[4];
	
	// Setup selection buffer
	glSelectBuffer(BUFFER_LENGTH, selectBuff);
	
	// Get the viewport
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	// Switch to projection and save the matrix
	glMatrixMode(GL_PROJECTION);
	
	glPushMatrix();
	{
		// Change render mode
		glRenderMode(GL_SELECT);
		
		// Establish new clipping volume to be unit cube around
		// mouse cursor point (xPos, yPos) and extending two pixels
		// in the vertical and horizontal direction
		glLoadIdentity();
		gluPickMatrix(xPos, viewport[3] - yPos + viewport[1], 0.1,0.1, viewport);
		
		// Apply perspective matrix 
		fAspect = (float)viewport[2] / (float)viewport[3];
		gluPerspective(45.0f, fAspect, 1.0, 425.0);
		
		
		// Render only those needed for selection
		glPushMatrix();    
		{
			setCamera();
			tbMatrixForSelection();
			
			drawScene();
		}
		glPopMatrix();
		
		
		// Collect the hits
		hits = glRenderMode(GL_RENDER);
		
		isTeapot1_selected = false;
		isTeapot2_selected = false;
		
		// If hit(s) occurred, display the info.
		if(hits != 0)
		{
		
			// Save current picked object.
			// Take only the nearest selection
			pickedObj = selectBuff[3];
			
			sprintf (titleString, "You clicked on %d", pickedObj);
			glutSetWindowTitle(titleString);
			
			if (pickedObj == 0) {
				isTeapot1_selected = true;
			}
			
			if (pickedObj == 1) {
				isTeapot2_selected = true;
			}
			
		}
		else
			glutSetWindowTitle("Nothing was clicked on!");
		
		
		// Restore the projection matrix
		glMatrixMode(GL_PROJECTION);
	}
	glPopMatrix();
	
	// Go back to modelview for normal rendering
	glMatrixMode(GL_MODELVIEW);
	
	glutPostRedisplay();
}

void mouse( int button, int state, int x, int y)
{
	tbMouse(button, state, x, y);
	
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		processSelection(x, y);
	
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		pickedObj = -1;
		glutPostRedisplay();
	}
}

void motion(int x, int y)
{
	tbMotion(x, y);
	
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;
	
	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	glutPostRedisplay();

}

void update (int value)
{
	int i = 0;
	while(nodeMap[i] != NULL) {
		if (nodeMap[i]->stop > counter && counter > nodeMap[i]->start) {
			nodeMap[i]->height+= .05;
		}
		i++;
	}
	glutPostRedisplay();
	glutTimerFunc(25,update,0);
}



int main (int argc, char *argv[])
{
	int win_width = 960;
	int win_height = 540;

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize( win_width, win_height );

	glutCreateWindow( "Final Project!!!" );
	setupShaders();
	setupRC();
	//createObjects();

	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mouse );
	glutMotionFunc( motion );
  //  glutTimerFunc(25, update, 0);
	glutMainLoop();
	delete shaderProg;
}
