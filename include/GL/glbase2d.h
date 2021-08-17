/// @file
///
/// @brief glbase2d.h - This is a header file for 2D panels.
///
/// Description: GLbase2dObj - Base object for drawing objects.
///
/// @copyright 2019 mmashimo - All rights reserved

#pragma once

#include <array>

// the GLUT and OpenGL libraries have to be linked correctly
#include <GL/glut.h>

#include "glbase.h"
#include "glbaseobj.h"
#include "glbasectrl.h"

/// @brief Default 2D drawing objects have specific normal controls.
///
/// These are unique starting instances - multiple but cannot be copied
///
/// 2D control objects include:
/// - GLbaseTimer - used fo basic animation
/// - GLbaseMouseKey - for scrolling, menus, etc.
class GLbase2dObj : public GLbaseDrawObj
{
public:
    GLbase2dObj();

    /// @brief Constructor sets the size
    GLbase2dObj(const GLfloat size);

    /// @brief Constructor
    GLbase2dObj(const GLfloat x, const GLfloat y, const GLfloat z);

    virtual ~GLbase2dObj() {}

#if 0
    /// @brief Sets the object rotating
    void startAnimation(const int time);

    /// @brief Stops animation, 
    void stopAnimation();
#endif

    /// @brief Initializes timer control
    virtual void initTimerControl(GLbaseWin* win);

    /// @brief Updates control from (timer) controller
    virtual bool timerUpdate(GLbaseWin* win);

    /// @brief Handle Special keys
    virtual bool specialResponse(bool keyDown, int key, int x, int y);

    /// @brief draws the defined object
    virtual void draw();

#if 0
    // NOTE: drawing object has to be derived
    virtual void drawObject();
#endif

    // Pan/center point
};
