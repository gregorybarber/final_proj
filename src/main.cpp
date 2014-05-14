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
#define NUM_TREES 28

GLfloat camRotX, camRotY, camPosX, camPosY, camPosZ;
GLint viewport[4];
GLdouble modelview[16];
GLdouble projection[16];

GLint counter = 0;
class Node;
class Tree;

int season = 2;
int angle = 0;

GLfloat redGlobal = 134;
GLfloat greenGlobal = 189;
GLfloat blueGlobal = 75;



GLuint pickedObj = -1;
char titleString[150];


int seeds[NUM_TREES];

// Lights & Materials
GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
GLfloat position[] = {4, 4, 4, 1.0};
GLfloat mat_diffuse[] = {0.4, 0.4, 0.4, 1.0};
GLfloat mat_specular[] = {0.4, 0.4, 0.4, 1.0};
GLfloat mat_shininess[] = {50.0};

GLfloat light1_amb[] = {0.0, 0.0, 0.0, 1.0};
GLfloat light1_pos[] = {0, 0, 4, 1.0};
GLfloat light1_dif[] = {0.1, 0.1, 0.1, 1.0};
GLfloat light1_spe[] = {1.0, 1.0, 1.0, 1.0};

static GLScreenCapturer screenshot("screenshot-%d.ppm");
static GLSLProgram*     shaderProg = NULL;
static GLSLProgram* windowShader = NULL;
static GLSLProgram* defaultShader = NULL;
static GLSLProgram* skyboxShader = NULL;
static GLSLProgram* floorShader = NULL;

const char* normal_file = "textures/drops.png";
const char* color_file = "textures/smooth.png";
const char* window_file = "textures/windows.png";
const char* asphalt_file = "textures/asphalt.png";
const char* perlin_file = "textures/perlin.png";

const char* skybox_front = "textures/sky-hack/plain_sky_front.png";
const char* skybox_back = "textures/sky-hack/plain_sky_back.png";
const char* skybox_up = "textures/sky-hack/plain_sky_top.png";
const char* skybox_down = "textures/sky-hack/plain_sky_bottom.png";
const char* skybox_right = "textures/sky-hack/plain_sky_right.png";
const char* skybox_left = "textures/sky-hack/plain_sky_left.png";

static GLuint normal_texture_id;
static GLuint color_texture_id;
static GLuint skybox_texture_id;
static GLuint asphalt_texture_id;
static GLuint perlin_texture_id;

GLuint skybox_id;

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

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
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
	camPosZ = -40.5f;   //-10.5

	glEnable(GL_DEPTH_TEST);
	//glShadeModel(GL_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	initLights();
}

void setShadeParam( void ) //give parameters as toggle and light direction
{
	shaderProg->set_uniform_3f("lightDir", -2.0f, 1.0f, 3.0f);
	shaderProg->bind_texture("normalMap", normal_texture_id, GL_TEXTURE_2D, 0);
	shaderProg->bind_texture("colorMap", color_texture_id, GL_TEXTURE_2D, 1);
	shaderProg->bind_texture("cubeMap", skybox_texture_id, GL_TEXTURE_CUBE_MAP, 0);
}


static void setCurrentShader(GLSLProgram* s) {
	shaderProg = s;
	s->enable();
	// setShadeParam();
	// glutPostRedisplay();
}

