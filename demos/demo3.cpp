//----------------------------------------------------------------------------
// Demo3 - Isosurfaces with Dual Contouring, CSG operations
//----------------------------------------------------------------------------

#include "demoappbaseimp.h"
#include "sphere.h"
#include "box.h"

using namespace ThreeD;

//----------------------------------------------------------------------------

#define CSG_VOXEL_SIZE   Vector(0.1f, 0.1f, 0.1f)

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
    // build a sphere with radius = 1.0

    Isosurface *sphereIso = new SphereIsosurface(1.8f);

    Transform sphereTrans;
    sphereTrans.translate(Vector(0.0f, -0.5f, -3.0f));
    sphereIso->setTransform(sphereTrans);

    // build a box of size (3,4,6)

    Isosurface *boxIso = new BoxIsosurface(Vector(3.0f, 4.0f, 6.0f));

    // subtract the sphere from the box

    CsgIsosurface *csgIso = new CsgIsosurface();
    csgIso->setCsgMode(CsgIsosurface::CSG_DIFFERENCE);
    csgIso->addChild(boxIso);
    csgIso->addChild(sphereIso);

    Isosurface *boxIso2 = new BoxIsosurface(Vector(1.0f, 1.0f, 1.0f));
    Transform boxTrans;
    boxTrans.translate(Vector(-1.0f, -2.1f, 0.4f));
    boxTrans.rotate(Vector(0.5f, 0.90f, 0.45f));
    boxIso2->setTransform(boxTrans);
    csgIso->addChild(boxIso2);

    Transform csgTrans;
    csgTrans.rotate(Vector(1.0f, 0.90f, 0.20f));
    csgTrans.translate(Vector(0.1f, 0.1f, 0.0f));
    csgIso->setTransform(csgTrans);

    // polygonize

    createMesh_DC(csgIso, CSG_VOXEL_SIZE);

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
