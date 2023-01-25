
#include "Physics/PhysicsEventListener.h"
#include "Physics/Collider.h"
#include "Core/Debug.h"

void PhysicsEventListener::onContact(const reactphysics3d::CollisionCallback::CallbackData& callbackData)
{
	int contactNumbers = callbackData.getNbContactPairs();
	
	if (contactNumbers <= 0)
		return;

	for (int i = 0; i < contactNumbers; i++)
	{
		CollisionInfo collision = {};

		reactphysics3d::CollisionCallback::ContactPair pair = callbackData.getContactPair(0);
		reactphysics3d::CollisionCallback::ContactPair::EventType type = pair.getEventType();

		switch (type)
		{
		case reactphysics3d::CollisionCallback::ContactPair::EventType::ContactStart:
			collision.collType = CollisionType::ENTER;
			break;
		case reactphysics3d::CollisionCallback::ContactPair::EventType::ContactStay:
			collision.collType = CollisionType::STAY;
			break;
		case reactphysics3d::CollisionCallback::ContactPair::EventType::ContactExit:
			collision.collType = CollisionType::EXIT;
			break;
		default:
			break;
		}

		reactphysics3d::Collider* coll1 = pair.getCollider1();
		Collider* collider1 = static_cast<Collider*>(coll1->getUserData());
		reactphysics3d::Collider* coll2 = pair.getCollider2();
		Collider* collider2 = static_cast<Collider*>(coll2->getUserData());
		
		collision.collider1 = collider1;
		collision.collider2 = collider2;

		if (pair.getNbContactPoints() > 0)
		{
			// Get the contact point 
			reactphysics3d::CollisionCallback::ContactPoint contactPoint = pair.getContactPoint(0);

			// Get the contact point on the first collider and convert it in world-space 
			reactphysics3d::Vector3 worldPoint = pair.getCollider1()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider1();
			collision.contactPoint = { worldPoint.x, worldPoint.y, worldPoint.z };
		}

		if (collider1 != nullptr)
			collider1->collisionEvent.Broadcast((void*)(&collision));


		if (collider2 != nullptr)
			collider2->collisionEvent.Broadcast((void*)(&collision));
	}
}

void PhysicsEventListener::onTrigger(const reactphysics3d::OverlapCallback::CallbackData& callbackData)
{
	int contactNumbers = callbackData.getNbOverlappingPairs();

	if (contactNumbers <= 0)
		return;

	for (int i = 0; i < contactNumbers; i++)
	{
		TriggerInfo trigger = {};

		reactphysics3d::OverlapCallback::OverlapPair pair = callbackData.getOverlappingPair(0);
		reactphysics3d::OverlapCallback::OverlapPair::EventType type = pair.getEventType();

		switch (type)
		{
		case reactphysics3d::OverlapCallback::OverlapPair::EventType::OverlapStart:
			trigger.collType = CollisionType::ENTER;
			break;
		case reactphysics3d::OverlapCallback::OverlapPair::EventType::OverlapStay:
			trigger.collType = CollisionType::STAY;
			break;
		case reactphysics3d::OverlapCallback::OverlapPair::EventType::OverlapExit:
			trigger.collType = CollisionType::EXIT;
			break;
		default:
			break;
		}

		reactphysics3d::Collider* coll1 = pair.getCollider1();
		Collider* collider1 = static_cast<Collider*>(coll1->getUserData());
		reactphysics3d::Collider* coll2 = pair.getCollider2();
		Collider* collider2 = static_cast<Collider*>(coll2->getUserData());

		trigger.collider1 = collider1;
		trigger.collider2 = collider2;

		if (collider1 != nullptr)
			collider1->triggerEvent.Broadcast((void*)(&trigger));

		if (collider2 != nullptr)
			collider2->triggerEvent.Broadcast((void*)(&trigger));
	}
}