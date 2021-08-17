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
// #include <GL/glut.h>

#include "GL/glbase.h"

using namespace std;

/// @brief Object Identifier is a high-water mark
static uint32_t s_objId{ 0 };

//---------------------------------------------------------
// Base Class Implementation
//---------------------------------------------------------
GLbase::GLbase(const uint32_t id)
    : m_id{ s_objId }
{
    if (id > s_objId)
    {
        // Not desirable, but this will skip IDs
        s_objId = id;
    }
    s_objId++;
}

GLbase::GLbase(const GLbase& ref)
    : m_id{ s_objId }
{
    if (ref.m_id > s_objId)
    {
        // Most likely the object was created outside this object
        s_objId = ref.m_id;
        // One can set an exception if needed
    }
    s_objId++;
}

//----------------------------------------------------------

GLbaseColor::GLbaseColor(const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat alpha)
    : m_color{ red, green, blue, alpha }
    , m_enable{true}
{
}

GLbaseColor::GLbaseColor(const GLbaseColor& ref)
{
    for (int i = 0; i < 4; i++)
    {
        m_color[i] = ref.m_color[i];
    }
    m_enable = ref.m_enable;
}


void GLbaseColor::setColor(const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat alpha)
{
    m_color[0] = red;
    m_color[1] = green;
    m_color[2] = blue;
    m_color[3] = alpha;
    m_enable = true;
}

void GLbaseColor::setWhite()
{
    m_color[0] = 1.f;
    m_color[1] = 1.f;
    m_color[2] = 1.f;
    m_color[3] = 1.f;
    m_enable = true;
}

void GLbaseColor::setBlack()
{
    m_color[0] = 0.f;
    m_color[1] = 0.f;
    m_color[2] = 0.f;
    m_color[3] = 1.f;
    m_enable = true;
}

void GLbaseColor::useColor() const
{
    if (m_enable)
    {
        if (m_color[3] == 1.f)
        {
            // opaque
            glColor3f(m_color[0], m_color[1], m_color[2]);
        }
        else
        {
            // transparent
            glColor4f(m_color[0], m_color[1], m_color[2], m_color[3]);
        }
    }
}

void GLbaseColor::useToClear() const
{
    if (m_enable)
    {
        // Alpha isn't really used, but passed
        glClearColor(m_color[0], m_color[1], m_color[2], m_color[3]);
    }
}

void GLbaseColor::useAmbientLight() const
{
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, m_color);
}

void GLbaseColor::useAsLighting(GLenum light, GLenum pname)
{
    glLightfv(light, pname, m_color);
}

#ifdef YET_TO_CODE
// TODO: these are cool operations, not needed yet
void GLbaseColor::dimBy()
{}

void GLbaseColor::brighten()
{}

void GLbaseColor::setTransparent()
{}
#endif

//----------------------------------------------------------

GLbaseLighting::GLbaseLighting(const GLenum light)
    : m_light{light}
    , m_useDefault{ true }
{
    m_pname = GL_DIFFUSE;
    m_color.setWhite();
}

GLbaseLighting::GLbaseLighting( const GLenum light,
                                const GLenum pname,
                                const GLbaseColor& color,
                                const GLbase3dAngle& angle)
    : m_light{ light }
    , m_pname{ pname }
    , m_color(color)
    , m_angle(angle)
    , m_useDefault(false)
{
}

// Set Lighting name
void GLbaseLighting::setLightingName(const GLenum light)
{
    m_light = light;
}

// Set before using
void GLbaseLighting::setLightingType(const GLenum pname)
{
    m_pname = pname;
}

// Set Lighting color
void GLbaseLighting::setLightingColor(const GLbaseColor& color)
{
    m_color = color;
}

// Set Angle
void GLbaseLighting::setLightingAngle(const GLbase3dAngle& angle)
{
    m_angle = angle;
}

// Use lighting in drawing
void GLbaseLighting::useLighting() const
{
    glLightfv(m_light, m_pname, m_color.asColorArray());
    useLightingAngle();
}

// Use new lighting color (only)
void GLbaseLighting::useLightingColor() const
{
    glLightfv(m_light, m_pname, m_color.asColorArray());
}

