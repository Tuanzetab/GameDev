#include "Game.h"
#include <iostream>
#include <cmath>
#include <cstdlib>


sf::Color getRandomColor() {
    int red = rand() % (245 - 10 + 1) + 10;
    int green = rand() % (245 - 10 + 1) + 10;
    int blue = rand() % (245 - 10 + 1) + 10;
    return sf::Color(red, green, blue);
}

Game::Game(const std::string& config)
{
    init(config);
}

void Game::init(const std::string& path)
{
    // Đọc file config ở đây

    m_window.create(sf::VideoMode(1280, 720), "Assignment 2");
    m_window.setFramerateLimit(60);

    spawnPlayer();
}

void Game::run()
{
    while (m_running)
    {

        //sMovement();
        sUserInput();

        m_entities.update();

        sLifespan();
        sRender();
        sMovement();
        //sEnemySpawner();
        sCollision();

        m_currentFrame++;
    }
}



void Game::spawnEnemy()
{
    auto eEnemy = m_entities.addEntity("enemy");


    float eX = 0;
    float eY = 0;
    int side = rand() % 4;  // 0: top, 1: bottom, 2: left, 3: right

    switch (side) {
    case 0: // top
        eX = static_cast<float>(rand() % (m_window.getSize().x - 100) + 50);
        eY = 50;
        break;
    case 1: // bottom
        eX = static_cast<float>(rand() % (m_window.getSize().x - 100) + 50);
        eY = static_cast<float>(m_window.getSize().y) - 50;
        break;
    case 2: // left
        eX = 50;
        eY = static_cast<float>(rand() % (m_window.getSize().y - 100) + 50);
        break;
    case 3: // right
        eX = static_cast<float>(m_window.getSize().x) - 50;
        eY = static_cast<float>(rand() % (m_window.getSize().y - 100) + 50);
        break;
    }

    int points = rand() % 6 + 1;
    sf::Color randomColor = getRandomColor();
    eEnemy->cShape = std::make_shared<CShape>(40.0f, points, randomColor, sf::Color::White, 4.0f);
    eEnemy->cTransform = std::make_shared<CTransform>(Vec2(eX, eY), Vec2(0.f, 0.f), 0);

    Vec2 playerPos = m_player->cTransform->pos;
    Vec2 direction = playerPos - eEnemy->cTransform->pos;
    Vec2 normalizedDirection = direction.normalize();

    float speed = 3.0f;
    eEnemy->cTransform->velocity = normalizedDirection * speed;

    m_lastEnemySpawnTime = m_currentFrame;
}

void Game::sEnemySpawner()
{
    int enemySpawnInterval = 100; // Đặt khoảng thời gian giữa mỗi lần sinh kẻ thù là 100 frame

    if (m_currentFrame % enemySpawnInterval == 0)
    {
        spawnEnemy();
    }
}


void Game::sMovement()
{
    m_player->cTransform->velocity = { 0, 0 };
    float speed = 10.f;

    if (m_player->cInput->up)
    {
        m_player->cTransform->velocity.y = -speed;
    }
    if (m_player->cInput->left)
    {
        m_player->cTransform->velocity.x = -speed;
    }
    if (m_player->cInput->right)
    {
        m_player->cTransform->velocity.x = speed;
    }
    if (m_player->cInput->down)
    {
        m_player->cTransform->velocity.y = speed;
    }

    m_player->cTransform->pos += m_player->cTransform->velocity;

    sf::Vector2u windowSize = m_window.getSize();
    float radius = m_player->cShape->circle.getRadius();
    if (m_player->cTransform->pos.x - radius < 0)
    {
        m_player->cTransform->pos.x = radius;
    }
    if (m_player->cTransform->pos.x + radius > windowSize.x)
    {
        m_player->cTransform->pos.x = windowSize.x - radius;
    }
    if (m_player->cTransform->pos.y - radius < 0)
    {
        m_player->cTransform->pos.y = radius;
    }
    if (m_player->cTransform->pos.y + radius > windowSize.y)
    {
        m_player->cTransform->pos.y = windowSize.y - radius;
    }

    auto& enemies = m_entities.getEntities("enemy");
    for (auto& enemy : enemies)
    {
        enemy->cTransform->pos += enemy->cTransform->velocity;
    }

    auto& bullets = m_entities.getEntities("bullet");
    for (auto& bullet : bullets)
    {
        bullet->cTransform->pos += bullet->cTransform->velocity;
    }
}

