#ifndef LUNA_ECS_TRANSFORM_HPP
#define LUNA_ECS_TRANSFORM_HPP

#include <glm.hpp>

namespace Luna::Ecs {

    struct Transform {
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 rotation = glm::vec3(0.0f); // Em radianos
        glm::vec3 scale = glm::vec3(1.0f);

        Transform() = default;
        Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& sca)
            : position(pos), rotation(rot), scale(sca) {
        }
    };

} // namespace Luna::Ecs

#endif // LUNA_ECS_TRANSFORM_HPP