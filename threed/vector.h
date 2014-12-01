//----------------------------------------------------------------------------
// ThreeD Vector
//----------------------------------------------------------------------------

#ifndef _THREED_VECTOR_H
#define _THREED_VECTOR_H

#pragma warning(disable: 4786)  // name truncated to 255 chars

#include <math.h>
#include <string.h>
#include <threed/misc.h>


namespace ThreeD {


/**
 * Vector, a specification of a vector in a 3-dimensional plane.
 */
class Vector
{
public:
    /**
     * Constructor.
     *
     * Constructs a vector in 3-space with components (0,0,0).
     */
    inline Vector() : _x(0), _y(0), _z(0) {}

    /**
     * Constructor.
     *
     * Constructs a vector in 3-space using an (x,y,z) specification.
     */
    inline Vector(float x, float y, float z) : _x(x), _y(y), _z(z) {}

    /**
     * Copy constructor.
     *
     * Constructs a vector in 3-space from another vector.
     */
    inline Vector(const Vector &other) : _x(other._x), _y(other._y), _z(other._z) {}

    /**
     * Operator=.
     *
     * @return this vector, after assigning the @p other vector to it.
     */
    inline Vector &operator=(const Vector &other)
    {
        memcpy(this, &other, sizeof(Vector));
        //_x = other._x;
        //_y = other._y;
        //_z = other._z;
        return *this;
    }

    /**
     * Computes and returns the length of a vector, which is the square-root
     * of the sum of the squares of the x, y and z coordinates.
     *
     * @returns the length of a vector.
     */
    double length() const { return sqrt(_x * _x + _y * _y + _z * _z); }

    /**
     * Computes and returns the length of a vector, which is the square-root
     * of the sum of the squares of the x, y and z coordinates.
     *
     * @returns the length of a vector.
     */
    double length2() const { return _x * _x + _y * _y + _z * _z; }

    /**
     * @return a vector that is a normalization of this vector.
     */
    Vector normalized() const
    {
        double len = length();
        if (len == 1.0 || len == 0.0)
            return *this;
        else
            return (*this / len);
    }

    /**
     * @return a vector with absolute coordinates.
     */
    Vector abs() const
    {
        return Vector((float)fabs(_x), (float)fabs(_y), (float)fabs(_z));
    }

    /**
     * @return the sum of the three components
     */
    operator float() const { return (float)(_x + _y + _z); }

    /**
     * @return the sum of the three components
     */
    operator double() const { return (double)(_x + _y + _z); }

    /**
     * @return true if this vector and vector @p v are close enough.
     */
    bool almostEqual(const Vector &v) const { return almostEqual(*this, v); }

    /**
     * @return true if this vector is between @p v1 and @p v2.
     */
    bool isBetween(const Vector &v1, const Vector &v2) const
    {
        return (_x >= v1._x && _x <= v2._x &&
                _y >= v1._y && _y <= v2._y &&
                _z >= v1._z && _z <= v2._z);
    }

    /** Round the vector
     */
    void round()
    {
        _x = roundFloat(_x);
        _y = roundFloat(_y);
        _z = roundFloat(_z);
    }
 
    /**
     * @return the x coordinate for this vector.
     */
    float x() const { return _x; }

    /**
     * @return the y coordinate for this vector.
     */
    float y() const { return _y; }

    /**
     * @return the z coordinate for this vector.
     */
    float z() const { return _z; }

    /**
     * X= operators.
     */
    Vector &operator+=(const Vector &v) { return operator=(*this + v); }
    Vector &operator-=(const Vector &v) { return operator=(*this - v); }
    Vector &operator*=(const Vector &v) { return operator=(*this * v); }
    Vector &operator/=(const Vector &v) { return operator=(*this / v); }

    Vector &operator+=(float f) { return operator=(*this + f); }
    Vector &operator-=(float f) { return operator=(*this - f); }
    Vector &operator*=(float f) { return operator=(*this * f); }
    Vector &operator/=(float f) { return operator=(*this / f); }

    Vector &operator+=(double f) { return operator=(*this + f); }
    Vector &operator-=(double f) { return operator=(*this - f); }
    Vector &operator*=(double f) { return operator=(*this * f); }
    Vector &operator/=(double f) { return operator=(*this / f); }

    /**
     * Unary operators.
     */
    Vector operator+() const { return *this; }
    Vector operator-() const { return Vector(-_x, -_y, -_z); }

    /**
     * Comparison operators.
     */
    friend inline bool operator==(const Vector &a, const Vector &b);
    friend inline bool operator!=(const Vector &a, const Vector &b);
    /*
    friend inline bool operator<(const Vector &a, const Vector &b);
    friend inline bool operator<=(const Vector &a, const Vector &b);
    friend inline bool operator>(const Vector &a, const Vector &b);
    friend inline bool operator>=(const Vector &a, const Vector &b);
    */

