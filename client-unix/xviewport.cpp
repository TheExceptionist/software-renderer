/*
 * xviewport.cpp
 *
 *      Author: flamingo
 *      E-mail: epiforce57@gmail.com
 */

#include "xviewport.h"
#include <cstdlib>

static const int DEFAULT_EVENT_MASK =
        ExposureMask |          // redraw window
        ButtonPressMask |       // button was pressed
        ButtonReleaseMask |     // button was released
        EnterWindowMask |       // entering in the window
        LeaveWindowMask |       // leaving window
        PointerMotionMask |     // trace cursor position
        ButtonMotionMask |      // trace cursor position when a button pressed
//        ResizeRedirectMask |    // resize event
        KeyPressMask |          // key pressed
        KeyReleaseMask;         // key released

XViewport::XViewport(int width, int height, boost::shared_ptr<rend::Camera> camera)
    : Viewport(width, height, camera),
      d(0), m_window(0)
{
    d = XOpenDisplay(getenv("DISPLAY"));
    if (!d)
        throw XServerException("Can't connect to X Server");

    m_window = XCreateSimpleWindow(d, RootWindow(d, 0), 0, 0, width, height, 0,
                                   BlackPixel(d, 0), BlackPixel(d, 0));

    if (!m_window)
        throw XServerException("Can't create X window");

    XSelectInput(d, m_window, DEFAULT_EVENT_MASK);
    XMapWindow(d, m_window);
    XFlush(d);
}

XViewport::~XViewport()
{
    // hide window
    if (d && m_window)
    {
        XUnmapWindow(d, m_window);
        XFlush(d);

        // destroy window
        XDestroyWindow(d, m_window);
    }

    // end session
    if (d)
        XCloseDisplay(d);
}

void XViewport::frameBegin()
{
}

void XViewport::flush(unsigned char *pixels)
{
}