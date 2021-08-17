/// @file
///
/// @brief GLbaseControl - base Control object used to collect

#include "GL/glbasectrl.h"
#include "GL/glbasewin.h"
#include "GL/glbaseobj.h"

//--------------------------------------------------------
// Implements Base Control Class
//--------------------------------------------------------
GLbaseControl::GLbaseControl(int type, GLbaseObj* object, int index)
    : m_enabled(true)
    , m_type(type)
    , m_objectIndex(index)
    , m_grfObject{ object }
{
    // Nothing else to do
}

GLbaseControl::~GLbaseControl()
{
    // Nothing else to do
}

void GLbaseControl::setControlObject(GLbaseObj* obj)
{
    // NOTE: if this object was already set, we will have a memory leak
    m_grfObject = obj;
}

void GLbaseControl::init(GLbaseWin* winObj)
{
    if (m_grfObject != nullptr)
    {
        m_grfObject->initControl(winObj);
    }
}

void GLbaseControl::init(GLbaseObj* drawObj)
{
}

#if 0
void GLbaseControl::update(GLbaseWin* winObj, GLbaseObj* drawObj)
{
}
#endif

//--------------------------------------------------------
// Implements Timer Control Class
//--------------------------------------------------------
GLbaseTimer::GLbaseTimer(GLbaseObj* object, const int time, const bool restart)
    : GLbaseControl(GLBASE_TIMER_CONTROL, object)
    , m_index(0)
    , m_time(time)
    , m_restart(restart)
    , m_running{ false }   // unless init, timer is not running
{
    if (m_time == 0)
    {
        // If timer is not set, clear restart
        m_restart = false;
    }
}

GLbaseTimer::~GLbaseTimer()
{
    // Nothing to do
}

void GLbaseTimer::setTime(const int time, const bool restart)
{
    if (time == 0)
    {
        // NOTE: does not reset the time - just stops on the next update
        m_restart = false;
    }
    else
    {
        m_time = time;
        m_restart = restart;
    }
}

void GLbaseTimer::restartTimer(GLbaseWin* winObj)
{
    // If timer is already running, keep it running
    if (m_restart && !m_running && winObj)
    {
        // Set timer after - usual
        winObj->setTimer(m_time, m_index);
        m_running = true;
    }
}

void GLbaseTimer::stopTimer()
{
    // Do not readjust the time -
    m_restart = false;
    // By setting running to false, we do not update objects using it
    m_running = false;
}

void GLbaseTimer::registerController(GLbaseWin* winObj)
{
    if (winObj != nullptr)
    {
        winObj->registerTimer(this);
    }
}

bool GLbaseTimer::updateTimer(GLbaseWin* winObj)
{
    // General update - timer is no longer running
    m_running = false;

    if (winObj == nullptr)
        return false;

    // This is called from the master window when time expires
    restartTimer(winObj);

	return true;
}

//--------------------------------------------------------

GLbaseKeyControl::GLbaseKeyControl(GLbaseObj* object)
    : GLbaseControl(GLBASE_KEYBOARD_CONTROL, object)
{
	// nothing else to do
}

/// @brief register controller to base window
void GLbaseKeyControl::registerController(GLbaseWin* winObj)
{
	// nothing to do
}

/// @brief Updates key entries from main window
bool GLbaseKeyControl::keyResponse(int key, int x, int y)
{
	return false;
}

//--------------------------------------------------------


GLbaseMouseControl::GLbaseMouseControl(GLbaseObj* object)
    : GLbaseControl(GLBASE_MOUSE_CONTROL, object)
{
	// nothing else to do
}

void GLbaseMouseControl::registerController(GLbaseWin* winObj)
{
    // nothing to do
}

//--------------------------------------------------------

GLbaseRotate::GLbaseRotate(bool animate, GLbaseObj* object)
    : GLbaseControl(GLBASE_ROTATE_CONTROL, object)
    , m_animate(animate)
{

}

void GLbaseRotate::setRotateAngle(const GLfloat angle, const GLfloat x, const GLfloat y, const GLfloat z)
{
    m_angle.set3dAngle(angle, x, y, z);
}

void GLbaseRotate::setDeltaAngle(const GLfloat angle, const GLfloat x, const GLfloat y, const GLfloat z)
{
    m_delta.set3dAngle(angle, x, y, z);
}