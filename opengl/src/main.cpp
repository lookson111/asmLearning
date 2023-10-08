#define _USE_MATH_DEFINES

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <math.h>

float vert[] = {
    1, 1, 1,
   -1, 0, 0,
    1,-1, 0
};

int main()
{
    sf::ContextSettings window_settings;
    window_settings.depthBits = 24; // Request a 24-bit depth buffer
    window_settings.stencilBits = 8;  // Request a 8 bits stencil buffer
    window_settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
    // create the window
    sf::Window window(sf::VideoMode(800, 800), "OpenGL", sf::Style::Default, window_settings);
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);

    // load resources, initialize the OpenGL states, ...

    glLoadIdentity();
    //glOrtho(-7, 7, -7, 7, -7, 7);
    glFrustum(-1,1, -1,1, 2,6);
    glTranslatef(0, 0, -2);

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
            case sf::Event::MouseWheelScrolled:
                break;
            default:
                break;
            }
        }

        // clear the buffers
        glClearColor(0.7f, 1.0f, 0.7f, 0.0f); // цвет очистки экрана

        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glTranslatef(0, 0, -0.01);
        glRotatef(0.1, 0, 0, 1);
        glVertexPointer(3, GL_FLOAT, 0, &vert);
        glEnableClientState(GL_VERTEX_ARRAY);
        {
            glColor3f(0, 1, 0);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            {
                glPushMatrix();
                glColor3f(1, 0, 0);
                glRotatef(180, 0, 0, 1);
                //glTranslatef(1, 0, -1);
                glDrawArrays(GL_TRIANGLES, 0, 3);
                glPopMatrix();
            }

        }
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}