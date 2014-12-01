//----------------------------------------------------------------------------
// DemoAppBaseImp
//----------------------------------------------------------------------------

#include "demoappbaseimp.h"

using namespace ThreeD;


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
    if (io().buttonPressed(SDL_BUTTON_LEFT))
        leftButtonPressed = true;
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

    if (io().keyPressed(SDLK_LEFT))
        cameraTranslationVector = Vector(+0.05f, 0, 0);
    else if (io().keyPressed(SDLK_RIGHT))
        cameraTranslationVector = Vector(-0.05f, 0, 0);
    else if (io().keyPressed(SDLK_UP))
        cameraTranslationVector = Vector(0, -0.05f, 0);
    else if (io().keyPressed(SDLK_DOWN))
        cameraTranslationVector = Vector(0, +0.05f, 0);

    else if (io().keyPressed(SDLK_KP_MINUS))
        cameraTranslationVector = Vector(0, 0, -0.05f);
    else if (io().keyPressed(SDLK_KP_PLUS))
        cameraTranslationVector = Vector(0, 0, +0.05f);

    else if (io().keyPressed(SDLK_KP4))
        cameraRotationVector = Vector(0, -0.05f, 0);
    else if (io().keyPressed(SDLK_KP6))
        cameraRotationVector = Vector(0, +0.05f, 0);

    else if (io().keyPressed(SDLK_a))
        meshRotationVector = Vector(0, -0.5f, 0);
    else if (io().keyPressed(SDLK_d))
        meshRotationVector = Vector(0, 0.5f, 0);
    else if (io().keyPressed(SDLK_w))
        meshRotationVector = Vector(-0.5f, 0, 0);
    else if (io().keyPressed(SDLK_x))
        meshRotationVector = Vector(+0.5f, 0, 0);
    else if (io().keyPressed(SDLK_q))
        meshRotationVector = Vector(0, 0, -0.5f);
    else if (io().keyPressed(SDLK_e))
        meshRotationVector = Vector(0, 0, +0.5f);

    else if (io().keyPressed(SDLK_1))
        meshScaleVector = Vector(1.1f, 1.1f, 1.1f);
    else if (io().keyPressed(SDLK_2))
        meshScaleVector = Vector(0.9f, 0.9f, 0.9f);

    else if (io().keyPressed(SDLK_ESCAPE))
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
