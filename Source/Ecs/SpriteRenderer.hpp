#ifndef LUNA_ECS_SPRITE_RENDERER_HPP
#define LUNA_ECS_SPRITE_RENDERER_HPP

#include <string>
#include <glm.hpp>

namespace Luna::Ecs {

    struct SpriteRenderer {
        std::string texturePath; // Caminho para o arquivo de textura
        glm::vec4 color = glm::vec4(1.0f); // Cor do sprite (RGBA)

        SpriteRenderer(const std::string& path = "", const glm::vec4& c = glm::vec4(1.0f))
            : texturePath(path), color(c) {
        }
    };

} // namespace Luna::Ecs

#endif // LUNA_ECS_SPRITE_RENDERER_HPP