//----------------------------------------------------------------------------
// ThreeD Transform
//----------------------------------------------------------------------------

#include <threed/transform.h>
#include <threed/opengl.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace ThreeD;

#include <iostream>
using std::cout;
using std::endl;

//----------------------------------------------------------------------------

void Transform::glMultMatrix()
{
    glMultMatrixf((const float *)matrix());
}
