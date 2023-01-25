#pragma once

#include "Renderer/Camera.h"
#include "Core/GameObject.h"

class CubeCollider;
class Animator;

class Player : public GameObject
{
private:

public:
	Player() {};
	Player(GameObject* playerGameObject);
	~Player();

	void PlayerMoving(GameObject* player);
	void PlayerAttackManager(GameObject* playerGameObject);

	CubeCollider* punchCollider = nullptr;
	CubeCollider* playerCollider = nullptr;

	unsigned int life = 0;
	unsigned int mana = 0;

	double speed = 0;

	bool playerDead = false;
	bool playerWalk = false;
	bool playerIdle = false;
	bool isAttacking = false;

	float animationTime = 0.f;

	Animator* walkAnimator = nullptr;
	Animator* attackAnimator = nullptr;
	Animator* idleAnimator = nullptr;
	GameObject* gameObject = nullptr;
};

class CameraFollow
{
private:
	
public:
	static Player player;
	static bool firstLoop;
	Camera camera;
	Vector3 offsetFromTarget = {};
	CameraFollow();
	void SetPlayer(GameObject* go);
	void Update(GameObject* toFollow, float deltatime);
};