    /**
     * Binary operators.
     */
    friend inline Vector operator+(const Vector &a, const Vector &b);
    friend inline Vector operator-(const Vector &a, const Vector &b);
    friend inline Vector operator*(const Vector &a, const Vector &b);
    friend inline Vector operator/(const Vector &a, const Vector &b);
    friend inline Vector operator%(const Vector &a, const Vector &b);

    friend inline Vector operator+(const Vector &v, float f);
    friend inline Vector operator-(const Vector &v, float f);
    friend inline Vector operator*(const Vector &v, float f);
    friend inline Vector operator/(const Vector &v, float f);

    friend inline Vector operator+(const Vector &v, double f);
    friend inline Vector operator-(const Vector &v, double f);
    friend inline Vector operator*(const Vector &v, double f);
    friend inline Vector operator/(const Vector &v, double f);

    friend inline Vector operator/(double f, const Vector &v);

    /**
     * Compares two vectors to a precision of 1e-3.
     *
     * @return true if equal, or almost equal, and false otherwise.
     */
    static bool almostEqual(const Vector &a, const Vector &b)
    {
        return ((fabs(a._x - b._x) < 1e-3) &&
                (fabs(a._y - b._y) < 1e-3) &&
                (fabs(a._z - b._z) < 1e-3));
    }

protected:
    float _x, _y, _z;
};


/**
 * Comparison operators.
 */

inline bool operator==(const Vector &a, const Vector &b)
{
    return (a._x == b._x) && (a._y == b._y) && (a._z == b._z);
}

inline bool operator!=(const Vector &a, const Vector &b)
{
    return (a._x != b._x) || (a._y != b._y) || (a._z != b._z);
}

/*
inline bool operator<(const Vector &a, const Vector &b)
{
    return (a._x < b._x) && (a._y < b._y) && (a._z < b._z);
}

inline bool operator<=(const Vector &a, const Vector &b)
{
    return (a._x <= b._x) && (a._y <= b._y) && (a._z <= b._z);
}

inline bool operator>(const Vector &a, const Vector &b)
{
    return (a._x > b._x) && (a._y > b._y) && (a._z > b._z);
}

inline bool operator>=(const Vector &a, const Vector &b)
{
    return (a._x >= b._x) && (a._y >= b._y) && (a._z >= b._z);
}
*/

/**
 * Friend operators.
 */

inline Vector operator+(const Vector &a, const Vector &b)
{
    return Vector( (a._x + b._x), (a._y + b._y), (a._z + b._z) );
}

inline Vector operator-(const Vector &a, const Vector &b)
{
    return Vector( (a._x - b._x), (a._y - b._y), (a._z - b._z) );
}

inline Vector operator*(const Vector &a, const Vector &b)
{
    return Vector( (a._x * b._x), (a._y * b._y), (a._z * b._z) );
}

inline Vector operator/(const Vector &a, const Vector &b)
{
    return Vector( (a._x / b._x), (a._y / b._y), (a._z / b._z) );
}

inline Vector operator%(const Vector &a, const Vector &b)
{
    // cross product
    return Vector( (a._y * b._z) - (a._z * b._y),
                   (a._z * b._x) - (a._x * b._z),
                   (a._x * b._y) - (a._y * b._x) );
}

inline Vector operator+(const Vector &v, float f)
{
    return Vector( (v._x + f), (v._y + f), (v._z + f) );
}

inline Vector operator-(const Vector &v, float f)
{
    return v + (-f);
}

inline Vector operator*(const Vector &v, float f)
{
    return Vector( (v._x * f), (v._y * f), (v._z * f) );
}

inline Vector operator/(const Vector &v, float f)
{
    return v * (1.0 / f);
}

inline Vector operator+(const Vector &v, double f)
{
    return Vector( (float)(v._x + f),
                   (float)(v._y + f),
                   (float)(v._z + f) );
}

inline Vector operator-(const Vector &v, double f)
{
    return v + (-f);
}

inline Vector operator*(const Vector &v, double f)
{
    return Vector( (float)(v._x * f),
                   (float)(v._y * f),
                   (float)(v._z * f) );
}

inline Vector operator/(const Vector &v, double f)
{
    return Vector((float)(v._x / f), (float)(v._y / f), (float)(v._z / f));
}

inline Vector operator/(double f, const Vector &v)
{
    return Vector((float)(f / v._x), (float)(f / v._y), (float)(f / v._z));
}

inline Vector abs(const Vector &v)
{
    return Vector((float)fabs(v.x()), (float)fabs(v.y()), (float)fabs(v.z()));
}


} // namespace ThreeD
#endif // _THREED_VECTOR_H
