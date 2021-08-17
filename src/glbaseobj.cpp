/// @file
///
/// @brief - Implements GLbaseObj - Base Drawing object
///
/// @copyright 2019 mmashimo - All rights reserved

// For Windows sake
#include "pch.h"

#include <cstddef>
#include <iostream>
#include <stdlib.h>

#include "GL/glbase.h"
#include "GL/glbaseobj.h"
#include "GL/glbasewin.h"

GLbaseObj::GLbaseObj()
    : m_color()    // Your basic invisible black
{
}

GLbaseObj::GLbaseObj(const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat alpha)
    : m_color{ red, green, blue, alpha }
    , m_enabled(GLBASE_OBJECT_ENABLED)
    , m_testHit{true}
{

}

/// @brief Destructor
GLbaseObj::~GLbaseObj()
{

}

void GLbaseObj::setColor(const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat alpha)
{
    m_color.setColor(red, green, blue, alpha);
}

/// @brief Initializes the object before Windows come up
void GLbaseObj::initControl(GLbaseWin* win)
{
    std::cout << "Base Object timer initialized" << std::endl;
}

void GLbaseObj::initInputControl(GLbaseWin* win)
{
    std::cout << "Base Object Input Control initialized" << std::endl;
}

void GLbaseObj::initTimerControl(GLbaseWin* win)
{
    std::cout << "Base GLbaseObj timer initialized" << std::endl;
}

/// @brief Updates control from controller
bool GLbaseObj::timerUpdate(GLbaseWin* win)
{
    std::cout << "Base drawing object GLbaseObj got timerUpdate (does nothing)" << std::endl;

    return false;
}

bool GLbaseObj::specialResponse(bool keyDown, int key, int x, int y)
{
    if (keyDown)
        std::cout << "Base GLbaseObj::specialReponse (DOWN does nothing) [key:" << key << " x:" << x << " y:" << y << "]" << std::endl;
    else
        std::cout << "Base GLbaseObj::specialReponse (UP does nothing) [key:" << key << " x:" << x << " y:" << y << "]" << std::endl;
    
    return false;
}

bool GLbaseObj::keyResponse(bool keyDown, unsigned char key, int x, int y)
{
    if (keyDown)
        std::cout << "Base GLbaseObj::keyResponse (DOWN does nothing) [key:" << (int)key << " x:" << x << " y:" << y << "]" << std::endl;
    else
        std::cout << "Base GLbaseObj::keyResponse  (UP does nothing)  [key:" << (int)key << " x:" << x << " y:" << y << "]" << std::endl;
    
    return false;
}

void GLbaseObj::useColor()
{
    m_color.useColor();
}

//---------------------------------------------------------
// Drawing object
//---------------------------------------------------------

GLbaseDrawObj::GLbaseDrawObj()
    : m_pos{ 0.f, 0.f, 0.f }
    , m_size{ 1.f }
{
    m_timer.setControlObject(this);
}

GLbaseDrawObj::GLbaseDrawObj(const GLfloat size)
    : m_pos{0.f, 0.f,  0.f}
    , m_size(size)
{
    m_timer.setControlObject(this);
}

GLbaseDrawObj::GLbaseDrawObj(const GLfloat x, const GLfloat y, const GLfloat z)
    : m_pos{ x, y, z }
    , m_size{ 1.f }
{
    setPosition(x, y, z);
    m_timer.setControlObject(this);
}

GLbaseDrawObj::~GLbaseDrawObj()
{
}

#if 0
void GLbaseDrawObj::setTimerControl(GLbaseTimer *timer)
{
    // To prevent this timer object from being deleted from main list,
    // set the timer object object list
    timer->setControlObject(this);
    m_timer = *timer;
}
void GLbaseDrawObj::initTimerControl(GLbaseWin* win)
{
    m_timer.setControlObject(this);
    m_timer.registerController(win);
    std::cout << "Base Object GLbaseDrawObj timer initialized" << std::endl;
}
#endif


void GLbaseDrawObj::draw()
{
    // Move object, if necessary
    m_pos.translate();

    // Most all objects have 
    m_color.useColor();

    // Draw
    drawObject();
}


void GLbaseDrawObj::drawObject()
{
    // This has to be overwritten to draw something other than its list
    auto itr = m_objects.begin();
    for (; itr != m_objects.begin(); itr++)
    {
        (*itr)->draw();
    }
}

void GLbaseDrawObj::setPosition(const GLfloat x, const GLfloat y, const GLfloat z)
{
    m_pos.set3dPos(x, y, z);
}

void GLbaseDrawObj::setSize(const GLfloat size)
{
    m_size = size;
}

//--------------------------------------------------