void drawBox( GLfloat height, GLfloat width )
{
      /* draws the sides of a unit cube (0,0,0)-(1,1,1) */

    glBegin(GL_POLYGON);/* f2: BACK */

    {
        glNormal3f(0.0f,0.0f,-1.0f);
        glTexCoord2f (0.0, 1 - (height / width));
        glVertex3f(0.0f,0.0f,0.0f);
        glTexCoord2f (1.0, 1 - (height / width));
        glVertex3f(width,0.0f,0.0f);
        glTexCoord2f (1.0, 1.0);
        glVertex3f(width,height,0.0f);
        glTexCoord2f (0.0, 1.0);
        glVertex3f(0.0f,height,0.0f);
    }
    glEnd();
    glBegin(GL_POLYGON);/* f4: FRONT */
    {
        glNormal3f(0.0f,0.0f,1.0f);
	    glTexCoord2f (1.0, 1.0);
        glVertex3f(width,height,width);
        glTexCoord2f (1.0, 1 - (height / width));
        glVertex3f(width,0.0f,width);
        glTexCoord2f (0.0, 1 - (height / width));
        glVertex3f(0.0f,0.0f,width);
        glTexCoord2f (0.0, 1.0);
        glVertex3f(0.0f,height,width);
    }
    glEnd();
    glBegin(GL_POLYGON);/* f5: LEFT */
    {
        glNormal3f(0.0f,1.0,0.0f);
		glTexCoord2f (0.0, -(height / width));
        glVertex3f(0.0f,0.0f,0.0f);
        glTexCoord2f (0.0, 0.0);
        glVertex3f(0.0f,height,0.0f);
        glTexCoord2f (1.0, 0.0);
        glVertex3f(0.0f,height,width);
        glTexCoord2f (1.0, -(height / width));
        glVertex3f(0.0f,0.0f,width);
    }
    glEnd();
    glBegin(GL_POLYGON);/* f6: RIGHT */
    {
		glNormal3f(0.0f,-1.0,0.0f); 
		glTexCoord2f (0.0, -(height / width));
        glVertex3f(width,0.0f,0.0f);
        glTexCoord2f (1.0, -(height / width));
        glVertex3f(width,0.0f,width);
        glTexCoord2f (1.0, 0.0);
        glVertex3f(width,height,width);
        glTexCoord2f (0.0, 0.0);
        glVertex3f(width,height,0.0f);
    }
    glEnd();
    glBegin(GL_POLYGON);/* f3:TOP */
    { 
        glNormal3f(1.0f,0.0f,0.0f);
        glTexCoord2f (0.0, 0.0);
        glVertex3f(width,height,0.0f);
         glTexCoord2f (0.0, 0.0);
        glVertex3f(width,height,width);
         glTexCoord2f (0.0, 1.0);
        glVertex3f(0.0f,height,width);
         glTexCoord2f (0.0, 1.0);
        glVertex3f(0.0f,height,0.0f);
    }
    glEnd();
}



void setCamera( void )
{
	glTranslatef(0, 0, camPosZ);
	glRotatef(camRotX, 1, 0, 0);
	glRotatef(camRotY, 0, 1, 0);

}



class Position {
public:
    Position(GLfloat x=0, GLfloat y=0, GLfloat z=0);
    Position(Position& other);
    GLfloat operator[](size_t i) { return pos[i]; }
    GLfloat pos[3];
};

Position::Position(GLfloat x, GLfloat y, GLfloat z) {
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

Position::Position(Position& other) {
    pos[0] = other.pos[0];
    pos[1] = other.pos[1];
    pos[2] = other.pos[2];
}

GLuint nodeCount = 0;
map<GLuint, Node*> nodeMap;

class Node {
public:
    Node(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0, GLfloat height=0.0, 
    GLfloat width=0.0, GLuint start=0, GLfloat rate=0.0); //color?
    Node(const Node& other);
    ~Node();
    void virtual drawSelf();
    GLfloat height,width,rate;
    GLuint start, id;
    Position pos;
};

Node::Node(GLfloat x, GLfloat y, GLfloat z, GLfloat height, GLfloat width, GLuint start, GLfloat rate): pos(x, y, z) {
	this->pos = pos;
	this->height = height;
	this->width = width;
	this->start = start;
	this->rate = rate;
    id = nodeCount++;
    nodeMap[id] = this;
}

Node::Node(const Node& other) {
	pos = other.pos;
	width = other.width;
	height = other.height;
	start = other.start;
	rate = other.rate;
    id = nodeCount++;
    nodeMap[id] = this;
}

Node::~Node() {
    nodeMap.erase(id);
}

void Node::drawSelf() {
    GLfloat color1[] = {5.0, 0.0, 0.0, 1};
    GLfloat currHeight = height;
    glColor4fv(color1);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color1);        
    if (counter > start) {
    	currHeight = (counter-start)*rate;
    	if (currHeight > height)
    		currHeight = height;
    	glPushMatrix();
    	{
    	    glTranslatef(pos[0], pos[1], pos[2]);
    	    drawBox(currHeight,width);
   		 }
   	    glPopMatrix();
   	}

}


void drawFloor( void )
{
	setCurrentShader(floorShader);
	shaderProg->set_uniform_3f("lightDir", -2.0f, 1.0f, 3.0f);
	shaderProg->bind_texture("normalMap", normal_texture_id, GL_TEXTURE_2D, 0);
	shaderProg->bind_texture("colorMap", asphalt_texture_id, GL_TEXTURE_2D, 1);
	shaderProg->bind_texture("cubeMap", skybox_texture_id, GL_TEXTURE_CUBE_MAP, 0);
	shaderProg->bind_texture("perlinMap", perlin_texture_id, GL_TEXTURE_2D, 2);
	shaderProg->set_uniform_1f("reflectance", 0.5);

	GLfloat color[] = {0.1, 0.1, 0.1, 0.7};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

	glBegin(GL_QUADS);
		glTexCoord2f(25, -25);
    	glVertex3f(50,0,-50);
    	glTexCoord2f(-25, -25);
    	glVertex3f(-50,0,-50);
    	glTexCoord2f(-25, 25);
    	glVertex3f(-50,0,50);
    	glTexCoord2f(25, 25);
    	glVertex3f(50,0,50);
    glEnd();
}


