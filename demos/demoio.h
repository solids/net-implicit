//----------------------------------------------------------------------------
// I/O Shell for a Demo Application
//----------------------------------------------------------------------------

#ifndef _DEMOIO_H
#define _DEMOIO_H

#include <SDL/SDL_keysym.h>
#include <SDL/SDL_mouse.h>


class DemoIO
{
public:
    /**
     * Construct a new demo I/O shell.  Default view size is 640x480.
     */
    DemoIO();
    
    /**
     * Destroy the I/O interface.  Leaves the shell if necessary.
     */
    virtual ~DemoIO();
    
    /**
     * Set view size to @p width by @p height pixels.
     */
    void setViewSize(int width, int height);
    
    /**
     * Enters the shell.
     */
    void enter();
    
    /**
     * Leaves the shell.
     */
    void leave();
    
    /**
     * Temporarily pause or resume the shell.  If @p doPause is omitted,
     * or is true, the shell is closed but not destroyed.  If @p doPause
     * is false, the shell is re-entered.
     */
    void pause(bool doPause = true);
    
    /**
     * Resumes the shell.  Same as pause(false).
     */
    void resume();
    
    /**
     * Flips the double-buffered display.  If @p clear is true, the buffer
     * that is hidden after the flip is cleared.
     */
    void flipFrame(bool clear = true);
    
    /**
     * Process SDL events.
     *
     * @return false if application should terminate.
     */
    bool doEvents();
    
    /**
     * @return true if the @p key is currently held
     */
    bool keyPressed(SDLKey key) const;

    /**
     * @return true if the @p button is currently held
     */
    bool buttonPressed(int button) const;

    /**
     * @return mouse X coordinate
     */
    int mouseX() const { return _mouseX; }

    /**
     * @return mouse Y coordinate
     */
    int mouseY() const { return _mouseY; }

protected:
    /**
     * data
     */

    bool _entered;
    bool _paused;
    int _width, _height;
    int _heldKeys[(SDLK_LAST + 1) / sizeof(int)];
    
    bool _heldButtons[3];
    int _mouseX, _mouseY;
};

#endif // _DEMOIO_H
