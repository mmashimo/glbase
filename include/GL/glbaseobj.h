/// @file
///
/// @brief glbaseobj.h - This is a header file for base drawing objects.
///
/// Description: GLbaseObj - Base object for drawing objects.
///
/// This class has:
/// - draw() - virtual, so has to be derived
/// - origin(x,y,z) - center point of the drawing - looking straight on
///
/// This base class is designed to be overwritten.
/// - Use GLdrawContainer - used for a list of GLbaseObj objects.
///
/// @copyright 2019 mmashimo - All rights reserved
#pragma once

#include <vector>

// the GLUT and OpenGL libraries have to be linked correctly
#include <GL/glut.h>

#include "glbase.h"
#include "glbasectrl.h"

//#include "glbasewin.h"
class GLbaseWin;

// Control object index that has not been assigned
#define GLBASE_UNASSIGNED (-1)
#define GLBASE_OBJECT_ENABLED (1)

/// @brief Base Displayable Object having a color
class GLbaseObj : public GLbase
{

public:
    /// @brief Default contructor
    GLbaseObj();

    /// @brief Construct details
    GLbaseObj(const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat alpha = 1.f);

    /// @brief Destructor
    virtual ~GLbaseObj();

    /// @brief Sets the drawing colors
    void setColor(const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat alpha = 1.f);


    /// @brief derive to draw an object
    virtual void draw() = 0;

    /// @brief Initializes the object before Windows come up
    virtual void initControl(GLbaseWin* win);

    /// @brief Initializes the object before Windows come up
    ///
    /// Designed to combine key and mouse inputs
    virtual void initInputControl(GLbaseWin* win);

    /// Used to register, and startup initialized timers
    virtual void initTimerControl(GLbaseWin* win);

    /// @brief Updates control from (timer) controller
    virtual bool timerUpdate(GLbaseWin* win);

    /// @brief Updates key response from parent
    virtual bool specialResponse(bool keyDown, int key, int x, int y);
    virtual bool keyResponse(bool keyDown, unsigned char key, int x, int y);

protected:
    /// @brief Use the color for this object when drawing
    virtual void useColor();

    /// @brief draws the defined object
    virtual void drawObject() = 0;

    /// @brief Basic color (include alpha)
    GLbaseColor m_color;

    /// @brief object state (0 - disabled)
    uint32_t    m_enabled;


    /// @brief Always hit test (unless disabled)
    bool        m_testHit;
};

/// @brief Drawing object that also has a position and a dimensional size
///
/// GLbaseDrawObj is a base object for drawing containers or panels
/// - Sub-windows or "Panels" (GLbaseWin) - Windowing layer
///   This base class derives:
///   - GLbase2D - glbase2d.cpp - 2D Windowing (which includes GUI controls)
///   - GLbase3D - glbase3d.cpp - 3D windowing controls
class GLbaseDrawObj : public GLbaseObj
{
public:
    /// @brief Default Constructor - 0-origin and unity size
    GLbaseDrawObj();

    /// @brief Constructor sets the size
    GLbaseDrawObj(const GLfloat size);

    /// @brief Constructor
    GLbaseDrawObj(const GLfloat x, const GLfloat y, const GLfloat z);

    /// @brief Destructor
    virtual ~GLbaseDrawObj();

    /// @brief Sets the origin position of the drawig object
    void setPosition(const GLfloat x, const GLfloat y, const GLfloat z);

    void setSize(const GLfloat size);

    /// @brief Draw using drawObject()
    virtual void draw();

    /// @brief draws the defined object
    virtual void drawObject();

protected:
    /// @brief Starting or position of origin
    GLbase3dPos m_pos;

    /// @brief General size, of the object
    GLfloat     m_size;

    /// @brief Adds reponses, updates from attached to base drawing object
    GLbaseTimer m_timer;

    /// @brief Container of itself or its derivatives
    std::vector<GLbaseObj*> m_objects;
};

