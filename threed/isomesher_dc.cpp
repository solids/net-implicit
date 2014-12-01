//----------------------------------------------------------------------------
// ThreeD Dual Contour Isosurface Mesh Generator
//----------------------------------------------------------------------------

#include <threed/isomesher_dc.h>
#include <threed/qef.h>
#include <threed/meshface.h>
#include <threed/misc.h>
//#include <malloc.h>
#include <stdlib.h>

using namespace ThreeD;

//----------------------------------------------------------------------------

IsoMesher_DC::IsoMesher_DC(Isosurface *iso)
: IsoMesher(iso)
{
}

//----------------------------------------------------------------------------

Mesh *IsoMesher_DC::createMesh()
{
    // compute unit-aligned bounding box

    BoundingBox bbox = _iso->getBoundingBox(Transform());
    int xmin = (int)floor(bbox.vmin().x());
    int ymin = (int)floor(bbox.vmin().y());
    int zmin = (int)floor(bbox.vmin().z());
    int xmax = (int)ceil(bbox.vmax().x() + 3 * _voxelSize.x());
    int ymax = (int)ceil(bbox.vmax().y() + 3 * _voxelSize.y());
    int zmax = (int)ceil(bbox.vmax().z() + 3 * _voxelSize.z());

    _xsize = (int)((xmax - xmin) / _voxelSize.x() + 1);
    int ysize = (int)((ymax - ymin) / _voxelSize.y() + 1);
    _zsize = (int)((zmax - zmin) / _voxelSize.z() + 1);
    _zsize = ((_zsize + 3) >> 2) << 2;  // round to x4

    if (_xsize < 1 || ysize < 2 || _zsize < 1)
        return 0;

    // precalculate first two row (y slice) of the grid.

    _mesh = new Mesh();

    Vector vRow = Vector(xmin, ymin, zmin);
    Vector deltaRow = Vector(0, _voxelSize.y(), 0);

    Row row_space[3];
    Row *rows[3];
    int y;

    for (y = 0; y < 3; ++y) {
        rows[y] = &row_space[y];
        rows[y]->points =
            (Vector *)malloc(sizeof(Vector) * _xsize * _zsize);
        rows[y]->densities =
            (float *)malloc(sizeof(float) * _xsize * _zsize);
        rows[y]->cubes =
            (Cube *)malloc(sizeof(Cube) * _xsize * _zsize);

        if (y == 0 || y == 1) {
            rows[y]->v = vRow;
            computePoints(rows[y]);
            vRow += deltaRow;
        }
    }

    computeCubes(&rows[0]);

    // go through the remaining rows, calculating each before
    // processing it and its preceeding row

    for (y = 2; y < ysize - 2; ++y) {
        int percent = (int)(y * 100.0f / (ysize - 2));

        rows[2]->v = vRow;
        if (computePoints(rows[2]))
            break;
        vRow += deltaRow;

        computeCubes(&rows[1]);

        generateQuads(&rows[0]);

        Row *rows_0_save = rows[0];
        rows[0] = rows[1];
        rows[1] = rows[2];
        rows[2] = rows_0_save;
    }

    if (y < ysize - 2) {        // cancelled?
        delete _mesh;
        _mesh = 0;
    }

    for (y = 0; y < 3; ++y) {
        free(rows[y]->points);
        free(rows[y]->densities);
        free(rows[y]->cubes);
    }

    return _mesh;
}

//----------------------------------------------------------------------------

bool IsoMesher_DC::computePoints(Row *row)
{
    float x0 = row->v.x();
    float y0 = row->v.y();
    float z0 = row->v.z();
    float dz = _voxelSize.z();

    for (int x = 0; x < _xsize; ++x) {
        float *densities = &row->densities[x * _zsize];
        _iso->fDensity(x0, y0, z0, dz, _zsize, densities);

        Vector *points = &row->points[x * _zsize];
        for (int z = 0; z < _zsize; ++z) {
            densities[z] += 1e-4f;
            points[z] = Vector(x0, y0, z0 + z * dz);
        }

        x0 += _voxelSize.x();
    }

    return false;
}

//----------------------------------------------------------------------------

bool IsoMesher_DC::computeCubes(Row *rows[2])
{
    int xsize_1 = _xsize - 1;
    int zsize_1 = _zsize - 1;

    for (int x = 0; x < xsize_1; ++x) {
        for (int z = 0; z < zsize_1; ++z) {
            Cube *cube = &rows[0]->cubes[x * _zsize + z];

#define DENSITY_VAL(xi,yi,zi) rows[yi]->densities[(x + xi) * _zsize + (z + zi)]
#define POINT_PTR(xi,yi,zi) &rows[yi]->points[(x + xi) * _zsize + (z + zi)]
#define CORNER(n,xi,yi,zi) \
    corners[n].density = DENSITY_VAL(xi, yi, zi);  \
    corners[n].v = POINT_PTR(xi, yi, zi);

            Point corners[8];
            CORNER(0,  0,0,0);
            CORNER(1,  1,0,0);
            CORNER(2,  1,1,0);
            CORNER(3,  0,1,0);
            CORNER(4,  0,0,1);
            CORNER(5,  1,0,1);
            CORNER(6,  1,1,1);
            CORNER(7,  0,1,1);

#undef CORNER
#undef POINT_PTR
#undef DENSITY_VAL

            int index = 0;

#define IF_DENSITY(i) if (! fsign(corners[i].density)) index |= (1 << i)

            IF_DENSITY(0);
            IF_DENSITY(1);
            IF_DENSITY(2);
            IF_DENSITY(3);
            IF_DENSITY(4);
            IF_DENSITY(5);
            IF_DENSITY(6);
            IF_DENSITY(7);

#undef IF_DENSITY

            if (_edgeTable[index] == 0) {
                cube->index = 0;
                cube->meshPoint = 0;
                continue;
            }

            cube->index = index;
            generateVertex(cube, corners);
        }

    }

    return false;
}

