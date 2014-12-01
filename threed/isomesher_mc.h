//----------------------------------------------------------------------------
// ThreeD Marching Cubes Isosurface Mesh Generator
//----------------------------------------------------------------------------

#ifndef _THREED_ISOMESHER_MC_H
#define _THREED_ISOMESHER_MC_H

#include <threed/isomesher.h>

namespace ThreeD {


/**
 * IsoMesher
 */
class IsoMesher_MC : public IsoMesher
{
public:
    /** Construct a mesh generator
     */
    IsoMesher_MC(Isosurface *iso);

    /**
     */
    virtual Mesh *createMesh();

protected:

    typedef Mesh::MeshPoint MeshPoint;

    struct Row {
        Vector *points;
        float *densities;
    };

    /** Compute a row (y-slice) of grid points
     */
    bool computeRow(const Vector &vRow, Row *row);

    /** Perform marching cubes on two adjacent voxel slices
     */
    bool marchCubes(Row *rows[2]);

    /** Generate triangles within a voxel
     */
    void generateFaces(Point corners[8], int index);

    /*
     * data
     */

    int _xsize;
    int _zsize;

    static int _edgeTable[256];
    static int _triTable[256][16];
};


} // namespace ThreeD
#endif // _THREED_ISOMESHER_MC_H
