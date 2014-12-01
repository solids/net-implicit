//----------------------------------------------------------------------------
// ThreeD Mesh Object
//----------------------------------------------------------------------------

#include <threed/mesh.h>
#include <threed/meshface.h>
#include <threed/transform.h>

using namespace ThreeD;

//----------------------------------------------------------------------------

#define TOLERANCE 1e-3

//----------------------------------------------------------------------------

Mesh::Mesh()
{
    _boundsCached = false;
}

//----------------------------------------------------------------------------

Object::~Object()
{
}

Mesh::~Mesh()
{
    // delete faces throughout all planes

    PlanesList::iterator itPlanes = _planes.begin();
    while (itPlanes != _planes.end()) {
        FacesList &faces = (*itPlanes).faces;
        while (! faces.empty()) {
            MeshFace *face = faces.front();
            delete face;
            faces.pop_front();
        }
        itPlanes = _planes.erase(itPlanes);
    }

    // delete points

    _points.clear();
}

//----------------------------------------------------------------------------

Mesh::MeshPoint *Mesh::addPoint(const Vector &v)
{
    // points are held in a sorted map, where the key of
    // a point is its vector distance from (0,0,0) times 10.
    float x = v.x();
    float y = v.y();
    float z = v.z();
    unsigned long key = (unsigned long)(10.0f * (
        (double)x * (double)x +
        (double)y * (double)y +
        (double)z * (double)z));

    // look at points in the vicinity of the point we're adding
    // and see if any of them is close enough to the new point
    PointsMap::iterator it0 = _points.lower_bound(key - 1);
    if (it0 == _points.end())
        it0 = _points.lower_bound(key);
    PointsMap::iterator it1 = _points.upper_bound(key + 1);
    
    PointsMap::iterator it = it0;
    while (it != it1) {
        const MeshPoint &mpoint = (*it).second;
        const Vector *v2 = &mpoint.point;
        if (fabs(x - v2->x()) < TOLERANCE &&
            fabs(y - v2->y()) < TOLERANCE &&
            fabs(z - v2->z()) < TOLERANCE)
                return (MeshPoint *)&mpoint; // v2
        ++it;
    }

    // if match not found, create a new point
    MeshPoint mpoint;
    mpoint.point = v;
    PointsMap::value_type pair(key, mpoint);
    if (it0 != _points.end())
        it = _points.insert(it0, pair);
    else
        it = _points.insert(pair);
    const MeshPoint &newmpoint = (*it).second;
    //return &newmpoint.point;
    return (MeshPoint *)&newmpoint;
}

//----------------------------------------------------------------------------

Mesh::MeshPlane *Mesh::addPlane(const Plane &plane1)
{
    Plane planeToAdd = plane1.normalized();

    PlanesList::iterator itPlanes = _planes.begin();
    if (itPlanes != _planes.end()) {
        const Plane *plane2 = &(*itPlanes).p;
        if (fabs(planeToAdd.a() - plane2->a()) < TOLERANCE &&
            fabs(planeToAdd.b() - plane2->b()) < TOLERANCE &&
            fabs(planeToAdd.c() - plane2->c()) < TOLERANCE &&
            fabs(planeToAdd.d() - plane2->d()) < TOLERANCE)
            return &(*itPlanes);
        while (1) {
            ++itPlanes;
            if (itPlanes == _planes.end())
                break;
            plane2 = &(*itPlanes).p;
            if (fabs(planeToAdd.a() - plane2->a()) < TOLERANCE &&
                fabs(planeToAdd.b() - plane2->b()) < TOLERANCE &&
                fabs(planeToAdd.c() - plane2->c()) < TOLERANCE &&
                fabs(planeToAdd.d() - plane2->d()) < TOLERANCE)
                return &(*itPlanes);
        }
    }

    MeshPlane mpNew;
    mpNew.p = planeToAdd;
    _planes.push_back(mpNew);
    return &(_planes.back());
}

//----------------------------------------------------------------------------

void Mesh::addFace(MeshFace *face, MeshPlane *mplane)
{
    // compute the area of the face, and drop it if
    // the area is too small
    double face_a = (*face->_v[1] - *face->_v[0]).length();
    double face_b = (*face->_v[2] - *face->_v[1]).length();
    double face_c = (*face->_v[0] - *face->_v[2]).length();
    double face_s = (face_a + face_b + face_c) / 2.0f;
    double face_area2 = face_s * (face_s - face_a) * (face_s - face_b) * (face_s - face_c);
    if (face_area2 < 1e-7)
        return;

    // find the plane for the face
    if (! mplane) {
        Plane p(face->vertex(0), face->vertex(1), face->vertex(2));
        mplane = addPlane(p);
    }

    // make sure the same face hasn't been added before
    const Vector *newVertex0 = face->vertexPtr(0);
    const Vector *newVertex1 = face->vertexPtr(1);
    const Vector *newVertex2 = face->vertexPtr(2);

#if 0
    const FacesList &mplane_faces = mplane->faces;
    FacesList::const_iterator it = mplane_faces.begin();
    while (it != mplane_faces.end()) {
        MeshFace *oldface = (*it);
        const Vector *oldVertex0 = oldface->vertexPtr(0);
        const Vector *oldVertex1 = oldface->vertexPtr(1);
        const Vector *oldVertex2 = oldface->vertexPtr(2);

        bool same0 = (oldVertex0 == newVertex0) ||
                     (oldVertex0 == newVertex1) ||
                     (oldVertex0 == newVertex2);
        bool same1 = (oldVertex1 == newVertex0) ||
                     (oldVertex1 == newVertex1) ||
                     (oldVertex1 == newVertex2);
        bool same2 = (oldVertex2 == newVertex0) ||
                     (oldVertex2 == newVertex1) ||
                     (oldVertex2 == newVertex2);

        if (same0 && same1 && same2)
            return;

        ++it;
    }
#endif

    // the face hasn't been added before so add it now
    mplane->faces.push_back(face);

    // add a reference to the face from each of its vertices
    MeshPoint *mpoint0 = (MeshPoint *)newVertex0;
    MeshPoint *mpoint1 = (MeshPoint *)newVertex1;
    MeshPoint *mpoint2 = (MeshPoint *)newVertex2;
    mpoint0->faces.push_back(face);
    mpoint1->faces.push_back(face);
    mpoint2->faces.push_back(face);

    _boundsCached = false;
}

