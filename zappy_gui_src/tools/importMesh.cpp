#include <string>
#include <irrlicht/irrlicht.h>
#include "Exceptions/GraphicalExceptions.hpp"

irr::scene::IAnimatedMeshSceneNode* importMesh(irr::scene::ISceneManager *smgr,
                                   irr::video::IVideoDriver *driver,
                                   std::string meshName,
                                   const irr::core::vector3df &position,
                                   const irr::core::vector3df &scale,
                                   const irr::core::vector3df &rotation) {
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
        node->setRotation(irr::core::vector3df(rotation));
        node->setPosition(position);
        node->setMD2Animation(irr::scene::EMAT_STAND);
        node->setMaterialTexture(0, driver->getTexture(pathTexture));
        node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        return node;
    }
    throw GUI::ShaderCompilationException("Error creating mesh node");
}
