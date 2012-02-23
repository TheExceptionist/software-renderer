#include "framebuffer.h"

#include <tcl/tk.h>
#include <tcl/tcl.h>
#include <third-party/cpptk-1.0.2/base/cpptkbase.h>

namespace rend
{

FrameBuffer::FrameBuffer(const int witdh, const int height)
    : m_pixels(0),
      m_width(witdh),
      m_height(height),
      m_xOrigin(0),
      m_yOrigin(0)
{
    m_pixels = new Color3[m_width * m_height];
}

FrameBuffer::~FrameBuffer()
{
    if (m_pixels)
        delete [] m_pixels;
}

void FrameBuffer::clear()
{
    memset(m_pixels, 0xFF, sizeof(Color3) * m_width * m_height);
}

void FrameBuffer::flush_tk()
{
    Tk_PhotoHandle fbphoto;
    Tk_PhotoImageBlock block;
    Tcl_Interp *fbinterp = Tk::globalTclInterpret;

    if ((fbphoto = Tk_FindPhoto(fbinterp, "canvas_photo")) == NULL)
    {
        *syslog << "Image creation unsuccessful in fb_open." << logerr;
        return;
    }

    block.pixelPtr = (unsigned char *)m_pixels;
    block.width = m_width;
    block.height = m_height;
    block.pitch = 3 * m_width;
    block.pixelSize = 3;
    block.offset[0] = 0;
    block.offset[1] = 1;
    block.offset[2] = 2;
    block.offset[3] = 0;

    Tk_PhotoPutBlock(fbinterp, fbphoto, &block, 0, 0, m_width, m_height, TK_PHOTO_COMPOSITE_SET);
}

void FrameBuffer::flush_win()
{
}

void FrameBuffer::flush_gl()
{
}

void FrameBuffer::scanline(const int x1, const int x2, const int y, const Color3 &color)
{
    for (int x = x1; x < x2; x++)
    {
        if (!(x >= 0 && x < m_width && y >= 0 && y < m_height))
            return;

        Color3 &currPix = m_pixels[m_width * y + x];
        currPix = color;
    }
}

}