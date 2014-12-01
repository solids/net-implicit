//----------------------------------------------------------------------------
// ThreeD Color   
//----------------------------------------------------------------------------

#ifndef _THREED_COLOR_H
#define _THREED_COLOR_H

namespace ThreeD {


/**
 * Color, a specification of an RGB color.
 */
class Color
{
public:
    /**
     * Constructor.
     *
     * Constructs a color using the three components @p red, @p green and
     * @p blue.
     * Parameters default to the color black.
     */
    Color(float red = 0.0f, float green = 0.0f, float blue = 0)
        : _r(red), _g(green), _b(blue) {};

    /*
    Color(float red, float green, float blue)
        : _r((int)(red / 65535.0f)), _g((int)(green / 65535.0f)), _b((int)(blue / 65535.0f)) {};
    */

    /**
     * Assignment operator.
     *
     * @return this color, after assigning the @p other color to it.
     */
    Color &operator=(const Color &other)
    {
        _r = other._r;
        _g = other._g;
        _b = other._b;
        return *this;
    }
    
    /**
     * @return the red component for this color.
     */
    float red() const { return _r; }
    
    /**
     * @return the green coordinate for this color.
     */
    float green() const { return _g; }
    
    /**
     * @return the blue coordinate for this color.
     */
    float blue() const { return _b; }
    
    /**
     * Operator==.
     *
     * @return true if colors @p a and @p b are equal.
     */
    friend inline bool operator==(const Color &a, const Color &b);
    
    /**
     * Operator!=.
     *
     * @return true if colors @p a and @p b are unequal.
     */
    friend inline bool operator!=(const Color &a, const Color &b);

    /**
     * X= operators.
     */
    Color &operator+=(const Color &v) { return operator=(*this + v); }
    Color &operator-=(const Color &v) { return operator=(*this - v); }
    Color &operator*=(const Color &v) { return operator=(*this * v); }
    Color &operator/=(const Color &v) { return operator=(*this / v); }

    Color &operator+=(float f) { return operator=(*this + f); }
    Color &operator-=(float f) { return operator=(*this - f); }
    Color &operator*=(float f) { return operator=(*this * f); }
    Color &operator/=(float f) { return operator=(*this / f); }

    Color &operator+=(double f) { return operator=(*this + f); }
    Color &operator-=(double f) { return operator=(*this - f); }
    Color &operator*=(double f) { return operator=(*this * f); }
    Color &operator/=(double f) { return operator=(*this / f); }

    /**
     * Unary operators.
     */
    Color operator+() const { return *this; }
    Color operator-() const { return Color(-_r, -_g, -_b); }

    /**
     * Binary operators.
     */
    friend inline Color operator+(const Color &a, const Color &b);
    friend inline Color operator-(const Color &a, const Color &b);
    friend inline Color operator*(const Color &a, const Color &b);
    friend inline Color operator/(const Color &a, const Color &b);

    friend inline Color operator+(const Color &v, float f);
    friend inline Color operator-(const Color &v, float f);
    friend inline Color operator*(const Color &v, float f);
    friend inline Color operator/(const Color &v, float f);

    friend inline Color operator+(const Color &v, double f);
    friend inline Color operator-(const Color &v, double f);
    friend inline Color operator*(const Color &v, double f);
    friend inline Color operator/(const Color &v, double f);
    
protected:
    float _r, _g, _b;
};


/**
 * Friend operators.
 */

inline bool operator==(const Color &a, const Color &b)
{
    return (a._r == b._r) && (a._g == b._g) && (a._b == b._b);
}

inline bool operator!=(const Color &a, const Color &b)
{
    return (a._r != b._r) || (a._g != b._g) || (a._b != b._b);
}

inline Color operator+(const Color &a, const Color &b)
{
    return Color( (a._r + b._r), (a._g + b._g), (a._b + b._b) );
}

inline Color operator-(const Color &a, const Color &b)
{
    return Color( (a._r - b._r), (a._g - b._g), (a._b - b._b) );
}

inline Color operator*(const Color &a, const Color &b)
{
    return Color( (a._r * b._r), (a._g * b._g), (a._b * b._b) );
}

inline Color operator/(const Color &a, const Color &b)
{
    return Color( (a._r / b._r), (a._g / b._g), (a._b / b._b) );
}

inline Color operator+(const Color &v, float f)
{
    return Color( (v._r + f), (v._g + f), (v._b + f) );
}

inline Color operator-(const Color &v, float f)
{
    return v + (-f);
}

inline Color operator*(const Color &v, float f)
{
    return Color( (v._r * f), (v._g * f), (v._b * f) );
}

inline Color operator/(const Color &v, float f)
{
    return v * (1.0 / f);
}

inline Color operator+(const Color &v, double f)
{
    return Color( (float)(v._r + f),
                   (float)(v._g + f),
                   (float)(v._b + f) );
}

inline Color operator-(const Color &v, double f)
{
    return v + (-f);
}

inline Color operator*(const Color &v, double f)
{
    return Color( (float)(v._r * f),
                   (float)(v._g * f),
                   (float)(v._b * f) );
}

inline Color operator/(const Color &v, double f)
{
    return v * (1.0 / f);
}


} // namespace ThreeD
#endif // _THREED_COLOR_H
