// #include "character.hpp"

// #include <IDKGameEngine/IDKGameEngine.hpp>
// #include <IDKEvents/IDKEvents.hpp>

// #include <libidk/idk_log.hpp>
// #include <IDKGraphics/UI/idk_ui.hpp>



// idkg::CharacterFactory::CharacterFactory( idk::EngineAPI &api )
// :   m_api(api)
// {

// }



// void
// idkg::CharacterFactory::update()
// {
//     for (idkg::Character *character: m_characters)
//     {
//         character->update(m_api);
//     }
// }



// idkg::Character::Character( idk::EngineAPI &api, int obj_id )
// :   idk::Identifiable (obj_id),
//     m_obj             (obj_id)
// {
    
//     idk::ECS2::giveComponent<idk::TransformCmp>(obj_id);
// }





// idkg::Enemy::Enemy( idk::EngineAPI &api, int obj_id )
// :   idkg::Character(api, obj_id)
// {
    
//     auto &ren = api.getRenderer();

//     // idk::ECS2::giveComponent<idk::ModelCmp>(m_obj);
//     // idk::ModelSys::assignModel(m_obj, "assets/models/chair.idkvi");
// }




// void
// idkg::Enemy::update( idk::EngineAPI &api )
// {
//     // std::cout << "idkg::Enemy::update()\n";
// }


// void
// idkg::Asshole::update( idk::EngineAPI &api )
// {
//     // std::cout << "idkg::Asshole::update()\n";
// }



