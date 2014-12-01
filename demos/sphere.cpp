
#include <threed/threed.h>
#include "sphere.h"

//----------------------------------------------------------------------------

SphereIsosurface::SphereIsosurface(float rad)
{
    _center = ThreeD::Vector();
    _radius = rad;

    ThreeD::Vector v1 = _center - _radius;
    ThreeD::Vector v2 = _center + _radius;
    Isosurface::addBoundingBox(ThreeD::BoundingBox(v1, v2));

    _mat.color = ThreeD::Color(1.0f, 0.0f, 0.0f);  // red
    _mat.ambient = ThreeD::Color();
    _mat.diffuse = 1.0f;
    _mat.specular = 1.0f;
    _mat.brilliance = 1.0f;

    Isosurface::setTransform(ThreeD::Transform());
}

//----------------------------------------------------------------------------

double SphereIsosurface::calcDensity(float xt, float yt, float zt)
{
    xt -= _center.x();
    yt -= _center.y();
    zt -= _center.z();

    double sqr_dist = xt * xt + yt * yt + zt * zt;
    double sqr_rad = _radius * _radius;
    double d = sqr_dist - sqr_rad;

    return d;
}

//----------------------------------------------------------------------------

void SphereIsosurface::fDensity(
    float x0, float y0, float z0,
    float dz, int num_points, float *densities)
{
    for (int i = 0; i < num_points; ++i) {
        float xt, yt, zt;
        _globalTransInv.transform(x0, y0, z0, &xt, &yt, &zt);

        densities[i] = calcDensity(xt, yt, zt);

        z0 += dz;
    }
}

//----------------------------------------------------------------------------

void SphereIsosurface::fNormal(
    const ThreeD::Vector *point, ThreeD::Vector *normal)
{
    float xt, yt, zt;
    _globalTransInv.transform(
        point->x(), point->y(), point->z(),
        &xt, &yt, &zt);

    double d = calcDensity(xt, yt, zt);
    float nx = calcDensity(xt + 0.001f, yt, zt) - d;
    float ny = calcDensity(xt, yt + 0.001f, zt) - d;
    float nz = calcDensity(xt, yt, zt + 0.001f) - d;

    _globalTransInv.transformNormal(nx, ny, nz, &nx, &ny, &nz);
    *normal = ThreeD::Vector(nx, ny, nz).normalized();
}

//----------------------------------------------------------------------------

const ThreeD::Isosurface::Material &SphereIsosurface::fMaterial(
    const ThreeD::Vector *point, float density)
{
    return _mat;
}
