//----------------------------------------------------------------------------
// ThreeD Light Source
//----------------------------------------------------------------------------

#include <threed/lightsource.h>
#include <threed/opengl.h>

//----------------------------------------------------------------------------

using namespace ThreeD;

//----------------------------------------------------------------------------

LightSource::LightSource(int lightNum)
{
    _lightNum = lightNum;
}

//----------------------------------------------------------------------------

LightSource::LightSource(const LightSource &model)
    : Transform(model),
      _lightNum(model._lightNum),
      _location(model._location),
      _ambient(model._ambient),
      _diffuse(model._diffuse)
{
}

//----------------------------------------------------------------------------

LightSource &LightSource::operator=(const LightSource &model)
{
    _lightNum = model._lightNum;
    _location = model._location;
    _ambient = model._ambient;
    _diffuse = model._diffuse;
    return *this;
}

//----------------------------------------------------------------------------

void LightSource::enable()
{
    int lightNum = GL_LIGHT0 + _lightNum;

    float ambient[4] =
        { _ambient.red(), _ambient.green(), _ambient.blue(), 1.0 };
    float diffuse[4] =
        { _diffuse.red(), _diffuse.green(), _diffuse.blue(), 1.0 };
    float position[4] =
        { _location.x(), _location.y(), -_location.z(), 1.0 };

    glPushMatrix();
    glLightfv(lightNum, GL_AMBIENT, ambient);
    glLightfv(lightNum, GL_DIFFUSE, diffuse);
    glLightfv(lightNum, GL_POSITION, position);
    glEnable(lightNum);
    glPopMatrix();
}

//----------------------------------------------------------------------------

void LightSource::disable()
{
    glDisable(GL_LIGHT0 + _lightNum);
}
