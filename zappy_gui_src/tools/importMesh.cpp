#include <string>
#include <irrlicht/irrlicht.h>
#include "Exceptions/GraphicalExceptions.hpp"
#include "tools.hpp"
#include "window.hpp"
#include <memory>

std::shared_ptr<Mesh> importMesh(std::string meshName,
        const Vec3d &position, const Vec3d &scale, const Vec3d &rotation) {
    irr::scene::ISceneManager *smgr = GUI::Window::i().smgr;
    irr::video::IVideoDriver *driver = GUI::Window::i().driver;

    irr::io::path pathObj =
        irr::io::path(("assets/" + meshName + ".obj").c_str());
    irr::io::path pathTexture =
        irr::io::path(("assets/Bake" + meshName + ".png").c_str());

    auto mesh = smgr->getMesh(pathObj);
    if (!mesh)
        throw GUI::ShaderCompilationException("Error loading mesh");
    auto node = smgr->addAnimatedMeshSceneNode(mesh);
    if (node) {
        node->setScale(scale);
        node->setRotation(Vec3d(rotation));
        node->setPosition(position);
        node->setMD2Animation(irr::scene::EMAT_STAND);
        node->setMaterialTexture(0, driver->getTexture(pathTexture));
        node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        return std::shared_ptr<Mesh>
                (node, [](Mesh* p) {(void) p;});
    }
    throw GUI::ShaderCompilationException("Error creating mesh node");
}
