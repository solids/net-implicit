//----------------------------------------------------------------------------
// ThreeD World   
//----------------------------------------------------------------------------

#ifndef _THREED_WORLD_H
#define _THREED_WORLD_H

#pragma warning(disable: 4786)  // name truncated to 255 chars

#include <threed/object.h>
#include <threed/transform.h>
#include <threed/lightsource.h>
#include <list>

namespace ThreeD {


class WorldLink : public Transform
{
public:
    WorldLink(Object *obj, const Vector &loc)
        : Transform(), _obj(obj), _location(loc)
        {};
    
    Object *object() const { return _obj; }

    inline void draw();
        
protected:
    Object *_obj;
    Vector _location;
};



/**
 * World, a collection of objects that compose a scene.
 */
class World
{
public:
    /**
     * Constructor.
     *
     * Constructs an empty world.
     */
    World();
    
    /**
     * Destructor.
     *
     * Destroys this world.
     */
    virtual ~World();
    
    /**
     * Attaches an object to this world.  The object @p obj will be inserted
     * with key @p id.  This key, which must be unique, may be used to look
     * the object up in this world.
     *
     * @return true if added ok, false if the key is not unique.
     */
    WorldLink *attach(Object *obj);
    
    /**
     * Detaches an object from this world.  The object identified by @p id
     * will be detached.
     *
     * @return a pointer to the object after detaching it, or 0 if object
     * not found.
     */
    Object *detach(WorldLink *link);
    
    /**
     * Erases an object from this world.  The object identified by @p id
     * will be detached and destroyed.
     *
     * @return true if detached and destroyed, or false if object not found.
     */
    void erase(WorldLink *link);

    /**
     * Erases an object from this world.  The object identified by @p id
     * will be detached and destroyed.
     *
     * @return true if detached and destroyed, or false if object not found.
     */
    void erase(Object *obj);

    /**
     * @return a pointer to the object identified by @p id, or 0 if
     * object not found.
     */
    Object *object(WorldLink *link) const;
    
    /** Adds a light source. */
    void addLight(const LightSource &lightSource);

    /** Erases all light sources */
    void eraseLights();

    /**
     * Draws this world, that is, each child Object.
     *
     * Note:  any parameters that should affect the output should be
     * effected into OpenGL prior to the invocation of this method.
     */
    virtual void draw();
    
    /**
     * Returns the bounding box for this object.  The origin of the box is
     * at the origin of the object, (0,0,0).  The negative bounding vertex
     * is place in @p v1, and the positive bounding vertex in @p v2.
     */
    virtual void getBoundingBox(Vector *v1, Vector *v2);
    
    /**
     * Returns the radius of the bounding sphere for this object.  The
     * origin of the sphere is at the origin of the object, (0,0,0).
     *
     * @return the radius of the bounding sphere.
     */
    virtual double getBoundingRadius();

    /**
     * Get object at (X,Y)
     */
    WorldLink *getSelection(int x, int y);
    
protected:
    //
    // data
    //
    std::list<WorldLink *> _links;
    bool _inSelection;

    bool _enableLights;
    std::list<LightSource> _lightSources;
};


} // namespace ThreeD
#endif // _THREED_WORLD_H
