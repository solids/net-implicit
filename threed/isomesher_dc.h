//----------------------------------------------------------------------------
// ThreeD Dual Contour Isosurface Mesh Generator
//----------------------------------------------------------------------------

#ifndef _THREED_ISOMESHER_DC_H
#define _THREED_ISOMESHER_DC_H

#include <threed/isomesher.h>

namespace ThreeD {


/**
 * IsoMesher, Dual Contour
 */
class IsoMesher_DC : public IsoMesher
{
public:
    /** Construct a mesh generator
     */
    IsoMesher_DC(Isosurface *iso);

    /**
     */
    virtual Mesh *createMesh();

protected:

    typedef Mesh::MeshPoint MeshPoint;

    struct Cube {
        int index;
        MeshPoint *meshPoint;
        Isosurface::Material mat;
    };

    struct Row {
        Vector v;
        Vector *points;
        float *densities;
        Cube *cubes;
    };

    /** Compute a row (y-slice) of grid points
     */
    bool computePoints(Row *row);

    /** Compute a row (y-slice) of cubes (voxels)
     */
    bool computeCubes(Row *rows[2]);

    /** Generate a new (QEF-minimizing) vertex
     */
    void generateVertex(Cube *cube, Point corners[8]);

    /** Generate a quad for voxels sharing an edge
     */
    bool generateQuads(Row *rows[2]);

    /*
     * data
     */

    int _xsize;
    int _zsize;

    static int _edgeTable[256];
};


} // namespace ThreeD
#endif // _THREED_ISOMESHER_DC_H
