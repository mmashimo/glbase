/// @file
///
/// @brief - Implements GLbase2dObj - 3D Drawing object
///
/// @copyright 2019 mmashimo - All rights reserved

// For Windows sake
#include "pch.h"

#include <cstddef>
#include <iostream>
#include <stdlib.h>

#include "GL/glbase2d.h"

//---------------------------------------------------------------------
GLbase2dObj::GLbase2dObj()
{
    m_timer.setControlObject(this);
}

GLbase2dObj::GLbase2dObj(const GLfloat size)
    : GLbaseDrawObj(size)
{
    m_timer.setControlObject(this);
}

GLbase2dObj::GLbase2dObj(const GLfloat x, const GLfloat y, const GLfloat z)
    : GLbaseDrawObj(x, y, z)
{
    m_timer.setControlObject(this);
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

void GLbase2dObj::initTimerControl(GLbaseWin* win)
{
    m_timer.setControlObject(this);
    m_timer.registerController(win);
    std::cout << "GLbase2dObj timer initialized" << std::endl;
}

// Timer update
bool GLbase2dObj::timerUpdate(GLbaseWin* win)
{
    return false;
}

void GLbase2dObj::draw()
{
    // Set object color
    m_color.useColor();

    // Draw the object
    drawObject();
}

bool GLbase2dObj::specialResponse(bool keyDown, int key, int x, int y)
{
    bool bUpdate = false;

    return bUpdate;
}
