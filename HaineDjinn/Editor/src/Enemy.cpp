#include "Enemy.h"

#include "Core/Transform.h"
#include "Core/Debug.h"
#include "Game/InputSystem.h"
#include "Physics/CubeCollider.h"
#include "Physics/Collider.h"

Enemy::Enemy(GameObject* _gameObject) : GameObject()
{
    gameObject = _gameObject;
    gameObject->entityType = "Enemy";

    idleAnim = new Animator(gameObject);
    walkAnim = new Animator(gameObject);
    punchAnim = new Animator(gameObject);
    hitAnim = new Animator(gameObject);
    dieAnim = new Animator(gameObject);

    //Load collider of the player
    std::deque<Component*> colliders = gameObject->GetComponents(ComponentType::CUBECOLLIDER);
    for (int i = 0; i < colliders.size(); i++)
    {
        CubeCollider* cubeC = static_cast<CubeCollider*>(colliders[i]);

        if (cubeC->name == "CubeCollider(0)")
            enemyCollider = cubeC;
        else if (cubeC->name == "CubeCollider(1)")
            punchCollider = cubeC;
    }
}

Enemy::~Enemy()
{
    delete idleAnim;
    delete walkAnim;
    delete punchAnim;
    delete hitAnim;
    delete dieAnim;
}

MoveEnemy::~MoveEnemy()
{
    for (int i = 0; i < enemies.size(); i++)
        delete enemies[i];
}

void MoveEnemy::SetEnemiesScene(Scene& scene)
{
    if (!enemies.empty())
        return;

    for (int i = 0; i < 17; i++)
    {
        //Set enemy array
        Enemy* enemy = new Enemy(scene.gameObjects[317 + i]);
        enemies.emplace_back(enemy);
    }
}

void MoveEnemy::Update(double deltaTime, GameObject* player, Scene& scene)
{
    //for (int i = 0; i < enemies.size(); i++)
    //{
    //    GameObject* go = enemies[i];
    //    EnemyComponent* enemy= go->GetComponent(ComponentType::ENEMY);
    //}


    for (int i = 0; i < enemies.size(); i++)
    {
        if (Maths::pointsDistance({ player->transform->position.x, player->transform->position.z }, { enemies[i]->gameObject->transform->position.x, enemies[i]->gameObject->transform->position.z }) <= 3.5f) // detection range
        {
            Vector3 moveDir = normalizeVector3((player->transform->position - enemies[i]->gameObject->transform->position));

            if (Maths::pointsDistance({ player->transform->position.x, player->transform->position.z }, { enemies[i]->gameObject->transform->position.x, enemies[i]->gameObject->transform->position.z }) >= 0.3f) //Stop near the player
                MovingEnemy(i, moveDir);
            else
            {
                AttackAnimationManager(i);
            }

            //Call event for moving properly
            enemies[i]->gameObject->transform->onMoveEvent.Broadcast(nullptr);
        }
        else
        {
            Animator* animTofollow = static_cast<Animator*>(enemies[i]->gameObject->GetComponent(ComponentType::ANIMATOR));
            //Set idle anim
            if (animTofollow->currentAnimation != enemies[i]->idleAnim->currentAnimation)
            {
                enemies[i]->idleAnim->SetAnimation(6);
                animTofollow->currentAnimation = enemies[i]->idleAnim->currentAnimation;
            }
        }
    }
}

void Enemy::OnTriggerEnter(void* parameters)
{
    if (parameters == nullptr)
    {
        HD_LOG("No parameters, ignoring rest of OnTriggerEnter", LogSeverity::LOG_WARNING);
        return;
    }

    TriggerInfo* trigger = static_cast<TriggerInfo*>(parameters);

    if (trigger == nullptr)
        return;

    for (int i = 0; i < components.size(); i++)
    {
        components[i]->OnTrigger(trigger);
    }
}

void MoveEnemy::MovingEnemy(int index, Vector3 moveDir)
{
    float dt = static_cast<float>(Time::DeltaTime());
    //enemy movement
    enemies[index]->gameObject->transform->position.x += dt * moveDir.x * enemies[index]->speed;
    enemies[index]->gameObject->transform->position.z += dt * moveDir.z * enemies[index]->speed;
    Vector3 eulerRot = { 0.f, atan2(moveDir.x, moveDir.z), 0.f };
    enemies[index]->gameObject->transform->rotation = QuaternionFromEuler(eulerRot);

    //SetAnim when walking
    Animator* animTofollow = enemies[index]->gameObject->GetComponent<Animator>(ComponentType::ANIMATOR);
    if (animTofollow->currentAnimation != enemies[index]->walkAnim->currentAnimation)
    {
        enemies[index]->walkAnim->SetAnimation(8);
        animTofollow->currentAnimation = enemies[index]->walkAnim->currentAnimation;
    }
}

void MoveEnemy::AttackAnimationManager(int index)
{
    static Timer timer(enemies[index]->delayAttack, false);
    timer.Update();

    if (timer.hasEnded)
        enemies[index]->canAttack = true;

    if (enemies[index]->canAttack)
    {
        Animator* animTofollow = static_cast<Animator*>(enemies[index]->gameObject->GetComponent(ComponentType::ANIMATOR));
        if (animTofollow->currentAnimation != enemies[index]->punchAnim->currentAnimation)
        {
            enemies[index]->punchAnim->SetAnimation(1);
            animTofollow->currentAnimation = enemies[index]->punchAnim->currentAnimation;
        }

        if (enemies[index]->animationTime <= 0.f)
            enemies[index]->animationTime = enemies[index]->punchAnim->currentAnimation->animation->mDuration / 24; //Duration of the anim = 10sec, there is 24 repetitions. We isolating one

        enemies[index]->animationTime -= Time::DeltaTime();

        if (enemies[index]->animationTime <= 0.f)
        {
            timer.Restart(0, false);
            enemies[index]->canAttack = false;
        }
    }
    else
    {
        Animator* animTofollow = static_cast<Animator*>(enemies[index]->gameObject->GetComponent(ComponentType::ANIMATOR));
        if (animTofollow != enemies[index]->idleAnim)
        {
            enemies[index]->punchAnim->SetAnimation(6);
            animTofollow->currentAnimation = enemies[index]->idleAnim->currentAnimation;
        }
    }
}
