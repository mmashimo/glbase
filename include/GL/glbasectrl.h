/// @file
///
/// @brief glbasectrl.h - This is a header file for window controls
///
/// Description: GLbaseControl - Base object for Window Control objects
///
/// Derived classes:
///     - GLbaseTimer - Control base timers
///     - GLbaseRotate - Controls translations for drawing objects
///     - 
///
/// @copyright 2019 mmashimo - All rights reserved
#pragma once

#include <array>

// the GLUT and OpenGL libraries have to be linked correctly
#include <GL/glut.h>

#include "glbase.h"

class GLbaseWin;
class GLbaseObj;

enum ControlType
{
    GLBASE_GENERIC = 0,
    GLBASE_IDLE_FUNCTION,
    GLBASE_TIMER_CONTROL,
    GLBASE_KEYBOARD_CONTROL,
    GLBASE_MOUSE_CONTROL,
    GLBASE_ROTATE_CONTROL,
};

// Control object index that has not been assigned
#define GLBASE_UNASSIGNED (-1)


/// @brief Control Object Base class
///
/// Description: This object is used for all controls associated with
/// drawing or window objects.
/// All "forms" (display objects) have observer-notifier constructs
///
class GLbaseControl : public GLbase
{
public:
    /// @brief Constructor
    GLbaseControl(int type = 0, GLbaseObj* object = nullptr, int index = GLBASE_UNASSIGNED);

    /// @brief Destructor
    virtual ~GLbaseControl();

    /// @brief Sets its own timer object
    virtual void setControlObject(GLbaseObj* obj);

    /// @brief Register Control object with Windows
    virtual void registerController(GLbaseWin* winObj) {};

    /// @brief Object Initialization
    virtual void init(GLbaseWin* winObj);
    virtual void init(GLbaseObj* drawObj);

#if 0
    /// @brief Override-able Update function - updates when timer expires
    virtual void update(GLbaseWin* winObj, GLbaseObj* drawObj);
#endif

    //----- Accessor functions -----
    GLbaseObj* getObject() { return m_grfObject; }


    virtual bool isIdle() const     { return false; }
    virtual bool isTimer() const    { return false; }
    virtual bool isKeyboard() const { return false; }
    virtual bool isMouse()          { return false; }
    virtual bool isRotate() const   { return false; }


    /// @brief Gets Object type
    int getType() const  { return m_type; }

    /// @brief Gets Object index
    int getIndex() const { return m_objectIndex; }

    /// @brief Checks if object is m_enabled
    bool isEnabled() const  { return m_enabled; }

protected:
    bool m_enabled;

private:
    int  m_type;
    int  m_objectIndex;

    // This is an object declaration to prevent main Window object
    // from deleting this object defined, elsewhere
    GLbaseObj *m_grfObject;
};

/// @brief Timer Control object
///
/// Description: Attaches Timer control to window (panel) or drawing object
class GLbaseTimer : public GLbaseControl
{
public:
    /// @brief Constructor
    /// @param[in] time
    /// @param[in] index
    /// @param[in] restart
    GLbaseTimer(GLbaseObj* object = nullptr, const int time = 0, const bool restart = true);

    /// @brief Destructor
    virtual ~GLbaseTimer();

    /// @brief Sets timer time - turns on restart unless specified
    // NOTE: The time runs 
    void setTime(const int time, const bool restart = true);

    int getTime() const { return m_time; }

    /// @brief - check if timer is running
    bool isRunning() const { return m_running; }

    /// @brief Starts timer if not running
    void restartTimer(GLbaseWin* winObj);

    /// @brief stops timer on the next update, if running
    void stopTimer();

    /// @brief register controller to base window
    virtual void registerController(GLbaseWin* winObj);

    /// @brief Updates timer object from main window
    virtual bool updateTimer(GLbaseWin* winObj);

    virtual bool isTimer() const    { return true; }

    int  m_index;
    int  m_time;
    bool m_restart;
    bool m_running;
};

class GLbaseKeyControl : public GLbaseControl
{
public:
    GLbaseKeyControl(GLbaseObj* object = nullptr);

    virtual ~GLbaseKeyControl() {}

    /// @brief register controller to base window
    virtual void registerController(GLbaseWin* winObj);

    /// @brief Updates key entries from main window
    virtual bool keyResponse(int key, int x, int y);

    virtual bool isKeyboard() const { return true; }
};

class GLbaseMouseControl : public GLbaseControl
{
public:
    GLbaseMouseControl(GLbaseObj* object = nullptr);

    virtual ~GLbaseMouseControl() {}

    /// @brief register controller to base window
    virtual void registerController(GLbaseWin* winObj);

    virtual bool isMouse() { return true; }
};

/// @brief Drawing Rotatation Control
class GLbaseRotate : public GLbaseControl
{
public:
    /// @brief Constructor
    GLbaseRotate(bool animate = false, GLbaseObj* object = nullptr);

    /// @brief Destructor
    virtual ~GLbaseRotate() {}

    void setRotateAngle(const GLfloat angle, const GLfloat x, const GLfloat y, const GLfloat z);

    void setDeltaAngle(const GLfloat angle, const GLfloat x, const GLfloat y, const GLfloat z);

    virtual bool isRotate() const   { return true; }

    // Angle to set the translation
    GLbase3dAngle m_angle;
    // Spin control sets
    GLbase3dAngle m_delta;
    GLbaseTimer   m_timer;

    // Used to sets object (used to spin the object)
    bool          m_animate;

};
