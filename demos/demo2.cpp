//----------------------------------------------------------------------------
// Demo2 - Isosurfaces with Dual Contouring
//----------------------------------------------------------------------------

#include "demoappbaseimp.h"
#include "sphere.h"
#include "box.h"

using namespace ThreeD;

//----------------------------------------------------------------------------

#define SPHERE_VOXEL_SIZE   Vector(0.25f, 0.25f, 0.25f)
#define BOX_VOXEL_SIZE      Vector(0.5f, 0.5f, 0.55f)

//----------------------------------------------------------------------------

class Demo1 : public DemoAppBaseImp
{
protected:
    virtual bool initDemo();

    void createMesh_DC(Isosurface *iso, ThreeD::Vector voxelSize);
};


static Demo1 myDemo1;
DemoAppBaseImp *TheDemoApp = &myDemo1;


bool Demo1::initDemo()
{
    // build a sphere with radius = 2

    Isosurface *sphereIso = new SphereIsosurface(2.0f);

    Transform sphereTrans;
    sphereTrans.translate(Vector(-2.0f, 2.0f, 0.0f));
    sphereIso->setTransform(sphereTrans);

    createMesh_DC(sphereIso, SPHERE_VOXEL_SIZE);

    // build a box of size (3,4,6)

    Isosurface *boxIso = new BoxIsosurface(Vector(3.0f, 4.0f, 6.0f));
    Transform boxTrans;
    boxTrans.translate(Vector(3.0f, 0.0f, 0.0f));
    boxIso->setTransform(boxTrans);

    createMesh_DC(boxIso, BOX_VOXEL_SIZE);

    // build another box of size (3,3,3)

    Isosurface *box2Iso = new BoxIsosurface(Vector(3.0f, 3.0f, 3.0f));
    Transform box2Trans;
    box2Trans.translate(Vector(-2.0f, -2.0f, 0.0f));
    box2Trans.rotate(Vector(0.45f, 0.45f, 0.45f));
    box2Iso->setTransform(box2Trans);

    createMesh_DC(box2Iso, BOX_VOXEL_SIZE);

    return true;
}


void Demo1::createMesh_DC(Isosurface *iso, ThreeD::Vector voxelSize)
{
    IsoMesher_DC mesher(iso);
    mesher.setVoxelSize(voxelSize.x(), voxelSize.y(), voxelSize.z());
    Mesh *mesh = mesher.createMesh();
    mesh->computeVertexNormals();
    wld->attach(mesh);
}
