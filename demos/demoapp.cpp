#include "demoapp.h"
#include "demoio.h"
#include <stdlib.h>
#include <iostream>


using std::cerr;
using std::endl;


bool DemoApp::init()
{
    _io = new DemoIO;
    _io->enter();
    return true;
}


void DemoApp::run()
{
    while (1) {
        if (!io().doEvents())
            break;
        if (!doEvent())
            break;
        draw();
        io().flipFrame(true);
    }
}


void DemoApp::close()
{
    delete _io;
}
