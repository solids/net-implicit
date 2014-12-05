//----------------------------------------------------------------------------
// ThreeD World
//----------------------------------------------------------------------------

#include <threed/world.h>
#include <threed/opengl.h>
#include <threed/mesh.h>
#include <OpenGL/glu.h>

//----------------------------------------------------------------------------

using namespace ThreeD;

int largestId = 0;

//----------------------------------------------------------------------------

void WorldLink::draw()
{
    MATRIX save;
    getValues(&save);
    translate(_location);
    Transform::glMultMatrix();
    _obj->draw();
    setValues(save);
}

//----------------------------------------------------------------------------

World::World()
{
    _inSelection = false;
    _enableLights = true;
}

//----------------------------------------------------------------------------

World::~World()
{
    while (! _links.empty()) {
        WorldLink *link = _links.front();
        erase(link->object());
    }

    eraseLights();
}

//----------------------------------------------------------------------------

WorldLink *World::attach(Object *obj)
{
    Mesh *mesh = (Mesh *)obj;
    //mesh->computeVertexNormals();

    Vector v1,v2;
    mesh->getBoundingBox(&v1,&v2);

    Vector loc1 = obj->centralize();
    Vector loc(loc1.x(), loc1.y(), -loc1.z());
    WorldLink *link = new WorldLink(obj, loc);
    link->id = ++largestId;
    _links.push_back(link);
    return link;
}

//----------------------------------------------------------------------------

Object *World::detach(WorldLink *link)
{
    Object *obj = link->object();
    _links.remove(link);
    delete link;
    return obj;
}

//----------------------------------------------------------------------------

void World::erase(WorldLink *link)
{
    Object *obj = link->object();
    _links.remove(link);
    delete obj;
    delete link;
}

//----------------------------------------------------------------------------

void World::erase(Object *obj)
{
    std::list<WorldLink *>::iterator it = _links.begin();
    while (it != _links.end()) {
        WorldLink *link = (*it);
        if (link->object() == obj) {
            it = _links.erase(it);
            delete obj;
            delete link;
            break;
        } else
            ++it;
    }
}

//----------------------------------------------------------------------------

Object *World::object(WorldLink *link) const
{
    return link->object();
}

//----------------------------------------------------------------------------

void World::addLight(const LightSource &lightSource)
{
    _lightSources.push_back(lightSource);
    _enableLights = true;
}

//----------------------------------------------------------------------------

void World::eraseLights()
{
    _lightSources.clear();
    _enableLights = false;
}

//----------------------------------------------------------------------------

void World::draw()
{
    _enableLights = true;
    if (!_inSelection && _enableLights) {
        for (std::list<LightSource>::iterator it = _lightSources.begin();
             it != _lightSources.end(); ++it)
            (*it).enable();
        _enableLights = false;
    }

    double cameraMatrix[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, cameraMatrix);

    std::list<WorldLink *>::iterator it = _links.begin();
    while (it != _links.end()) {
        WorldLink *link = (*it);
        ++it;
        if (_inSelection)
            glLoadName(link->id);
        glLoadMatrixd(cameraMatrix);
        link->draw();
    }
}

//----------------------------------------------------------------------------

void World::getBoundingBox(Vector *v1, Vector *v2)
{
}

//----------------------------------------------------------------------------

double World::getBoundingRadius()
{
    return 0;
}

//----------------------------------------------------------------------------

WorldLink *World::findLink (int id) {
  std::list<WorldLink *>::iterator it = _links.begin();
  while (it != _links.end()) {
    WorldLink *link = (*it);
    ++it;
    if (id == link->id) {
      return link;
    }
  }
}

WorldLink *World::getSelection(int x, int y)
{
    _inSelection = true;

    glMatrixMode(GL_PROJECTION);
    double oldProjectionMatrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX, oldProjectionMatrix);
    glPushMatrix();
    glLoadIdentity();
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluPickMatrix(x, viewport[3] - y, 4, 4, viewport);
    glMultMatrixd(oldProjectionMatrix);
    glMatrixMode(GL_MODELVIEW);

    GLuint output[512];
    glSelectBuffer(sizeof(output) / sizeof(GLuint), output);
    glRenderMode(GL_SELECT);
    glPushName(0);
    draw();
    glFlush();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    GLint numHints = glRenderMode(GL_RENDER);
    GLuint zMin = 0xFFFFFFFF;
    GLuint zMax = 0;
    WorldLink *hitLink = 0;
    GLuint *output_ptr = output;
    for (int i = 0; i < numHints; i++) {
        int numelems = output_ptr[0];
        if (output_ptr[1] < zMin) {     // minimum depth for hit
            zMin = output_ptr[1];
            hitLink = findLink(output_ptr[3]);
        }
        if (output_ptr[2] > zMax)       // maximum depth for hit
            zMax = output_ptr[2];
        output_ptr += numelems + 3;
    }

    _inSelection = false;
    return hitLink;
}
