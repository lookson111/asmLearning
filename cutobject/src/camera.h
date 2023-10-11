#pragma once

#define _USE_MATH_DEFINES
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

class Camera {
private:
    float posx_ = 5;
    float posy_ = 5;
    float posz_ = 5;
    float xRot_ = 70, zRot_ = -40;
    sf::Window& window_;

public:
    Camera(sf::Window& window) : window_(window) {}
    void Apply();
    void Rotation(float xAngle, float zAngle);
    void Move();
};