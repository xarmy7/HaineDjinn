#pragma once

#include "Core/GameObject.h"
#include "Core/Scene.h"
#include "Physics/Collider.h"
#include <Physics/PhysicsEventListener.h>

#include <reactphysics3d/reactphysics3d.h>

class Enemy : public GameObject
{
private:

public:
	Enemy() = default;
	Enemy(GameObject* gameObject);
	~Enemy();

	//void OnCollisionEnter(void* parameters) override;
	void OnTriggerEnter(void* parameters) override;

	int life = 1;

	float speed = 0.3f;
	double animationTime = 0.f;
	float delayAttack = 1.f;

	bool canAttack = false;

	GameObject* gameObject = nullptr;

	Collider* enemyCollider = nullptr;
	Collider* punchCollider = nullptr;	

	Animator* idleAnim = nullptr;
	Animator* walkAnim = nullptr;
	Animator* punchAnim = nullptr;
	Animator* hitAnim = nullptr;
	Animator* dieAnim = nullptr;
};

// Move enemy system
class MoveEnemy
{
private:


public:
	MoveEnemy() {};
	~MoveEnemy();

	std::deque<Enemy*> enemies = {}; // std::vector<GameObject*> enemies;

	void SetEnemiesScene(Scene& scene);
	void MovingEnemy(int index, Vector3 moveDir);
	void AttackAnimationManager(int index);
	void Update(double deltaTime, GameObject* player, Scene& scene);
};