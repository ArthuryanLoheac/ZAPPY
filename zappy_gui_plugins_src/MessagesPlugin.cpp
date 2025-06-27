#include "zappy_gui_plugins_src/MessagesPlugin.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>

extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<MessagesPlugin>();
    }
}

void MessagesPlugin::drawMessageHistory(
std::shared_ptr<irr::gui::IGUIFont> font, irr::video::IVideoDriver* driver) {
    int height = driver->getScreenSize().Height;
    int delta = 30;
    int y = height - 30 - (data.messages.size() * delta);
    int x = 5;
    UICol color = UICol(255, 255, 255, 255);

    for (const auto &message : data.messages) {
        if (message.content.empty())
            continue;
        std::string text = std::to_string(message.playerId) +
            ": " + message.content;
        if (text.size() > 50)
            text = text.substr(0, 50) + "...";
        drawImage("assets/UI/BottomBig.png", 0, y, 550, delta, driver);
        font->draw(text.c_str(), irr::core::rect<irr::s32>(
            x, y + 5, 650, y + delta), color);
        y += delta;
    }
}

void MessagesPlugin::initParticle(irr::video::IVideoDriver *driver,
irr::core::vector3df position, irr::core::vector3df direction, int age) {
    irr::scene::IParticleSystemSceneNode *ps =
        smgr->addParticleSystemSceneNode(false);
    age *= speedParticle;
    irr::core::vector3df dir = direction / (100.f * speedParticle);

    irr::scene::IParticleEmitter* em = ps->createBoxEmitter(
        irr::core::aabbox3d<irr::f32>(-7, 0, -7, 7, 1, 7),  // emitter size
        dir,                                                // initial direction
        50, 100,                                            // emit rate
        irr::video::SColor(0, 255, 255, 255),               // darkest color
        irr::video::SColor(0, 255, 255, 255),               // brightest color
        age, age,                                           // min and max age
        0,                                                  // angle creation
        irr::core::dimension2df(0.05f, 0.05f),              // min size
        irr::core::dimension2df(0.2f, 0.2f));               // max size

    ps->setEmitter(em);

    irr::scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();

    ps->addAffector(paf);
    paf->drop();

    ps->setPosition(position);
    ps->setScale(irr::core::vector3df(0.001f, 0.001f, 0.001f));
    ps->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    ps->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
    ps->setMaterialTexture(0, driver->getTexture("assets/UI/dataParticle.png"));
    ps->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);

    irr::ITimer* timer = device->getTimer();
    Particle particle;
    particle.particleSystem = ps;
    particle.end = timer->getTime() + age;
    particle.kill = timer->getTime() + (age * 2);
    particle.emitter = em;
    particles.push_back(particle);
}

void MessagesPlugin::checkDeleteParticles() {
    int i = 0;

    for (auto particle : particles) {
        if (particle.end < device->getTimer()->getTime() && !particle.stopped
            && particle.emitter) {
            particle.emitter->setMinParticlesPerSecond(0);
            particle.emitter->setMaxParticlesPerSecond(0);
            particle.stopped = true;
        }
        if (particle.particleSystem && smgr &&
            particle.kill < device->getTimer()->getTime()) {
            if (!particle.particleSystem->isVisible()) {
                try {
                    smgr->registerNodeForRendering(particle.particleSystem,
                        irr::scene::ESNRP_AUTOMATIC);
                    smgr->addToDeletionQueue(particle.particleSystem);
                } catch (const std::exception &e) {
                    std::cerr << "Exception occurred: " << e.what()
                        << std::endl;
                }
            }
            particles.erase(particles.begin() + i);
            i--;
        }
        i++;
    }
}

void MessagesPlugin::SendParticlesToAll(irr::video::IVideoDriver *driver,
pluginsData::Player p) {
    for (auto &player : data.players) {
        if (player.id == p.id || !player.PlayerMesh || !p.PlayerMesh)
            continue;
        std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> playerMesh = player.PlayerMesh;
        std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> pplayerMesh = p.PlayerMesh;
        if (!playerMesh || !pplayerMesh)
            continue;
        irr::core::vector3df pos = playerMesh->getPosition();
        irr::core::vector3df ppos = pplayerMesh->getPosition();
        irr::core::vector3df dir = pos - ppos;
        int dist = (pos - ppos).getLength();
        dir.normalize();
        dist *= 100;
        initParticle(driver, ppos, dir, dist);
    }
}

void MessagesPlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
    irr::video::IVideoDriver *driver) {
    if (!driver || !font)
        return;
    for (auto message : data.messagesThisFrame) {
        pluginsData::Player p = data.getPlayer(message.playerId);
        if (p.PlayerMesh)
            SendParticlesToAll(driver, p);
    }
    drawMessageHistory(font, driver);
    checkDeleteParticles();
}
