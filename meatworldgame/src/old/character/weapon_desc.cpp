#include "weapon.hpp"


// meatworld::WeaponDesc
// meatworld::Glock::getDesc()
// {
//     WeaponDesc desc = {
//         // .rest = glm::vec3(0.1f, -0.05f, -0.2f),
//         // .aim  = glm::vec3(0.0f, 0.0f, -0.25f),
//         .rest = glm::vec3(0.0f),
//         .aim = glm::vec3(0.0f),

//         .sway_speed = glm::vec3(0.00025f),
//         .rest_speed = glm::vec3(0.01f),
//         .aim_speed  = glm::vec3(0.1f),

//         .name       = "Glock",
//         .model_path = "assets/models/weapons/glock.idkvi",
//         .audio_path = "assets/audio/glock.wav",

//         .recoil     = 0.25f,
//         .aim_fov    = -15.0f,
//         .shots      = 1,
//         .damage     = 1.0f,

//         .inner_prob   = 0.25f,
//         .inner_spread = 0.05f,
//         .outer_spread = 0.15f
//     };

//     return desc;
// }


// meatworld::WeaponDesc
// meatworld::HL2_AR2::getDesc()
// {
//     WeaponDesc desc = {
//         .rest = glm::vec3(0.1f, -0.05f, -0.2f),
//         .aim  = glm::vec3(0.0f,  0.0f,  -0.32f),

//         .sway_speed = glm::vec3(0.00025f),
//         .rest_speed = glm::vec3(0.01f),
//         .aim_speed  = glm::vec3(0.1f),

//         .name       = "AR2",
//         .model_path = "assets/models/weapons/hl2-ar2.idkvi",
//         .audio_path = "assets/audio/rifle.wav",

//         .recoil     = 0.5f,
//         .aim_fov    = -45.0f,
//         .shots      = 1,
//         .damage     = 5.0f,

//         .inner_prob   = 0.75f,
//         .inner_spread = 0.025f,
//         .outer_spread = 0.1f
//     };

//     return desc;
// }




meatworld::WeaponDesc
meatworld::Glock::getDesc()
{
    WeaponDesc desc = {
        .rest = glm::vec4(0.1f, -0.05f, -0.2f, glm::radians(-10.0f)),
        .aim  = glm::vec4(0.0f, 0.0f, -0.25f, 0.0f),

        .sway_speed = glm::vec3(0.00025f),
        .rest_speed = glm::vec3(0.00025f),
        .aim_speed  = glm::vec3(0.000125f),

        .name       = "Glock",
        .model_path = "assets/models/weapons/glock.idkvi",
        .audio_path = "assets/audio/glock.wav",

        .recoil     = 0.25f,
        .aim_fov    = -25.0f,
        .shots      = 1,
        .damage     = 1.0f,

        .inner_prob   = 0.25f,
        .inner_spread = 0.04f,
        .outer_spread = 0.05f
    };

    return desc;
}


meatworld::WeaponDesc
meatworld::HL2_AR2::getDesc()
{
    WeaponDesc desc = {
        .rest = glm::vec4(0.13f, -0.05f, -0.22f, glm::radians(-10.0f)),
        .aim  = glm::vec4(0.0f, 0.0f, -0.25f, 0.0f),
    
        .sway_speed = glm::vec3(0.0005f),
        .rest_speed = glm::vec3(0.0005f),
        .aim_speed  = glm::vec3(0.00025f),

        .name       = "AR2",
        .model_path = "assets/models/weapons/hl2-ar2.idkvi",
        .audio_path = "assets/audio/rifle.wav",

        .recoil     = 0.35f,
        .aim_fov    = -45.0f,
        .shots      = 1,
        .damage     = 2.0f,

        .inner_prob   = 0.75f,
        .inner_spread = 0.015f,
        .outer_spread = 0.08f
    };

    return desc;
}
