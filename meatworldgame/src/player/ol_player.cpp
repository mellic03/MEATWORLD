#include "ol_player.hpp"
#include "../../../meatnet/src/meatnet.hpp"


void
meatworld::OLPlayer::init()
{
    using namespace idk;

    m_obj_id    = ECS2::createGameObject("OL Player", false);
    m_model_obj = ECS2::createGameObject("model", false);

    ECS2::giveComponent<TransformCmp>(m_obj_id);
    ECS2::giveComponent<ModelCmp>(m_model_obj);
    ModelSys::assignModel(m_model_obj, "assets/models/head.idkvi");

    ECS2::giveChild(m_obj_id, m_model_obj);


    TransformSys::getLocalPosition(m_obj_id)    = glm::vec3(0.0f);
    TransformSys::getLocalPosition(m_model_obj) = glm::vec3(0.0f);
    TransformSys::getXYZScale(m_model_obj) = glm::vec3(0.6f);

    m_flashlight.init(m_obj_id);
    m_flashlight.off();
}


meatworld::OLPlayer::OLPlayer()
{
    using namespace idk;
}


meatworld::OLPlayer::~OLPlayer()
{
    using namespace idk;

    if (ECS2::gameObjectExists(m_obj_id))
    {
        ECS2::deleteGameObject(m_obj_id);
    }
}




void
meatworld::OLPlayer::update( idk::EngineAPI &api, const meatnet::PeerData &data  )
{
    using namespace idk;

    float dtime  = api.getEngine().deltaTime();
    auto &ren    = api.getRenderer();
    
    auto &events = api.getEventSys();
    auto &K      = events.keylog();

    TransformSys::setWorldPosition(m_obj_id, data.position);
    TransformSys::getTransform(m_obj_id).rotation = glm::quat(glm::vec3(0.0f));

    auto &cmp = TransformSys::getTransformCmp(m_obj_id);
    cmp.pitch = 2.0f * M_PI * float(data.pitch) / 255.0f;
    cmp.yaw   = 2.0f * M_PI * float(data.yaw) / 255.0f;

    if (data.state & meatnet::STATE_FLASHLIGHT)
    {
        m_flashlight.on();
    }

    else
    {
        m_flashlight.off();
    }

    if (m_weapon)
    {
        m_weapon->update(api, 0.0f, 0.0f);

        if (data.action & meatnet::ACTION_SHOOT)
        {
            m_weapon->attack(api);
        }
    }


    if (data.weapon)
    {
        uint32_t flags = data.weapon;

        std::string name = "none";

        if (m_weapon)
        {
            name = m_weapon->getDesc().name;
        }

        if ((flags & meatnet::WEAPON_GLOCK) && name != "Glock")
        {
            giveWeapon<NPC_Glock>();
        }

        else if ((flags & meatnet::WEAPON_AR2) && name != "AR2")
        {
            giveWeapon<NPC_HL2_AR2>();
        }
    }


};

