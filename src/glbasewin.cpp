/// @file
///
/// @brief glbase.cpp - This file contains the base object for startup window.
///
/// Description: GLbase - is a light-weight strictly OpenGL windowing program
/// used to create nnear standard windowing behaviors for rapid prototyping
/// of underlying projects.
///
/// Program execution begins and ends here.
///
/// @copyright 2019 mmashimo - All rights reserved

#include "pch.h"
#include <iostream>
#include <stdlib.h>
#include <functional>

// the GLUT and OpenGL libraries have to be linked correctly
#include <GL/glut.h>

#include "GL/glbasewin.h"
#include "GL/glbase.h"
#include "GL/glbasectrl.h"

static GLbaseWin* s_singleInstance = nullptr;

static constexpr int UnassignedWindowHandle{ -1 };

static int s_timerIndexHighWater{0};

//--------------------------------------------------------------------
// Main Window object
//--------------------------------------------------------------------

GLbaseWin::GLbaseWin(int argc, char **argv)
    : m_ambientColor( 1.f, 1.f, 1.f, 1.f )
    , m_windowHandle(UnassignedWindowHandle)
{
    glutInit(&argc, argv);

    // Request double buffered true color window with Z-buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Since this is a singleton, we have to keep THE pointer
    if (s_singleInstance == nullptr)
    {
        s_singleInstance = this;
    }
    else
    {
        // TODO: We exception handling here - if an apptemt was made to duplicate this
    }

    std::cout << "GLbaseWin initialized" << std::endl;
}

GLbaseWin::~GLbaseWin()
{
    if (!m_drawObjects.empty())
    {
        auto itr = m_drawObjects.begin();
        for (; itr != m_drawObjects.end(); itr++)
        {
            // Delete drawing objects
            delete *itr;
        }
    }
    m_drawObjects.clear();

    if (!m_controlObjects.empty())
    {
        auto itd = m_controlObjects.begin();
        for (; itd != m_controlObjects.end(); itd++)
        {
            delete (*itd);
        }
    }
    m_controlObjects.clear();
}

void GLbaseWin::setViewPosition(const GLfloat x, const GLfloat y, const GLfloat z)
{
    m_viewingPosition.set3dPos(x, y, z);
}

void GLbaseWin::setBackgroundColor(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a)
{
    m_backColor.setColor(r, g, b, a);
}

void GLbaseWin::setAmbientColor(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a)
{
    m_ambientColor.setColor(r, g, b, a);
}

/// @brief Add Drawing objects
void GLbaseWin::addObject(GLbaseObj* object)
{
    // TODO: Insert by layer / priorities
    m_drawObjects.push_back(object);
}


void GLbaseWin::initializeWindow()
{
    // Set background color
    m_backColor.useToClear();

    // Set the viewing position
    m_viewingPosition.set3dPos(0.0f, 0.0f, -5.0f);

    // Set the window size
    m_winProp.useToInitWindow();

    std::cout << "GLbaseWin::initializeWindow initialized" << std::endl;
}


void GLbaseWin::setWindowPosAndSize(int32_t posTop, int32_t posLeft, int32_t szHeight, int32_t szWidth)
{
    glutInitWindowSize(szWidth, szHeight);
    glutInitWindowPosition(posLeft, posTop);
    std::cout << "GLbase::setWindowPosAndSize()" << std::endl;
}

void GLbaseWin::setTimer(const int time, const int value)
{
    glutTimerFunc(time, _timerUpdate, value);
}

void GLbaseWin::registerTimer(GLbaseTimer* timer)
{
    if (m_timerObjects.empty())
    {
        m_timerObjects.insert(std::pair<int, GLbaseTimer*>(s_timerIndexHighWater, timer));
        timer->m_index = s_timerIndexHighWater;
        s_timerIndexHighWater++;
    }
    else
    {
        // New timer object is indexed to 0
        auto found = m_timerObjects.find(timer->m_index);
        if (found != m_timerObjects.end() && (found->second == timer))
        {
            // We have the object already registered
        }
        else
        {
            if (timer->m_index != 0)
            {
                s_timerIndexHighWater = timer->m_index + 1;
            }
            else
            {
                timer->m_index = s_timerIndexHighWater;
                s_timerIndexHighWater++;
            }
            
            // Index is set (somehow) - use it, but it becomes the high-water
            m_timerObjects.insert(std::pair<int, GLbaseTimer*>(timer->m_index, timer));
        }
    }
}


