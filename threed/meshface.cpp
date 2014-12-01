//----------------------------------------------------------------------------
// ThreeD Triangle Mesh Face
//----------------------------------------------------------------------------

#include <threed/meshface.h>
#include <threed/opengl.h>

using namespace ThreeD;

//----------------------------------------------------------------------------

void MeshFace::getMinMax(Vector *vmin, Vector *vmax) const
{
    float vmin_x, vmax_x;
    float vmin_y, vmax_y;
    float vmin_z, vmax_z;
    vmin_x = vmax_x = vertex(0).x();
    vmin_y = vmax_y = vertex(0).y();
    vmin_z = vmax_z = vertex(0).z();

    for (int n = 1; n < 3; n++) {
        const Vector &v = vertex(n);
        if (v.x() < vmin_x)
            vmin_x = v.x();
        if (v.x() > vmax_x)
            vmax_x = v.x();
        if (v.y() < vmin_y)
            vmin_y = v.y();
        if (v.y() > vmax_y)
            vmax_y = v.y();
        if (v.z() < vmin_z)
            vmin_z = v.z();
        if (v.z() > vmax_z)
            vmax_z = v.z();
    }

    *vmin = Vector(vmin_x, vmin_y, vmin_z);
    *vmax = Vector(vmax_x, vmax_y, vmax_z);
}

//----------------------------------------------------------------------------

void MeshFace::draw(const Color &highlight)
{
    if (_c == Color())
        return;

    float ambient[4] =
        { _ambient.red(), _ambient.blue(), _ambient.green(), 1.0f };
    float diffuse[4] =
        { _diffuse.red(), _diffuse.green(), _diffuse.blue(), 1.0f };
    float specular[4] =
        { _specular.red(), _specular.green(), _specular.blue(), 1.0f };

    int i;

    glBegin(GL_TRIANGLES);
    for (i = 0; i < 3; ++i) {
        const Vector &vertex = _v[i][0];
        const Vector &normal = _v[i][1];
        if (normal == ThreeD::Vector())
            glNormal3f(planeNormal().x(), planeNormal().y(), -planeNormal().z());
        else
            glNormal3f(normal.x(), normal.y(), -normal.z());

        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, _shininess);
        glColor3f(_c.red(), _c.green(), _c.blue());

        glVertex3f(vertex.x(), vertex.y(), -vertex.z());
    }

    glEnd();

//#define TD_DRAW_NORMALS
#ifdef TD_DRAW_NORMALS
    const Vector va((v1 + v2 + v3) / 3.0);
    const Vector vb(va - planeNormal);

    glColor3f(0xFFFF, 0xFFFF, 0);

    glBegin(GL_LINES);
    glVertex3f(va.x(), va.y(), -va.z());
    glVertex3f(vb.x(), vb.y(), -vb.z());
    glEnd();
#endif

//#define TD_DRAW_VERTEX_NORMALS
#ifdef TD_DRAW_VERTEX_NORMALS
    glColor3f(0xFFFF, 0xFFFF, 0);
    glBegin(GL_LINES);

    for (i = 0; i < 3; ++i) {
        const Vector &va = _v[i][0];
        Vector vb;
        if (_v[i][1] == ThreeD::Vector())
            vb = va + planeNormal();
        else
            vb = va + _v[i][1];

        glVertex3f(va.x(), va.y(), -va.z());
        glVertex3f(vb.x(), vb.y(), -vb.z());
    }

    glEnd();
#endif

    // if highlight requested, draw the lines of the contour
    // of this mesh face

    if (highlight != Color()) {
        glColor3f(highlight.red(), highlight.green(), highlight.blue());

        glBegin(GL_LINE_LOOP);
        for (i = 0; i < 3; ++i) {
            const Vector &vertex = _v[i][0];
            glVertex3f(vertex.x(), vertex.y(), -vertex.z());
        }
        glEnd();
    }
}
