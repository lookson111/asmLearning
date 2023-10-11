#include "camera.h"

void Camera::Apply()
{
    glRotatef(-xRot_, 1, 0, 0);
    glRotatef(-zRot_, 0, 0, 1);
    glTranslatef(-posx_, -posy_, -posz_);
}

void Camera::Rotation(float xAngle, float zAngle)
{
    zRot_ += zAngle;
    if (zRot_ < 0)
        zRot_ += 360;
    if (zRot_ > 360)
        zRot_ -= 360;
    xRot_ += xAngle;
    if (xRot_ < 0)
        xRot_ = 0;
    if (xRot_ > 180)
        xRot_ = 180;
}

void Camera::Move()
{
    if (!window_.hasFocus())
        return;
    sf::Vector2u current = window_.getSize();
    sf::Vector2i base = { (int)current.x / 2, (int)current.y / 2 };
    static constexpr float baseSpeed = 0.1;

    float angle = -zRot_ / 180 * M_PI;
    float speed = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        speed = baseSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        speed = -baseSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        speed = baseSpeed;
        angle -= M_PI * 0.5;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        speed = baseSpeed;
        angle += M_PI * 0.5;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        posz_ += baseSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        posz_ -= baseSpeed;
    if (speed != 0)
    {
        posx_ += sin(angle) * speed;
        posy_ += cos(angle) * speed;
    }
    sf::Vector2i cur = sf::Mouse::getPosition(window_);
    Rotation((base.y - cur.y) / 15.0, (base.x - cur.x) / 15.0);
    sf::Mouse::setPosition(base, window_);
}
