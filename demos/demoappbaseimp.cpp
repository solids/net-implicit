//----------------------------------------------------------------------------
// DemoAppBaseImp
//----------------------------------------------------------------------------

#include "demoappbaseimp.h"
#include <iostream>

using namespace ThreeD;
using namespace std;

bool DemoAppBaseImp::init()
{
    if (!DemoApp::init())
        return false;

    wld = new World();
    camera = new Camera(*wld);
    camera->translate(Vector(0.0, 0.0, -10.0f));
    selectedLink = 0;

    LightSource *ls = new LightSource(0);
    ls->setDiffuseColor(Color(0.5f, 0.5f, 0.5f));
    ls->setLocation(Vector(0.0, 10.0, -10.0f));
    wld->addLight(*ls);

    if (!initDemo())
        return false;
}


bool DemoAppBaseImp::doEvent()
{
    doMouseEvent();
    return doKeyEvent();
}


void DemoAppBaseImp::doMouseEvent()
{
    if (io().buttonPressed(SDL_BUTTON_LEFT)) {
      leftButtonPressed = true;
    }
    if (leftButtonPressed && !io().buttonPressed(SDL_BUTTON_LEFT)) {
        leftButtonPressed = false;
        if (selectedLink)
          selectedLink->object()->highlight(Color());
        selectedLink = camera->getSelection(io().mouseX(), io().mouseY());
        if (selectedLink)
          selectedLink->object()->highlight(Color(50.0f, 50.0f, 50.0f));
    }
}


bool DemoAppBaseImp::doKeyEvent()
{
    Vector cameraTranslationVector;
    Vector cameraRotationVector;
    Vector meshRotationVector;
    Vector meshScaleVector;

    if (io().keyPressed(SDL_SCANCODE_LEFT))
        cameraTranslationVector = Vector(+0.05f, 0, 0);
    else if (io().keyPressed(SDL_SCANCODE_RIGHT))
        cameraTranslationVector = Vector(-0.05f, 0, 0);
    else if (io().keyPressed(SDL_SCANCODE_UP))
        cameraTranslationVector = Vector(0, -0.05f, 0);
    else if (io().keyPressed(SDL_SCANCODE_DOWN))
        cameraTranslationVector = Vector(0, +0.05f, 0);

    else if (io().keyPressed(SDL_SCANCODE_K))
        cameraTranslationVector = Vector(0, 0, -0.05f);      
    else if (io().keyPressed(SDL_SCANCODE_L))
        cameraTranslationVector = Vector(0, 0, +0.05f);

    else if (io().keyPressed(SDL_SCANCODE_M))
        cameraRotationVector = Vector(0, -0.05f, 0);
    else if (io().keyPressed(SDL_SCANCODE_N))
        cameraRotationVector = Vector(0, +0.05f, 0);

    else if (io().keyPressed(SDL_SCANCODE_A))
        meshRotationVector = Vector(0, -0.5f, 0);
    else if (io().keyPressed(SDL_SCANCODE_D))
        meshRotationVector = Vector(0, 0.5f, 0);
    else if (io().keyPressed(SDL_SCANCODE_W))
        meshRotationVector = Vector(-0.5f, 0, 0);
    else if (io().keyPressed(SDL_SCANCODE_X))
        meshRotationVector = Vector(+0.5f, 0, 0);
    else if (io().keyPressed(SDL_SCANCODE_Q))
        meshRotationVector = Vector(0, 0, -0.5f);
    else if (io().keyPressed(SDL_SCANCODE_E))
        meshRotationVector = Vector(0, 0, +0.5f);

    else if (io().keyPressed(SDL_SCANCODE_1))
        meshScaleVector = Vector(1.1f, 1.1f, 1.1f);
    else if (io().keyPressed(SDL_SCANCODE_2))
        meshScaleVector = Vector(0.9f, 0.9f, 0.9f);

    else if (io().keyPressed(SDL_SCANCODE_ESCAPE))
        return false;

    if (selectedLink) {
        bool cancelHighlight = false;
        if (meshRotationVector != Vector()) {
            selectedLink->rotateDeg(meshRotationVector);
            cancelHighlight = true;
        }
        if (meshScaleVector.length() != 0) {
            selectedLink->scale(meshScaleVector);
            cancelHighlight = true;
        }
        if (cancelHighlight)
            selectedLink->object()->highlight(Color());
    }

    camera->translate(cameraTranslationVector);
    camera->rotate(cameraRotationVector);
    return true;
}


void DemoAppBaseImp::draw()
{
    camera->draw();
}



int main()
{
    TheDemoApp->init();
    TheDemoApp->run();
    TheDemoApp->close();
    return 0;
}
