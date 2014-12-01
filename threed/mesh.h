//----------------------------------------------------------------------------
// ThreeD Mesh Object
//----------------------------------------------------------------------------

#ifndef _THREED_MESH_H
#define _THREED_MESH_H

#pragma warning(disable: 4786)  // name truncated to 255 chars

#include <threed/object.h>
#include <threed/plane.h>
#include <map>
#include <list>

namespace ThreeD {


class MeshFace;


/**
 * Mesh, a three-dimesional object composed of individual mesh faces.
 */
class Mesh : public Object
{
public:
    typedef std::list<MeshFace *> FacesList;

    struct MeshPlane {
        Plane p;
        FacesList faces;
    };

    typedef std::list<MeshPlane> PlanesList;

    // A MeshPoint may also be addressed as Vector[2],
    // so keep the order of the first two fields
    struct MeshPoint {
        Vector point;
        Vector normal;
        FacesList faces;
    };

    typedef std::multimap<unsigned long, MeshPoint> PointsMap;

    /**
     * Constructor.
     *
     * Constructs an empty mesh.
     */
    Mesh();

    /**
     * Destructor.
     *
     * Destroys this mesh.
     */
    virtual ~Mesh();

    /**
     * Defines a new point in this mesh.  This creates a new
     * point element unless such a point already exists in
     * the mesh.
     *
     * @return the index for the point.
     */
    MeshPoint *addPoint(const Vector &v);

    /** Adds a plane to the mesh. */
    MeshPlane *addPlane(const Plane &plane1);

    /**
     * Associates a mesh face with this mesh.
     * IMPORTANT:  The vertices that make up the mesh face
     * should be those returned by Mesh::addPoint().
     */
    void addFace(MeshFace *face, MeshPlane *mplane = 0);

    /**
     * Apply the transform @p trans into this object.
     */
    virtual void transform(const Transform &trans);
    
    /**
     * Draws this mesh.
     *
     * Note:  any parameters that should affect the output should be
     * effected into OpenGL prior to the invocation of this method.
     */
    virtual void draw();
    
    /**
     * Returns the bounding box for this object.  The origin of the box is
     * at the origin of the object, (0,0,0).  The negative bounding vertex
     * is place in @p v1, and the positive bounding vertex in @p v2.
     */
    virtual void getBoundingBox(Vector *v1, Vector *v2);
    
    /**
     * Returns the radius of the bounding sphere for this object.  The
     * origin of the sphere is at the origin of the object, (0,0,0).
     *
     * @return the radius of the bounding sphere.
     */
    virtual float getBoundingRadius();
    
    /**
     * Centralizes all vertices around (0,0,0), by computing the offset
     * of the center of the object to (0,0,0) and shifting all the
     * vertices by that offset.
     *
     * @return the offset from the original center to (0,0,0).
     */
    virtual Vector centralize();

    /** Compute vertex normals. */
    void computeVertexNormals();

    /**
     * Specifies the color to highlight the object with.
     * Specification of an empty Color() implies no highlighting.
     */
    virtual void highlight(const Color &color);

protected:
    /*
     * data
     */
    PointsMap _points;
    PlanesList _planes;

    Vector _bounds[2];              // top-left and bottom-right
    bool _boundsCached;             // if _bounds is ok to use

    Color _highlight;


    friend class Mesh_Opt;          // mesh optimizer
};


} // namespace ThreeD
#endif // _THREED_MESH_H
