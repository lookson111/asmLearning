#define _USE_MATH_DEFINES

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <math.h>

int main()
{
    // create the window
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);

    // load resources, initialize the OpenGL states, ...

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
            case sf::Event::Resized:
                window.setSize({ static_cast<unsigned int>(event.size.width), static_cast<unsigned int>(event.size.height) });
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    running = false;
                break;
            default:
                break;
            }
        }

        // clear the buffers
        glClearColor(0.0f, 0.5f, 0.2f, 0.0f); // цвет очистки экрана
        glClear(GL_COLOR_BUFFER_BIT);
        {
            glPointSize(10);
            glBegin(GL_POINTS);
            glColor3f(0, 0, 0);
            glVertex2f(0.5, 0.5);
            glVertex2f(-0.5, 0.5);
            glVertex2f(0.5, -0.5);
            glVertex2f(-0.5, -0.5);
            glEnd();
        }
        {
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, 0x00ff);
            glLineWidth(5);
            glBegin(GL_LINE_LOOP);
            glColor3f(0, 0.4, 0);
            glVertex2f(0.5, 0.5);
            glVertex2f(-0.5, 0.5);
            glColor3f(0, 0.4, 0.4);
            glVertex2f(0.5, -0.5);
            glVertex2f(-0.5, -0.5);
            glEnd();
        }

        {
            glBegin(GL_TRIANGLE_STRIP);
            glColor3f(0, 0.4, 0);
            glVertex2f(0.5, 0.5);
            glVertex2f(-0.5, 0.5);
            glColor3f(1, 0.4, 0.4);
            glVertex2f(-0.5, -0.5);
            glColor3f(0, 1, 0);
            glVertex2f(0.5, -0.5);
            glEnd();
        }

        { // circle
            float x, y;
            float cnt = 50;
            float l = 0.5;
            float a = M_PI * 2 / cnt;
            glBegin(GL_TRIANGLE_FAN);
            glColor3f(1, 0, 1);
            glVertex2f(0, 0);
            for (int i = -1; i < cnt; i++) {
                x = sin(a * i) * l;
                y = cos(a * i) * l;
                glVertex2f(x, y);
            }
            glEnd();
        }
        
        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}