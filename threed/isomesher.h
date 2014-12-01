//----------------------------------------------------------------------------
// ThreeD Interface for an Isosurface Mesh Generator
//----------------------------------------------------------------------------

#ifndef _THREED_ISOMESHER_H
#define _THREED_ISOMESHER_H

#include <threed/isosurface.h>
#include <threed/mesh.h>

namespace ThreeD {


/**
 * IsoMesher
 */
class IsoMesher
{
public:
    /** Construct a mesh generator
     */
    IsoMesher(Isosurface *iso);

    /** Set the voxel size
     */
    void setVoxelSize(float x, float y, float z);

    /** Set the progress function
     */
    void setProgressFunc(bool (*func)(void *, int), void *parm);

    /**
     */
    virtual Mesh *createMesh() = 0;

protected:

    struct Point {
        float density;
        Vector *v;
    };

    /** Intersect a voxel edge along the x axis
     */
    void intersect_xaxis(Point *p0, Point *p1, Point *out) const;

    /** Intersect a voxel edge along the y axis
     */
    void intersect_yaxis(Point *p0, Point *p1, Point *out) const;

    /** Intersect a voxel edge along the z axis
     */
    void intersect_zaxis(Point *p0, Point *p1, Point *out) const;

    /** Invoke progress function to update the percent
     */
    bool invokeProgressFunc(int percent)
    {
        if (! _progressFunc)
            return false;
        _progressPercent = percent;
        return _progressFunc(_progressParm, _progressPercent);
    }

    /** Invoke progress function without updating the percent
     */
    bool invokeProgressFunc();

    /*
     * data
     */

    Isosurface *_iso;
    Vector _voxelSize;
    Mesh *_mesh;

    bool (*_progressFunc)(void *, int);
    void *_progressParm;
    int _progressPercent;
    long _progressTime;
};


} // namespace ThreeD
#endif // _THREED_ISOMESHER_H
