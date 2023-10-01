#include "Game.h"
#include <iostream>
#include "Entity.h"
#include "EntityManager.h"
#include <cmath>
#include <random>
#include <ctime>
#include <string>
#include <fstream>

int score = 0;
int maxEnemySpawned = 6;

struct WindowConfig
{
    std::string title;
    int width;
    int height;
    int frameLimit;
    int screenMode;
};

struct FontConfig
{
    std::string fontPath;
    int fontSize;
    int colorRed;
    int colorGreen;
    int colorBlue;
};

struct PlayerConfig
{
    int shapeRadius;
    int collisionRadius;
    float speed;
    int fillColorRed;
    int fillColorGreen;
    int fillColorBlue;
    int outlineColorRed;
    int outlineColorGreen;
    int outlineColorBlue;
    int outlineThickness;
    int shapeVertices;
};

struct EnemyConfig
{
    int shapeRadius;
    int collisionRadius;
    float minSpeed;
    float maxSpeed;
    int outlineColorRed;
    int outlineColorGreen;
    int outlineColorBlue;
    int outlineThickness;
    int minShapeVertices;
    int maxShapeVertices;
    int smallLifespan;
    int spawnInterval;
};

struct BulletConfig
{
    int shapeRadius;
    int collisionRadius;
    float speed;
    int fillColorRed;
    int fillColorGreen;
    int fillColorBlue;
    int outlineColorRed;
    int outlineColorGreen;
    int outlineColorBlue;
    int outlineThickness;
    int shapeVertices;
    int lifespan;
};

struct WindowConfig wConf;
struct FontConfig fConf;
struct PlayerConfig pConf;
struct EnemyConfig eConf;
struct BulletConfig bConf;


void loadConfigFile(const std::string & fileName)
{
    std::ifstream inputFile(fileName);
    std::string firstWordInLine;

    while(inputFile >> firstWordInLine)
    {
        if (firstWordInLine == "Window")
        {
            inputFile >> wConf.width >> wConf.height >> wConf.frameLimit >> wConf.screenMode;
        }
        else if (firstWordInLine == "Font" )
        {
            inputFile >> fConf.fontPath >> fConf.fontSize >> fConf.colorRed >> fConf.colorGreen>> fConf.colorBlue;
        }
        else if (firstWordInLine == "Player")
        {
            inputFile >> pConf.shapeRadius >> pConf.collisionRadius >> pConf.speed >>
                        pConf.fillColorRed >> pConf.fillColorGreen >> pConf.fillColorBlue >>
                        pConf.outlineColorRed >> pConf.outlineColorGreen >> pConf.outlineColorBlue >>
                        pConf.outlineThickness >> pConf.shapeVertices;

        }
        else if (firstWordInLine == "Enemy")
        {
            inputFile >> eConf.shapeRadius >> eConf.collisionRadius >> eConf.minSpeed >> eConf.maxSpeed >>
                        eConf.outlineColorRed >> eConf.outlineColorGreen >> eConf.outlineColorBlue >>
                        eConf.outlineThickness >> eConf.minShapeVertices>> eConf.maxShapeVertices >>
                        eConf.smallLifespan >> eConf.spawnInterval;
        }
        else if (firstWordInLine == "Bullet")
        {
            inputFile >> bConf.shapeRadius >> bConf.collisionRadius >> bConf.speed >>
                        bConf.fillColorRed >> bConf.fillColorGreen >> bConf.fillColorBlue >>
                        bConf.outlineColorRed >> bConf.outlineColorGreen >> bConf.outlineColorBlue >>
                        bConf.outlineThickness >> bConf.shapeVertices >> bConf.lifespan;
        }

    }
    std::cout << fConf.fontSize << "\n";

}


Game::Game()
{
    init();
}

