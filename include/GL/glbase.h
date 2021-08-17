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

// GLUT declarations needed here
#include <GL/glut.h>

using namespace std;

class GLbase
{
public:
    /// @brief Default constructor
    GLbase(const uint32_t id = 0);

    /// @brief Copy Contrustor
    GLbase(const GLbase& ref);

    /// @brief Destructor
    virtual ~GLbase(){};

protected:
    uint32_t m_id;

private:
};


/// @brief This object handles glutSetColor function from this object
class GLbaseColor
{
public:
    /// @brief Default constructor
    // Creates your basic black
    GLbaseColor(const GLfloat red = 0.f, const GLfloat green = 0.f, const GLfloat blue = 0.f, const GLfloat alpha = 1.f);

    /// @brief Copy constructor
    GLbaseColor(const GLbaseColor& ref);

    ~GLbaseColor() {}

    void setColor(const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat alpha);

    void setWhite();
    void setBlack();

    void enable(bool set = true) { m_enable = set; }
    bool isEnabled() const { return m_enable; }

    /// @brief uses color for most all objects
    void useColor() const;

    /// @brief sets the background color (for clearing)
    void useToClear() const;

    void useAmbientLight() const;

    void useAsLighting(GLenum light, GLenum pname);

#ifdef YET_TO_CODE
    // TODO: these are cool operations, not needed yet
    void dimBy();

    void brighten();

    void setTransparent();
#endif

    // Accessors
    GLfloat red() const   { return m_color[0]; }
    GLfloat green() const { return m_color[1]; }
    GLfloat blue() const  { return m_color[2]; }
    GLfloat alpha() const { return m_color[3]; }

    const GLfloat* asColorArray() const { return static_cast<const GLfloat*>(m_color);  }

protected:
    /// @brief Basic color (include alpha)
    GLfloat m_color[4];
    bool    m_enable;
};


//----------------------------------------------------------
// Basic Point in space defined by 3 points.
// Position can be 3-coordinates; 3-axis angle; any 3-point
// descriptor
//----------------------------------------------------------
class GLbase3dPos
{
public:
    /// @brief Default constructor
    GLbase3dPos(const GLfloat x = 0.f, const GLfloat y = 0.f, const GLfloat z = 0.f);

    /// @brief Copy constructor
    GLbase3dPos(const GLbase3dPos& ref);

    // Inline destructor
    ~GLbase3dPos() {}

    // Sets the variables, if not set already
    void set3dPos(const GLfloat x, const GLfloat y, const GLfloat z);

    // Use coordinates to translate
    void translate() const;

    void adjustAngle(const GLfloat delta);

    const GLfloat& x() const { return m_x; }
    const GLfloat& y() const { return m_y; }
    const GLfloat& z() const { return m_z; }

    GLfloat& x() { return m_x; }
    GLfloat& y() { return m_y; }
    GLfloat& z() { return m_z; }

// protected:
    GLfloat m_x;
    GLfloat m_y;
    GLfloat m_z;
};

//----------------------------------------------------------
// Used for 4-position angle such as rotation
//----------------------------------------------------------
class GLbase3dAngle
{
public:
    GLbase3dAngle();

    GLbase3dAngle(const GLbase3dAngle& ref);

    GLbase3dAngle(const GLfloat angle, const GLfloat x, const GLfloat y, const GLfloat z);

    ~GLbase3dAngle(){}

    // Operators
    // Assignment operators
    GLbase3dAngle& operator=(const GLbase3dAngle& ref);
    GLbase3dAngle& operator+=(const GLbase3dAngle& ref);
    GLbase3dAngle& operator-=(const GLbase3dAngle& ref);

    GLbase3dAngle operator+(const GLbase3dAngle& ref);
    GLbase3dAngle operator-(const GLbase3dAngle& ref);

    void set3dAngle(const GLfloat angle, const GLfloat x, const GLfloat y, const GLfloat z);

    void rotate3d() const;

    // Delta is multiplied (positive) or divided (negative)
    // Note: If you just multiply with negative no. it will oscillate
    void adjustExponential(const GLbase3dAngle& delta);

    GLfloat angle() const { return m_angle; }
    GLfloat x() const { return m_x; }
    GLfloat y() const { return m_y; }
    GLfloat z() const { return m_z; }

private:
    GLfloat m_angle;
    GLfloat m_x;
    GLfloat m_y;
    GLfloat m_z;
};

class GLbaseLighting
{
public:
    // Generic Light is all white
    GLbaseLighting(const GLenum light);

    GLbaseLighting(const GLenum light, const GLenum pname, const GLbaseColor& color, const GLbase3dAngle& angle);

    ~GLbaseLighting() {}

    // Set Lighting name
    void setLightingName(const GLenum light);

    // Set before using
    void setLightingType(const GLenum pname);

    // Set Lighting color
    void setLightingColor(const GLbaseColor& color);

    // Set Angle
    void setLightingAngle(const GLbase3dAngle& angle);

    // Sets the lighting as default lighting and does not
    void setDefaultUse(const bool use) { m_useDefault = use; }

    // Use lighting in drawing
    void useLighting() const;

    // Use new lighting color (only)
    void useLightingColor() const;

    // Use new light poistion (only)
    void useLightingAngle() const;

    // Test if this is a default use
    bool isDefaultUse() const { return m_useDefault; }

    GLenum m_light;
    GLenum m_pname;

    GLbaseColor m_color;
    GLbase3dAngle m_angle;

    bool   m_useDefault;
};

