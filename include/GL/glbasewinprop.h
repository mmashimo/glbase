/// @file
///
/// @brief This is the header file for all OpenGL windowing parameters
///


#pragma once

#include <cstdint>

// the GLUT and OpenGL libraries have to be linked correctly
#include <GL/glut.h>

//----------------------------------------------------------
#define DEFAULT_SQUARE_WINDOW_SIZE (800)

class GLbaseWinProp
{
public:
    /// @brief Parameter for a square region of 'size'
    GLbaseWinProp(const int size = DEFAULT_SQUARE_WINDOW_SIZE);

    /// @brief Parater for a rectangular region
    GLbaseWinProp(const int width, const int height, const int left = 0, const int top = 0);

    /// @brief Destructor
    virtual ~GLbaseWinProp() {};

    /// @brief Sets the position without changing the size
    void setWindowPosition(const int x, int y);

    /// @brief Sets the size without changing the position
    void setWindowSize(const int width, const int height);

    //-------------------------------------------------------------------------
    // Utilities used when drawing
    //-------------------------------------------------------------------------

    /// @brief Sets GLUT function for Window initialization
    void useToInitWindow() const;

    /// @brief Use when "resize" if call for the main windowing
    void useToHandleResize(int w, int h);

// private:
    int m_posTop;
    int m_posLeft;
    int m_szWidth;
    int m_szHeight;

    // Used for window perspective
    float m_xyAspect;

    // Used for gluPerspective
    // Angle of view in the yz-plane (must be 0.-180.)
    double m_fovy;
    // Aspect ratio of the frustum
    double m_aspect;
    // Distant between viewpoint and clipping planes z-axis (must be positive #)
    double m_near;
    double m_far;
};