void initSky() {
    // GLfloat green[] = {(GLfloat)(91)/255.0,(GLfloat)(153)/255.0, (GLfloat)(84)/255.0, 1.0};
    // glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
    GLUquadricObj *sphere=NULL;
     sphere = gluNewQuadric();
     gluQuadricDrawStyle(sphere, GLU_FILL);
     gluQuadricTexture(sphere, GLU_TRUE);
     gluQuadricNormals(sphere, GLU_SMOOTH);
      // Making a display list
     skybox_id = glGenLists(1);
     glNewList(skybox_id, GL_COMPILE);
      gluSphere(sphere, 50, 20, 20);
     glEndList();
     gluDeleteQuadric(sphere);
}




void drawTallObjects( void ) 
{
    int i;
    for(size_t i=0; i < 9; i++) {
        nodeMap[i]->drawSelf();

    }
}

void drawShortObjects( void ) 
{
    int i;
    for(size_t i=9; i < 18; i++) {
        nodeMap[i]->drawSelf();

    }
}

void drawVeryShortObjects( void )
{
    int i;
    for(size_t i=18; i < nodeMap.size(); i++) {
        nodeMap[i]->drawSelf();

    }
}

float randFloat(const float& min, const float& max) {
    float range = max - min;
    float num = range * rand() / RAND_MAX;
    return (num + min);
}

typedef struct branch branch;

struct branch {
    GLdouble bottomWidth, topWidth;
    GLdouble length;
    branch *child;
} ;




