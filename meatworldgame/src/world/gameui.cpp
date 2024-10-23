#include "world.hpp"

#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKGraphics/idk_render_settings.hpp>


meat::GameUI::GameUI()
{
    using namespace idk;

    idk::glTextureConfig config = {
        .internalformat = GL_RGBA8,
        .format         = GL_RGBA,
        .minfilter      = GL_LINEAR,
        .magfilter      = GL_LINEAR,
        .datatype       = GL_UNSIGNED_BYTE,
        .genmipmap      = GL_FALSE
    };


    textures.push_back(gltools::loadTexture("assets/img/space.png", config));
    textures.push_back(gltools::loadTexture("assets/img/space-lq.png", config));
    textures.push_back(gltools::loadTexture("assets/img/blood-eye.png", config));
    textures.push_back(gltools::loadTexture("assets/img/blood-eye-lq.png", config));
    textures.push_back(gltools::loadTexture("assets/img/Outline/crosshair012.png", config));
    textures.push_back(gltools::loadTexture("assets/img/hit.png", config));
    
}