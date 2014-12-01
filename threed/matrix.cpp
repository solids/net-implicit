//----------------------------------------------------------------------------
// POV-Ray transform value
//----------------------------------------------------------------------------

#include <threed/matrix.h>

#include <math.h>

using namespace ThreeD;

//----------------------------------------------------------------------------

const Matrix::MATRIX Matrix::_identity = {
    { 1, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 1 }
};

//----------------------------------------------------------------------------

void Matrix::rotate(float x, float y, float z)
{
    float cosx = (float)cos(x);
    float cosy = (float)cos(y);
    float cosz = (float)cos(z);
    float sinx = (float)sin(x);
    float siny = (float)sin(y);
    float sinz = (float)sin(z);

    MATRIX mx, my, mz;

    memcpy(mx, &_identity, sizeof(MATRIX));
    mx[1][1] = cosx;
    mx[2][2] = cosx;
    mx[1][2] = sinx;
    mx[2][1] = -sinx;

    memcpy(my, &_identity, sizeof(MATRIX));
    my[0][0] = cosy;
    my[2][2] = cosy;
    my[0][2] = -siny;
    my[2][0] = siny;

    memcpy(mz, &_identity, sizeof(MATRIX));
    mz[0][0] = cosz;
    mz[1][1] = cosz;
    mz[0][1] = sinz;
    mz[1][0] = -sinz;

    MATRIX mxy, mxyz;

    multiply(&mxy, mx, my);
    multiply(&mxyz, mxy, mz);
    multiply(&_matrix, _matrix, mxyz);
}

//----------------------------------------------------------------------------

void Matrix::translate(float x, float y, float z)
{
    MATRIX m;
    memcpy(m, &_identity, sizeof(MATRIX));
    m[3][0] = x; //-x
    m[3][1] = y; //-y
    m[3][2] = z;

    multiply(&_matrix, _matrix, m);
}

//----------------------------------------------------------------------------

void Matrix::scale(float x, float y, float z)
{
    MATRIX m;
    memset(m, 0, sizeof(MATRIX));
    m[0][0] = x;
    m[1][1] = y;
    m[2][2] = z;
    m[3][3] = 1.0f;

    multiply(&_matrix, _matrix, m);
}

//----------------------------------------------------------------------------

void Matrix::rotateAboutAxis(const Vector &axis, double angle)
{
    double cosx = cos(angle);
    double sinx = sin(angle);

    double axis_len = axis.length2();
    double x = axis.x();
    double y = axis.y();
    double z = axis.z();
    if (axis_len != 1.0) {
        axis_len = sqrt(axis_len);
        x /= axis_len;
        y /= axis_len;
        z /= axis_len;
    }

    MATRIX m;
    memcpy(m, &_identity, sizeof(MATRIX));

    m[0][0] = (float)(x * x + cosx * (1.0 - x * x));
    m[0][1] = (float)(x * y * (1.0 - cosx) + z * sinx);
    m[0][2] = (float)(x * z * (1.0 - cosx) - y * sinx);

    m[1][0] = (float)(x * y * (1.0 - cosx) - z * sinx);
    m[1][1] = (float)(y * y + cosx * (1.0 - y * y));
    m[1][2] = (float)(y * z * (1.0 - cosx) + x * sinx);

    m[2][0] = (float)(x * z * (1.0 - cosx) + y * sinx);
    m[2][1] = (float)(y * z * (1.0 - cosx) - x * sinx);
    m[2][2] = (float)(z * z + cosx * (1.0 - z * z));

    multiply(&_matrix, _matrix, m);
}

//----------------------------------------------------------------------------

void Matrix::coordinate(const Vector &origin, const Vector &direction,
                        float radius, float length)
{
    scale(radius, radius, length);

    Vector t;
    float dirz = direction.z();
    if (fabs(dirz) > 1.0 - 0.001) {
        t = Vector(1.0f, 0.0f, 0.0f);
        dirz = dirz < 0.0f ? -1.0f : 1.0f;
    } else
        t = Vector(-direction.y(), direction.x(), 0.0f);
    rotateAboutAxis(t, acos(dirz));

    translate(origin.x(), origin.y(), origin.z());
}

//----------------------------------------------------------------------------