void drawTwigs( int count, branch* trunk, GLUquadricObj *quadObj )
{    
    int j;
    branch* limb = trunk->child;
    GLfloat lengthMult = (GLfloat)(rand()%+3)/10.0;
    limb->bottomWidth = trunk->topWidth;
    limb->topWidth = (limb->bottomWidth)*(lengthMult);
    limb->length = ((trunk->length)*(lengthMult))-((GLdouble)(count+1)/10);
    if (limb->length > .07)
	limb->length = .07;
    while (count < 3) {
        glPushMatrix();
        {
	    //Draw Twigs (line segments, rather than tapered cylinders)
	    glTranslatef(0,0,trunk->length*((GLfloat)(count+1)/5.0));
        count++;
	    GLfloat angleY = (GLfloat)(rand()%90-45);
        GLfloat angleZ = (GLfloat)(rand()%360);
        glRotated(angleY,1,1,0);
        glRotated(angleZ,0,1,1);
	    glLineWidth(.07);
        glBegin(GL_LINES);
	    glVertex3f(0,0,0);
	    glVertex3f(0,0,limb->length);
 	    glEnd();
        GLfloat radius = (GLfloat)((rand()%2+2)/10);
        for (j = 0; j<8; j++) {
        //Draw Seasonal Foliage
            angleY = (GLfloat)(rand()%90-45);
            angleZ = (GLfloat)(rand()%360);
            if (season != 4) {
                glPushMatrix();
                {
                if (season == 1){
              		GLfloat green[] = {(GLfloat)(91)/255.0,(GLfloat)(153)/255.0, (GLfloat)(84)/255.0, .4};
               		glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
           		}
                if (season == 2) {
                    GLfloat summer[] = {(GLfloat)(redGlobal)/255.0,(GLfloat)(greenGlobal)/255.0, (GLfloat)(blueGlobal)/255.0, .4};
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, summer);
                }
			     //Two fall colors
		        if (season == 3) {
			        if (j < 2) {
			            GLfloat fallTwo[] = {(GLfloat)(232)/255.0,(GLfloat)(32)/255.0, (GLfloat)(59)/255.0, .4};
                    	glMaterialfv(GL_FRONT, GL_DIFFUSE, fallTwo);
		            }
			   		else {
				    	GLfloat fallThree[] = {(GLfloat)(250)/255.0,(GLfloat)(74)/255.0, (GLfloat)(15)/255.0, .4};
                    	glMaterialfv(GL_FRONT, GL_DIFFUSE, fallThree);
		        	}    		
			   }	     
                 glRotated(angleY,1,1,0);
                 glRotated(angleZ,0,1,0);
			     //Draw foliage - partial spheres
		        gluSphere(quadObj, .5, 3, 2);
			     //if spring, draw flowers at the edges of the foliage
                if (season == 1) {
		            GLfloat flowerColor[] = {(GLfloat)(200)/255.0,(GLfloat)(152)/255.0, (GLfloat)(248)/255.0, .75};
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, flowerColor);
			    	glRotated(angleY,1,1,0);
			    	glRotated(angleZ,0,0,1);
                    glTranslatef(0,0,radius);
                    glutSolidSphere(.02,3,5);
		        }
		    }
                    glPopMatrix();
                   }
           	}	
                GLfloat colorReset[] = {(GLfloat)(171.0)/255.0,(GLfloat)(158)/255.0, (GLfloat)(114)/255.0, 1};
                glMaterialfv(GL_FRONT, GL_DIFFUSE, colorReset);
	        drawTwigs(count,limb, quadObj);
   	}
	glPopMatrix();
    }
}

        
void drawBranches( int numBranches, branch* trunk, GLUquadricObj *quadObj)
{
    branch* limb = trunk->child;
    int level = 7;
    int count = 0;
    limb->length = trunk->length;
    numBranches--;
    GLfloat color[] = {(GLfloat)(171.0)/255.0,(GLfloat)(158)/255.0, (GLfloat)(114)/255.0, 1};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    GLfloat angleY;
    while (count < 4 && limb->length>.5) {
	//Generate randomized branches using tapered cylinders   	
        GLfloat lengthMult = (GLfloat)(rand()%4+4)/10.0;
        limb->bottomWidth = trunk->topWidth;
        limb->topWidth = (limb->bottomWidth)*(lengthMult);
        limb->length = ((trunk->length)*(lengthMult));
		glPushMatrix();
    	{  
	    //constrain child branches to the tops of their parent
	
	    if (level > 3)
            level--;
        else
		 	level=7;
		count++;
		glTranslatef(0,0,(trunk->length)*((GLdouble)(level)/7)); 
        if (level > 3)
			angleY = (GLfloat)(rand()%90-45);
        else
			angleY = (GLfloat)(rand()%70);
        GLfloat angleZ = (GLfloat)(rand()%180-180);
        glLoadName(1);
        glRotated(angleZ,0,0,1);
        glRotated(angleY,0,1,0);
        gluSphere(quadObj, (limb->bottomWidth)*1.1, 25, 50);
        gluCylinder(quadObj, limb->bottomWidth, limb->topWidth, limb->length, 10, 100);
        drawBranches(numBranches,limb, quadObj);
    	}    
    	glPopMatrix();
    }
    //Once chain of branches is complete, draw "twigs" on the last branch
    drawTwigs(numBranches,limb, quadObj);

}

void drawTreeGround( branch *trunk)
{
    int i;
    GLfloat randZ, randX;
    glPushMatrix();
    GLUquadricObj *quadObj;
    glLoadName(0);
    int numBranches = 5;
    quadObj = gluNewQuadric ();
    gluQuadricDrawStyle (quadObj, GLU_FILL);
    gluQuadricNormals (quadObj, GLU_FLAT);
    glRotatef(-90,1,0,0);
    GLfloat ground[] = {(GLfloat)(65.0)/255.0,(GLfloat)(186.0)/255.0, (GLfloat)(83.0)/255.0, 1};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ground);
    if (season == 4) {
	    GLfloat winterGround[] = {(GLfloat)(255.0)/255.0,(GLfloat)(255.0)/255.0, (GLfloat)(255.0)/255.0, 1};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, winterGround);
    }

    //in fall, generate falling leaves
    /*for (i=0; i<100; i++) { 
		randZ = randFloat(-6,6);
        randX = randFloat(-6,6);
		GLfloat height = randFloat(10.0,18.0);
        if (height+falling < 0.0) {
	    	height = 0.0;
		}
    	if (season == 3) {                                   
	   		GLfloat fallOne[] = {(GLfloat)(212)/255.0,(GLfloat)(61)/255.0, (GLfloat)(76)/255.0, .9};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, fallOne);
            glBegin(GL_POINTS);
            glVertex3f(randX,randZ,falling+height);
            glEnd();
        }
    }*/

    //generate trunk of tree
    GLfloat color[] = {(GLfloat)(171.0)/255.0,(GLfloat)(158)/255.0, (GLfloat)(114)/255.0, 1};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    gluCylinder(quadObj, 0.04, 0.02, 1.30, 10, 100);
    trunk->bottomWidth = .03;
    trunk->topWidth = .02;
    trunk->length = 1.3;
    //generate branches off of starting trunk
    drawBranches(numBranches,trunk,quadObj);
    glPopMatrix();
}

