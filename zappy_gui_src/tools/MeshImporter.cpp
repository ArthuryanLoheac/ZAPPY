#include <irrlicht/irrlicht.h>

#include <string>
#include <memory>
#include <random>
#include <vector>

#include "Exceptions/GraphicalExceptions.hpp"
#include "DataManager/PathManager.hpp"
#include "tools/MeshImporter.hpp"
#include "Graphic/Window/window.hpp"

#define img std::shared_ptr<irr::video::IImage>

/**
 * @brief Imports a 3D mesh and creates a scene node for it.
 *
 * @param meshName Name of the mesh file (without extension).
 * @param teamName Name of the team (optional).
 * @param position Initial position of the mesh.
 * @param scale Scale of the mesh.
 * @param rotation Rotation of the mesh.
 * @return std::shared_ptr<Mesh> Shared pointer to the created mesh scene node.
 */
std::shared_ptr<Mesh> MeshImporter::importMesh(std::string meshName,
const std::string &teamName, const Vec3d &position, const Vec3d &scale,
const Vec3d &rotation) {
    auto mesh = getMesh(meshName);
    if (!mesh)
        throw GUI::ShaderCompilationException("Error loading mesh");
    std::shared_ptr<Mesh> node = std::shared_ptr<Mesh>(GUI::Window::i()
        .smgr->addAnimatedMeshSceneNode(mesh), [](Mesh* p) {(void) p;});
    if (node) {
        node->setScale(scale * GUI::PathManager::i().getScale(meshName));
        node->setRotation(Vec3d(rotation));
        std::random_device rd;
        node->setID(rd());
        node->setPosition(position);
        node->setMD2Animation(irr::scene::EMAT_STAND);
        setTexture(meshName, teamName, node);
        node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
        return node;
    }
    throw GUI::ShaderCompilationException("Error creating mesh node");
}

/**
 * @brief Retrieves a mesh by its name.
 *
 * @param meshName Name of the mesh file (without extension).
 * @return irr::scene::IAnimatedMesh* Pointer to the loaded mesh.
 */
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

/**
 * @brief Retrieves a texture by its name.
 *
 * @param textureName Name of the texture file (without extension).
 * @return irr::video::ITexture* Pointer to the loaded texture.
 */
irr::video::ITexture *MeshImporter::getTexture(std::string textureName) {
    irr::io::path pathTexture = irr::io::path(("assets/Bake" +
        textureName + ".png").c_str());

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

/**
 * @brief Sets a texture for a mesh node, optionally modifying it based on the team name.
 *
 * @param meshName Name of the mesh file.
 * @param teamName Name of the team (optional).
 * @param node Shared pointer to the mesh scene node.
 */
void MeshImporter::setTexture(std::string meshName, const std::string &teamName,
    std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> node) {
    irr::video::ITexture* texture = getTexture(meshName);
    if (teamName == "") {
        node->setMaterialTexture(0, texture);
        return;
    }
    img image = img(GUI::Window::i().driver->createImage(texture,
        irr::core::position2d<irr::s32>(0, 0), texture->getOriginalSize()),
        [](irr::video::IImage* i) {(void) i;});
    irr::video::SColor newColor = getColor(teamName);

    for (irr::u32 y = 0; y < image->getDimension().Height; ++y) {
        for (irr::u32 x = 0; x < image->getDimension().Width; ++x) {
            irr::video::SColor pixel = image->getPixel(x, y);
            if (pixel.getBlue() >= 200 && pixel.getRed() >= 200 &&
                pixel.getGreen() >= 200)
                image->setPixel(x, y, newColor);
        }
    }
    node->setMaterialTexture(0, GUI::Window::i().driver->addTexture(
        irr::io::path("ModifiedTexture"), image.get()));
    image->drop();
}

/**
 * @brief Retrieves a color associated with a team name.
 *
 * @param teamName Name of the team.
 * @return irr::video::SColor Color associated with the team.
 */
irr::video::SColor MeshImporter::getColor(std::string teamName) {
    if (colorTeam.find(teamName) == colorTeam.end()) {
        if (iColor >= colors.size())
            iColor = 0;
        colorTeam[teamName] = colors[iColor++];
    }
    return colorTeam[teamName];
}