// Use new light poistion (only)
void GLbaseLighting::useLightingAngle() const
{
    GLfloat lightPos0[] = { m_angle.x(), m_angle.y(), m_angle.z(), m_angle.angle() };
    glLightfv(m_light, GL_POSITION, lightPos0);
}

//----------------------------------------------------------

GLbase3dPos::GLbase3dPos(const GLfloat x, const GLfloat y, const GLfloat z)
    : m_x{ x } , m_y{ y } , m_z{ z }
{
}

GLbase3dPos::GLbase3dPos(const GLbase3dPos& ref)
    : m_x{ ref.m_x }, m_y{ ref.m_y }, m_z{ ref.m_z }
{
}


void GLbase3dPos::set3dPos(const GLfloat x, const GLfloat y, const GLfloat z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

// Use coordinates to translate
void GLbase3dPos::translate() const
{
//    if ((m_x != 0.f) && (m_y != 0.f) && (m_z != 0.f))
//    {
//        glTranslatef(m_x, m_y, m_z);
//    }
        glTranslatef(m_x, m_y, m_z);
}

void GLbase3dPos::adjustAngle(const GLfloat delta)
{

}

//----------------------------------------------------------
// Angles have an inherent limit of +360 to -360 degrees
static constexpr GLfloat GLAngleLimit{360.f};
static void checkLimit(GLfloat& angle)
{
    if(angle > GLAngleLimit)
    {
        angle -= GLAngleLimit;
    }
    else if (angle < -GLAngleLimit)
    {
        angle += GLAngleLimit;
    }
}
GLbase3dAngle::GLbase3dAngle()
    : m_angle{ 0.f }, m_x{ 0.f }, m_y{ 0.f }, m_z{ 0.f }
{
}

GLbase3dAngle::GLbase3dAngle(const GLbase3dAngle& ref)
    : m_angle{ ref.m_angle }, m_x{ ref.m_x }, m_y{ ref.m_y }, m_z{ ref.m_z }
{

}

GLbase3dAngle::GLbase3dAngle(const GLfloat angle, const GLfloat x, const GLfloat y, const GLfloat z)
    : m_angle{ angle }, m_x{ x }, m_y{ y }, m_z{ z }
{
}

GLbase3dAngle& GLbase3dAngle::operator=(const GLbase3dAngle& ref)
{
    return *this;
}
GLbase3dAngle& GLbase3dAngle::operator+=(const GLbase3dAngle& ref)
{
    m_angle += ref.angle();
//    checkLimit(m_angle);

    m_x += ref.x();
//    checkLimit(m_x);

    m_y += ref.y();
//    checkLimit(m_y);

    m_z += ref.z();
//    checkLimit(m_z);

    return *this;
}

GLbase3dAngle& GLbase3dAngle::operator-=(const GLbase3dAngle& ref)
{
    return *this;
}

GLbase3dAngle GLbase3dAngle::operator+(const GLbase3dAngle& ref)
{
    GLbase3dAngle obj(*this);
    obj += ref;
    return obj;
}
GLbase3dAngle GLbase3dAngle::operator-(const GLbase3dAngle& ref)
{
    GLbase3dAngle obj(*this);
    obj -= ref;
    return obj;
}

void GLbase3dAngle::set3dAngle(const GLfloat angle, const GLfloat x, const GLfloat y, const GLfloat z)
{
    m_angle = angle;
    m_x = x;
    m_y = y;
    m_z = z;
}

void GLbase3dAngle::rotate3d() const
{
    glRotatef(m_angle, m_x, m_y, m_z);
}

// Delta is multiplied (positive) or divided (negative)
void GLbase3dAngle::adjustExponential(const GLbase3dAngle& delta)
{
    if (delta.m_angle < 0.f)
    {
        m_angle /= delta.angle();
    }
    else
    {
        m_angle *= delta.angle();
    }

    if (delta.x() < 0.f)
    {
        m_x /= delta.x();
    }
    else
    {
        m_x *= delta.x();
    }

    if (delta.y() < 0.f)
    {
        m_y /= delta.y();
    }
    else
    {
        m_y *= delta.y();    
    }

    if (delta.z() < 0.f)
    {
        m_z /= delta.z();
    }
    else
    {
        m_z *= delta.z();    
    }
}