void drawTree( branch* trunk )
{
    
    float currentColor[4];
    glGetFloatv(GL_CURRENT_COLOR, currentColor);

    // Initialize the name stack
    glInitNames();
    glPushName(0);
    drawTreeGround(trunk);
    glColor4fv(currentColor);
}

GLuint treeCount = 0;
map<GLuint, Tree*> treeMap;

class Tree {
public:
    Tree(GLfloat x, GLfloat y, GLfloat z); 
    Tree(const Tree& other);
    ~Tree();
    void virtual drawSelf();
    branch* trunk;
    GLuint id;
    Position pos;
};

Tree::Tree(GLfloat x, GLfloat y, GLfloat z): pos(x, y, z) {
    this->pos = pos;
    this->trunk = trunk;
    id = treeCount++;
    treeMap[id] = this;
}

Tree::Tree(const Tree& other) {
    pos = other.pos;
    trunk = other.trunk;
    id = treeCount++;
    treeMap[id] = this;
}

Tree::~Tree() {
    treeMap.erase(id);
}

void Tree::drawSelf() {
        glPushMatrix();
        {
            glTranslatef(pos[0], pos[1], pos[2]);
            drawTree(trunk);
         }
        glPopMatrix();
}

void drawForest( void )
{
	setCurrentShader(defaultShader);
	shaderProg->set_uniform_3f("lightDir", -2.0f, 1.0f, 3.0f);
	shaderProg->bind_texture("normalMap", normal_texture_id, GL_TEXTURE_2D, 0);
	shaderProg->bind_texture("colorMap", color_texture_id, GL_TEXTURE_2D, 1);
	shaderProg->bind_texture("cubeMap", skybox_texture_id, GL_TEXTURE_CUBE_MAP, 0);

	glPushMatrix();
    
    glEnable( GL_POINT_SMOOTH); 
    glPointSize(6.0);
   
    glShadeModel(GL_SMOOTH);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glBlendEquation(GL_FUNC_ADD);
    glClearColor(0.0,0.0,0.0,1.0);
    initLights();
    int i;
    glTranslatef(6,0,7);
    for (size_t i =0;i<treeMap.size();i++) {
        glPushMatrix();
        glTranslatef(treeMap[i]->pos[0],0,treeMap[i]->pos[2]);
        srand(seeds[i]);
        treeMap[i]->drawSelf();
        glPopMatrix();
    }
    glPopMatrix();
}

GLfloat snowFall = 0.0;
GLfloat rainFall = 0.0;
GLfloat rainAngle = 0.0;

void precipitation( void )
{

	setCurrentShader(defaultShader);
	shaderProg->set_uniform_3f("lightDir", -2.0f, 1.0f, 3.0f);
	shaderProg->bind_texture("normalMap", normal_texture_id, GL_TEXTURE_2D, 0);
	shaderProg->bind_texture("colorMap", color_texture_id, GL_TEXTURE_2D, 1);
	shaderProg->bind_texture("cubeMap", skybox_texture_id, GL_TEXTURE_CUBE_MAP, 0);

    int i;

        GLfloat streak = randFloat(0,1);
        GLfloat rainColor[] = {1, 1, 1, .5};
        GLfloat xAngling = randFloat(-.3,.3)*(rainAngle);

        for (i=0;i<10000;i++) {
            GLfloat zPos = randFloat(-30,30);
            GLfloat xPos = randFloat(-30,30);
            GLfloat height = randFloat(0,40);
            
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, rainColor);
            if (season != 4) {
                glLineWidth(1);
                 glBegin(GL_LINES);
                 glVertex3f(xPos,height-rainFall,zPos);
                 glVertex3f(xPos-xAngling,height-rainFall-streak,zPos);
                glEnd();
            }
            
            else { 
                GLfloat winterColor[] = {1, 1, 1, .5};
               glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, winterColor);
                glPointSize(3);
                glPushMatrix();
                 glTranslatef(xPos, height-rainFall, zPos);
                 glutSolidSphere(.05,5,5);
                glPopMatrix();
               // glBegin(GL_POINTS);
               // glVertex3f(xPos,height-rainFall,zPos);
              //  glEnd();
            }
        }
}

