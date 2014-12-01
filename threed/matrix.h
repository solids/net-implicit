//----------------------------------------------------------------------------
// 4x4 matrix
//----------------------------------------------------------------------------

#ifndef _THREED_MATRIX_H
#define _THREED_MATRIX_H

#include <threed/vector.h>
#include <string.h>


namespace ThreeD {


class Matrix {
public:
    typedef float MATRIX[4][4];     // [column][row]

    /** Creates an identity matrix. */
    Matrix() { setIdentity(); }

    /** Creates a matrix preloaded with specific values.
     *  @param matrix specifies a 4x4 matrix with the primary
     *         dimension holding columns.  eg, matrix[1][3]
     *         means column 1, row 3.
     */
    Matrix(float matrix[4][4]) { setValues(matrix); }

    /** Creates a copy of another matrix. */
    Matrix(const Matrix &other) { setValues(other._matrix); }

    /** Copies the other matrix into this one. */
    Matrix &operator=(const Matrix &other)
    {
        setValues(other._matrix);
        return *this;
    }

    /** Initializes the current matrix to be the identity matrix.
     */
    void setIdentity() { setValues(_identity); }

    /** Initializes the current matrix with arbitrary values.
     */
    void setValues(const float values[4][4])
    {
        memcpy(_matrix, values, sizeof(MATRIX));
    }

    /** Returns the matrix values.
     */
    void getValues(MATRIX *values)
    {
        memcpy(values, _matrix, sizeof(MATRIX));
    }

    /** Returns a reference to the matrix. */
    const MATRIX &matrix() const { return _matrix; }

    /** Applies rotation into the transformation.
     *  Creates a rotational matrix for the specified angles,
     *  then multiplies that matrix into the current matrix.
     *  @param x angle of rotation, in radians, around the x axis.
     *  @param y angle of rotation, in radians, around the y axis.
     *  @param z angle of rotation, in radians, around the z axis.
     */
    void rotate(float x, float y, float z);

    void rotate(const Vector &v) { rotate(v.x(), v.y(), v.z()); }

    /** Applies translation into the transformation.
     *  Creates a translation matrix for the specified distances,
     *  then multiplies that matrix into the current matrix.
     *  @param x distance to translate on the x axis
     *  @param y distance to translate on the y axis
     *  @param z distance to translate on the z axis
     */
    void translate(float x, float y, float z);

    void translate(const Vector &v) { translate(v.x(), v.y(), v.z()); }

    /** Applies scaling into the transformation.
     *  Creates a scaling matrix for the specified factors,
     *  then multiplies that matrix into the current matrix.
     *  @param x scaling factor on the x axis
     *  @param y scaling factor on the y axis
     *  @param z scaling factor on the z axis
     */
    void scale(float x, float y, float z);

    /** Applies scaling into the transformation.
     *  @param v scaling vector
     *  @see   scale(float x, float y, float z)
     */
    void scale(const Vector &v) { scale(v.x(), v.y(), v.z()); }

    /** Applies rotation into the transformation.
     *  Creates a rotational matrix for the specified angle
     *  along the specified axis, then multiplies that matrix
     *  into the current matrix.
     *  @param axis axis of rotation
     *  @param angle of rotation, in radians, around the axis.
     */
    void rotateAboutAxis(const Vector &axis, double angle);

    /** Applies coordinate transformation into the transformation.
     *  Creates a canonical transformation matrix, then multiplies
     *  that matrix into the current matrix.
     */
    void coordinate(const Vector &origin, const Vector &direction,
                    float radius, float length);

    /** Inverts the current matrix.
     *  @return false if the current matrix cannot be inverted,
     *          in other words, if it is a singular matrix.
     *          Otherwise inverts the matrix and returns true.
     */
    bool invert();

    /** Transpose the current matrix.
     */
    /*Matrix transpose();*/

    /** Multiplies the current matrix with the specified matrix. */
    Matrix &operator*=(const Matrix &m);

    /** Transforms (x,y,z) by the current matrix. */
    void transform(
        float xi, float yi, float zi,
        float *xo, float *yo, float *zo) const;

    /** Transforms (nx,ny,nz) by the current matrix. */
    void transformNormal(
        float xi, float yi, float zi,
        float *xo, float *yo, float *zo) const;

    /** Transforms (x,y,z) by the current matrix. */
    void transform(float *x, float *y, float *z) const
    {
        transform(*x, *y, *z, x, y, z);
    }
    
    /** Transforms vector by the current matrix. */
    void transform(Vector *v) const
    {
        float x, y, z;
        transform(v->x(), v->y(), v->z(), &x, &y, &z);
        *v = Vector(x, y, z);
    }

    /** Transforms vector by current matrix, returning another vector. */
    Vector transform(const Vector &v) const
    {
        float x, y, z;
        transform(v.x(), v.y(), v.z(), &x, &y, &z);
        return Vector(x, y, z);
    }

private:
    static void multiply(MATRIX *result, const MATRIX ma, const MATRIX mb);

    MATRIX _matrix;
    static const MATRIX _identity;
};


} // namespace ThreeD
#endif // _THREED_MATRIX_H
