//----------------------------------------------------------------------------
// ThreeD Camera
//----------------------------------------------------------------------------

#ifndef _THREED_CAMERA_H
#define _THREED_CAMERA_H

#include <threed/transform.h>
#include <threed/world.h>

namespace ThreeD {


class World;


/**
 * Camera, a point of view in a three dimesional world.
 */
class Camera : public Transform
{
public:
    /**
     * Constructor.
     *
     * Creates a camera for world @p world.
     */
    Camera(World &world);
    
    /** Copy constructor.
     */
    Camera(const Camera &model)
        : Transform(model), _world(model._world) {}

    /**
     * Draws the world.
     *
     * Note:  any parameters that should affect the output should be
     * effected into OpenGL prior to the invocation of this method.
     */
    void draw();

    /**
     * Get object at (X,Y)
     */
    WorldLink *getSelection(int x, int y);
    
protected:
    World &_world;
};


} // namespace ThreeD
#endif // _THREED_CAMERA_H
