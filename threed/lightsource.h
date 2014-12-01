//----------------------------------------------------------------------------
// ThreeD Light Source
//----------------------------------------------------------------------------

#ifndef _THREED_LIGHTSOURCE_H
#define _THREED_LIGHTSOURCE_H

#include <threed/transform.h>
#include <threed/vector.h>
#include <threed/color.h>

namespace ThreeD {


/**
 * LightSource.
 */
class LightSource : public Transform
{
public:
    /**
     * Constructor.
     *
     * Creates a camera for world @p world.
     */
    LightSource(int lightNum);
    
    /** Copy constructor.
     */
    LightSource(const LightSource &model);

    /** Assignment.
     */
    LightSource &operator=(const LightSource &model);

    /** Set location of light source.
     */
    void setLocation(const Vector &v) { _location = v; }

    /** Set ambient color of light source.
     */
    void setAmbientColor(const Color &c) { _ambient = c; }

    /** Set diffuse color of light source.
     */
    void setDiffuseColor(const Color &c) { _diffuse = c; }

    /** Enable the light source.
     */
    void enable();

    /** Disable the light source.
     */
    void disable();

protected:
    int _lightNum;
    Vector _location;
    Color _ambient;
    Color _diffuse;
};


} // namespace ThreeD
#endif // _THREED_LIGHTSOURCE_H
