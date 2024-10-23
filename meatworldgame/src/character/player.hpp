// #pragma once

// #include "character.hpp"


// namespace meat
// {
//     class  CharacterFreecam;
//     class  CharacterPlayerFPS;
//     class  ControllerPlayer;
// }



// class meat::CharacterFreecam: public ActorBase
// {
// protected:
//     int &m_cam_obj;
//     int  m_hinge_obj;

// public:
//     CharacterFreecam( World &world, const glm::vec3 &pos=glm::vec3(0.0f), float yaw=0.0f );

//     virtual void look( const glm::vec2& ) override;
//     virtual void move( idk::EngineAPI&, const glm::vec3& ) override;
// };



// class meat::CharacterPlayerFPS: public CharacterFreecam
// {
// private:
//     int m_sounds0[4];
//     int m_sounds1[4];
//     int m_sounds2[4];

// public:
//     CharacterPlayerFPS( World &world, const glm::vec3 &pos=glm::vec3(0.0f), float yaw=0.0f );

//     virtual void move( idk::EngineAPI&, const glm::vec3& ) override;
//     virtual void jump() override;

// };


// class meat::ControllerPlayer: public ControllerBase2
// {
// public:
//     virtual void update( idk::EngineAPI&, ActorBase* );
// };



