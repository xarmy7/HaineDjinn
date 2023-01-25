#pragma once

#include <reactphysics3d/engine/EventListener.h>

class PhysicsEventListener : public reactphysics3d::EventListener
{
private:

public:
    void onContact(const reactphysics3d::CollisionCallback::CallbackData& callbackData) override;

    void onTrigger(const reactphysics3d::OverlapCallback::CallbackData& callbackData) override;
};
