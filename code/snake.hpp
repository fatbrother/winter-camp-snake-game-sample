#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "context.hpp"

class Snake
{
public:
    Snake();
    void update(float deltaTime);
    void draw(sf::RenderWindow &window);
    void grow();
    void setDirection(int dir);
    bool isDead();
    bool isEating(sf::FloatRect foodPos);
    sf::Vector2f getHeadPos();
    std::deque<sf::FloatRect> getBody();

private:
    int direction;
    int nextDirection;
    int growCnt;
    std::deque<sf::Vector3f> body;
};

Snake::Snake()
{
    growCnt = 3;
    direction = RIGHT;
    nextDirection = RIGHT;
    body.push_back(sf::Vector3f(WIDTH / 2, HEIGHT / 2, 0));
}

void Snake::update(float deltaTime)
{
    double deltaDistance = SPEED * deltaTime;
    double newX = body[0].x + deltaDistance * DIR[direction];
    double newY = body[0].y + deltaDistance * DIR[direction + 1];

    if (newX > ceil(body[0].x) || newX < floor(body[0].x))
    {
        deltaDistance = abs(newX - body[0].x);
        direction = nextDirection;
        if (growCnt == 0)
            body.pop_back();
        else
            growCnt--;
        body[0].x = getClosestInt(newX);
        body[0].y = getClosestInt(newY);
        body[0].z = direction;
        body.push_front(body[0]);
    }

    if (newY > ceil(body[0].y) || newY < floor(body[0].y))
    {
        deltaDistance = abs(newY - body[0].y);
        direction = nextDirection;
        if (growCnt == 0)
            body.pop_back();
        else
            growCnt--;
        body[0].x = getClosestInt(newX);
        body[0].y = getClosestInt(newY);
        body[0].z = direction;
        body.push_front(body[0]);
    }

    body[0].x += deltaDistance * DIR[direction];
    body[0].y += deltaDistance * DIR[direction + 1];
    body.back().x += deltaDistance * DIR[(int)body.back().z];
    body.back().y += deltaDistance * DIR[(int)body.back().z + 1];
}

void Snake::draw(sf::RenderWindow &window)
{
    for (auto &i : body)
    {
        sf::RectangleShape rect(sf::Vector2f(GRIDSIZE, GRIDSIZE));
        rect.setPosition(i.x * GRIDSIZE, i.y * GRIDSIZE);
        rect.setFillColor(sf::Color::Green);
        window.draw(rect);
    }
}

void Snake::grow()
{
    growCnt++;
}

void Snake::setDirection(int dir)
{
    if (dir == UP && this->direction != DOWN)
        this->nextDirection = UP;
    else if (dir == DOWN && this->direction != UP)
        this->nextDirection = DOWN;
    else if (dir == LEFT && this->direction != RIGHT)
        this->nextDirection = LEFT;
    else if (dir == RIGHT && this->direction != LEFT)
        this->nextDirection = RIGHT;
}

bool Snake::isDead()
{
    if (body[0].x < 0 || body[0].x + .8 >= WIDTH || body[0].y < 0 || body[0].y + .8 >= HEIGHT)
        return true;

    sf::FloatRect head = sf::FloatRect(body[0].x * GRIDSIZE, body[0].y * GRIDSIZE, GRIDSIZE, GRIDSIZE);
    for (size_t i = 4; i < body.size(); i++)
    {
        sf::FloatRect bodyPart = sf::FloatRect(body[i].x * GRIDSIZE, body[i].y * GRIDSIZE, GRIDSIZE, GRIDSIZE);
        if (head.intersects(bodyPart))
            return true;
    }
    return false;
}

bool Snake::isEating(sf::FloatRect foodPos)
{
    return foodPos.intersects(sf::FloatRect(body[0].x * GRIDSIZE, body[0].y * GRIDSIZE, GRIDSIZE, GRIDSIZE));
}

sf::Vector2f Snake::getHeadPos()
{
    return sf::Vector2f(body[0].x, body[0].y);
}

std::deque<sf::FloatRect> Snake::getBody()
{
    std::deque<sf::FloatRect> bodyPos;
    for (auto &i : body)
        bodyPos.push_back(sf::FloatRect(i.x * GRIDSIZE, i.y * GRIDSIZE, GRIDSIZE, GRIDSIZE));
    return bodyPos;
}