//----------------------------------------------------------------------------

void IsoMesher_DC::generateVertex(Cube *cube, Point corners[8])
{
    //
    // part 1  compute intersection points, their normals.
    //

    static int intersections[12][2] = {
        { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 }, { 4, 5 }, { 5, 6 },
        { 6, 7 }, { 7, 4 }, { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 }
    };

    int edgeInfo = _edgeTable[cube->index];

    Point points[12];
    Vector points_v[12];
    Vector normals[12];
    Vector massPoint;
    int numIntersections = 0;
    int i;

    for (i = 0; i < 12; ++i) {
        if (! (edgeInfo & (1 << i)))
            continue;

        int n1 = intersections[i][0];
        int n2 = intersections[i][1];

        if (fabs(corners[n1].density) < 1e-3)
            points[i] = corners[n1];
        else if (fabs(corners[n2].density) < 1e-3)
            points[i] = corners[n2];
        else if (fabs(corners[n1].density - corners[n2].density) < 1e-3)
            points[i] = corners[n1];
        else {
            points[i].v = &points_v[i];
            Vector v_diff = abs(*corners[n1].v - *corners[n2].v);
            if (v_diff.x() > 1e-3)
                intersect_xaxis(&corners[n1], &corners[n2], &points[i]);
            else if (v_diff.y() > 1e-3)
                intersect_yaxis(&corners[n1], &corners[n2], &points[i]);
            else if (v_diff.z() > 1e-3)
                intersect_zaxis(&corners[n1], &corners[n2], &points[i]);
        }

        _iso->fNormal(points[i].v, &normals[i]);

        massPoint += *points[i].v;
        ++numIntersections;
    }

    massPoint /= (float)numIntersections;

    //
    // part 2  compute the QEF-minimizing point
    //

    Vector newPointNormal;
    Isosurface::Material newPointMat;

    newPointMat.diffuse = 0.0;
    newPointMat.specular = 0.0;
    newPointMat.brilliance = 0.0;

    double matrix[12][3];
    double vector[12];
    int rows = 0;

    for (i = 0; i < 12; ++i) {
        if (! (edgeInfo & (1 << i)))
            continue;

        const Vector &normal = normals[i];

        matrix[rows][0] = normal.x();
        matrix[rows][1] = normal.y();
        matrix[rows][2] = normal.z();

        Vector p = *points[i].v - massPoint;
        vector[rows] = (double)(normal * p);

        ++rows;

        newPointNormal += normal;

        Isosurface::Material tmpMat =
            _iso->fMaterial(points[i].v, points[i].density);
        newPointMat.color += tmpMat.color;
        newPointMat.ambient += tmpMat.ambient;
        newPointMat.diffuse += tmpMat.diffuse;
        newPointMat.specular += tmpMat.specular;
        newPointMat.brilliance += tmpMat.brilliance;
    }

    Vector newPointV;
    QEF::evaluate(matrix, vector, rows, &newPointV);
    newPointV += massPoint;

    cube->meshPoint = _mesh->addPoint(newPointV);
    cube->meshPoint->normal = newPointNormal.normalized();

    cube->mat.color = newPointMat.color / (float)rows;
    cube->mat.ambient = newPointMat.ambient / (float)rows;
    cube->mat.diffuse = newPointMat.diffuse / (float)rows;
    cube->mat.specular = newPointMat.specular / (float)rows;
    cube->mat.brilliance = newPointMat.brilliance / (float)rows;
}

//----------------------------------------------------------------------------

