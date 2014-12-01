//----------------------------------------------------------------------------
// ThreeD Isosurface
//----------------------------------------------------------------------------

#include <threed/isosurface.h>

using namespace ThreeD;


//----------------------------------------------------------------------------

Isosurface::~Isosurface()
{
}

//----------------------------------------------------------------------------

void Isosurface::setTransform(const Transform &t)
{
    _localTrans = t;
}

//----------------------------------------------------------------------------

void Isosurface::addBoundingBox(const BoundingBox &bbox)
{
    _bbox.merge(bbox);
}

//----------------------------------------------------------------------------

BoundingBox Isosurface::getBoundingBox(
    const Transform &combinedTrans)
{
    _globalTrans = _localTrans;
    _globalTrans *= combinedTrans;
    _globalTransInv = _globalTrans;
    _globalTransInv.invert();

    BoundingBox bbox(_bbox);
    bbox.transform(_globalTrans);
    return bbox;
}
