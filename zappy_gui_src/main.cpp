#include <irrlicht/irrlicht.h>

using namespace irr;

int main() {
    // Crée le device (fenêtre + moteur)
    IrrlichtDevice *device = createDevice(video::EDT_OPENGL,
                                          core::dimension2d<u32>(800, 600),
                                          16, false, false, false, 0);

    if (!device) return 1;

    device->setWindowCaption(L"Irrlicht Cube Demo");

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();

    // Ajoute un cube à la scène
    scene::ISceneNode* cube = smgr->addCubeSceneNode(10.0f);
    if (cube) {
        cube->setPosition(core::vector3df(0, 0, 30));
        cube->setMaterialFlag(video::EMF_LIGHTING, false); // désactiver la lumière
    }

    // Caméra libre
    smgr->addCameraSceneNodeFPS();
    device->getCursorControl()->setVisible(false);

    // Boucle de rendu
    while (device->run()) {
        driver->beginScene(true, true, video::SColor(255, 100, 101, 140));
        smgr->drawAll();
        driver->endScene();
    }

    device->drop(); // libère la mémoire
    return 0;
}
