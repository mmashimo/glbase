// Example program to create a basic GLUT window and 3D rotate using arcball
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
#include "GL/glbase3d.h"
#include "GL/glbasectrl.h"

using namespace std;

//int wireframe = 0;
//int segments = 8;
//int main_window = -1;

static int     s_time{25};
static float   s_angle{0.f};
static float   s_angleDelta{1.f};

class Teapot : public GLbase3dObj
{
public:
    Teapot()
        : GLbase3dObj(0.5f)
    {
        // Set the object teapot color here
        setColor(0.f, 0.5f, 0.5f, 1.f);

        m_angle.set3dAngle(s_angle, s_angle, s_angle, s_angle);
        m_delta.set3dAngle(s_angleDelta, s_angleDelta, s_angleDelta, s_angleDelta);

        m_timer.setTime(25, true);
        m_timer.setControlObject(this);
    }

    virtual ~Teapot() {}

    // Base object to draw
    virtual void drawObject()
    {
        // Draw the object
        glutSolidTeapot( m_size );
    }


};


// Override the MainWindow handle to do just one thing
// Everything is in-lined
class RotateTeapot : public GLbaseWin
{
public:
    /// @brief Default constructor
    RotateTeapot(int argc, char **argv)
        : GLbaseWin(argc, argv)
    {
        // Set the viewing location
        setViewPosition(0.0f, 0.0f, -5.0f);

        // Set background "erase color" as white (default color)
        setBackgroundColor(0.f, 0.f, 0.f);

        // Set background ambient color
        setAmbientColor( 0.5f, 0.5f, 0.5f, 1.f );

        // Add the drawing object
        addObject(new Teapot());

        std::cout << "RotateTeapot initialized" << std::endl;
    }

    /// @brief Destructor
    virtual ~RotateTeapot() {}

};


//-------- Main Function ----------

int main(int argc, char **argv)
{
    RotateTeapot TeapotWin(argc, argv);

    // Add the drawing object
    // TeapotWin.addObject(new Teapot());

    TeapotWin.showWindow("Rotating Teapot");

    TeapotWin.run();

    return 0;
}