//----------------------------------------------------------------------------

void Mesh::draw()
{
    PlanesList::const_iterator itPlanes = _planes.begin();
    while (itPlanes != _planes.end()) {
        const FacesList &faces = (*itPlanes).faces;
        FacesList::const_iterator itFaces = faces.begin();
        while (itFaces != faces.end()) {
            (*itFaces)->draw(_highlight);
            ++itFaces;
        }
        ++itPlanes;
    }
}

//----------------------------------------------------------------------------

void Mesh::transform(const Transform &trans)
{
    PointsMap::iterator it = _points.begin();
    while (it != _points.end()) {
        MeshPoint &mpoint = (*it).second;
        Vector *v = &mpoint.point;
        trans.transform(v);
        ++it;
    }

    _boundsCached = false;
}

//----------------------------------------------------------------------------

void Mesh::getBoundingBox(Vector *v1, Vector *v2)
{
    if (_boundsCached) {
        *v1 = _bounds[0];
        *v2 = _bounds[1];
        return;
    }

    float vmin_x = 99999999.0f;
    float vmin_y = 99999999.0f;
    float vmin_z = 99999999.0f;
    float vmax_x = -99999999.0f;
    float vmax_y = -99999999.0f;
    float vmax_z = -99999999.0f;

    PlanesList::const_iterator itPlanes = _planes.begin();
    while (itPlanes != _planes.end()) {
        const FacesList &faces = (*itPlanes).faces;
        FacesList::const_iterator itFaces = faces.begin();
        while (itFaces != faces.end()) {

            Vector vmin, vmax;
            (*itFaces)->getMinMax(&vmin, &vmax);
            if (vmin.x() < vmin_x)
                vmin_x = vmin.x();
            if (vmin.y() < vmin_y)
                vmin_y = vmin.y();
            if (vmin.z() < vmin_z)
                vmin_z = vmin.z();
            if (vmax.x() > vmax_x)
                vmax_x = vmax.x();
            if (vmax.y() > vmax_y)
                vmax_y = vmax.y();
            if (vmax.z() > vmax_z)
                vmax_z = vmax.z();

            ++itFaces;
        }
        ++itPlanes;
    }

    _bounds[0] = Vector(vmin_x, vmin_y, vmin_z);
    _bounds[1] = Vector(vmax_x, vmax_y, vmax_z);
    _boundsCached = true;

    *v1 = _bounds[0];
    *v2 = _bounds[1];
}

//----------------------------------------------------------------------------

float Mesh::getBoundingRadius()
{
    Vector vmin, vmax;
    getBoundingBox(&vmin, &vmax);
    return vmin.length();
}

//----------------------------------------------------------------------------

Vector Mesh::centralize()
{
    Vector vmin, vmax;
    getBoundingBox(&vmin, &vmax);
    Vector vcenter = (vmin + vmax) / 2.0f;
    Transform t;
    t.translate(-vcenter);
    transform(t);
    return vcenter;
}

//----------------------------------------------------------------------------

void Mesh::computeVertexNormals()
{
    PointsMap::iterator itPoints = _points.begin();
    while (itPoints != _points.end()) {
        MeshPoint &mpoint = (*itPoints).second;
        ++itPoints;

        Vector sum;
        int count = 0;

        FacesList &faces = mpoint.faces;
        if (! faces.empty()) {

            FacesList::iterator itFaces = faces.begin();
            MeshFace *face1 = (*itFaces);
            sum += face1->planeNormal();

            ++itFaces;
            while (itFaces != faces.end()) {
                MeshFace *face2 = (*itFaces);
                float dotprod = face1->planeNormal() * face2->planeNormal();
                if (dotprod > -0.1 && dotprod < 0.1) {
                    count = 0;
                    break;
                } else {
                    sum += face2->planeNormal();
                    ++count;
                    ++itFaces;
                }
            }
        }

        //count = 0;

        if (count > 0) {
            sum /= (float)count;
            mpoint.normal = sum.normalized();
        } else
            mpoint.normal = Vector();
    }
}

//----------------------------------------------------------------------------

void Mesh::highlight(const Color &color)
{
    _highlight = color;
}
