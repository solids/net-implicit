//----------------------------------------------------------------------------
// ThreeD Plane     
//----------------------------------------------------------------------------

#ifndef _THREED_PLANE_H
#define _THREED_PLANE_H

#include <threed/vector.h>

namespace ThreeD {


/**
 * Plane, the mathematical entity defined by a point and a normal vector.
 */
class Plane
{
public:
    /**
     * Constructor.
     *
     * Constructs an XY plane.
     */
    Plane() : _a(0.0), _b(0.0), _c(1.0), _d(0.0) {}
    
    /**
     * Constructor.
     *
     * Constructs a plane using the components @p a, @p b, @p c and @p d.
     */
    Plane(double a, double b, double c, double d)
        : _a(a), _b(b), _c(c), _d(d) {}
    
    /**
     * Constructor.
     *
     * Constructs a plane using the three vertices @p v0, @p v1 and @p v2.
     */
    Plane(const Vector &v0, const Vector &v1, const Vector &v2)
    {
        // _abc = (v0 - v1) % (v1 - v2);
        double v0_v1_x = (v0 - v1).x();
        double v0_v1_y = (v0 - v1).y();
        double v0_v1_z = (v0 - v1).z();
        double v1_v2_x = (v1 - v2).x();
        double v1_v2_y = (v1 - v2).y();
        double v1_v2_z = (v1 - v2).z();
        _a = (v0_v1_y * v1_v2_z) - (v0_v1_z * v1_v2_y);
        _b = (v0_v1_z * v1_v2_x) - (v0_v1_x * v1_v2_z);
        _c = (v0_v1_x * v1_v2_y) - (v0_v1_y * v1_v2_x);
        // _d = -(v0 * _abc);
        _d = -( (v0.x() * _a) + (v0.y() * _b) + (v0.z() * _c) );
    }

    /**
     * Constructor.
     *
     * Constructs a plane to be a copy of the @p model plane.
     */
    Plane(const Plane &model) { operator=(model); }
 
    /**
     * Operator=.
     *
     * @return a plane, after assigning the @p other plane to it.
     */
    Plane &operator=(const Plane &other)
    {
        _a = other._a;
        _b = other._b;
        _c = other._c;
        _d = other._d;
        return *this;
    }

    /**
     * @return a plane that is a normalization of this plane.
     */
    Plane normalized() const
    {
        double len = sqrt(_a * _a + _b * _b + _c * _c);
        if (len == 0.0)
            len = 1.0;
        return Plane(_a / len, _b / len, _c / len, _d / len);
    }
    
    /**
     * @return the A component for this plane.
     */
    double a() const { return _a; }
    
    /**
     * @return the B component for this plane.
     */
    double b() const { return _b; }
    
    /**
     * @return the C component for this plane.
     */
    double c() const { return _c; }
    
    /**
     * @return the D component for this plane.
     */
    double d() const { return _d; }
    
    /**
     * @return the normal for this plane.
     */
    const Vector normal() const
    {
        return Vector((float)_a, (float)_b, (float)_c);
    }
    
    /**
     * @return the product of this plane with the vector @p v.
     *
     * May be used to classify a point (a vector) with regards to the plane:
     * a zero result means the point is on the plane;  a negative result
     * means the point is behind the plane;  a positive result means the
     * point is in front of the plane. 
     */
    double classify(const Vector &v) const
    {
        return - (_a * v.x() + _b * v.y() + _c * v.z() + _d);
    }
    
    /**
     * Determines if a line bound by @p v1 and @p v2 intersects this plane.
     *
     * If it does, @p vx is set to the point of intersection, and the
     * return value reflects the side of the intersection:  negative if
     * behind, positive if in front.
     *
     * If there is no intersection, a zero is returned.
     *
     * @return the side of the intersection, or zero if no intersection.
     */
    int intersect(const Vector &v1, const Vector &v2, Vector *vx) const;
    
    /**
     * Comparison operators.
     */
    friend inline bool operator==(const Plane &a, const Plane &b);
    friend inline bool operator!=(const Plane &a, const Plane &b);
    
protected:
    /*
     * data
     */

    double _a;
    double _b;
    double _c;
    double _d;
};


/**
 * Comparison operators.
 */

inline bool operator==(const Plane &a, const Plane &b)
{
    return (fabs(a._a - b._a) < 1e-1 &&
            fabs(a._b - b._b) < 1e-1 &&
            fabs(a._c - b._c) < 1e-1 &&
            fabs(a._d - b._d) < 1e-1);
}

inline bool operator!=(const Plane &a, const Plane &b)
{
    return (fabs(a._a - b._a) > 1e-1 ||
            fabs(a._b - b._b) > 1e-1 ||
            fabs(a._c - b._c) > 1e-1 ||
            fabs(a._d - b._d) > 1e-1);
}


} // namespace ThreeD
#endif // _THREED_PLANE_H
