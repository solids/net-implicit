//----------------------------------------------------------------------------
// ThreeD Object  
//----------------------------------------------------------------------------

#ifndef _THREED_OBJECT_H
#define _THREED_OBJECT_H

#include <threed/color.h>
#include <threed/vector.h>

namespace ThreeD {


class Transform;


/**
 * Object, base class for an entity in three-dimensional space.
 */
class Object
{
public:
    /**
     * Destroys this object.
     */
    virtual ~Object() = 0;

    /**
     * Draws this object.
     *
     * Note:  any parameters that should affect the output should be
     * effected into OpenGL prior to the invocation of this method.
     */
    virtual void draw() = 0;
    
    /**
     * Apply the transform @p trans into this object.
     */
    virtual void transform(const Transform &trans) = 0;
    
    /**
     * Returns the bounding box for this object.  The origin of the box is
     * at the origin of the object, (0,0,0).  The negative bounding vertex
     * is place in @p v1, and the positive bounding vertex in @p v2.
     */
    virtual void getBoundingBox(Vector *v1, Vector *v2) = 0;
    
    /**
     * Returns the radius of the bounding sphere for this object.  The
     * origin of the sphere is at the origin of the object, (0,0,0).
     *
     * @return the radius of the bounding sphere.
     */
    virtual float getBoundingRadius() = 0;

    /**
     * Centralizes all vertices around (0,0,0), by computing the offset
     * of the center of the object to (0,0,0) and shifting all the
     * vertices by that offset.
     *
     * @return the offset from the original center to (0,0,0).
     */
    virtual Vector centralize() = 0;

    /**
     * Specifies the color to highlight the object with.
     * Specification of an empty Color() implies no highlighting.
     */
    virtual void highlight(const Color &color) = 0;
};


} // namespace ThreeD
#endif // _THREED_OBJECT_H
