#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "context.hpp"
#include "snake.hpp"

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();
    void handlePlayerInput(sf::Keyboard::Key key);
    void reset();

    bool isGameOver;
    sf::RenderWindow window;
    sf::RectangleShape food;
    Snake snake;
    std::vector<sf::FloatRect> detectDots;
};

Game::Game()
{
    isGameOver = false;
    window.create(sf::VideoMode(WIDTH * GRIDSIZE, HEIGHT * GRIDSIZE), "Snake");
    window.setFramerateLimit(FPS);
    snake = Snake();
    food.setSize(sf::Vector2f(GRIDSIZE * 0.8, GRIDSIZE * 0.8));
    food.setFillColor(sf::Color::Red);
    food.setPosition(sf::Vector2f(rand() % WIDTH * GRIDSIZE + GRIDSIZE * 0.1, rand() % HEIGHT * GRIDSIZE + GRIDSIZE * 0.1));
    for (int i = 0; i < WIDTH; i++)
        for (int j = 0; j < HEIGHT; j++)
            detectDots.push_back(sf::FloatRect(i * GRIDSIZE, j * GRIDSIZE, GRIDSIZE * 0.9, GRIDSIZE * 0.9));
}

void Game::run()
{
    sf::Clock clock;
    float deltaTime = 0.0f;
    while (window.isOpen())
    {
        processEvents();
        deltaTime += clock.restart().asSeconds();
        while (deltaTime > 1.0f / FPS)
        {
            deltaTime -= 1.0f / FPS;
            processEvents();
            if (!isGameOver)
                update(1.0f / FPS);
        }
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::KeyPressed:
            handlePlayerInput(event.key.code);
            break;
        case sf::Event::Closed:
            window.close();
            break;
        default:
            break;
        }
    }
}

void Game::update(float deltaTime)
{
    snake.update(deltaTime);

    if (snake.isEating(food.getGlobalBounds()))
    {
        snake.grow();
        auto snakeBody = snake.getBody();
        std::vector<sf::Vector2f> ValidDots;
        for (auto dot : detectDots)
        {
            bool valid = true;
            for (auto body : snakeBody)
                if (dot.intersects(body))
                {
                    valid = false;
                    break;
                }

            if (valid)
                ValidDots.push_back(sf::Vector2f(dot.left, dot.top));
        }

        auto position = ValidDots[rand() % ValidDots.size()];
        food.setPosition(sf::Vector2f(position.x + GRIDSIZE * 0.1, position.y + GRIDSIZE * 0.1));
    }

    if (snake.isDead())
        isGameOver = true;
}

void Game::render()
{
    window.clear();
    for (int i = 0; i < WIDTH; i++)
        for (int j = 0; j < HEIGHT; j++)
        {
            sf::RectangleShape rect(sf::Vector2f(GRIDSIZE * 0.8, GRIDSIZE * 0.8));
            rect.setPosition(i * GRIDSIZE + GRIDSIZE * 0.1, j * GRIDSIZE + GRIDSIZE * 0.1);
            rect.setFillColor(sf::Color(50, 50, 50));
            window.draw(rect);
        }

    window.draw(food);
    snake.draw(window);
    window.display();
}

void Game::handlePlayerInput(sf::Keyboard::Key key)
{
    switch (key)
    {
    case sf::Keyboard::Up:
        snake.setDirection(UP);
        break;
    case sf::Keyboard::Down:
        snake.setDirection(DOWN);
        break;
    case sf::Keyboard::Left:
        snake.setDirection(LEFT);
        break;
    case sf::Keyboard::Right:
        snake.setDirection(RIGHT);
        break;
    case sf::Keyboard::R:
        reset();
        break;
    default:
        break;
    }
}

void Game::reset()
{
    isGameOver = false;
    snake = Snake();
    food.setPosition(sf::Vector2f(rand() % WIDTH * GRIDSIZE + GRIDSIZE * 0.1, rand() % HEIGHT * GRIDSIZE + GRIDSIZE * 0.1));
}