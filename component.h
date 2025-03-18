#pragma once

namespace game {
    class GameObject;

    // Component class, only used to derive from
    class Component {
    public:
        virtual ~Component() = default;
        virtual void Update(double delta_time) {}
        GameObject* GetParent() const;

    protected:
        GameObject* parent = nullptr;
    };
}