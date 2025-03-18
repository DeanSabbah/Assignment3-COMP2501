#pragma once

namespace game {
    class GameObject;

    // Component class, only used to derive from
    class Component {
    public:
        virtual ~Component() = default;
        virtual void Update(double delta_time) {}
        virtual void init() {}
        GameObject* GetParent() const;
        void setParent(GameObject* parent);
    protected:
        GameObject* parent = nullptr;
    };
}