void Game::spawnPlayer()
{
    auto entity = m_entities.addEntity("player");

    entity->cTransform = std::make_shared<CTransform>(
        Vec2(m_window.getSize().x / 2, m_window.getSize().y / 2),
        Vec2(1.0f, 1.0f),
        0.0f
    );
    entity->cShape = std::make_shared<CShape>(35.0f, 8, sf::Color::Black, sf::Color::Blue, 8.0);
    entity->cInput = std::make_shared<CInput>();

    m_player = entity;
}

void Game::sRender()
{
    m_window.clear();

    for (auto& e : m_entities.getEntities())
    {
        e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
        e->cTransform->angle += 1.0f;
        e->cShape->circle.setRotation(e->cTransform->angle);
        m_window.draw(e->cShape->circle);
    }

    m_window.display();
}

void Game::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_running = false;
        }

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            bool pressed = (event.type == sf::Event::KeyPressed);
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                m_player->cInput->up = pressed;
                break;
            case sf::Keyboard::A:
                m_player->cInput->left = pressed;
                break;
            case sf::Keyboard::D:
                m_player->cInput->right = pressed;
                break;
            case sf::Keyboard::S:
                m_player->cInput->down = pressed;
                break;
            default:
                break;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
            }
        }
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                spawnEnemy();
            }
        }
    }
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
    auto bullet = m_entities.addEntity("bullet");

    bullet->cTransform = std::make_shared<CTransform>(Vec2(m_player->cTransform->pos.x, m_player->cTransform->pos.y), Vec2(1.0f, 1.0f), 0.0f);

    Vec2 direction = target - m_player->cTransform->pos;
    Vec2 normalizedDirection = direction.normalize();
    float speed = 15.0f;
    bullet->cTransform->velocity = normalizedDirection * speed;

    bullet->cShape = std::make_shared<CShape>(3.0f, 8, sf::Color::White, sf::Color::White, 4.0);
    bullet->cLifespan = std::make_shared<CLifespan>(50);
    bullet->cCollision = std::make_shared<CCollision>(3.0f);
}


void Game::sLifespan()
{
    auto& bullets = m_entities.getEntities("bullet");
    for (auto& bullet : bullets)
    {
        if (bullet->cLifespan)
        {
            bullet->cLifespan->remaining--;
            if (bullet->cLifespan->remaining <= 0)
            {
                bullet->destroy();
            }
        }
    }
}

void Game::sCollision()
{


    auto bullets = m_entities.getEntities("bullet");
    auto enemies = m_entities.getEntities("enemy");
    auto player = m_entities.getEntities("player");

    for (auto& bullet : bullets)
    {
        if (bullet->is_active())
        {
            float bulletRadius = bullet->cShape->circle.getRadius();

            if (bullet->cTransform->pos.x - bulletRadius < 0 ||
                bullet->cTransform->pos.x + bulletRadius > m_window.getSize().x ||
                bullet->cTransform->pos.y - bulletRadius > m_window.getSize().y)
            {
                std::cout << "Da va cham\n";
                bullet->destroy();
                continue;
            }

            for (auto& enemy : enemies)
            {
                if (enemy->is_active())
                {
                    float enemyRadius = enemy->cShape->circle.getRadius();

                    if (bullet->cTransform->pos.distance(enemy->cTransform->pos) < bulletRadius + enemyRadius)
                    {
                        std::cout << "Da va cham\n";
                        bullet->destroy();
                        enemy->destroy();
                        break;
                    }
                }
            }
        }
    }

    for (auto& enemy : enemies)
    {
        float radius = enemy->cShape->circle.getRadius();

        if (enemy->cTransform->pos.x - radius < 0 ||
            enemy->cTransform->pos.x + radius > m_window.getSize().x ||
            enemy->cTransform->pos.y - radius < 0 ||
            enemy->cTransform->pos.y + radius > m_window.getSize().y)
        {
            enemy->cTransform->velocity *= -1;
        }

        for (auto& ePlayer : player)
        {
            if (enemy->cTransform->pos.distance(ePlayer->cTransform->pos) < radius + ePlayer->cShape->circle.getRadius())
            {
                ePlayer->destroy();
                enemy->destroy();


                break;
            }
        }
    }
}