//----------------------------------------------------------------------------
// DemoAppBaseImp - base implementation of DemoApp for all Isosurface demos
//----------------------------------------------------------------------------

#include "demoapp.h"
#include <threed/threed.h>

// demo applications should subclass DemoAppBaseImp.  the derived class
// should implement the pure virtual initDemo() to do demo-specific
// initializations (like creating the meshes that inhabit the 3d world).
// the application should instantiate its subclass globally and have
// the global pointer variable TheDemoApp point to it.

class DemoAppBaseImp : public DemoApp
{
public:
    virtual bool init();

protected:
    virtual bool initDemo() = 0;

    virtual bool doEvent();
    virtual void doMouseEvent();
    virtual bool doKeyEvent();
    virtual void draw();

    ThreeD::World *wld;
    ThreeD::Camera *camera;

    bool leftButtonPressed;

    ThreeD::WorldLink *selectedLink;
};

extern DemoAppBaseImp *TheDemoApp;
