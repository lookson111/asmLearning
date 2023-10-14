#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>
#include "camera.h"
#include "model.h"
#include "view.h"
#include "load_data.h"

using namespace std::literals;


static int width = 1200;
static int height = 600;


void ResizeWindow(int w, int h)
{
    glViewport(0, 0, w, h);
    glLoadIdentity();
    float k = (float)w / (float)h;
    float sz = 0.2f;
    glFrustum(-k * sz, k * sz, -sz, sz, sz * 2, 10000.0f);
}

void SpaceInit()
{
    glEnable(GL_DEPTH_TEST);
    ResizeWindow(width, height);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Need write path to data file." << std::endl;
        return 0;
    }
    std::string path = argv[1];

    sf::ContextSettings window_settings;
    window_settings.depthBits = 24; // Request a 24-bit depth buffer
    window_settings.stencilBits = 8;  // Request a 8 bits stencil buffer
    window_settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
    // create the window
    sf::Window window(sf::VideoMode(width, height), "OpenGL", sf::Style::Default, window_settings);
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorGrabbed(true);
    // activate the window
    window.setActive(true);

    // load resources, initialize the OpenGL states, ...
    window.setFramerateLimit(60);
    SpaceInit();
    model::Axes axes;
    Camera camera(window);
    std::unique_ptr<view::ObjectsView> viewer;
    auto data = load::LoadData(path);
    if (data == std::nullopt) {
        std::cout << "File not found." << std::endl;
        return 0;
    }
    else {
        viewer = std::make_unique<view::ObjectsView>(axes, data.value().second, data.value().first);
    }


    // run the main loop
    bool running = true;
    while (running)
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                running = false;
                break;
            case sf::Event::Resized: {
                width = event.size.width;
                height = event.size.height;
                ResizeWindow(width, height);
                break; 
            }
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    running = false;
                if (event.key.code == sf::Keyboard::E)
                    viewer->PlainOnOff();
                break;
            case sf::Event::MouseWheelScrolled:
                break;
            case sf::Event::MouseButtonPressed: {
                break;
            }
            case sf::Event::MouseEntered: {
                //window.setMouseCursorVisible(false);
            }
            case sf::Event::MouseMoved: {
                //std::cout << "new mouse x: " << event.mouseMove.x << std::endl;
                //std::cout << "new mouse y: " << event.mouseMove.y << std::endl;
            }
            default:
                break;
            }
        }

        // clear the buffers
        glClearColor(0.6f, 0.8f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.Move();
        glPushMatrix();
        {
            camera.Apply();
            viewer->ShowAll();
        }
        glPopMatrix();

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}
