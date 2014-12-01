//----------------------------------------------------------------------------
// I/O Shell for a Demo Application
//----------------------------------------------------------------------------

#include "demoio.h"
#include <threed/opengl.h>
#include <OpenGL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <memory.h>

//----------------------------------------------------------------------------

DemoIO::DemoIO()
{
    _entered = false;
    _paused = false;
    _width = 320;
    _height = 240;
    memset(_heldKeys, 0, sizeof(_heldKeys));
    memset(_heldButtons, 0,sizeof(_heldButtons));
    _mouseX = _mouseY = 0;
}

DemoIO::~DemoIO()
{
    leave();
}

//----------------------------------------------------------------------------

void DemoIO::setViewSize(int width, int height)
{
    _width = width;
    _height = height;
}

//----------------------------------------------------------------------------

void DemoIO::enter()
{
    if (_entered)
        return;

    int rc = SDL_Init(SDL_INIT_VIDEO);
    if (rc < 0) {
        throw SDL_GetError();
        return;
    }

    const SDL_VideoInfo *info = SDL_GetVideoInfo();
    if (!info) {
        throw SDL_GetError();
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, /* bpp */ 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    int flags = SDL_OPENGL /*| SDL_FULLSCREEN*/;
    SDL_Surface *surface = SDL_SetVideoMode(_width, _height, 32, flags);
    if (!surface) {
        throw SDL_GetError();
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, _width, _height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = (float)_width / (float)_height;
    gluPerspective(45.0, ratio, 1.0, 500.0);

    glMatrixMode(GL_MODELVIEW);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);

    //glPolygonMode(GL_FRONT, GL_LINE);
}

//----------------------------------------------------------------------------

void DemoIO::leave()
{
    if (_entered) {
        SDL_Quit();
        _entered = false;
        _paused = false;
    }
}

//----------------------------------------------------------------------------

void DemoIO::pause(bool doPause)
{
}

//----------------------------------------------------------------------------

void DemoIO::resume()
{
    pause(false);
}

//----------------------------------------------------------------------------

void DemoIO::flipFrame(bool clear)
{
    SDL_GL_SwapBuffers();
    if (clear)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------------------------

bool DemoIO::doEvents()
{
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return false;

        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            int index = (int)event.key.keysym.sym;
            int byteOffset = index / sizeof(int);
            int bitOffset = index - byteOffset * sizeof(int);
            _heldKeys[byteOffset] = (_heldKeys[byteOffset] & ~(1 << bitOffset)) |
                                    ((event.type == SDL_KEYDOWN) << bitOffset);
        }

        if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
            _mouseX = (int)event.button.x;
            _mouseY = (int)event.button.y;
            if (event.button.button >= SDL_BUTTON_LEFT &&
                event.button.button <= SDL_BUTTON_RIGHT) {
                _heldButtons[event.button.button - SDL_BUTTON_LEFT] =
                    (event.type == SDL_MOUSEBUTTONDOWN);
            }
        }

    }
    return true;
}

//----------------------------------------------------------------------------

bool DemoIO::keyPressed(SDLKey key) const
{
    int index = (int)key;
    int byteOffset = index / sizeof(int);
    int bitMask = 1 << (index - byteOffset * sizeof(int));
    return ((_heldKeys[byteOffset] & bitMask) == bitMask);
}

//----------------------------------------------------------------------------

bool DemoIO::buttonPressed(int button) const
{
    bool pressed = false;
    if (button >= SDL_BUTTON_LEFT && button <= SDL_BUTTON_RIGHT)
        pressed = _heldButtons[button - SDL_BUTTON_LEFT];
    return pressed;
}
