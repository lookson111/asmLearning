#define _USE_MATH_DEFINES

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <math.h>

void ShowObj() {
    glBegin(GL_TRIANGLES);
    glColor3f(0, 0, 0); glVertex2f(0, 0);
    glColor3f(1, 0, 0); glVertex2f(0.5, 0);
    glColor3f(0, 1, 0); glVertex2f(0, 0.5);
    glEnd();
}

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
        glClearColor(0.7f, 1.0f, 0.7f, 0.0f); // цвет очистки экрана
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        glScalef(0.5, 0.5, 1);
        glTranslatef(1, 1, 0);
        for (int i = 0; i < 7; i++) {
            glPushMatrix();
            //glScalef(0.3, 0.3, 1);
            glRotatef(45*i, 0, 0, 1);
            glTranslatef(0.5, 0, 0);
            ShowObj();
            glPopMatrix();
        }


        
        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}