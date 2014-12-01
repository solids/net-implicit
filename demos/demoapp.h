//----------------------------------------------------------------------------
// Framework for a Demo Application
//----------------------------------------------------------------------------

#ifndef _DEMOAPP_H
#define _DEMOAPP_H

#include "demoio.h"


class DemoApp
{
public:
    virtual bool init();
    virtual void run();
    virtual void close();
    
protected:
    DemoIO &io() const { return *_io; }
    virtual bool doEvent() = 0;
    virtual void draw() = 0;
    
private:
    DemoIO *_io;
};

#endif // _DEMOAPP_H
