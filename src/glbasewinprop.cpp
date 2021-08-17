


#include "GL/glbasewinprop.h"

//----------------------------------------------------------
GLbaseWinProp::GLbaseWinProp(const int size)
    : m_posTop{ 0 }
    , m_posLeft{ 0 }
    , m_szWidth{ size }
    , m_szHeight{ size }
    , m_xyAspect{1.f}    // It's a square
    , m_fovy{45.f}
{

}

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

GLbaseWinProp::GLbaseWinProp(const int width, const int height, const int left, const int top)
    : m_posTop{ top }
    , m_posLeft{ left }
    , m_szWidth{ width }
    , m_szHeight{ height }
    , m_xyAspect((float)width/(float)height)
{
    // Use basic components
}

/// @brief Sets the position without changing the size
void GLbaseWinProp::setWindowPosition(const int x, int y)
{
    m_posLeft = x;
    m_posTop = y;
}

/// @brief Sets the size without changing the position
void GLbaseWinProp::setWindowSize(const int width, const int height)
{

}

void GLbaseWinProp::useToInitWindow() const
{
    glutInitWindowSize(m_szWidth, m_szHeight);
    glutInitWindowPosition(m_posLeft, m_posTop);
}

void GLbaseWinProp::useToHandleResize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}