void drawBuildings( void) {

			setCurrentShader(windowShader);
			shaderProg->set_uniform_3f("lightDir", -2.0f, 1.0f, 3.0f);
			shaderProg->bind_texture("normalMap", normal_texture_id, GL_TEXTURE_2D, 0);
			shaderProg->bind_texture("colorMap", color_texture_id, GL_TEXTURE_2D, 1);
			shaderProg->bind_texture("cubeMap", skybox_texture_id, GL_TEXTURE_CUBE_MAP, 0);

	        //DRAW TALL CLUSTERS
			drawTallObjects();

	        glPushMatrix();
			glTranslatef(0.0,0.0,10.0);
			drawTallObjects();
	        glPopMatrix();

	        glPushMatrix();
	        glTranslatef(20.0,0.0,0.0);
	        drawTallObjects();
	        glPopMatrix();

	        //DRAW SHORTER CLUSTERS
	        glPushMatrix();
	        glTranslatef(0.0,0.0,-10.0);
	        drawShortObjects();
	        glPopMatrix();

	        glPushMatrix();
	        glTranslatef(10.0,0.0,0.0);
	        drawShortObjects();
	        glPopMatrix();

	        

	        glPushMatrix();
	        glTranslatef(10.0,0.0,-10.0);
	        drawShortObjects();
	        glPopMatrix();

	        glPushMatrix();
	        glTranslatef(-10.0,0.0,-10.0);
	        drawShortObjects();
	        glPopMatrix();

	        glPushMatrix();
	        glTranslatef(10.0,0.0,30.0);
	        drawShortObjects();
	        glPopMatrix();

	        glPushMatrix();
	        glTranslatef(0.0,0.0,20.0);
	        drawShortObjects();
	        glPopMatrix();

	        glPushMatrix();
	        glTranslatef(25.0,0.0,10.0);
	        drawShortObjects();
	        glPopMatrix();

	        glPushMatrix();
	        glTranslatef(-10.0,0.0,10.0);
	        drawShortObjects();
	        glPopMatrix();

	        glPushMatrix();
	        glTranslatef(20.0,0.0,-20.0);
	        drawShortObjects();
	        glPopMatrix();

	        glPushMatrix();
	        glTranslatef(20.0,0.0,-10.0);
	        drawShortObjects();
	        glPopMatrix();

            glPushMatrix();
            glTranslatef(10.0,0.0,-20.0);
            drawShortObjects();
            glPopMatrix();


	        //DRAW SHORTEST OBJECTS
	        glPushMatrix();
	        glTranslatef(0.0,0.0,-20.0);
	        drawVeryShortObjects();
	        glPopMatrix();

            glPushMatrix();
            glTranslatef(-10.0,0.0,0.0);
            drawVeryShortObjects();
            glPopMatrix();

	        glPushMatrix();
	        glTranslatef(-10.0,0.0,-20.0);
	        drawVeryShortObjects();
	        glPopMatrix();

	        glPushMatrix();
	        glTranslatef(20.0,0.0,25.0);
	        drawVeryShortObjects();
	        glPopMatrix();

}

void drawSky( void) {

		setCurrentShader(skyboxShader);
		shaderProg->set_uniform_3f("lightDir", -2.0f, 1.0f, 3.0f);
		shaderProg->bind_texture("normalMap", normal_texture_id, GL_TEXTURE_2D, 0);
		shaderProg->bind_texture("colorMap", color_texture_id, GL_TEXTURE_2D, 1);
		shaderProg->bind_texture("cubeMap", skybox_texture_id, GL_TEXTURE_CUBE_MAP, 0);
		glCallList(skybox_id);

}

void drawReflection( void)
{

	glPushMatrix();
	glScalef(1.0, -1.0, 1.0);
	drawBuildings();
    drawForest();
	glPopMatrix();
}

void drawScene( void )
{
	GLfloat density = 0.3;
	GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};
	glEnable (GL_FOG);
	glFogi (GL_FOG_MODE, GL_EXP2);
	glFogfv (GL_FOG_COLOR, fogColor);
	glFogf (GL_FOG_DENSITY, density);
	glFogf(GL_FOG_START, 0.0f);
    glFogf(GL_FOG_END, 20.0f);
	float currentColor[4];

	glGetFloatv(GL_CURRENT_COLOR, currentColor);

	GLfloat selectedColor[] = {0, 1, 0, 1};
	GLfloat unselectedColor[] = {1, 0, 0, 1};

	// Initialize the name stack
	glInitNames();
	glPushName(0);

	drawReflection();

	glPushMatrix();
	{
		drawSky();
		drawFloor();
		drawForest();
		drawBuildings();
		precipitation();            
	}
	glPopMatrix();


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

