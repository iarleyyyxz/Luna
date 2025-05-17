#ifndef COMPONENT_HPP
#define COMPONENT_HPP

namespace Component {

    class Component {
    public:
        virtual ~Component() = default;
        virtual void update(float deltaTime) {}
        virtual void render() {}
    };

} // namespace Component

#endif // COMPONENT_HPP