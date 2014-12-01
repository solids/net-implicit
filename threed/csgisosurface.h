//----------------------------------------------------------------------------
// ThreeD Isosurface with CSG operations
//----------------------------------------------------------------------------

#ifndef _THREED_CSGISOSURFACE_H
#define _THREED_CSGISOSURFACE_H

#include <threed/isosurface.h>

namespace ThreeD {


/**
 * CsgIsosurface
 */
class CsgIsosurface : public Isosurface
{
public:
    enum CSG_Mode {
        CSG_UNION,
        CSG_INTERSECTION,
        CSG_DIFFERENCE
    };

    /**
     *
     */
    CsgIsosurface();

    /**
     * destructor
     */
    virtual ~CsgIsosurface();

    /**
     */
    void setCsgMode(CSG_Mode csg_mode);

    /**
     *  This isosurface takes ownership of the child.
     */
    void addChild(Isosurface *child);

    /**
     *
     */
    virtual BoundingBox getBoundingBox(
        const Transform &combinedTrans);

    /**
     *
     */
    void fDensity_n(
        float x0, float y0, float z0,
        float dz, int num_points, float *densities);

    /**
     *
     */
    virtual void fDensity(
        float x0, float y0, float z0,
        float dz, int num_points, float *densities);

    /**
     *
     */

    virtual void fNormal(
        const Vector *point, Vector *normal);

    /**
     *
     */
    virtual const Material &fMaterial(
        const Vector *point, float density);

protected:
    /*
     *
     */
    inline Isosurface *findIsosurface(float x0, float y0, float z0);

    /*
     * data
     */

    CSG_Mode _csg_mode;

    std::list<Isosurface *> _children;

    float *_densities;
    int _maxPoints;
};


} // namespace ThreeD
#endif // _THREED_CSGISOSURFACE_H
