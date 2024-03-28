#include "civillian.hpp"


idkg::Civillian::Civillian( idk::EngineAPI &api, int planet )
{
    auto &ecs = api.getECS();

    m_ID = ecs.createGameObject("Civillian");
    ecs.giveChild(planet, m_ID);

    ecs.giveComponent<idk::TransformCmp>(m_ID);
    idk::TransformSys::setPositionLocalspace(m_ID, glm::vec3(0.0f, 60.0f, 0.0f));

    ecs.giveComponent<idk::PhysXKinematicControllerCmp>(m_ID);

    idk::ModelSys::assignModel(m_ID, "assets/models/dr-SDS.idkvi");

}



void
idkg::Civillian::update( idk::EngineAPI &api, int player )
{
    heading += 0.001f;

    idk::PhysXSys::moveTowardsCCT(m_ID, player, 0.0001f);

}