static void setupShaders()
{
	printf("MSG: Initialize GLSL Shaders ...\n");

	glewInit();
	if ( !glewIsSupported("GL_VERSION_2_0 GL_ARB_multitexture GL_EXT_framebuffer_object") ) 
	{
		fprintf(stderr, "Required OpenGL extensions missing\n");
		exit(2);
	}

	windowShader = new GLSLProgram(windowVS, windowFS);
	defaultShader = new GLSLProgram(defaultVS, defaultFS);
	skyboxShader = new GLSLProgram(skyVS, skyFS);
	floorShader = new GLSLProgram(floorVS, floorFS);

	normal_texture_id = SOIL_load_OGL_texture(window_file, 
		                               SOIL_LOAD_AUTO,
		                               SOIL_CREATE_NEW_ID,
		                               SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);
	if (normal_texture_id == 0)
		cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << window_file << ")" << endl;

	color_texture_id = SOIL_load_OGL_texture(window_file, 
		                               SOIL_LOAD_AUTO,
		                               SOIL_CREATE_NEW_ID,
		                               SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);
	if (color_texture_id == 0)
		cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << color_file << ")" << endl;

	skybox_texture_id = SOIL_load_OGL_cubemap(skybox_back, skybox_front, 
											  skybox_up, skybox_down, 
											  skybox_right, skybox_left,
											  SOIL_LOAD_RGB,
											  SOIL_CREATE_NEW_ID,
											  0
											);
	if (skybox_texture_id == 0) 
		cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << "skybox" << ")" << endl;
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	asphalt_texture_id = SOIL_load_OGL_texture(asphalt_file, 
		                               SOIL_LOAD_AUTO,
		                               SOIL_CREATE_NEW_ID,
		                               SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);
	if (asphalt_texture_id == 0)
		cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << asphalt_file << ")" << endl;

	perlin_texture_id = SOIL_load_OGL_texture(perlin_file,
		                               SOIL_LOAD_AUTO,
		                               SOIL_CREATE_NEW_ID,
		                               SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);
	if (perlin_texture_id == 0)
		cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << perlin_file << ")" << endl;

	glActiveTexture(GL_TEXTURE0);
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

		// If hit(s) occurred, display the info.
		if(hits != 0)
		{

			// Save current picked object.
			// Take only the nearest selection
			pickedObj = selectBuff[3];

			sprintf (titleString, "You clicked on %d", pickedObj);
			glutSetWindowTitle(titleString);

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

	// if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	// 	processSelection(x, y);

	// if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	// {
	// 	pickedObj = -1;
	// 	glutPostRedisplay();
	// }
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

int state = 0;
int precipCounter = 0;

void updatePrecip( void )
{
    precipCounter++;

    if (rainFall > 20)
        rainFall = 0;
    else
        if (season != 4)
            rainFall+=1.0;
        else
            rainFall+=.4;

    if (rainAngle<20 && state == 1) {
        rainAngle+=1.0;
        state = 1;
    }
    else if (rainAngle == 20) {
        state = 0;
        rainAngle = 19;
    }
    else if (rainAngle > 0 && state == 0) {
        rainAngle-=1.0;
        state = 0;
    }
    else if (rainAngle == 0 && state == 0)
        state = 1;

    if (precipCounter > 90) {
        precipCounter = 0;
    }

    if(counter%200 == 0) {
        if (season == 2)
            season = 4;
        else {
            season = 2;
        }
    }




}

void updateTree( void )
{

    if (season == 2) {
         if (redGlobal < 255)
               redGlobal+=.25;

         if (greenGlobal > 40)
                greenGlobal-=2.0;

         if (blueGlobal > 55)
              blueGlobal-=.5;
    }
    else {
        redGlobal = 134;
        greenGlobal = 189;
        blueGlobal = 75;

    }


}


void update(int value)
{
	counter++;
    
    updatePrecip();

    if (counter > 100) 
    updateTree();
    // fprintf(stderr,"%d\n", counter);

	//SET DIFF CAMERA POSITIONS AND CONDITIONS
	if (camRotX > 730) {
		camRotX-= .09;
	//	camRotY+= .3;
	}
	if (camRotY < 692) {
		camRotY+= .07;
		//camPosZ += .2;
	}
	if (camPosZ < -19) {
	//	camPosZ +=.07;
	}
	display();
	glutTimerFunc(20, update, 0);
}

void randSeeds( void )
{
    int i;
    for (i=0; i<NUM_TREES; i++) {
        seeds[i] = (int)rand();
    }
}


void createTrees( void )
{
    int i, j;
    for (i=0; i<NUM_TREES; i++) {
        GLfloat x = randFloat(0.0,5.0);
        GLfloat z = randFloat(0.0,5.0);
        Tree *newTree = new Tree(x,0.0,z);
        branch *trunk = new branch();
        newTree->trunk = trunk;
        branch *prev = trunk;
        for (j=0; j< 20; j++) {
            branch *curr = new branch();
            prev->child = curr;
            prev = curr;
        }
    }
}

void createObjects( void ) 
{
    Node *nodeOne = new Node(0.0f,0.0f,0.0f,13.0f,3.0f,0,0.06f);
    Node *nodeTwo = new Node(-2.5f,0.0f,0.0f,5.0f,2.0f,50,0.04f);
    Node *nodeThree = new Node(0.0f,0.0f,3.5f,2.0f,2.0f,75,0.06f);
    Node *nodeFour = new Node(0.0f,0.0f,-2.5f,6.0f,2.0f,100,0.08f);
    Node *nodeFive = new Node(-5.0f,0.0f,0.0f,8.0f,1.5f,60,0.07f);
    Node *nodeSix = new Node(2.5f,0.0f,-2.5f,5.0f,2.0f,150,0.10f);
    Node *nodeSeven = new Node(-7.5f,0.0f,2.5f,10.0f,2.0f,100,0.08f);
    Node *nodeEight = new Node(-5.0f,0.0f,2.5f,4.0f,2.0f,65,0.09f);
    Node *nodeNine = new Node(-5.0f,0.0f,-5.0f,18.0f,3.0f,20,0.08f);

    Node *nodeTen = new Node(0.0f,0.0f,0.0f,2.0f,3.0f,50,0.04f);
    Node *nodeElev = new Node(-2.5f,0.0f,0.0f,1.0f,2.0f,100,0.02f);
    Node *nodeTwel = new Node(0.0f,0.0f,3.5f,2.0f,2.0f,125,0.03f);
    Node *nodeThir = new Node(0.0f,0.0f,-2.5f,3.0f,2.0f,150,0.04f);
    Node *nodeFourt = new Node(-5.0f,0.0f,0.0f,2.0f,1.5f,110,0.06f);
    Node *nodeFift = new Node(2.5f,0.0f,-2.5f,3.0f,2.0f,200,0.05f);
    Node *nodeSixt = new Node(-7.5f,0.0f,2.5f,4.0f,2.0f,150,0.04f);
    Node *nodeSevent = new Node(-5.0f,0.0f,2.5f,5.0f,3.0f,120,0.05f);
    Node *nodeEighte = new Node(-5.0f,0.0f,-5.0f,3.0f,3.0f,70,0.06f);

    Node *lowOne = new Node(0.0f,0.0f,0.0f,1.0f,6.0f,100,0.06f);
    Node *lowTwo = new Node(-2.5f,0.0f,0.0f,1.0f,2.0f,150,0.04f);
    Node *lowThree = new Node(0.0f,0.0f,3.5f,0.5f,1.0f,175,0.06f);
    Node *lowFour = new Node(0.0f,0.0f,-2.5f,2.0f,4.0f,200,0.08f);
    Node *lowFive = new Node(-5.0f,0.0f,0.0f,1.0f,1.5f,160,0.07f);
    Node *lowSix = new Node(2.5f,0.0f,-2.5f,0.3f,2.0f,250,0.10f);
    Node *lowSeven = new Node(-7.5f,0.0f,2.5f,4.0f,5.0f,200,0.08f);
    Node *lowEight = new Node(-5.0f,0.0f,2.5f,2.0f,2.0f,165,0.09f);
    Node *lowNine = new Node(-5.0f,0.0f,-5.0f,1.8f,3.0f,120,0.08f); 

    createTrees();
    
    initSky();
}


int main (int argc, char *argv[])
{
	int win_width = 960;
	int win_height = 540;
    randSeeds();
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize( win_width, win_height );

	glutCreateWindow( "Final Project!!!" );
	setupShaders();
	setupRC();
	createObjects();
  

	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mouse );
	glutMotionFunc( motion );
    glutTimerFunc(20, update, 0);
	glutMainLoop();
	delete shaderProg;
}