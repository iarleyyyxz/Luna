#ifndef LUNA_ECS_ENTITY_HPP
#define LUNA_ECS_ENTITY_HPP

namespace Luna::Ecs {

    using EntityID = unsigned int;

    class Entity {
    public:
        Entity() : id(nextID++) {}
        Entity(EntityID existingID) : id(existingID) {}

        EntityID getID() const { return id; }

    private:
        EntityID id;
        static EntityID nextID;
    };

} // namespace Luna::Ecs

#endif // LUNA_ECS_ENTITY_HPP