void GLbaseWin::showWindow(const std::string& titleText)
{
    // Request double buffered true color window with Z-buffer
    initializeWindow();

    m_windowHandle = glutCreateWindow(titleText.c_str());

    m_windowText = titleText;

    std::cout << "GLbaseWin::showWindow started" << std::endl;
}

/// @brief Executes OpenGL loop
void GLbaseWin::run()
{
    // Once Window is up, we need to initialize initRendering
    initRendering();

    // Start the run
    glutMainLoop();
}

void GLbaseWin::initRendering()
{
    std::cout << "GLbaseWin::initRendering started" << std::endl;

    // Always Enable Z-buffer depth test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    // Set "erase color" for background
    m_backColor.useToClear();

    // Enable all lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    // Set the ambient lighting
    m_ambientColor.useAmbientLight();

    GLfloat lightPos0[] = { -5.f, 5.f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    // Initialize objects
    auto itr = m_drawObjects.begin();
    for (; itr != m_drawObjects.end(); itr++)
    {
        (*itr)->initControl(this);
    }

    // Initialize callback functions
    initializeCallbacks();

    // Setup Enabled timer objects
    initializeTimerCallbacks();

    std::cout << "GLbaseWin::initRendering finished" << std::endl;
}

// Called when the window is resized
void GLbaseWin::handleResize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
    std::cout << "GLbaseWin::handleResize() called width=" << w << " height=" << h << std::endl;
}

/// @brief Draw a layer
/// NOTE: 
void GLbaseWin::draw()
{
    // Reset transformations
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Clear GL buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set viewing position - Position ambient light
    m_viewingPosition.translate();

    // Draws all the objects in the draw-object list
    drawObjects();

    //---------------------------------------------
    // Do the actual drawing by pushing it out to buffer
    glFlush();
    glutSwapBuffers();
}

void GLbaseWin::drawObjects()
{
    if (!m_drawObjects.empty())
    {
        auto itr = m_drawObjects.begin();
        for (; itr != m_drawObjects.end(); itr++)
        {
            try
            {
                (*itr)->draw();
            }
            catch (...)
            {
                std::cout << "!!! Drawing object exception" << std::endl;
            }
        }
    }
}

// Set up for drawing update
void GLbaseWin::updateTimer(int value)
{
    // If no timer objects, nothing to update
    if (m_timerObjects.empty())
    {
        return;
    }

    // Update object for timer
    auto timerObj = m_timerObjects.find(value);
    // If object not found, exit
    if (timerObj == m_timerObjects.end())
    {
        return;
    }

    bool updateView = false;

    GLbaseTimer* timer = timerObj->second;

    if ((timer != nullptr) && (timer->isRunning()))
    {
        GLbaseObj* grfObject = timer->getObject();
        if (grfObject != nullptr)
        {
            updateView = grfObject->timerUpdate(this);
        }
        else
        {
            auto itr = m_drawObjects.begin();
            for (; itr != m_drawObjects.end(); itr++)
            {
                // Update all objects even if we did not find a timer object
                updateView |= (*itr)->timerUpdate(this);
            }
        }

        // Redisplay - this should be result of the update
        if (updateView)
        {
            glutPostRedisplay();
        }

        // Check if timer object needs to be updated
        timer->updateTimer(this);
    }
    else
    {
        // timer object not found or is no longer running,
        // let timer update expire
    }
}

// Special Key function(s) - glutSpecialFunc()
void GLbaseWin::specialKeyResponse(bool keyDown, int key, int x, int y)
{
    if (!m_drawObjects.empty())
    {
        bool handled = false;
        // Get the timer object
        auto itr = m_drawObjects.begin();
        for (; itr != m_drawObjects.end(); itr++)
        {
            handled |= (*itr)->specialResponse(keyDown, key, x, y);
        }

        // Redraw as needed
        if (handled)
        {
            // Request display update - could be done thru flags
            glutPostRedisplay();
        }
    }
}

