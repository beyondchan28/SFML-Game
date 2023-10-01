#pragma once
#include "SFML/Graphics.hpp"
#include "EntityManager.h"
#include <iostream>

class Game
{
    sf::RenderWindow m_window;
    sf::Font m_font;
    sf::Text m_text;

    EntityManager m_entities;
    std::shared_ptr<Entity> m_player;

    bool m_paused = false;
    bool m_running = true;

    int m_lastEnemySpawnTime = 0;
    int m_currentFrame = 0;

    void init();

    void setPaused(bool paused);

    void spawnPlayer();
    void spawnBullet(const Vec2 & playerPos, const Vec2 & target);

    void spawnSmallEnemies(const std::shared_ptr<Entity> & parentEnemy, const Vec2 & vel, const float & angle);


    void sMovement();
    void spawnEnemy();
    void sEnemySpawner();
    void sLifespan();

    void sCollision();
    bool circleIntersect(float x1, float y1, float r1, float x2, float y2, float r2);

    void sUserInput();
    void sRender();

public:

    Game();
    void run();

};
