/// @file
///
/// @brief glbase.h - This is a header file for the base startup window controls.
///
/// Description: GLbase - is a light-weight strictly OpenGL windowing program
/// used to create near standard windowing behaviors for rapid prototyping
/// of underlying projects.
///
/// This base object derives:
/// - GLbaseWin - Sub-windows or "Panels"
///
/// GLbase object has containers for:
/// - Sub-windows or "Panels" (GLbaseWin) - Windowing layer
///   This base class derives:
///   - GLbase2D - glbase2d.cpp - 2D Windowing (which includes GUI controls)
///   - GLbase3D - glbase3d.cpp - 3D windowing controls
/// - Draw object Layers (GLbaseLayer) - glbaselayer for drawing layers
/// - Drawing object (GLbaseObj) - glbaseobj.cpp (base class)
/// - Key control object (GLbaseKey) - glbasekey.cpp (base class)
/// - Mouse control object (GLbaseMouse) - glbasemouse.cpp (base class)
///
/// Much of the technology owes its development ideas to GLUI, but few things were
/// noticed when working with GLUI that had to be addressed:
/// (1) It was difficult to create their own GUI from parts of other GUI structures
/// (2) Copy-and-paste method was often used to transfer window behavior
/// (3) Pop-ups and modal dialogs were not easy to create intuitively
/// (4) Hard to transfer code from OWL, MFC or other old time GUIs
///
/// Program execution begins and ends here.
///
/// @copyright 2019 mmashimo - All rights reserved

#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <map>

// #include <GL/glut.h>

#include "glbase.h"
#include "glbasewinprop.h"

#include "glbaseobj.h"
#include "glbasectrl.h"

class GLbaseObj;
class GLbaseControl;
class GLbaseTimer;

using namespace std;

/// @brief Base Windows - Base Window the main GLUT window. 
///
/// Description: THis is a singleton object so it cannot be duplicated or copied
/// Subwindows can be made from this.
///
/// This is, and can be a huge object since this starts the entire OpenGL windows
/// context.
class GLbaseWin : public GLbase
{
public:
    /// @brief Constructor from main
    /// @param[in] argc - number of arguments passed into main fruntion (NOTE: [0] is execution)
    GLbaseWin(int argc, char **argv);

    /// @brief Destructor
    virtual ~GLbaseWin();

    /// @brief Sets the view angle of the Window
    void setViewPosition(const GLfloat x, const GLfloat y, const GLfloat z);

    /// @brief Set background GLbaseColor - default white
    void setBackgroundColor(const GLfloat r = 1.f, const GLfloat g = 1.f, const GLfloat b = 1.f, const GLfloat a = 1.f);

    /// @brief Set ambient color
    void setAmbientColor(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a = 1.f);

    /// @brief Add Drawing objects
    virtual void addObject(GLbaseObj* object);

    /// @brief Initializes Window properties before "showWindow"
    virtual void initializeWindow();

    /// @brief Initializes 3D rendering runs before the main-loop
    virtual void initRendering();

    //----- Override-able Functions -----

    /// @brief Draw a layer
    virtual void draw();

    /// @brief Called when the window is resized
    /// @param[in] w
    /// @param[in] h
    virtual void handleResize(const int w, const int h);

    // Set up for drawing update
    virtual void updateTimer(int value);

    // Need Keyboard function(s)
    virtual void specialKeyResponse(bool keyDown, int key, int x, int y);

    virtual void keyboardResponse(bool keyDown, unsigned char key, int x, int y);

    virtual void mouseMove(int x, int y);

    virtual void mouseAction(int button, int button_state, int x, int y);

    // Creates and shows window
    virtual void showWindow(const std::string& titleText);

    /// @brief Executes OpenGL loop
    virtual void run();

    //----------------------------------------------------------------
    // Functions that really do not need to be overwritten
    //----------------------------------------------------------------

    /// @brief Sets base window size
    void setWindowPosAndSize(int32_t posTop, int32_t posLeft, int32_t szHeight, int32_t szWidth);

    /// @brief Sets up timer
    /// @param[in] time - tick counts in milliseconds
    /// @param[in] value - timer function identify
    void setTimer(const int time, const int value);

    /// @brief Registers timer by setting the timer object.
    /// @param[in] timer - timer control object
    /// By registering, the timer is ready to start
    void registerTimer(GLbaseTimer* timer);

    //----- Data Member -----

protected:
    //----- Method member ---
    /// @brief draws all the items in the list
    void drawObjects();

    /// @brief Initializes callback just before main-loop 'run()'
    void initializeCallbacks();

    /// @brief Initializes timer objects with callbacks
    void initializeTimerCallbacks();


    //----- Data member -----
    GLbaseWinProp   m_winProp;
    GLbaseColor     m_backColor;
    GLbaseColor     m_ambientColor;
    GLbase3dPos     m_viewingPosition;

    /// @brief Handler (ID) to Created Window
    int  m_windowHandle;

private:
    //----- Private Method Member -----


protected:   // TODO: make this private during development

    //----- Data Member -----

    /// @brief Text shown at the top system-bar
    std::string  m_windowText;

    std::vector<GLbaseLighting*> m_lightingObjects;

    /// @brief List of Drawing Items (should be in a panel, which this is)
    std::vector<GLbaseObj*> m_drawObjects;

    /// @brief List of Control objects
    std::vector<GLbaseControl*> m_controlObjects;

    /// @brief List of Timer Control objects
    std::map<int, GLbaseTimer*> m_timerObjects;

    //----- Static Methods ----
    // These are used by GLUT as callbacks that not be dynamically
    // allocated

    /// @brief GLUT callback for drawing function used to redraw
    static void _draw();

    /// @brief Callback for handling resizing
    static void _handleResize(const int w, const int h);

    /// @brief GLUT callback for Timer Function update
    static void _timerUpdate(int value);

    /// @brief Special keys are scan-code 0 keys
    static void _specialKeys(int key, int x, int y);
    static void _specialKeysUp(int key, int x, int y);

    /// @brief GLUT callback for keyboard activity response
    static void _keyboardResponse(unsigned char key, int x, int y);
    static void _keyboardUpResponse(unsigned char key, int x, int y);

    /// @brief GLUT callback for Mouse button response
    static void _mouseAction(int button, int button_state, int x, int y);

    /// @brief GLUT callback for Mouse movement
    static void _mouseMove(int x, int y);

};
