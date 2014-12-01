//----------------------------------------------------------------------------
// ThreeD Transform
//----------------------------------------------------------------------------

#ifndef _THREED_TRANSFORM_H
#define _THREED_TRANSFORM_H

#include <threed/matrix.h>
#include <threed/vector.h>

#ifndef M_PI_180
#define M_PI_180 0.01745329251994329576
#endif

namespace ThreeD {


/**
 * Transform, manages object transformation.
 */
class Transform : public Matrix
{
public:
    /**
     * Constructor.
     *
     * Constructs a transform with zero translation and rotation vectors.
     */
    Transform() : Matrix() {}
    
    /**
     * Copy constructor.
     */
    Transform(const Transform &other)
        : Matrix(other) {}

    /**
     * Transform from matrix.
     */
    Transform(const Matrix &other)
        : Matrix(other) {}
    
    /**
     * Translates, moves an object around.  The object of this transform
     * is translated.  If @p relative is true, the @p vec vector is added
     * to the translation vector.  If @p relative is false, the vector is
     * assigned as the translation vector.
     */
    void translate(const Vector &vec) { Matrix::translate(vec); }
    
    /**
     * Rotates an object around its origin.  The object of this transform
     * is rotated.  If @p relative is true, the @p vec vector is added
     * to the rotation vector.  If @p relative is false, the vector is
     * assigned as the rotation vector.
     */
    void rotate(const Vector &vec) { Matrix::rotate(vec); }

    /**
     * Rotates an object around its origin.  The object of this transform
     * is rotated.  If @p relative is true, the @p vec vector is added
     * to the rotation vector.  If @p relative is false, the vector is
     * assigned as the rotation vector.
     */
    void rotateDeg(float x, float y, float z)
    {
        Matrix::rotate((float)(x * M_PI_180),
                       (float)(y * M_PI_180),
                       (float)(z * M_PI_180));
    }

    void rotateDeg(const Vector &vec) { rotateDeg(vec.x(), vec.y(), vec.z()); }
    
    /**
     * Scales an object.  The object of this transform is scaled.  If
     * @p relative is true, the @p vec vector is added to the scale vector.
     * If @p relative is false, the vector is assigned as the scale vector.
     */
    void scale(const Vector &vec) { Matrix::scale(vec); }
    
    /**
     * Multiplies this transform into the current OpenGL matrix.
     *
     * Note:  any parameters that should affect the output should be
     * effected into OpenGL prior to the invocation of this method.
     */
    void glMultMatrix();
#if 0    
    /**
     * Transforms vector @p v according to this transform, placing the
     * result in that same vector.
     */
    void transform(Vector *v) const { Matrix::transform(v); }
#endif
};


} // namespace ThreeD
#endif // _THREED_TRANSFORM_H