void Game::init()
{
    //load the config file hered
    loadConfigFile("config.txt");

    //creating the window
    m_window.create(sf::VideoMode(wConf.width, wConf.height), "Game");
    m_window.setFramerateLimit(wConf.frameLimit);

    if(!m_font.loadFromFile(fConf.fontPath))
    {
        std::cout << "Cant load the font file ! \n";
    }

    m_text.setFont(m_font);
    m_text.setCharacterSize(fConf.fontSize);
    m_text.setFillColor(sf::Color(fConf.colorRed, fConf.colorGreen, fConf.colorBlue));
    m_text.setStyle(sf::Text::Bold);

    spawnPlayer();
}

void Game::setPaused(bool paused)
{
    m_paused = paused;
}

void Game::run()
{
    while(m_running)
    {

        m_entities.update();

        sUserInput();

        if(!m_paused)
        {
            sLifespan();
            sEnemySpawner();
            sMovement();
            sCollision();

        }

        sRender();

        m_currentFrame++;

    }
}

void Game::sRender()
{
    m_window.clear(sf::Color::Black);

    //rendering all game object here

    for (auto& e : m_entities.getEntities())
    {
        e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
        e->cTransform->angle += 1.0f;
        e->cShape->circle.setRotation(e->cTransform->angle);

        m_window.draw(e->cShape->circle);
    }


    m_text.setString("SCORE : " + std::to_string(score));
    m_window.draw(m_text);


    m_window.display();
}

void Game::sUserInput()
{


    sf::Event event;
    while(m_window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            m_running = false;
            m_window.close();
        }

        if(event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Escape :
                m_running = false;
                m_window.close();
                break;
            case sf::Keyboard::W :
                m_player->cInput->up = true;
                break;
            case sf::Keyboard::S :
                m_player->cInput->down = true;
                break;
            case sf::Keyboard::A :
                m_player->cInput->left = true;
                break;
            case sf::Keyboard::D :
                m_player->cInput->right = true;
                break;

            case sf::Keyboard::F1 :
                setPaused(true);
                break;
            case sf::Keyboard::F2 :
                setPaused(false);
                break;
            default :
                break;

            }
        }

        if(event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::W :
                m_player->cInput->up = false;
                break;
            case sf::Keyboard::S :
                m_player->cInput->down = false;
                break;
            case sf::Keyboard::A :
                m_player->cInput->left = false;
                break;
            case sf::Keyboard::D :
                m_player->cInput->right = false;
                break;
            default :
                break;

            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                spawnBullet(m_player->cTransform->pos, Vec2(event.mouseButton.x, event.mouseButton.y));
            }
        }
    }


}


void Game::sMovement()
{
    m_player->cTransform->velocity = {0,0};


    if (m_player->cInput->up &&
            m_player->cTransform->pos.y - m_player->cShape->circle.getRadius() > 0.0f)
    {
        m_player->cTransform->velocity.y = -1.0f;
    }

    if (m_player->cInput->down &&
            m_player->cTransform->pos.y + m_player->cShape->circle.getRadius() < m_window.getSize().y)
    {
        m_player->cTransform->velocity.y = 1.0f;
    }

    if (m_player->cInput->left &&
            m_player->cTransform->pos.x - m_player->cShape->circle.getRadius() > 0.0f)
    {
        m_player->cTransform->velocity.x = -1.0f;
    }

    if (m_player->cInput->right &&
            m_player->cTransform->pos.x + m_player->cShape->circle.getRadius() < m_window.getSize().x)
    {
        m_player->cTransform->velocity.x = 1.0f;
    }


    //std::cout << m_player->cTransform->velocity.x << "," << m_player->cTransform->velocity.y  << "\n";
    if (m_player->cTransform->velocity.length() != 0.0f)
    {
        m_player->cTransform->velocity.normalized();
        //std::cout << m_player->cTransform->velocity.length() << "\n";
    }

    m_player->cTransform->pos += (m_player->cTransform->velocity * pConf.speed ); // velocity times speed

    for (auto &  b : m_entities.getEntities("Bullet"))
    {
        float angle = std::atan2(b->cTransform->velocity.x, b->cTransform->velocity.y);
        b->cTransform->pos.x += bConf.speed * std::sin(angle);
        b->cTransform->pos.y += bConf.speed * std::cos(angle);

        //std::cout << std::cos(angle) << "," << std::sin(angle) << "\n";

    }

    for (auto & sM : m_entities.getEntities("SmallEnemy"))
    {
        float sMSpeed =  eConf.minSpeed + (float) (rand()) / (float) (RAND_MAX / (eConf.maxSpeed - eConf.minSpeed));
        //std::cout << sMSpeed << "\n";

        sM->cTransform->pos.x += sMSpeed * sM->cTransform->velocity.x;
        sM->cTransform->pos.y += sMSpeed * sM->cTransform->velocity.y;
    }

}