bool Matrix::invert()
{
#define MIJ(i,j) float &m##i##j = _matrix[i][j];
#define MI(i) MIJ(i,0) MIJ(i,1) MIJ(i,2) MIJ(i,3)
    MI(0) MI(1) MI(2) MI(3)
#undef MI
#undef MIJ

    double d00 = m11 * m22 * m33 + m12 * m23 * m31 + m13 * m21 * m32
               - m31 * m22 * m13 - m32 * m23 * m11 - m33 * m21 * m12;
    double d01 = m10 * m22 * m33 + m12 * m23 * m30 + m13 * m20 * m32
               - m30 * m22 * m13 - m32 * m23 * m10 - m33 * m20 * m12;
    double d02 = m10 * m21 * m33 + m11 * m23 * m30 + m13 * m20 * m31
               - m30 * m21 * m13 - m31 * m23 * m10 - m33 * m20 * m11;
    double d03 = m10 * m21 * m32 + m11 * m22 * m30 + m12 * m20 * m31
               - m30 * m21 * m12 - m31 * m22 * m10 - m32 * m20 * m11;

    double d10 = m01 * m22 * m33 + m02 * m23 * m31 + m03 * m21 * m32
               - m31 * m22 * m03 - m32 * m23 * m01 - m33 * m21 * m02;
    double d11 = m00 * m22 * m33 + m02 * m23 * m30 + m03 * m20 * m32
               - m30 * m22 * m03 - m32 * m23 * m00 - m33 * m20 * m02;
    double d12 = m00 * m21 * m33 + m01 * m23 * m30 + m03 * m20 * m31
               - m30 * m21 * m03 - m31 * m23 * m00 - m33 * m20 * m01;
    double d13 = m00 * m21 * m32 + m01 * m22 * m30 + m02 * m20 * m31
               - m30 * m21 * m02 - m31 * m22 * m00 - m32 * m20 * m01;

    double d20 = m01 * m12 * m33 + m02 * m13 * m31 + m03 * m11 * m32
               - m31 * m12 * m03 - m32 * m13 * m01 - m33 * m11 * m02;
    double d21 = m00 * m12 * m33 + m02 * m13 * m30 + m03 * m10 * m32
               - m30 * m12 * m03 - m32 * m13 * m00 - m33 * m10 * m02;
    double d22 = m00 * m11 * m33 + m01 * m13 * m30 + m03 * m10 * m31
               - m30 * m11 * m03 - m31 * m13 * m00 - m33 * m10 * m01;
    double d23 = m00 * m11 * m32 + m01 * m12 * m30 + m02 * m10 * m31
               - m30 * m11 * m02 - m31 * m12 * m00 - m32 * m10 * m01;

    double d30 = m01 * m12 * m23 + m02 * m13 * m21 + m03 * m11 * m22
               - m21 * m12 * m03 - m22 * m13 * m01 - m23 * m11 * m02;
    double d31 = m00 * m12 * m23 + m02 * m13 * m20 + m03 * m10 * m22
               - m20 * m12 * m03 - m22 * m13 * m00 - m23 * m10 * m02;
    double d32 = m00 * m11 * m23 + m01 * m13 * m20 + m03 * m10 * m21
               - m20 * m11 * m03 - m21 * m13 * m00 - m23 * m10 * m01;
    double d33 = m00 * m11 * m22 + m01 * m12 * m20 + m02 * m10 * m21
               - m20 * m11 * m02 - m21 * m12 * m00 - m22 * m10 * m01;

    double d = (m00 * d00) - (m01 * d01) + (m02 * d02) - (m03 * d03);
    if (d == 0.0)
        return false;

    m00 = (float)(d00 / d);
    m01 = (float)(-d10 / d);
    m02 = (float)(d20 / d);
    m03 = (float)(-d30 / d);

    m10 = (float)(-d01 / d);
    m11 = (float)(d11 / d);
    m12 = (float)(-d21 / d);
    m13 = (float)(d31 / d);

    m20 = (float)(d02 / d);
    m21 = (float)(-d12 / d);
    m22 = (float)(d22 / d);
    m23 = (float)(-d32 / d);

    m30 = (float)(-d03 / d);
    m31 = (float)(d13 / d);
    m32 = (float)(-d23 / d);
    m33 = (float)(d33 / d);

    return true;
}

//----------------------------------------------------------------------------

#if 0

Matrix Matrix::transpose()
{
    MATRIX result;

    result[0][0] = _matrix[0][0];
    result[0][1] = _matrix[1][0];
    result[0][2] = _matrix[2][0];
    result[0][3] = _matrix[3][0];

    result[1][0] = _matrix[0][1];
    result[1][1] = _matrix[1][1];
    result[1][2] = _matrix[2][1];
    result[1][3] = _matrix[3][1];

    result[2][0] = _matrix[0][2];
    result[2][1] = _matrix[1][2];
    result[2][2] = _matrix[2][2];
    result[2][3] = _matrix[3][2];

    result[3][0] = _matrix[0][3];
    result[3][1] = _matrix[1][3];
    result[3][2] = _matrix[2][3];
    result[3][3] = _matrix[3][3];

    return result;
}

