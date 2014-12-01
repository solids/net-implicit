//----------------------------------------------------------------------------
// ThreeD Isosurface Mesh Generator
//----------------------------------------------------------------------------

#include <threed/isomesher.h>

using namespace ThreeD;

#include <sys/time.h>
uint32_t GetTickCount(void) {
  struct timeval tv;
  if (gettimeofday(&tv, (struct timezone *)0)) {
    return (uint32_t)0;
  }
  return (uint32_t)((tv.tv_sec & 0x003FFFFF) * 1000L + tv.tv_usec / 1000L);
}




//----------------------------------------------------------------------------

#define TOLERANCE_DENSITY 1e-3
#define TOLERANCE_COORD   1e-5

//----------------------------------------------------------------------------

IsoMesher::IsoMesher(Isosurface *iso)
{
    _iso = iso;
    _progressFunc = 0;
}

//----------------------------------------------------------------------------

void IsoMesher::setVoxelSize(float x, float y, float z)
{
    _voxelSize = Vector(x, y, z);
}

//----------------------------------------------------------------------------

void IsoMesher::setProgressFunc(
    bool (*func)(void *, int), void *parm)
{
    _progressFunc = func;
    _progressParm = parm;
    _progressPercent = 0;
    _progressTime = GetTickCount();
}

//----------------------------------------------------------------------------

bool IsoMesher::invokeProgressFunc()
{
    bool cancel = false;
    if (_progressFunc) {
        long time = GetTickCount();
        if (time - _progressTime > 500) {
            cancel = _progressFunc(_progressParm, _progressPercent);
            _progressTime = time;
        }
    }
    return cancel;
}

//----------------------------------------------------------------------------


// select root solver

#undef   BISECTION
#define  FALSE_POSITION


//----------------------------------------------------------------------------
//
// Bisection (midpoint) root solver
//
//----------------------------------------------------------------------------


#ifdef BISECTION


void IsoMesher::intersect_xaxis(
    Point *p0, Point *p1, Point *out) const
{
    float xa, xb;
    if (fsign(p0->density)) {       // p0 < 0  ,  p1 > 0
        xa = p0->v->x();
        xb = p1->v->x();
    } else {                        // p1 < 0  ,  p0 > 0
        xa = p1->v->x();
        xb = p0->v->x();
    }
    float y = p0->v->y();
    float z = p0->v->z();
    float xm;
    float density;

    while (1) {
        xm = (xa + xb) * 0.5f;
        _iso->fDensity(xm, y, z, 0, 1, &density);
        density += 1e-4f;
        if (fabs(density) < TOLERANCE_DENSITY)
            break;
        if (fabs(xa - xb) < TOLERANCE_COORD)
            break;
        
        if (fsign(density))         // pm < 0
            xa = xm;
        else                        // pm > 0
            xb = xm;
    }

    out->density = density;
    *out->v = Vector(xm, y, z);
}

//----------------------------------------------------------------------------

void IsoMesher::intersect_yaxis(
    Point *p0, Point *p1, Point *out) const
{
    float ya, yb;
    if (fsign(p0->density)) {       // p0 < 0  ,  p1 > 0
        ya = p0->v->y();
        yb = p1->v->y();
    } else {                        // p1 < 0  ,  p0 > 0
        ya = p1->v->y();
        yb = p0->v->y();
    }
    float x = p0->v->x();
    float z = p0->v->z();
    float ym;
    float density;

    while (1) {
        ym = (ya + yb) * 0.5f;
        _iso->fDensity(x, ym, z, 0, 1, &density);
        density += 1e-4f;
        if (fabs(density) < TOLERANCE_DENSITY)
            break;
        if (fabs(ya - yb) < TOLERANCE_COORD)
            break;

        if (fsign(density))         // pm < 0
            ya = ym;
        else                        // pm > 0
            yb = ym;
    }

    out->density = density;
    *out->v = Vector(x, ym, z);
}

//----------------------------------------------------------------------------

void IsoMesher::intersect_zaxis(
    Point *p0, Point *p1, Point *out) const
{
    float za, zb;
    if (fsign(p0->density)) {       // p0 < 0  ,  p1 > 0
        za = p0->v->z();
        zb = p1->v->z();
    } else {                        // p1 < 0  ,  p0 > 0
        za = p1->v->z();
        zb = p0->v->z();
    }
    float x = p0->v->x();
    float y = p0->v->y();
    float zm;
    float density;

    while (1) {
        zm = (za + zb) * 0.5f;
        _iso->fDensity(x, y, zm, 0, 1, &density);
        density += 1e-4f;
        if (fabs(density) < TOLERANCE_DENSITY)
            break;
        if (fabs(za - zb) < TOLERANCE_COORD)
            break;

        if (fsign(density))         // pm < 0
            za = zm;
        else                        // pm > 0
            zb = zm;
    }

    out->density = density;
    *out->v = Vector(x, y, zm);
}


