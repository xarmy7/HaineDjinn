#include "CameraFollow.h"
#include "Core/Transform.h"
#include "Game/InputSystem.h"
#include "Game/SoundSystem.h"
#include "Core/Debug.h"
#include "Core/Time.h"
#include "Renderer/Animator.h"
#include "Physics/CubeCollider.h"

#include <iostream>

Player CameraFollow::player = NULL;
bool CameraFollow::firstLoop = true;


CameraFollow::CameraFollow() : camera(1280, 720)
{
    camera.position = {};
    offsetFromTarget = { 0.f, 3.1f, 1.8f };
    camera.yaw = 43.978f;
    camera.pitch = 1.240f;
}

Player::Player(GameObject* playerGameObject)
{
    //gameObject = playerGameObject;
    //gameObject->entityType = "Player";

    //
    ////Load Animator of the player
    //attackAnimator = new Animator(gameObject);
    //walkAnimator = new Animator(gameObject);
    //idleAnimator = new Animator(gameObject);

    //life = 3;
    //mana = 20;
    //speed = 0.75f;


    ////Load collider of the player
    //std::deque<Component*> collider = gameObject->GetComponents(ComponentType::CUBECOLLIDER);
    //if (collider.size() != 0)
    //{
    //    CubeCollider* cubeC = nullptr;

    //    for (int i = 0; i < collider.size(); i++)
    //    {
    //        cubeC = static_cast<CubeCollider*>(collider[i]);

    //        if (cubeC->name == "CubeCollider(0)")
    //            playerCollider = cubeC;
    //        else if (cubeC->name == "CubeCollider(1)")
    //            punchCollider = cubeC;
    //    }
    //}
}

void CameraFollow::SetPlayer(GameObject* go)
{
    if (firstLoop == true)
    {
        player.gameObject = go;
        player.gameObject->entityType = "Player";
        player.attackAnimator = new Animator(go);
        player.walkAnimator = new Animator(go);
        player.idleAnimator = new Animator(go);
        player.life = 3;
        player.mana = 20;
        player.speed = 0.75f;
        firstLoop = false;
    }
}

void CameraFollow::Update(GameObject* toFollow, float deltatime)
{
    /*static Player p( toFollow);*/

    if (toFollow == nullptr) 
    {
        HD_LOG("The GameObject doesn't exist!", LogSeverity::LOG_ERROR);
        return;
    }

    camera.position = toFollow->transform->position + offsetFromTarget; //Update camera position

#pragma region MOVE-PLAYER-AND-CAMERA

    Vector2 movement = {};
    movement.x = InputSystem::GetAxis("Horizontal") * player.speed * (float)(Time::DeltaTime());
    movement.y = -InputSystem::GetAxis("Vertical") * player.speed * (float)(Time::DeltaTime());

    toFollow->transform->position.x += movement.x;
    toFollow->transform->position.z += movement.y;
    toFollow->transform->position.y = 0.15f;

    if (movement.x == 0 && movement.y == 0 && !player.playerDead)
    {
        // Idle
        if (!player.playerIdle)
        {
            player.idleAnimator->SetAnimation(6);
            Animator* animTofollow = static_cast<Animator*>(toFollow->GetComponent(ComponentType::ANIMATOR));
            animTofollow->currentAnimation = player.idleAnimator->currentAnimation;

            player.playerWalk = false;
            player.playerIdle = true;
        }
    }
    else
    {
        // Movement
        if (!player.playerWalk)
        {
            player.walkAnimator->SetAnimation(8);
            Animator* animTofollow = static_cast<Animator*>(toFollow->GetComponent(ComponentType::ANIMATOR));
            animTofollow->currentAnimation = player.walkAnimator->currentAnimation;


            player.playerWalk = true;
            player.playerIdle = false;
        }

        toFollow->transform->rotation = QuaternionIdentity() * QuaternionFromAxisAngle({ 0, 1, 0 }, atan2(-movement.y, movement.x) + M_PI / 2);
    }

    static Timer timer(0.5f, false);
    timer.Update();

    if (timer.hasEnded)
    {
        if (InputSystem::GetMouseButtonDown(1))
        {
            player.playerIdle = true;
            player.playerWalk = true;
            player.isAttacking = true;

            player.attackAnimator->SetAnimation(1);
            Animator* animTofollow = static_cast<Animator*>(toFollow->GetComponent(ComponentType::ANIMATOR));
            //animTofollow = player.attackAnimator;
            animTofollow->currentAnimation = player.attackAnimator->currentAnimation;
    

            SoundSystem::PlaySound2D("Sounds/Slash_Sword.mp3"); 
            timer.Restart(0, false);
        }
    }

    if (player.isAttacking)
    {
       if (player.animationTime <= 0.f)
           player.animationTime = player.attackAnimator->currentAnimation->animation->mDuration / 24;

       player.animationTime -= Time::DeltaTime();

        if (player.animationTime <= 0)
        {
            player.playerIdle = false;
            player.playerWalk = false;
            player.isAttacking = false;
        }
    }

#pragma endregion

    if (InputSystem::GetInputDown("P"))
        player.life -= 1;

    if (player.life <= 0)
        player.playerDead = true;

    toFollow->transform->onMoveEvent.Broadcast(nullptr);
}

