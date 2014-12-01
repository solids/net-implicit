//----------------------------------------------------------------------------
// ThreeD Camera   
//----------------------------------------------------------------------------

#include <threed/camera.h>
#include <threed/world.h>
#include <threed/opengl.h>

using namespace ThreeD;

//----------------------------------------------------------------------------

Camera::Camera(World &world) :
    Transform(),
    _world(world)
{
}

//----------------------------------------------------------------------------

void Camera::draw()
{
    //Matrix saveMatrix = _matrix();
    //invert();
    glLoadIdentity();
    Transform::glMultMatrix();
    _world.draw();
    
}

//----------------------------------------------------------------------------

WorldLink *Camera::getSelection(int x, int y)
{
    glLoadIdentity();
    Transform::glMultMatrix();
    WorldLink *link = _world.getSelection(x, y);
    return link;
}
