//----------------------------------------------------------------------------
// ThreeD Plane    
//----------------------------------------------------------------------------

#include "threed/plane.h"

using namespace ThreeD;

//----------------------------------------------------------------------------

#define TOLERANCE 1e-3

//----------------------------------------------------------------------------

int Plane::intersect(const Vector &v1, const Vector &v2, Vector *vx) const
{
    // based on Graphics Gems III, partition3d
    
    int sign1, sign2;
    
    double c1 = classify(v1);
    if (c1 < -TOLERANCE)
        sign1 = -1;
    else if (c1 > TOLERANCE)
        sign1 = 1;
    else    // no intersection;  cases 1, 6, 7
        return 0;
    
    double c2 = classify(v2);
    if (c2 < -TOLERANCE)
        sign2 = -1;
    else if (c2 > TOLERANCE)
        sign2 = 1;
    else {  // intersection at v2;  cases 8, 9
        *vx = v2;
        return sign1;
    }
    
    if (sign1 == sign2)     // no intersection;  cases 2, 3
        return 0;
    
    // intersection;  cases 4, 5
    /*
    Vector vd = v2 - v1;
    double tt = - (double(_abc * v1) + _d) / double(_abc * vd);
    *vx = v1 + (vd * tt);
    */

    double vd_x = (double)v2.x() - (double)v1.x();
    double vd_y = (double)v2.y() - (double)v1.y();
    double vd_z = (double)v2.z() - (double)v1.z();
    double abc_v1_x = (double)_a * (double)v1.x();
    double abc_v1_y = (double)_b * (double)v1.y();
    double abc_v1_z = (double)_c * (double)v1.z();
    double abc_v1 = abc_v1_x + abc_v1_y + abc_v1_z;
    double abc_vd_x = (double)_a * vd_x;
    double abc_vd_y = (double)_b * vd_y;
    double abc_vd_z = (double)_c * vd_z;
    double abc_vd = abc_vd_x + abc_vd_y + abc_vd_z;
    double ttt = - (((double)_d + abc_v1) / abc_vd);
    double vd_ttt_x = vd_x * ttt;
    double vd_ttt_y = vd_y * ttt;
    double vd_ttt_z = vd_z * ttt;
    double vx_x = (double)v1.x() + vd_ttt_x;
    double vx_y = (double)v1.y() + vd_ttt_y;
    double vx_z = (double)v1.z() + vd_ttt_z;
    *vx = Vector((float)vx_x, (float)vx_y, (float)vx_z);

    return sign1;
}
