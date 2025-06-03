#include <irrlicht/irrlicht.h>

#include <string>
#include <memory>
#include <random>

#include "Exceptions/GraphicalExceptions.hpp"
#include "tools/MeshImporter.hpp"
#include "Graphic/Window/window.hpp"

std::shared_ptr<Mesh> MeshImporter::importMesh(std::string meshName,
        const Vec3d &position, const Vec3d &scale, const Vec3d &rotation) {
    auto mesh = getMesh(meshName);
    if (!mesh)
        throw GUI::ShaderCompilationException("Error loading mesh");
    auto node = GUI::Window::i().smgr->addAnimatedMeshSceneNode(mesh);
    if (node) {
        node->setScale(scale);
        node->setRotation(Vec3d(rotation));
        std::random_device rd;
        node->setID(rd());
        node->setPosition(position);
        node->setMD2Animation(irr::scene::EMAT_STAND);
        node->setMaterialTexture(0, getTexture(meshName));
        node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        return std::shared_ptr<Mesh>(node, [](Mesh* p) {(void) p;});
    }
    throw GUI::ShaderCompilationException("Error creating mesh node");
}

irr::scene::IAnimatedMesh *MeshImporter::getMesh(std::string meshName) {
    irr::io::path pathObj =
        irr::io::path(("assets/" + meshName + ".obj").c_str());

    if (meshes.find(meshName) == meshes.end()) {
        auto mesh = GUI::Window::i().smgr->getMesh(pathObj);
        if (!mesh)
            throw GUI::ShaderCompilationException("Error loading mesh: "
                + meshName);
        meshes[meshName] = mesh;
        return mesh;
    }
    return meshes[meshName];
}

irr::video::ITexture *MeshImporter::getTexture(std::string textureName) {
    irr::io::path pathTexture =
        irr::io::path(("assets/Bake" + textureName + ".png").c_str());

    if (textures.find(textureName) == textures.end()) {
        auto mesh = GUI::Window::i().driver->getTexture(pathTexture);
        if (!mesh)
            throw GUI::ShaderCompilationException("Error loading mesh: "
                + textureName);
        textures[textureName] = mesh;
        return mesh;
    }
    return textures[textureName];
}