void GLbaseWin::keyboardResponse(bool keyDown, unsigned char key, int x, int y)
{
    if (!m_drawObjects.empty()) 
    {
        // Get the timer object
        auto itr = m_drawObjects.begin();
        for (; itr != m_drawObjects.end(); itr++)
        {
            (*itr)->keyResponse(keyDown, key, x, y);
        }
    }
}

// 
void GLbaseWin::mouseMove(int x, int y)
{
    std::cout << "GLbaseWin::mouseMove base class: [x:" << x << " y:" << y << "]" << std::endl;
}

void GLbaseWin::mouseAction(int button, int button_state, int x, int y)
{
    std::cout << "GLbaseWin::mouseAction base class: [button:" << button << " state:" << button_state << "] [x:" << x << " y:" << y << "]" << std::endl;
}

void GLbaseWin::initializeCallbacks()
{
    // Instance dependent function
    glutDisplayFunc(_draw);

    // Resize() - Base or Overridden function
    glutReshapeFunc(_handleResize);

    // Always setup special keyboard function
    glutSpecialFunc(_specialKeys);
    glutSpecialUpFunc(_specialKeysUp);

    // TODO: initialize keyboard response
    glutKeyboardFunc(_keyboardResponse);
    glutKeyboardUpFunc(_keyboardUpResponse);

    // TODO: initialize mouse actions
    glutMouseFunc(_mouseAction);  // Mouse with mouse buttons
    glutMotionFunc(_mouseMove);   // Generic mouse movements
}

void GLbaseWin::initializeTimerCallbacks()
{
    auto itr = m_drawObjects.begin();
    for (; itr != m_drawObjects.end(); itr++)
    {
        (*itr)->initTimerControl(this);
    }

    // Go through timer objects,
   int enabledCount = 0;
    if (!m_timerObjects.empty())
    {
        for (auto ittr = m_timerObjects.begin(); ittr != m_timerObjects.end(); ittr++)
        {
            ittr->second->restartTimer(this);
        }
        std::cout << "GLbase::initializeCallbacks() started " << enabledCount << " timers" << std::endl;
    }
    else
    {
        // Use timer objects registered with drawing objects
        std::cout << "GLbase::initializeCallbacks() - has no timer object" << std::endl;
    }

}


void GLbaseWin::_draw()
{
    //--------------------------------------------
    // Draw
    if (s_singleInstance != nullptr)
    {
        s_singleInstance->draw();
    }
}

void GLbaseWin::_handleResize(const int w, const int h)
{
    if (s_singleInstance != nullptr)
    {
        s_singleInstance->handleResize(w, h);
    }
}

void GLbaseWin::_timerUpdate(int value)
{
    if (s_singleInstance != nullptr)
    {
        s_singleInstance->updateTimer(value);
    }
}

void GLbaseWin::_specialKeys(int key, int x, int y)
{
    if (s_singleInstance != nullptr)
    {
        s_singleInstance->specialKeyResponse(true, key, x, y);
    }
}

void GLbaseWin::_specialKeysUp(int key, int x, int y)
{
    if (s_singleInstance != nullptr)
    {
        s_singleInstance->specialKeyResponse(false, key, x, y);
    }
}

void GLbaseWin::_keyboardResponse(unsigned char key, int x, int y)
{
    if (s_singleInstance != nullptr)
    {
        s_singleInstance->keyboardResponse(true, key, x, y);
    }
}

void GLbaseWin::_keyboardUpResponse(unsigned char key, int x, int y)
{
    if (s_singleInstance != nullptr)
    {
        s_singleInstance->keyboardResponse(false, key, x, y);
    }
}

void GLbaseWin::_mouseAction(int button, int button_state, int x, int y)
{
    if (s_singleInstance != nullptr)
    {
        s_singleInstance->mouseAction(button, button_state, x, y);
    }
}

void GLbaseWin::_mouseMove(int x, int y)
{
    if (s_singleInstance != nullptr)
    {
        s_singleInstance->mouseMove(x, y);
    }
}