void Player::PlayerMoving(GameObject* player)
{
    Vector2 movement = {};
    movement.x = InputSystem::GetAxis("Horizontal") * speed * (float)(Time::DeltaTime());
    movement.y = -InputSystem::GetAxis("Vertical") * speed * (float)(Time::DeltaTime());

    player->transform->position.x += movement.x;
    player->transform->position.z += movement.y;
    player->transform->position.y = 0.036f;

    if (movement.x == 0 && movement.y == 0 && !playerDead)
    {
        // Idle
        if (!playerIdle)
        {
            idleAnimator->SetAnimation(6);
            Animator* animTofollow = static_cast<Animator*>(player->GetComponent(ComponentType::ANIMATOR));
            animTofollow->currentAnimation = idleAnimator->currentAnimation;

            playerWalk = false;
            playerIdle = true;
        }
    }
    else
    {
        // Movement
        if (!playerWalk)
        {
            walkAnimator->SetAnimation(8);
            Animator* animTofollow = static_cast<Animator*>(player->GetComponent(ComponentType::ANIMATOR));
            animTofollow->currentAnimation = walkAnimator->currentAnimation;

            playerWalk = true;
            playerIdle = false;
        }

        player->transform->rotation = QuaternionIdentity() * QuaternionFromAxisAngle({ 0, 1, 0 }, atan2(-movement.y, movement.x) + M_PI / 2);
    }
}

void Player::PlayerAttackManager(GameObject* playerGameObject)
{
    static Timer timer(0.5f, false);
    timer.Update();

    if (timer.hasEnded)
    {
        if (InputSystem::GetMouseButtonDown(1))
        {
            playerIdle = true;
            playerWalk = true;
            isAttacking = true;

            attackAnimator->SetAnimation(1);
            Animator* animTofollow = static_cast<Animator*>(playerGameObject->GetComponent(ComponentType::ANIMATOR));
            animTofollow->currentAnimation = attackAnimator->currentAnimation;

            SoundSystem::PlaySound2D("Sounds/Slash_Sword.mp3");
            timer.Restart(0, false);
        }
    }

    if (isAttacking)
    {
        if (animationTime <= 0.f)
            animationTime = attackAnimator->currentAnimation->animation->mDuration / 24;

        animationTime -= Time::DeltaTime();

        if (animationTime <= 0)
        {
            playerIdle = false;
            playerWalk = false;
            isAttacking = false;
        }
    }
}

Player::~Player()
{
    delete attackAnimator;
    delete walkAnimator;
}
