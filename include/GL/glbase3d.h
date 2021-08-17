/// @file
///
/// @brief glbase3d.h - This is a header file for 3D draw panels.
///
/// Description: GLbase3dObj - Base object for drawing objects.
///
/// @copyright 2019 mmashimo - All rights reserved

#pragma once

#include <array>

// the GLUT and OpenGL libraries have to be linked correctly
#include <GL/glut.h>

#include "glbase.h"
#include "glbaseobj.h"
#include "glbasectrl.h"

/// @brief Default 3D drawing objects have specific 3D controls.
///
/// These are unique instances - multiple but cannot be copied
///
/// 3D control objects include:
/// - GLbaseTimer - used fo basic animation
/// - GLbaseMouseKey - for controlling
class GLbase3dObj : public GLbaseDrawObj
{
public:
    GLbase3dObj();

    /// @brief Constructor sets the size
    GLbase3dObj(const GLfloat size);

    /// @brief Constructor
    GLbase3dObj(const GLfloat x, const GLfloat y, const GLfloat z);

    virtual ~GLbase3dObj() {}

    // Set the starting angle for the object
    void setStartAngle(const GLfloat angle = 0.f, const GLfloat x = 0.f, const GLfloat y = 0.f, const GLfloat z =0.f);

    /// @brief Sets the "delta" or spinning ange.
    ///
    /// It must have a non-zero angle to start rotation
    void setSpinAngle(const GLfloat angle, const GLfloat x, const GLfloat y, const GLfloat z);

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

    // Rotation objects
    GLbase3dAngle m_angle;
    GLbase3dAngle m_delta;
    GLbase3dPos   m_pan;
    GLbase3dPos   m_panDelta;

    /// @brief Uses Spin Control when timer updates.

    bool          m_spinOnTimer;

    // Pan/center point
};