bool IsoMesher_DC::generateQuads(Row *rows[2])
{
    int xsize_2 = _xsize - 2;
    int zsize_2 = _zsize - 2;

#define CUBE_PTR(xi,yi,zi) &rows[yi]->cubes[(x + xi) * _zsize + (z + zi)]

    for (int x = 0; x < xsize_2; ++x) {
        for (int z = 0; z < zsize_2; ++z) {

            Mesh::MeshPlane *meshPlane = 0;

            Cube *cubes[4];
            cubes[0] = CUBE_PTR(0,0,0);
            int cube0_edgeInfo = _edgeTable[cubes[0]->index];
            int flip_if_nonzero = 0;

            for (int i = 0; i < 3; ++i) {

                if (i == 0 && cube0_edgeInfo & (1 << 10)) {
                    cubes[1] = CUBE_PTR(1,0,0);
                    cubes[2] = CUBE_PTR(1,1,0);
                    cubes[3] = CUBE_PTR(0,1,0);
                    flip_if_nonzero = (cubes[0]->index & (1 << 6));
                } else if (i == 1 && cube0_edgeInfo & (1 << 6)) {
                    cubes[1] = CUBE_PTR(0,0,1);
                    cubes[2] = CUBE_PTR(0,1,1);
                    cubes[3] = CUBE_PTR(0,1,0);
                    flip_if_nonzero = (cubes[0]->index & (1 << 7));
                } else if (i == 2 && cube0_edgeInfo & (1 << 5)) {
                    cubes[1] = CUBE_PTR(1,0,0);
                    cubes[2] = CUBE_PTR(1,0,1);
                    cubes[3] = CUBE_PTR(0,0,1);
                    flip_if_nonzero = (cubes[0]->index & (1 << 5));
                } else
                    continue;

                // create triangles (cube0,cube2,cube1)
                //              and (cube0,cube3,cube2)
                // flipping last two vertices if necessary

                MeshPoint *p0 = cubes[0]->meshPoint;
                const Isosurface::Material &mat0 = cubes[0]->mat;

                for (int j = 1; j < 3; ++j) {
                    int ja, jb;
                    if (flip_if_nonzero) {
                        ja = j + 0;
                        jb = j + 1;
                    } else {
                        ja = j + 1;
                        jb = j + 0;
                    }

                    MeshPoint *p1 = cubes[ja]->meshPoint;
                    MeshPoint *p2 = cubes[jb]->meshPoint;

                    const Isosurface::Material &mat1 = cubes[ja]->mat;
                    const Isosurface::Material &mat2 = cubes[jb]->mat;

                    Isosurface::Material mat;
                    mat.color = (mat0.color + mat1.color + mat2.color) / 3.0f;
                    mat.ambient = (mat0.ambient + mat1.ambient + mat2.ambient) / 3.0f;
                    mat.diffuse = (mat0.diffuse + mat1.diffuse + mat2.diffuse) / 3.0f;
                    mat.specular = (mat0.specular + mat1.specular + mat2.specular) / 3.0f;
                    mat.brilliance = (mat0.brilliance + mat1.brilliance + mat2.brilliance) / 3.0f;

                    MeshFace *face = new MeshFace(
                        &p2->point, &p1->point, &p0->point,
                        mat.color, mat.ambient, mat.diffuse, mat.specular, mat.brilliance);

                    if (!meshPlane) {
                        Plane p(face->vertex(0), face->vertex(1), face->vertex(2));
                        meshPlane = _mesh->addPlane(p);
                    }

                    _mesh->addFace(face, meshPlane);
                }
            }
        }

        if (invokeProgressFunc())
            return true;
    }

#undef CUBE_PTR

    return false;
}

//----------------------------------------------------------------------------

#if 0

void IsoMesher_DC::generateFace(
    MeshPoint *p0, MeshPoint *p1, MeshPoint *p2, int flip_if_nonzero)
{
    float dummy;
#define nsign(x) (dummy = x, dummy == 0.0 ? 0 : 1 - 2 * fsign(dummy))

    int s0x = nsign(p0->normal.x());
    int s0y = nsign(p0->normal.y());
    int s0z = nsign(p0->normal.z());
    int s1x = nsign(p1->normal.x());
    int s1y = nsign(p1->normal.y());
    int s1z = nsign(p1->normal.z());
    int s2x = nsign(p2->normal.x());
    int s2y = nsign(p2->normal.y());
    int s2z = nsign(p2->normal.z());

    Vector a = (p0->point - p2->point);
    Vector b = (p1->point - p2->point);
    Vector n9 = a % b;

    int s9x = nsign(n9.x());
    int s9y = nsign(n9.y());
    int s9z = nsign(n9.z());

    bool swap = false;

    if (s0x == s1x && s1x == s2x && s9x != s0x)
        swap = true;
    if (s0y == s1y && s1y == s2y && s9y != s0y)
        swap = true;
    if (s0z == s1z && s1z == s2z && s9z != s0z)
        swap = true;

    if (flip_if_nonzero) {
        MeshPoint *tmp = p0;
        p0 = p2;
        p2 = tmp;
    }

    Isosurface::Material mat;
    mat.color = Color(1.0f, 1.0f, 1.0f);

    MeshFace *face = new MeshFace(
        &p2->point, &p1->point, &p0->point,
        mat.color, mat.ambient, mat.diffuse, mat.specular, mat.brilliance);
    _mesh->addFace(face);
}

#endif

//----------------------------------------------------------------------------

int IsoMesher_DC::_edgeTable[256] = {
    0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
    0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
    0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
    0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
    0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
    0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
    0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
    0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
    0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
    0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
    0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
    0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
    0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
    0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
    0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
    0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
    0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
    0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
    0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
    0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
    0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
    0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
    0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
    0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
    0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
    0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
    0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
    0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
    0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
    0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
    0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
    0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0
};