#endif


//----------------------------------------------------------------------------
//
// False-position root solver
//
//----------------------------------------------------------------------------


#ifdef FALSE_POSITION


void IsoMesher::intersect_xaxis(
    Point *p0, Point *p1, Point *out) const
{
    float fa, fb;
    float xa, xb;
    if (fsign(p0->density)) {       // p0 < 0  ,  p1 > 0
        fa = p0->density;
        fb = p1->density;
        xa = p0->v->x();
        xb = p1->v->x();
    } else {                        // p1 < 0  ,  p0 > 0
        fa = p1->density;
        fb = p0->density;
        xa = p1->v->x();
        xb = p0->v->x();
    }
    float y = p0->v->y();
    float z = p0->v->z();
    float xm;
    float density;

    while (1) {
        xm = xb - (fb * (xb - xa) / (fb - fa));
        _iso->fDensity(xm, y, z, 0, 1, &density);
        density += 1e-4f;
        if (fabs(density) < TOLERANCE_DENSITY)
            break;
        if (fabs(xa - xb) < TOLERANCE_COORD)
            break;
        
        if (fsign(density)) {       // pm is negative
            xa = xm;
            fa = density;
        } else {                    // pm is positive
            xb = xm;
            fb = density;
        }

        xm = (xa + xb) * 0.5f;
        _iso->fDensity(xm, y, z, 0, 1, &density);
        density += 1e-4f;
        if (fsign(density)) {       // pm < 0
            xa = xm;
            fa = density;
        } else {                    // pm > 0
            xb = xm;
            fb = density;
        }
    }

    out->density = density;
    *out->v = Vector(xm, y, z);
}



void IsoMesher::intersect_yaxis(
    Point *p0, Point *p1, Point *out) const
{
    float fa, fb;
    float ya, yb;
    if (fsign(p0->density)) {       // p0 < 0  ,  p1 > 0
        fa = p0->density;
        fb = p1->density;
        ya = p0->v->y();
        yb = p1->v->y();
    } else {                        // p1 < 0  ,  p0 > 0
        fa = p1->density;
        fb = p0->density;
        ya = p1->v->y();
        yb = p0->v->y();
    }
    float x = p0->v->x();
    float z = p0->v->z();
    float ym;
    float density;

    while (1) {
        ym = yb - (fb * (yb - ya) / (fb - fa));
        _iso->fDensity(x, ym, z, 0, 1, &density);
        density += 1e-4f;
        if (fabs(density) < TOLERANCE_DENSITY)
            break;
        if (fabs(ya - yb) < TOLERANCE_COORD)
            break;
        
        if (fsign(density)) {       // pm is negative
            ya = ym;
            fa = density;
        } else {                    // pm is positive
            yb = ym;
            fb = density;
        }

        ym = (ya + yb) * 0.5f;
        _iso->fDensity(x, ym, z, 0, 1, &density);
        density += 1e-4f;
        if (fsign(density)) {       // pm < 0
            ya = ym;
            fa = density;
        } else {                    // pm > 0
            yb = ym;
            fb = density;
        }
    }

    out->density = density;
    *out->v = Vector(x, ym, z);
}



void IsoMesher::intersect_zaxis(
    Point *p0, Point *p1, Point *out) const
{
    float fa, fb;
    float za, zb;
    if (fsign(p0->density)) {       // p0 < 0  ,  p1 > 0
        fa = p0->density;
        fb = p1->density;
        za = p0->v->z();
        zb = p1->v->z();
    } else {                        // p1 < 0  ,  p0 > 0
        fa = p1->density;
        fb = p0->density;
        za = p1->v->z();
        zb = p0->v->z();
    }
    float x = p0->v->x();
    float y = p0->v->y();
    float zm;
    float density;

    while (1) {
        zm = zb - (fb * (zb - za) / (fb - fa));
        _iso->fDensity(x, y, zm, 0, 1, &density);
        density += 1e-4f;
        if (fabs(density) < TOLERANCE_DENSITY)
            break;
        if (fabs(za - zb) < TOLERANCE_COORD)
            break;
        
        if (fsign(density)) {       // pm is negative
            za = zm;
            fa = density;
        } else {                    // pm is positive
            zb = zm;
            fb = density;
        }

        zm = (za + zb) * 0.5f;
        _iso->fDensity(x, y, zm, 0, 1, &density);
        density += 1e-4f;
        if (fsign(density)) {       // pm < 0
            za = zm;
            fa = density;
        } else {                    // pm > 0
            zb = zm;
            fb = density;
        }
    }

    out->density = density;
    *out->v = Vector(x, y, zm);
}


#endif
