//----------------------------------------------------------------------------
// ThreeD Isosurface
//----------------------------------------------------------------------------

#ifndef _THREED_ISOSURFACE_H
#define _THREED_ISOSURFACE_H

#include <threed/boundingbox.h>
#include <threed/color.h>
#include <threed/vector.h>
#include <threed/transform.h>

#include <list>

namespace ThreeD {


class Mesh;


/**
 * Isosurface
 */
class Isosurface
{
public:

    struct Material {
        Color color;
        Color ambient;
        float diffuse;
        float brilliance;
        float specular;
    };

    /** destructor
     */
    virtual ~Isosurface();

    /**
     */
    void setTransform(const Transform &t);

    /**
     */
    void addBoundingBox(const BoundingBox &bbox);

    /**
     *
     */
    virtual BoundingBox getBoundingBox(
        const Transform &combinedTrans);

    /**
     *
     */
    virtual void fDensity(
        float x0, float y0, float z0,
        float dz, int num_points, float *densities) = 0;

    /**
     *
     */
    virtual void fNormal(
        const Vector *point, Vector *normal) = 0;

    /**
     *
     */
    virtual const Material &fMaterial(
        const Vector *point, float density) = 0;

protected:
    /*
     * data
     */

    Transform _localTrans;
    Transform _globalTrans;
    Transform _globalTransInv;
    BoundingBox _bbox;
};


} // namespace ThreeD
#endif // _THREED_ISOSURFACE_H
