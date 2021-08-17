/// @file
///
/// @brief - Implements GLbase3dObj - 3D Drawing object
///
/// @copyright 2019 mmashimo - All rights reserved

// For Windows sake
#include "pch.h"

#include <cstddef>
#include <iostream>
#include <stdlib.h>

#include "GL/glbase3d.h"

static constexpr float s_panKeyDelta{0.05f};

//---------------------------------------------------------------------
GLbase3dObj::GLbase3dObj()
    : m_panDelta(s_panKeyDelta, s_panKeyDelta, s_panKeyDelta)
{
    m_timer.setControlObject(this);
}

GLbase3dObj::GLbase3dObj(const GLfloat size)
    : GLbaseDrawObj(size)
    , m_panDelta(s_panKeyDelta, s_panKeyDelta, s_panKeyDelta)
{
    m_timer.setControlObject(this);
}

GLbase3dObj::GLbase3dObj(const GLfloat x, const GLfloat y, const GLfloat z)
    : GLbaseDrawObj(x, y, z)
    , m_panDelta(s_panKeyDelta, s_panKeyDelta, s_panKeyDelta)
{
    m_timer.setControlObject(this);
}

void GLbase3dObj::setStartAngle(const GLfloat angle, const GLfloat x, const GLfloat y, const GLfloat z)
{
    m_angle.set3dAngle(angle, x, y, z);
}

void GLbase3dObj::setSpinAngle(const GLfloat angle, const GLfloat x, const GLfloat y, const GLfloat z)
{
    m_delta.set3dAngle(angle, x, y, z);
}

#if 0
void GLbase3dObj::startAnimation(const int time)
{
    // If start has not started already
    m_timer.setTime(time, true);

    m_spinOnTimer = true;
}

void GLbase3dObj::stopAnimation()
{
    m_timer.m_restart = false;
    m_spinOnTimer = false;
}
#endif

void GLbase3dObj::initTimerControl(GLbaseWin* win)
{
    m_timer.setControlObject(this);
    m_timer.registerController(win);
    std::cout << "GLbase3dObj timer initialized" << std::endl;
}

// Timer update
bool GLbase3dObj::timerUpdate(GLbaseWin* win)
{
    // Change the translation angle
    m_angle += m_delta;
    
    return true;
}

void GLbase3dObj::draw()
{
    m_pan.translate();

    // Set the rotation before setting the color
    m_angle.rotate3d();

    // Set object color
    m_color.useColor();

    // Draw the object
    drawObject();
}

bool GLbase3dObj::specialResponse(bool keyDown, int key, int x, int y)
{
    bool bTime = false;
    bool bUpdate = false;

    if (keyDown)
    {
        if (key == GLUT_KEY_RIGHT)
        {
            m_pan.x() += m_panDelta.x();
            std::cout << "RIGHT key=" << key << " [x=" << x << " y=" << y << "] delta=" << m_panDelta.x() << std::endl;
            bUpdate = true;
        }
        else if (key == GLUT_KEY_LEFT)
        {
            m_pan.x() -= m_panDelta.x();
            bUpdate = true;
            std::cout << "LEFT key=" << key << " [x=" << x << " y=" << y << "] delta=" << m_panDelta.x() << std::endl;
        }
        else if (key == GLUT_KEY_DOWN)
        {
            // Going up - (lower y value)
            m_pan.y() -= m_panDelta.y();
            std::cout << "DOWN key=" << key << " [x=" << x << " y=" << y << "] delta=" << m_panDelta.y() << std::endl;
            bUpdate = true;
        }
        else if (key == GLUT_KEY_UP)
        {
            m_pan.y() += m_panDelta.y();
            std::cout << "UP key = " << key << "[x = " << x << " y = " << y << "] delta = " << m_panDelta.y() << std::endl;
            bUpdate = true;
        }
        else
        {
            std::cout << "Key (unhandled) = " << key << " [x = " << x << " y = " << y << "]" << std::endl;
        }
    }
    else
    {
        std::cout << "KeyDown (unhandled) = " << key << " [x = " << x << " y = " << y << "]" << std::endl;
    }
    
    return bUpdate;
}