void Game::spawnPlayer()
{
    auto newEntity = m_entities.addEntity("Player");
    newEntity->cTransform = std::make_shared<CTransform>(Vec2(200.0f, 200.0f), Vec2(1.0f, 1.0f), 0.0f);
    newEntity->cShape = std::make_shared<CShape>(pConf.shapeRadius, pConf.shapeVertices,
                                                 sf::Color(pConf.fillColorRed, pConf.fillColorGreen, pConf.fillColorBlue),
                                                 sf::Color(pConf.outlineColorRed, pConf.outlineColorGreen, pConf.outlineColorBlue),
                                                 pConf.outlineThickness);
    newEntity->cCollision = std::make_shared<CCollision>(pConf.collisionRadius);
    newEntity->cInput = std::make_shared<CInput>();

    m_player = newEntity;
}

void Game::spawnBullet(const Vec2 & playerPos, const Vec2 & target)
{
    auto newBullet = m_entities.addEntity("Bullet");

    Vec2 dist = target - playerPos;

    newBullet->cTransform = std::make_shared<CTransform>(playerPos, dist, 0.0f);
    newBullet->cShape = std::make_shared<CShape>(bConf.shapeRadius, bConf.shapeVertices,
                                                 sf::Color(bConf.fillColorRed, bConf.fillColorGreen, bConf.fillColorBlue),
                                                 sf::Color(bConf.outlineColorRed, bConf.outlineColorGreen, bConf.outlineColorBlue),
                                                 bConf.outlineThickness);
    newBullet->cLifespan = std::make_shared<CLifespan>(bConf.lifespan);
    newBullet->cCollision = std::make_shared<CCollision>(bConf.collisionRadius);


}

void Game::spawnEnemy()
{
    auto newEntity = m_entities.addEntity("Enemy");

    float ex = rand() % (m_window.getSize().x - eConf.shapeRadius); //subtract its radius
    float ey = rand() % (m_window.getSize().y - eConf.shapeRadius);

    size_t vert  = (size_t) rand() % (eConf.maxShapeVertices - eConf.minShapeVertices + 1) + eConf.minShapeVertices;
    std::cout << vert << "\n";

    newEntity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(0.0f, 0.0f), 0.0f);
    newEntity->cShape = std::make_shared<CShape>(eConf.shapeRadius, vert,
                                                 sf::Color(255, 0, 0),
                                                 sf::Color(eConf.outlineColorRed, eConf.outlineColorGreen, eConf.outlineColorBlue),
                                                 eConf.outlineThickness);
    //newEntity->cLifespan = std::make_shared<CLifespan>(300);
    newEntity->cCollision = std::make_shared<CCollision>(eConf.collisionRadius);

    m_lastEnemySpawnTime = m_currentFrame;

}

void Game::spawnSmallEnemies(const std::shared_ptr<Entity> & parentEnemy, const Vec2 & vel, const float & angle)
{
    auto newSmallEnemy = m_entities.addEntity("SmallEnemy");

    //Vec2 posVec = {parentEnemy->cTransform->pos.normalized().y;

    newSmallEnemy->cTransform = std::make_shared<CTransform>(parentEnemy->cTransform->pos, vel, angle);
    newSmallEnemy->cShape = std::make_shared<CShape>((float)(parentEnemy->cShape->circle.getRadius() / 3), parentEnemy->cShape->circle.getPointCount(),
                                                 sf::Color(255, 0, 0),
                                                 sf::Color(eConf.outlineColorRed, eConf.outlineColorGreen, eConf.outlineColorBlue),
                                                 eConf.outlineThickness);
    newSmallEnemy->cCollision = std::make_shared<CCollision>((parentEnemy->cCollision->radius / 3));
    newSmallEnemy->cLifespan = std::make_shared<CLifespan>(eConf.smallLifespan);

}

