#include "modules/FrankLeboeufModule.hpp"
#include "Interface/Interface.hpp"

void FrankLeboeufModule::execute() {
    AI::Interface::i().sendCommand(
    R"(Broadcast Salut c'est Frank Leboeuf, vous
    voulez vendre votre voiture ? Alors rendez vous sur
    vendezvotrevoiture.fr ! Rentrez simplement les détails de votre
    véhicule et recevez rapidement son prix de vente. Et vendez ensuite
    votre voiture dans une de nos agences. On s'occupe de tout. Mon
    conseil ? Obtenez votre prix de vente sur vendezvotrevoiture.fr.)");
}

float FrankLeboeufModule::getPriority() {
    return 1.0f;
}