#endif

//----------------------------------------------------------------------------

Matrix &Matrix::operator*=(const Matrix &m)
{
    multiply(&_matrix, _matrix, m._matrix);
    return *this;
}

//----------------------------------------------------------------------------

void Matrix::multiply(MATRIX *result, const MATRIX ma, const MATRIX mb)
{
    MATRIX rr;
#define MULT3(i,j,k) (ma[i][k] * mb[k][j])
#define MULT(i,j) rr[i][j] = MULT3(i,j,0) + MULT3(i,j,1) + MULT3(i,j,2) + MULT3(i,j,3)

    MULT(0,0); MULT(0,1); MULT(0,2); MULT(0,3);
    MULT(1,0); MULT(1,1); MULT(1,2); MULT(1,3);
    MULT(2,0); MULT(2,1); MULT(2,2); MULT(2,3);
    MULT(3,0); MULT(3,1); MULT(3,2); MULT(3,3);

#undef MULT
#undef MULT_K
    memcpy(result, rr, sizeof(MATRIX));
}

//----------------------------------------------------------------------------

//#define USE_SSE

void Matrix::transform(
    float xi, float yi, float zi,
    float *xo, float *yo, float *zo) const
{

#ifdef USE_SSE

    const MATRIX *mat = &_matrix;
    __declspec(align(16)) float result[4];

    __asm {
        movss  xmm0, xi             // xmm0 = xi
        movss  xmm1, yi             // xmm1 = yi
        movss  xmm2, zi             // xmm2 = zi
        movss  xmm4, xmm0           // xmm4 = xi
        movss  xmm5, xmm1           // xmm5 = yi
        movss  xmm6, xmm2           // xmm6 = zi
        shufps xmm4, xmm0, 0        // xmm4 = xi/xi/xi/xi
        shufps xmm5, xmm1, 0        // xmm5 = yi/yi/yi/yi
        shufps xmm6, xmm2, 0        // xmm6 = zi/zi/zi/zi

        mov    eax, mat
        movups xmm0, [eax]          // xmm0 = matrix[0]
        movups xmm1, [eax+16]       // xmm1 = matrix[1]
        movups xmm2, [eax+32]       // xmm2 = matrix[2]
        movups xmm3, [eax+48]       // xmm3 = matrix[3]

        mulps  xmm0, xmm4           // xmm0 *= xi
        mulps  xmm1, xmm5           // xmm1 *= yi
        mulps  xmm2, xmm6           // xmm2 *= zi

        lea    eax, result
        addps  xmm2, xmm3           // xmm0 += xmm1
        addps  xmm0, xmm1           //         xmm2
        addps  xmm0, xmm2           //         xmm3
        movaps [eax], xmm0
    }

    *xo = result[0];
    *yo = result[1];
    *zo = result[2];

    if (result[3] != 1.0) {
        double inv_wt = 1.0 / result[3];
        *xo = (float)(result[0] * inv_wt);
        *yo = (float)(result[1] * inv_wt);
        *zo = (float)(result[2] * inv_wt);
    }

#else /* ! USE_SSE */

#define M(row,col) _matrix[row][col]

    double xt = M(0, 0) * xi + M(1, 0) * yi + M(2, 0) * zi + M(3, 0);
    double yt = M(0, 1) * xi + M(1, 1) * yi + M(2, 1) * zi + M(3, 1);
    double zt = M(0, 2) * xi + M(1, 2) * yi + M(2, 2) * zi + M(3, 2);
    double wt = M(0, 3) * xi + M(1, 3) * yi + M(2, 3) * zi + M(3, 3);

#undef M

    if (wt != 1.0) {
        double inv_wt = 1.0 / wt;
        *xo = (float)(xt * inv_wt);
        *yo = (float)(yt * inv_wt);
        *zo = (float)(zt * inv_wt);
    } else {
        *xo = (float)xt;
        *yo = (float)yt;
        *zo = (float)zt;
    }

#endif
}

//----------------------------------------------------------------------------

void Matrix::transformNormal(
    float xi, float yi, float zi,
    float *xo, float *yo, float *zo) const
{
#define M(row,col) _matrix[row][col]

    *xo = (float)(M(0, 0) * xi + M(0, 1) * yi + M(0, 2) * zi);
    *yo = (float)(M(1, 0) * xi + M(1, 1) * yi + M(1, 2) * zi);
    *zo = (float)(M(2, 0) * xi + M(2, 1) * yi + M(2, 2) * zi);
}