void Game::sLifespan()
{
    for (auto & b : m_entities.getEntities("Bullet"))
    {
        if (b->isActive())
        {
            b->cLifespan->remaining--;
            //std::cout << b->cLifespan->remaining << "\n";
            if (b->cLifespan->remaining <= 0)
            {
                b->destroy();
            }
        }
    }

    for (auto & sM : m_entities.getEntities("SmallEnemy"))
    {
        if(sM->isActive())
        {
            sM->cLifespan->remaining--;

            if(sM->cLifespan->remaining <= 0)
            {
                sM->destroy();
            }
        }
    }

    /**for (auto & e : m_entities.getEntities("Enemy"))
    {
        if (e->isActive())
        {
            e->cLifespan->remaining--;

            if (e->cLifespan->remaining <= 0)
            {
                e->destroy();
            }
        }
    }**/
}

void Game::sEnemySpawner()
{

    if (m_entities.getEntities("Enemy").size() <= maxEnemySpawned)
    {
        if ((m_currentFrame - m_lastEnemySpawnTime) % eConf.spawnInterval == 0)
        {
            spawnEnemy();
            //std::cout << m_entities.getEntities("Enemy").size() << "\n";
        }
    }
}


bool Game::circleIntersect(float x1, float y1, float r1, float x2, float y2, float r2)
{
    float sqrDeltaPos = std::pow((x1 - x2), 2) + std::pow((y1 - y2),2);
    float sqrRad = std::pow((r1 + r2), 2);
    return sqrDeltaPos <= sqrRad;
}
void Game::sCollision()
{
    for(auto & e : m_entities.getEntities("Enemy"))
    {
        for(auto & b : m_entities.getEntities("Bullet"))
        {
            bool check = circleIntersect(e->cTransform->pos.x, e->cTransform->pos.y, e->cCollision->radius,
                                        b->cTransform->pos.x, b->cTransform->pos.y, b->cCollision->radius);

            if (check)
            {
                //std::cerr << check << "\n";
                // Vec2 vel = {};
                float angle = 0.0f;
                for(int i = 0; i < e->cShape->circle.getPointCount(); ++i)
                {
                    angle += (((float) e->cShape->circle.getPointCount() -  2.0f) * 180.0f ) / (float) e->cShape->circle.getPointCount();
                    Vec2 vel = {std::sin(angle), std::cos(angle)};
                    spawnSmallEnemies(e, vel, angle);
                }
                e->destroy();
                b->destroy();
                angle = 0.0f;
                score++;
            }

            for (auto & sM : m_entities.getEntities("SmallEnemy"))
            {
                bool checkToB = circleIntersect(sM->cTransform->pos.x, sM->cTransform->pos.y, sM->cCollision->radius,
                                                b->cTransform->pos.x, b->cTransform->pos.y, b->cCollision->radius);
                //looping through the player to check collision here

                if(checkToB)
                {
                    std::cout << "Bullet Damaged Small Enemy. \n";
                    sM->destroy();
                }
            }

        }

        bool checkToP = circleIntersect(e->cTransform->pos.x, e->cTransform->pos.y, e->cCollision->radius,
                                       m_player->cTransform->pos.x, m_player->cTransform->pos.y, m_player->cCollision->radius);
        //looping through the player to check collision here

        if(checkToP)
        {
            std::cout << "Player Damaged By Enemy. \n";
            e->destroy();
        }
    }

    for (auto & sM : m_entities.getEntities("SmallEnemy"))
    {
        bool checkToP = circleIntersect(sM->cTransform->pos.x, sM->cTransform->pos.y, sM->cCollision->radius,
                                        m_player->cTransform->pos.x, m_player->cTransform->pos.y, m_player->cCollision->radius);
        //looping through the player to check collision here

        if(checkToP)
        {
            std::cout << "Player Damaged By Small Enemy. \n";
            sM->destroy();
        }
    }
}

