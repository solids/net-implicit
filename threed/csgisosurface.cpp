//----------------------------------------------------------------------------
// ThreeD Isosurface with CSG operations
//----------------------------------------------------------------------------

#include <threed/csgisosurface.h>
#include <stdlib.h>

using namespace ThreeD;

//----------------------------------------------------------------------------

CsgIsosurface::CsgIsosurface()
{
    _maxPoints = 0;
}

//----------------------------------------------------------------------------

CsgIsosurface::~CsgIsosurface()
{
    std::list<Isosurface *>::iterator it = _children.begin();
    while (it != _children.end()) {
        Isosurface *child = (*it);
        it = _children.erase(it);
        delete child;
    }

    if (_maxPoints) {
        free(_densities);
        _maxPoints = 0;
    }
}

//----------------------------------------------------------------------------

void CsgIsosurface::setCsgMode(CSG_Mode csg_mode)
{
    _csg_mode = csg_mode;
}

//----------------------------------------------------------------------------

void CsgIsosurface::addChild(Isosurface *child)
{
    _children.push_back(child);
}

//----------------------------------------------------------------------------

BoundingBox CsgIsosurface::getBoundingBox(
    const Transform &combinedTrans)
{
    Transform t(_localTrans);
    t *= combinedTrans;

    BoundingBox bbox;
    std::list<Isosurface *>::iterator it = _children.begin();
    while (it != _children.end()) {
        Isosurface *child = (*it);
        ++it;
        const BoundingBox &child_bbox = child->getBoundingBox(t);
        bbox.merge(child_bbox);
    }

    return bbox;
}

//----------------------------------------------------------------------------

void CsgIsosurface::fDensity_n(
    float x0, float y0, float z0,
    float dz, int num_points, float *densities)
{
    // the case where there are no actual children in the csg
    if (_children.empty()) {
        for (int i = 0; i < num_points; ++i)
            densities[i] = 1.0f;
        return;
    }

    // do the first child isosurface
    std::list<Isosurface *>::const_iterator it = _children.begin();
    Isosurface *child = (*it);
    ++it;
    child->fDensity(x0, y0, z0, dz, num_points, densities);

    // allocate temporary storage for CSG operation
    if (num_points > _maxPoints) {
        if (_maxPoints)
            free(_densities);
        _maxPoints = num_points;
        _densities = (float *)malloc(sizeof(float) * _maxPoints);
    }

    // union

    if (_csg_mode == CSG_UNION) {
        while (it != _children.end()) {
            Isosurface *child = (*it);
            ++it;
            child->fDensity(x0, y0, z0, dz, num_points, _densities);
            for (int i = 0; i < num_points; ++i)
                if (_densities[i] < densities[i])
                    densities[i] = _densities[i];
        }

    // intersection

    } else if (_csg_mode == CSG_INTERSECTION) {
        while (it != _children.end()) {
            Isosurface *child = (*it);
            ++it;
            child->fDensity(x0, y0, z0, dz, num_points, _densities);
            for (int i = 0; i < num_points; ++i)
                if (_densities[i] > densities[i])
                    densities[i] = _densities[i];
        }

    // difference

    } else if (_csg_mode == CSG_DIFFERENCE) {
        while (it != _children.end()) {
            Isosurface *child = (*it);
            ++it;
            child->fDensity(x0, y0, z0, dz, num_points, _densities);
            for (int i = 0; i < num_points; ++i)
                if (-_densities[i] > densities[i])
                    densities[i] = -_densities[i];
        }
    }
}

//----------------------------------------------------------------------------

void CsgIsosurface::fDensity(
    float x0, float y0, float z0,
    float dz, int num_points, float *densities)
{
    if (num_points != 1) {
        fDensity_n(x0, y0, z0, dz, num_points, densities);
        return;
    }

    // the case where there are no actual children in the csg
    if (_children.empty()) {
        *densities = 1.0f;
        return;
    }

    float density, density2;

    // do the first child isosurface
    std::list<Isosurface *>::const_iterator it = _children.begin();
    (*it)->fDensity(x0, y0, z0, 0, 1, &density);
    ++it;

    while (it != _children.end()) {
        (*it)->fDensity(x0, y0, z0, 0, 1, &density2);
        ++it;
        if (_csg_mode == CSG_DIFFERENCE)
            density2 = -density2;
        if ((_csg_mode == CSG_UNION && density2 < density) ||
            (_csg_mode != CSG_UNION && density2 > density))
            density = density2;
    }

    *densities = density;
}

//----------------------------------------------------------------------------

Isosurface *CsgIsosurface::findIsosurface(
    float x, float y, float z)
{
    float density, density2;

    // try the first child isosurface
    std::list<Isosurface *>::const_iterator it = _children.begin();
    Isosurface *iso = (*it);
    ++it;
    iso->fDensity(x, y, z, 0, 1, &density);

    while (it != _children.end()) {
        Isosurface *iso2 = (*it);
        ++it;
        iso2->fDensity(x, y, z, 0, 1, &density2);
        if (_csg_mode == CSG_DIFFERENCE)
            density2 = -density2;
        if ((_csg_mode == CSG_UNION && density2 < density) ||
            (_csg_mode != CSG_UNION && density2 > density)) {
            density = density2;
            iso = iso2;
        }
    }

    return iso;
}

//----------------------------------------------------------------------------

void CsgIsosurface::fNormal(const Vector *point, Vector *normal)
{
    Isosurface *iso =
        findIsosurface(point->x(), point->y(), point->z());
    iso->fNormal(point, normal);
}

//----------------------------------------------------------------------------

const CsgIsosurface::Material &CsgIsosurface::fMaterial(
    const Vector *point, float density)
{
    Isosurface *iso =
        findIsosurface(point->x(), point->y(), point->z());
    return iso->fMaterial(point, density);
}
