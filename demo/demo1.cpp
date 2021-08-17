// Example program to overwrite the entire base functions
//
// g++ demo1.cpp -lm -lglbase -lglut -lGL -lGLU -o demo1
// -lm - math
// -lglbase - test if installed
// -l(GLUT objects)

#include "pch.h"

#include <iostream>
#include <stdlib.h>

// the GLUT and OpenGL libraries have to be linked correctly
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// For "windowing" (or "panels")
#include "GL/glbase.h"
#include "GL/glbasewin.h"
// For drawing object
#include "GL/glbaseobj.h"

using namespace std;

int wireframe = 0;
int segments = 8;
int main_window = -1;

// The coordinates for the vertices of the cube
static GLfloat s_x = 0.6f;
static GLfloat s_y = 0.6f;
static GLfloat s_z = 0.6f;

static int     s_time = 25;
static float   s_angle = 0.f;
static float   s_angleDelta = 1.f;

// Override the Main Window to do just one thing
class RotateCube : public GLbaseWin
{
public:
    /// @brief Default constructor
    RotateCube(int argc, char **argv)
        : GLbaseWin(argc, argv)
    {
        std::cout << "GLbaseWin initialized" << std::endl;
    }

    /// @brief Destructor
    virtual ~RotateCube()
    { // Nothing here
    }

    // Called just before showWindow
    virtual void initializeWindow()
    {
        // Set the window size
        glutInitWindowSize(700, 700);
        glutInitWindowPosition(100, 100);

        // Add a timer for the update(...) function
        setTimer(25, 0);

        std::cout << "GLbaseWin::initializeWindow initialized" << std::endl;
    }

    // Called just before run() (main_loop)
    virtual void initRendering()
    {
        // Always Enable Z-buffer depth test
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_COLOR_MATERIAL);

        // Set the color of the background
        // glClearColor(0.7f, 0.8f, 1.0f, 1.0f);
        glClearColor(0.f, 0.f, 0.f, 0.f);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_NORMALIZE);

        // Initialize callback functions
        initializeCallbacks();
    }

    /// Draws the cube
    virtual void draw()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Reset transformations
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Set viewing position - Position ambient light
        glTranslatef(0.0f, 0.0f, -5.0f);

        // GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
        GLfloat ambientColor[] = { .5f, .5f, .5f, 0.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

        // Add a positioned light
        // GLfloat lightPos0[] = { 4.0f, 0.0f, 8.0f, 1.0f };
        GLfloat lightPos0[] = { 4.0f, 0.0f, 5.0f, 1.0f };
        GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
        // glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
        glLightfv(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, lightColor0);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

        //--------------------------------------------
        // Position object before drawing
        // glTranslatef(0.5f, 1.0f, 0.0f);
        glTranslatef(0.f, 0.f, 0.0f);

        // Rotate object
        glRotatef(s_angle, s_angle, s_angle, s_angle);

        // Create drawing object
        glColor3f(0.f, 0.5f, 0.f);
        glutSolidTeapot( .5 );

        //---------------------------------------------
        // Do the actual drawing by pushing it out to buffer
        glFlush();
        glutSwapBuffers();
    }

    // Set up for drawing update
    // Function for increasing the angle variable smoothly, 
    // keeps it <=360
    // It can also be implemented using the modulo operator.
    virtual void updateTimer(int value)
    {
        s_angle += s_angleDelta;
        if (s_angle > 360)
        {
            s_angle -= 360;
        }
    }

    // Set up keyboard function - even if not used
    virtual void keyResponse(bool keyDown, int key, int x, int y)
    {
        bool bTime = false;
        bool bUpdate = false;

        if (key == GLUT_KEY_RIGHT)
        {
            s_angleDelta *= 1.5f;
            std::cout << "RIGHT " << s_time << std::endl;
            bUpdate = true;
        }
        else if (key == GLUT_KEY_LEFT)
        {
            s_angleDelta /= 1.5f;
            if (s_angleDelta < 0.05f)
            {
                s_angleDelta = 0.05f;
            }
            bUpdate = true;
            std::cout << "LEFT " << s_time << std::endl;
        }
        else if (key == GLUT_KEY_DOWN)
        {
            s_x -= 0.1f;
            s_y -= 0.1f;
            s_z -= 0.1f;
            std::cout << "DOWN" << std::endl;
            bUpdate = true;
        }
        else if (key == GLUT_KEY_UP)
        {
            s_x += 0.1f;
            s_y += 0.1f;
            s_z += 0.1f;
            std::cout << "UP" << std::endl;
            bUpdate = true;
        }

        if (bUpdate)
        {
            // Request display update - could be done thru flags
            glutPostRedisplay();
        }
    }
};


//-------- Main Function ----------

int main(int argc, char **argv)
{
    RotateCube cube(argc, argv);

    cube.showWindow("Rotating Cube");

    cube.run();

    return 0;
}
